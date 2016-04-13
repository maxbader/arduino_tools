## @package ArduinoMessages
#  defines messages for serial communication


import struct
import copy
    
## class to receive and send via serial link
class Pose:
    
    ## unique type id to identify received objects
    TYPE = 100
    struct = struct.Struct("fff")
    
    
    ## constructor
    def __init__(self, x = 0., y = 0., theta = 0.):
        self.x = x
        self.y = y
        self.theta = theta

    ## de-serialized a received string into the class variables
    def unpack(self, msg):
        self.x, self.y, self.z = self.struct.unpack(msg)
        
    ## serializes the class variables into a buffer 
    # @return serialized buffer
    def pack(self):
        msg = self.struct.pack(self.x, self.y, self.theta)
        return msg
    
    def __str__(self):
        return '{:f}, {:f}, {:f}'.format(self.x, self.y, self.theta)
        
        
           
## class to receive and send via serial link
class Text:
    
    ## unique type id to identify received objects
    TYPE = 200
    struct = struct.Struct("64s")
    
    
    ## constructor
    def __init__(self, text = ''):
        self.text = copy.copy(text)

    ## de-serialized a received string into the class variables
    def unpack(self, msg):
        self.text = copy.copy(msg)
        
    ## serializes the class variables into a buffer 
    # @return serialized buffer
    def pack(self):
        return copy.copy(self.text) 
        