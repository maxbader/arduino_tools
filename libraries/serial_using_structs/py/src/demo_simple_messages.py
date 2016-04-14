## Demo program to demonstrate a serial link between a PC and an Arduino board
#!/usr/bin/env python

import time
from datetime import datetime
import argparse
import struct
import numpy as np
from tuw.Arduino import ComMessage
from tuw.ArduinoMessages import Pose
from tuw.ArduinoMessages import Text

parser = argparse.ArgumentParser()
parser.add_argument('-p', dest="port", type=str, default="/dev/ttyACM0", help="port such as /dev/ttyACM0")
parser.add_argument('-b', dest="baudrate", type=int, default="115200", help="baudrate such as 115200")
parser.add_argument('-o', dest="timeout", type=float, default="1", help="timeout such as 1.0")
args = parser.parse_args()
print 'port: {:s}, baudrate {:d}, timeout {:f} '.format(args.port, args.baudrate, args.timeout)

   
if __name__ == '__main__':
    com = ComMessage(args.port, args.baudrate, args.timeout)
    loop = True    
    pose = Pose()
    while loop:
        com.receive()
        if( com.rx ) :
            type_msg = com.get_type()                
            if(type_msg == Pose.TYPE) :
                pose = com.get_object(pose)
                print "rx " + str(com) + ": " + str(pose)
                pose.y = pose.y + 10
                com.stamp.set_to_now()
                com.set_object(pose)
                com.send()
                print "tx " + str(com) + ": " + str(pose)
            elif(type_msg == Text.TYPE) :  
                text = com.get_object(Text())
                print "rx " + str(com) + ": " + str(text)
            elif(type_msg == ComMessage.TYPE_SYNC_REQUEST) :  
                print "rx " + str(com) + ": sync request"
                com.clear()
                com.push_sync()
                com.send()
            elif(type_msg == ComMessage.TYPE_ERROR) :  
                print "rx " + str(com) + ": error in object type"
                com.rx = False
            elif(type_msg == ComMessage.TYPE_EMPTY) :  
                print "rx " + str(com) + ": empty"
                com.rx = False
            else :  
                print ' unkown type: {:d} '.format(type_msg)
        time.sleep(0.01)           
    print "exit"