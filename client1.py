import time
import logging

import tango

logging.basicConfig(format="%(threadName)s:%(message)s")

dev1 = tango.DeviceProxy("test/device1/1")
dev2 = tango.DeviceProxy("test/device2/1")

id_ = dev1.subscribe_event("attr1",
                           tango.EventType.ATTR_CONF_EVENT,
                           tango.utils.EventCallback())
time.sleep(1)  # just to not mix prints
dev2.subscribe_event("attr1",
                     tango.EventType.CHANGE_EVENT,
                     tango.utils.EventCallback())
time.sleep(1)  # just to not mix prints


def cb(event):
    global id_
    if event.err:
        print("ERROR:", event.errors[0].reason)
    elif id_ is not None:
        dev1.unsubscribe_event(id_)
        id_ = None

dev2.subscribe_event("attr2", tango.EventType.CHANGE_EVENT, cb)
time.sleep(30)  # wait for API_EventTimeout
