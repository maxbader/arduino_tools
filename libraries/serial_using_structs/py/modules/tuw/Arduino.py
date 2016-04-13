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
        
    def __str__(self):
        return self.time().strftime('%Y-%m-%d %H:%M:%S.%f')

    
## header class to handle message for serial communication
class ComHeader:
    
    ## unique type id to identify received objects
    struct_header = struct.Struct("=HIii")
    
    ## constructor
    def __init__(self, size = 0, seq = 0, sec = 0, nsec = 0):
        ## set on true on successful received messages
        self.rx = False
        ## maximum object size to receive
        self.max_data_size = 0xFF
        ## total size of message
        self.size = np.uint16(size)
        ## unique message id
        self.seq = np.uint32(seq)
        ## timestamp 
        self.stamp = Time(sec, nsec)
        ## serialized data
        self.data = []

    ## de-serialized a received string into the class variables
    def unpack(self, msg):
        self.size, self.seq, self.stamp.sec, self.stamp.nsec, = self.struct_header.unpack(msg)
        
    ## serializes the class variables into a buffer 
    # @return serialized buffer
    def pack(self):
        msg = self.struct_header.pack(len(self.data), self.seq, self.stamp.sec, self.stamp.nsec)
        return msg
        
    def set_sync(self):
        self.size = 0
        self.stamp.set_to_now();
      
    def __str__(self):
        return 'seq={:04d}, size={:d}:, stamp={:s}'.format(self.seq, self.size, str(self.stamp) )
     
## header class send messages via serial link
class ComMessage(ComHeader):
    
    ## unique type id to identify received objects
    TYPE_EMPTY   = 0
    TYPE_NA      = -1
    TYPE_TIME_REQUEST = 10
    TYPE_TIME = 11
    struct_type = struct.Struct("=H")
    
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
        
    ## opens serial link
    def pop_type(self):
        type = -1
        if  (len(self.data) >= self.struct_type.size):
            type, = self.struct_type.unpack(self.data[0:self.struct_type.size] )
            self.data = self.data[self.struct_type.size:]
        elif (len(self.data) == 0):
            type = 0
        return type
    
    def pop_msg(self, size_msg):
        msg = []
        if  (len(self.data) >= size_msg):
            msg = self.data[0:size_msg]
            self.data = self.data[size_msg:]
        return msg
        
    def push_sync(self):
        self.set_sync()
    
    def clear(self):
        self.size = 0
        self.data = [self.data, TYPE_TIME]
        
    ## receives message
    # @see check the rx variable to see if the receiving was successful 
    def receive(self):            
        """ read serial interface and opens/reconnects if needed """
        try:
            self.rx = False
            if(self.com.isOpen() == False):
                self.open()
            msg = self.com.read(self.struct_header.size)
            if(len(msg) != self.struct_header.size) :
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
            
            