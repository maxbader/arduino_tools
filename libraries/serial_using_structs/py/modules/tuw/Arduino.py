## @package Arduino
#  module to simplify the serial communcation with an arduino using structs


import serial
import time
import struct
import numpy as np
import datetime as dt
    
## class to handle time stamps in a ROS fashion (www.ros.org)
class Time:
    ## constructor
    def __init__(self, sec = 0, nsec = 0):
        ## timestamp secound
        self.sec = np.int32(sec)
        ## timestamp nanosecound
        self.nsec = np.int32(nsec)
    
    ## sets the timestamp to now
    def set_to_now(self):
        t = time.time();
        self.sec = int(t)
        self.nsec = int( (t - self.sec) * 1000 * 1000 * 1000)
        
    ## returns the time stamp a datetime object
    def time(self):
        t = dt.datetime.fromtimestamp(self.sec)
        t = t + dt.timedelta( microseconds = self.nsec/1000)
        return t
        

    
## header class to handle message for serial communication
class ComHeader:
    
    ## unique type id to identify received objects
    TYPE_NA   = 0
    TYPE_SYNC = 10
    TYPE_TIME = 11
    struct = struct.Struct("HHIii")
    
    ## constructor
    def __init__(self, size = 0, type = 0, seq = 0, sec = 0, nsec = 0):
        ## set on true on successful received messages
        self.rx = False
        ## maximum object size to receive
        self.max_data_size = 0xFF
        ## size of object to receive without header
        self.size = np.uint16(size)
        ## type of object to receive
        self.type = np.uint16(type)
        ## unique message id
        self.seq = np.uint32(seq)
        ## timestamp 
        self.stamp = Time(sec, nsec)
        ## serialized data
        self.data = []

    ## de-serialized a received string into the class variables
    def unpack(self, msg):
        self.size, self.type, self.seq, self.stamp.sec, self.stamp.nsec, = self.struct.unpack(msg)
        
    ## serializes the class variables into a buffer 
    # @return serialized buffer
    def pack(self):
        msg = self.struct.pack(len(self.data), self.type, self.seq, self.stamp.sec, self.stamp.nsec)
        return msg
        
    def size_header(self):
        return self.struct.size
    
    def set_sync(self):
        self.size = 0
        self.type = self.TYPE_TIME
        self.stamp.set_to_now();
      
## header class send messages via serial link
class ComMessage(ComHeader):
    
    ## constructor
    def __init__(self, port , baudrate = 115200, timeout = 1.0, xonxoff = False):
        ComHeader.__init__(self)
        self.com = serial.Serial()
        self.com.port = port
        self.com.baudrate = baudrate
        self.com.timeout = timeout
        self.com.xonxoff = xonxoff
        self.quit = False
        
        
    ## closes serial link
    def close(self):     
        try:
            self.com.close()
        except serial.serialutil.SerialException:
            print "Could close port: " +  self.com.port
        
    ## opens serial link
    def open(self):   
        try:
            self.com.open()
            return True
        except serial.serialutil.SerialException:
            print "Could not open port: " +  self.com.port
            self.close()
            return False
        
    ## receives message
    # @see check the rx variable to see if the receiving was successful 
    def receive(self):            
        """ read serial interface and opens/reconnects if needed """
        try:
            self.rx = False
            if(self.com.isOpen() == False):
                self.open()
            msg = self.com.read(self.size_header())
            if(len(msg) != self.size_header()) :
                print "incorrect header received"
                self.com.flushInput()
                return self.rx
            self.unpack(msg)
            if(self.size >= self.max_data_size):
                print "incorrect header received, data size = " + str(self.size)
                self.com.flushInput()
                return self.rx
            self.data = self.com.read(self.size)
            if(len(self.data) != self.size) :
                print "incorrect data received"
                self.com.flushInput()
                return self.rx
            self.rx = True
        except struct.error as e:
            print "struct.error"        
            print e        
            print "serial.SerialTimeoutException = " + e
        except serial.SerialTimeoutException as e:
            print "serial.SerialTimeoutException = " + e
        except serial.serialutil.SerialException as e:
            print "serial.serialutil.SerialException = " + e
        return self.rx
    
    ## sends message
    # @param msg serialized object
    def send(self, msg=''):
        """ read serial interface and opens/reconnects if needed """
        try:
            self.com.write(self.pack())
            if(len(msg) > 0):
                self.com.write(msg)
        except serial.SerialTimeoutException as e:
            print "serial.SerialTimeoutException = " + e
        except serial.serialutil.SerialException as e:
            print "serial.serialutil.SerialException = " + e
            
            