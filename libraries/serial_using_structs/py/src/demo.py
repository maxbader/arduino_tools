## Demo program to demonstrate a serial link between a PC and an Arduino board
#!/usr/bin/env python

import time
from datetime import datetime
import argparse
import struct
import numpy as np
from tuw.Arduino import ComMessage
from tuw.Arduino import Pose

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
            if(com.type == Pose.TYPE) :
                p = Pose()
                p.unpack(com.data)
                t = com.stamp.time().strftime('%Y-%m-%d %H:%M:%S.%f')
                print 'rx {:d} {:s}: {:f}, {:f}, {:f}'.format(com.seq, str(t), p.x,p.y,p.theta)
                p.y = p.y + 1
                com.send(p.pack())  
            if(com.type == ComMessage.TYPE_SYNC) :  
                print 'rx sync request {:d}'.format(com.seq) 
                com.set_sync()
                com.send()
                t = com.stamp.time().strftime('%Y-%m-%d %H:%M:%S.%f')
                print 'tx {:s} = sec {:d}, nsec {:d}'.format(str(t), com.stamp.sec, com.stamp.nsec) 
                 
        time.sleep(0.001)           
    print "exit"