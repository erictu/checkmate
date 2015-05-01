#Player 2
import sys
from socket import *

import serial,time

def main():
    ser = serial.Serial('/dev/cu.usbmodem1411', 9600)   #replace string with your path to arduino
    time.sleep(2)
    ser.write("Hello World!")
    while True:
        # ser.write("Hello World! \n")
        print(ser.readline())
    sys.exit(0)

if __name__ == "__main__":
    main()