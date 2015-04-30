#Player 1
import sys
from socket import *
import time

def receive_data(data):
    print("Received message [Player 2]: " + data)

def main():
    host = "localhost" # set to IP address of target computer
    port = 12000
    addr = (host, port)
    UDPSock = socket(AF_INET, SOCK_DGRAM)
    buf = 1024
    while True:
        data = raw_input("Enter your move: ")
        UDPSock.sendto(data, addr)
        print("Waiting for Player 2's Move")
        (rcv_msg, addr) = UDPSock.recvfrom(buf)
        receive_data(rcv_msg)
    UDPSock.close()
    sys.exit(0)

if __name__ == "__main__":
    main()
