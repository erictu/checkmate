#Player 2
import sys
from socket import *

def send_to_board(data):
    ser.write(data)

def main():
    host = ""
    port = 12000
    buf = 1024
    addr = (host, port)
    UDPSock = socket(AF_INET, SOCK_DGRAM)
    UDPSock.bind(addr)
    while True:
        print("Waiting for Player 1's Move from Computer")
        (rcv_msg, addr) = UDPSock.recvfrom(buf)
        send_to_board(rcv_msg) #got move from other player, now send to board

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
    UDPSock.close()
    sys.exit(0)

if __name__ == "__main__":
    main()