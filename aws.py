import time
from awscrt import io, mqtt
from awscrt.auth import AwsCredentialsProvider
from awsiot import mqtt_connection_builder
from awsiot.iotshadow import IotShadowClient

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
        ca_filepath="./certs/RootCA3.pem",
        client_id="RPI",
        clean_session=False,
        keep_alive_secs=6)


def get_camera_state() -> str:
    with open('camera_recording.txt') as f:
        return f.read()


def on_message_received(topic, payload, **kwargs):
    payload = payload.decode("utf-8")
    print(f"Received message from topic {topic}: {payload}")
    if "camera_request" in payload:
        with open('camera_request.txt', 'w') as f:
            f.write("1")
        # mqtt_connection.publish(
        #     topic="camera",
        #     payload=f"response_camera_state: {get_camera_state()}",
        #     qos=mqtt.QoS.AT_LEAST_ONCE
        # )


if __name__ == "__main__":
    print("Connecting")
    mqtt_connection = get_connection()
    connected_f = mqtt_connection.connect()
    client = IotShadowClient(mqtt_connection)
    connected_f.result()
    print("Connected")

    start_time = time.time()

    subscribe_f, packet_id = mqtt_connection.subscribe(
        topic="camera",
        qos=mqtt.QoS.AT_LEAST_ONCE,
        callback=on_message_received
    )
    subscribe_result = subscribe_f.result()
    print(f"Subscribed to camera")

    while True:
        if int(time.time() - start_time) % 5 == 0:
            mqtt_connection.publish(
                topic="camera",
                payload=f"camera_state: {get_camera_state()}",
                qos=mqtt.QoS.AT_LEAST_ONCE
            )
        
        time.sleep(1)
