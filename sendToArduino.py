from time import sleep
import serial
ser = serial.Serial('/dev/ttyACM0', 9600) # Establish the connection on a specific port
while True:
    send_msg = raw_input("Enter your move: ")
    ser.write(send_msg)
    print ("arduino output below:")
    # Replace data with the code block below
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
            else:
                print "wrong length of data received"
                print data

