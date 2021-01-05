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
        last_print = time.monotonic()
        while True:

            self.gps.update()

            current = time.monotonic()
            if current - last_print >= 60.0:
                last_print = current
                if not self.gps.has_fix:
                    print('Waiting for fix...')
                    continue
                queue.put(
                    {"latitude": "{0:.6f}".format(self.gps.latitude),
                     "longitude": "{0:.6f}".format(self.gps.longitude)}
                    if self.gps.has_fix
                    else {"latitude": None, "longitude": None}
                )
