#Player 1
import sys
from socket import *
import serial,time

def send_to_board(data):
    print("Received message [Player 2]: " + data)
    print("----------------------------")
    # ser.write(data)

def main():
    host = "10.142.1.103" # set to IP address of target computer
    port = 12000
    buf = 1024
    addr = (host, port)
    UDPSock = socket(AF_INET, SOCK_DGRAM)
    # ser = serial.Serial('/dev/ttyACM0', 9600)
    while True:
        looking = True
        while (looking == True): #getting a move to send to other computer, either from android or physical
            # print("currently looking")
            tiebreaker = open("log.txt", "r")
            move = tiebreaker.readline()
            move = move[:len(move)-1]
            # print(len(move))
            if ((len(move) == 7) or (len(move) == 8)): #got a correct move, ready to send to computer, for some reason 6 is 8
                UDPSock.sendto(move, addr)
                open('log.txt', 'w').close() #completely empty the file
                looking = False
                print("got it")
                print(move)
                print("------------------------")
            else:
                print(len(move))
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