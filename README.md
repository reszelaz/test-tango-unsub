# Test to reproduce PyTango#292

The architecture is as following. We have one DS called `DeviceServer` which
exports in total two devices, each of a different class. These two classes are:
* `Device1` with one attribute `attr1`
* `Device2` with two attributes `attr1` and `attr2`

What we will do:
1. Subscribe to `attr1` configuration events of the `Device1` device.
2. Subscribe to `attr1` change events of the `Device2` device.
3. Subscribe to `attr2` change events of the `Device2` device
   with a callback which will unsubscribe from the previously subscribed
   `attr1` configuration events of the `Device1` device.  

In order to reproduce the problem:
1. Register in Tango Database one DeviceServer DS with instance name `test`
   with 2 devices of `Device1` and `Device2` classes, with the following names:
    `test/device1/1`, `test/device2/1` respectively.
    ```console
    tango_admin --add-server DeviceServer/test Device1 test/device1/1    
    tango_admin --add-server DeviceServer/test Device2 test/device2/1
    ```
2. Start DeviceServer: `python3 DeviceServer.py test`
3. Start client1: `python3 client1.py`

The output should look similar to:

```
/usr/bin/python3.5 /home/zreszela/workspace/test-tango-unsub/client1.py
MainThread:__DeviceProxy.__subscribe_event_attrib(attr1): before self.__subscribe_event
2020-02-14 23:48:04.251696 TEST/DEVICE1/1 ATTR1 ATTR_CONF label='attr1'; unit=''
MainThread:__DeviceProxy.__subscribe_event_attrib(attr1): after self.__subscribe_event; id = 1
MainThread:__DeviceProxy.__subscribe_event(1): before event_map_lock acquire
MainThread:__DeviceProxy.__subscribe_event(1): after event_map_lock acquire
MainThread:__DeviceProxy.__subscribe_event(1): after event_map_lock release
MainThread:__DeviceProxy.__subscribe_event_attrib(attr1): before self.__subscribe_event
2020-02-14 23:48:05.258605 TEST/DEVICE2/1 ATTR1 CHANGE [ATTR_VALID] 0.0
MainThread:__DeviceProxy.__subscribe_event_attrib(attr1): after self.__subscribe_event; id = 2
MainThread:__DeviceProxy.__subscribe_event(2): before event_map_lock acquire
MainThread:__DeviceProxy.__subscribe_event(2): after event_map_lock acquire
MainThread:__DeviceProxy.__subscribe_event(2): after event_map_lock release
MainThread:__DeviceProxy.__subscribe_event_attrib(attr2): before self.__subscribe_event
MainThread:__DeviceProxy__unsubscribe_event(1): before event_map_lock acquire
MainThread:__DeviceProxy__unsubscribe_event(1): after event_map_lock acquire
MainThread:__DeviceProxy__unsubscribe_event(1): after event_map_lock release
MainThread:__DeviceProxy__unsubscribe_event(1): before self.__unsubscribe_event
MainThread:__DeviceProxy__unsubscribe_event(1): after self.__unsubscribe_event
MainThread:__DeviceProxy.__subscribe_event_attrib(attr2): after self.__subscribe_event; id = 3
MainThread:__DeviceProxy.__subscribe_event(3): before event_map_lock acquire
MainThread:__DeviceProxy.__subscribe_event(3): after event_map_lock acquire
MainThread:__DeviceProxy.__subscribe_event(3): after event_map_lock release
2020-02-14 23:48:16.236785 TEST/DEVICE2/1 ATTR1 CHANGE [API_EventTimeout] Event channel is not responding anymore, maybe the server or event system is down
2020-02-14 23:48:16.237584 TEST/DEVICE2/1 ATTR1 IDL5_CHANGE [ATTR_VALID] 0.0
ERROR: API_EventTimeout
2020-02-14 23:48:26.242668 TEST/DEVICE2/1 ATTR1 CHANGE [API_EventTimeout] Event channel is not responding anymore, maybe the server or event system is down
2020-02-14 23:48:26.243339 TEST/DEVICE2/1 ATTR1 IDL5_CHANGE [ATTR_VALID] 0.0
ERROR: API_EventTimeout
2020-02-14 23:48:36.248675 TEST/DEVICE2/1 ATTR1 CHANGE [API_EventTimeout] Event channel is not responding anymore, maybe the server or event system is down
2020-02-14 23:48:36.249320 TEST/DEVICE2/1 ATTR1 IDL5_CHANGE [ATTR_VALID] 0.0
ERROR: API_EventTimeout
```

Alternatively build and run the C++ client:
1. `g++ client1.cpp -o client1 -I/usr/include/tango -ltango -lzmq -lomniORB4 -lomnithread -lCOS4 -lomniDynamic4 -g`
2. `./client1`