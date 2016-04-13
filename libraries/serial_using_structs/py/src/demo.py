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
    while loop:
        com.receive()
        if( com.rx ) :
            time_rx = com.stamp.time().strftime('%Y-%m-%d %H:%M:%S.%f')
            if(com.type == Pose.TYPE) :
                pose = Pose()
                pose.unpack(com.data)
                print 'rx {:04d} {:s}: {:s}'.format(com.seq, time_rx, str(pose) )
                pose.y = pose.y + 1
                com.send(pose.pack())  
            elif(com.type == ComMessage.TYPE_SYNC) :  
                print 'rx sync request {:d}'.format(com.seq) 
                com.set_sync()
                com.send()
                time_tx = com.stamp.time().strftime('%Y-%m-%d %H:%M:%S.%f')
                print 'tx {:s} = sec {:d}, nsec {:d}'.format(time_tx, com.stamp.sec, com.stamp.nsec) 
            elif(com.type == Text.TYPE) :  
                text = Text()
                text.unpack(com.data)
                print 'rx {:04d} {:s}: {:s}'.format(com.seq, time_rx, str(text))
            else :  
                print 'rx {:04d} {:s}: unkown type {:d}'.format(com.seq, str(time_rx),  com.type)
                 
        time.sleep(0.001)           
    print "exit"