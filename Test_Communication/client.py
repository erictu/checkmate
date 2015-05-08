#Player 1
import sys
from socket import *
import serial,time

def sendDataToArduino(data):
    print("Received message [Player 2]: " + data)
    ser.write(data)

def getDataFromArduino():
    received = False
    try:
        while not received:
            # ser.write(send_msg)
            data = ser.readline() # Read the newest output from the Arduino
            if len(data) == 6:
                print (data)
                received = True
                # ser.write("z")
                sleep(.2) # Delay for one tenth of a second
                return data
            else:
                print "wrong length of data received"
                print data
                #find some way to exit out of this

    except Exception, e:
        print e
        while not received:
            # ser.write(send_msg)
            data = ser.readline() # Read the newest output from the Arduino
            if len(data) == 6:
                print (data)
                received = True
                # ser.write("z")
                sleep(.2) # Delay for one tenth of a second
                return data
            else:
                print "wrong length of data received"
                print data

def main():
    host = "127.0.0.1" # set to IP address of target computer
    port = 13000
    buf = 1024
    addr = (host, port)
    UDPSock = socket(AF_INET, SOCK_DGRAM)
    ser = serial.Serial('/dev/ttyACM0', 9600)
    while True:
        # data = raw_input("Enter your move: ")
        send_msg = getDataFromArduino()
        UDPSock.sendto(send_msg, addr)
        print("Waiting for Player 2's Move")
        (rcv_msg, addr) = UDPSock.recvfrom(buf) #get msg over internet
        sendDataToArduino(rcv_msg)
    UDPSock.close()
    sys.exit(0)

if __name__ == "__main__":
    main()