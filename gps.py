from queue import Queue
import threading
import time
import serial
import adafruit_gps


class GPS:

    def __init__(self):
        uart = serial.Serial("/dev/gps0", baudrate=9600, timeout=10)
        self.gps = adafruit_gps.GPS(uart, debug=False)
        self.gps.send_command(b'PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0')
        self.gps.send_command(b'PMTK220,1000')

    def run(self, queue):
        first = True
        last_time = time.monotonic()
        while True:
            self.gps.update()
            current = time.monotonic()
            if current - last_time >= 5 or first:
                first = False
                last_time = current
                if not self.gps.has_fix:
                    queue.put({"latitude": None, "longitude": None})
                    continue
                queue.put(
                    {"latitude": "{0:.6f}".format(self.gps.latitude),
                     "longitude": "{0:.6f}".format(self.gps.longitude)}
                )


if __name__ == "__main__":
    gps = GPS()
    q = Queue()
    threading.Thread(target=gps.run, args=[q]).start()
    while True:
        if not q.empty():
            print(q.get())
