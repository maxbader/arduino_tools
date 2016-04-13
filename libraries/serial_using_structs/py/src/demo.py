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
    sync_request = False;
    while loop:
        com.receive()
        if( com.rx ) :
            print "rx " + str(com)
            while (len(com.data) > 0):
                type_msg = com.pop_type()
                
                if(type_msg == Pose.TYPE) :
                    pose = Pose()
                    msg = com.pop_msg(Pose.struct.size)
                    pose.unpack(msg)
                    print " " + str(pose)
                    #pose.y = pose.y + 1
                    #com.send(pose.pack())  
                elif(type_msg == Text.TYPE) :  
                    text = Text()
                    msg = com.pop_msg(text.struct.size)
                    text.unpack(msg)
                    print " " + str(text)
                elif(type_msg == ComMessage.TYPE_TIME_REQUEST) :  
                    print ' sync request' 
                    sync_request = True
                elif(type_msg == TYPE_NA) :  
                    print ' problem in message'
                elif(type_msg == TYPE_EMPTY) :  
                    print ' empty'
                else :  
                    print ' unkown type: {:d} ',format(type)
        if (sync_request):
            com.push_sync()
            com.send()
            print "tx " + str(com)
        
        time.sleep(0.001)           
    print "exit"