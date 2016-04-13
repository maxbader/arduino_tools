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
        send_sync = False;
        send_pose = False;
        com.receive()
        if( com.rx ) :
            print "rx " + str(com)
            while (com.rx and (len(com.data) > 0)):
                type_msg = com.pop_type()                
                if(type_msg == Pose.TYPE) :
                    pose = com.pop_object(pose)
                    print " " + str(pose)
                    send_pose = True
                elif(type_msg == Text.TYPE) :  
                    text = com.pop_object(Text())
                    print " " + str(text)
                elif(type_msg == ComMessage.TYPE_SYNC_REQUEST) :  
                    print ' sync request' 
                    send_sync = True
                elif(type_msg == ComMessage.TYPE_NA) :  
                    print ' problem in message'
                    com.rx = False
                elif(type_msg == ComMessage.TYPE_EMPTY) :  
                    print ' empty'
                    com.rx = False
                else :  
                    print ' unkown type: {:d} '.format(type_msg)
        com.clear()
        if (send_sync):
            com.push_sync()
        if (send_pose):
            pose.y = pose.y + 10
            #buffer = com.pack_type(pose.TYPE) + pose.pack()
            com.push_object(pose)
        if (send_pose or send_sync):
            com.send()
            print "tx " + str(com)
            if(send_sync): 
                print " sync"
            if(send_pose): 
                print " " + str(pose)
        
        time.sleep(0.01)           
    print "exit"