import sys
from socket import *
import time, serial

# def sendDataToArduino(data):
#     print("Received message [Player 2]: " + data)
#     ser.write(data)

def flush_to_log(data):
    # print("Received message [Player 1]: " + data)
    fh = open("log.txt","a")
    fh.write(data)
    print("in data" + data)
    fh.close()

def getDataFromArduino(ser):
    received = False
    try:
        while not received:
            # ser.write(send_msg)
            data = ser.readline() # Read the newest output from the Arduino
            if (len(data) == 8): #length 6
                print (data)
                received = True
                # ser.write("z")
                # sleep(.2) # Delay for one tenth of a second
                return data
            # else:
            #     print "wrong length of data received"
            #     print(len(data))
            #     print(data)
            #     print("-----")

    except Exception, e:
        print e
        while not received:
            # ser.write(send_msg)
            print ("in exception")
            data = ser.readline() # Read the newest output from the Arduino
            if len(data) == 8: #length 8
                print (data)
                received = True
                # ser.write("z")
                # sleep(.2) # Delay for one tenth of a second
                return data
            # else:
            #     print "wrong length of data received"
            #     print(len(data))
            #     print(data)
            #     print("------")

def main():
    # host = ""
    # port = 13000
    # buf = 1024
    # addr = (host, port)
    # UDPSock = socket(AF_INET, SOCK_DGRAM)
    # UDPSock.bind(addr)
    ser = serial.Serial('/dev/cu.usbmodem1421', 9600)
    while True:
        print("Waiting for Player 1's Move")
        # (rcv_msg, addr) = UDPSock.recvfrom(buf) #get msg over internet
        # sendDataToArduino(rcv_msg)
        # send_msg = raw_input("Enter your move: ")
        rcv_msg = getDataFromArduino(ser)
        print("got message from arduino")
        print(rcv_msg)
        print("----------------------------------------")
        flush_to_log(rcv_msg)
        # UDPSock.sendto(send_msg, addr)
    # UDPSock.close()
    # sys.exit(0)

if __name__ == "__main__":
    main()