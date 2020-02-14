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