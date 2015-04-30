#Player 2
import sys
from socket import *

def receive_data(data):
    print("Received message [Player 1]: " + data)
    fh = open("log.txt","a")
    fh.write(data + "\n")
    fh.close()

def android_connection():
    soc = socket()         # Create a socket object
    print("OPEN")
    host = "" # Get local machine name
    port = 13000                # Reserve a port for your service.

    soc.bind((host, port))       # Bind to the port
    soc.listen(5)                 # Now wait for client connection.
    while True:
        conn, addr = soc.accept()     # Establish connection with client.
        print ("Got connection from", addr)
        msg = conn.recv(1024)
        return msg
        # send_msg = raw_input("Enter your move: ")
        # conn.sendto(send_msg, addr)

def main():
    # #android -> computer
    # soc = socket.socket()         # Create a socket object
    # host = "" # Get local machine name
    # port = 13000       # Reserve a port for your service.
    # soc.bind((host, port))       # Bind to the port
    # soc.listen(5)                 # Now wait for client connection.


    #computer -> computer
    host = ""
    port = 12000
    buf = 1024
    addr = (host, port)
    UDPSock = socket(AF_INET, SOCK_DGRAM)
    UDPSock.bind(addr)
    while True:
        print("Waiting for Player 1's Move from Computer")
        (rcv_msg, addr) = UDPSock.recvfrom(buf)
        receive_data(rcv_msg)

        tiebreaker = open("log.txt", "r")
        move = tiebreaker.readline()
        print (move[:len(move)-1])
        open('log.txt', 'w').close()

        send_msg = raw_input("Enter your move: ")
        UDPSock.sendto(send_msg, addr)
    UDPSock.close()
    sys.exit(0)

if __name__ == "__main__":
    main()