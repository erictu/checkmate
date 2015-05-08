import socket               # Import socket module

soc = socket.socket()         # Create a socket object
print("OPEN")
host = "" # Get local machine name
port = 13000                # Reserve a port for your service.

soc.bind((host, port))       # Bind to the port
soc.listen(5)                 # Now wait for client connection.
while True:
    conn, addr = soc.accept()     # Establish connection with client.
    print ("Got connection from",addr)
    msg = conn.recv(1024)
    print (msg)
    fh = open("log.txt","a")
    fh.write(msg + "\n")
    fh.close()
    # send_msg = raw_input("Enter your move: ")
    # conn.sendto(send_msg, addr)
