#Player 1
import sys
from socket import *
import serial,time

def send_to_board(data):
    print("Received message [Player 2]: " + data)
    ser.write(data)

def main():
    host = "127.0.0.1" # set to IP address of target computer
    port = 13000
    buf = 1024
    addr = (host, port)
    UDPSock = socket(AF_INET, SOCK_DGRAM)
    # ser = serial.Serial('/dev/ttyACM0', 9600)
    while True:
        looking = True
        while looking: #getting a move to send to other computer, either from android or physical
            tiebreaker = open("log.txt", "r")
            move = tiebreaker.readline()
            move = move[:len(move)-1]
            if len(move) == 6: #got a correct move, ready to send to computer
                UDPSock.sendto(move, addr)
                open('log.txt', 'w').close() #completely empty the file
                looking = False
            else:
                print move
        # data = raw_input("Enter your move: ")
        # send_msg = getDataFromArduino()
        # UDPSock.sendto(send_msg, addr)
        print("Waiting for Player 2's Move")
        (rcv_msg, addr) = UDPSock.recvfrom(buf) #get msg over internet
        send_to_board(rcv_msg)
    UDPSock.close()
    sys.exit(0)

if __name__ == "__main__":
    main()