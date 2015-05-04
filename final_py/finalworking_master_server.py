#Player 2
import sys
from socket import *
import serial
import time

def send_to_board(data, ser):
    print("writing to serial" + data)
    ser.write(data)

def main():
    host = ""
    port = 12000
    buf = 1024
    addr = (host, port)
    UDPSock = socket(AF_INET, SOCK_DGRAM)
    UDPSock.bind(addr)
    ser = serial.Serial('/dev/tty.usbmodemfd121', 9600)
    while True:
        print("Waiting for Player 1's Move from Computer")
        (rcv_msg, addr) = UDPSock.recvfrom(buf)
        print(rcv_msg)
        print("------------------")
        # time.sleep(10)
        ser.write(rcv_msg)
        print("send to board")
        # send_to_board(rcv_msg, ser) #got move from other player, now send to board
        # while True:
        #     print(ser.readline())
        looking = True
        while looking: #getting a move to send to other computer, either from android or physical
            tiebreaker = open("log.txt", "r")
            move = tiebreaker.readline()
            move = move[:len(move)-1]
            if ((len(move) == 7) or (len(move)== 8)): #got a correct move, ready to send to computer
                UDPSock.sendto(move, addr)
                open('log.txt', 'w').close() #completely empty the file
                looking = False
                print ("got it")
                print ("-----------------------")
            # else:
            #     print(len(move))
    UDPSock.close()
    sys.exit(0)

if __name__ == "__main__":
    main()