import json
import threading
import time
from queue import Queue

from awscrt import io, mqtt
from awscrt.auth import AwsCredentialsProvider
from awsiot import mqtt_connection_builder
from awsiot.iotshadow import IotShadowClient

from gps import GPS

io.init_logging(getattr(io.LogLevel, "Error"), 'stderr')
event_loop_group = io.EventLoopGroup(1)
host_resolver = io.DefaultHostResolver(event_loop_group)
client_bootstrap = io.ClientBootstrap(event_loop_group, host_resolver)
credentials_provider = AwsCredentialsProvider.new_default_chain(
    client_bootstrap)


def get_connection() -> mqtt.Connection:
    return mqtt_connection_builder.websockets_with_default_aws_signing(
        endpoint="a3mxizmj3briy4-ats.iot.eu-west-3.amazonaws.com",
        client_bootstrap=client_bootstrap,
        region="eu-west-3",
        credentials_provider=credentials_provider,
        websocket_proxy_options=None,
        ca_filepath="/home/stijn/iot/certs/RootCA3.pem",
        client_id="RPI",
        clean_session=False,
        keep_alive_secs=6)


def get_camera_state() -> str:
    with open('/home/stijn/iot/camera_recording.txt') as f:
        return f.read()


def on_request_received(topic, payload, **kwargs):
    payload = payload.decode("utf-8")
    if json.loads(payload)["data"] == 1:
        with open('/home/stijn/iot/camera_request.txt', 'w') as f:
            f.write("1")

if __name__ == "__main__":
    print("Connecting")
    mqtt_connection = get_connection()
    connected_f = mqtt_connection.connect()
    client = IotShadowClient(mqtt_connection)
    connected_f.result()
    print("Connected")

    gps_data_queue = Queue()
    gps = GPS()
    threading.Thread(target=gps.run, args=[gps_data_queue]).start()

    subscribe_f, packet_id = mqtt_connection.subscribe(
        topic="camera/request",
        qos=mqtt.QoS.AT_LEAST_ONCE,
        callback=on_request_received
    )
    subscribe_result = subscribe_f.result()
    print(f"Subscribed to camera/request")

    start_time = time.time()

    while True:
        if int(time.time() - start_time) % 2 == 0 or int(time.time() - start_time) == 0:
            mqtt_connection.publish(
                topic="camera/state",
                payload=json.dumps({ "data": int(get_camera_state()) }),
                qos=mqtt.QoS.AT_LEAST_ONCE
            )
        if not gps_data_queue.empty():
            mqtt_connection.publish(
                topic="camera/location",
                payload=json.dumps(gps_data_queue.get()),
                qos=mqtt.QoS.AT_LEAST_ONCE
            )

        time.sleep(1)
