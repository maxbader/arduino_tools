# Serial Using Structure's
This library contains a demo with a python program for your PC to show a time sync and message exchange with an Arduino board.
There are to operation modes
- simple_messages: A simple_message has one header, a object type id and the object encoded
```header, id, obj```
- packed_messages: A packed_message has one header, and multiple packed object type id and the objects encoded
```header, id0, obj0, id0, obj0, id0, obj0, ...```
## Hardware
it should work on all Arduinos

##Install
###Linux
Step into your library
### simple messages

##Demo
### simple messages
On the PC run the python program demo_simple_messages:
```
export PYTHONPATH="$PYTHONPATH:py/modules/"
python py/src/demo_simple_messages.py 
```
On the Arduino run the example demo_simple_messages 
### packed messages
On the PC run the python program demo_packed_messages:
```
export PYTHONPATH="$PYTHONPATH:py/modules/"
python py/src/demo_packed_messages.py 
```
On the Arduino run the example demo_packed_messages
