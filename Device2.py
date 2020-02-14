import tango
from tango.server import Device, attribute, run


class Device2(Device):

    attr1 = attribute()
    attr2 = attribute()

    def init_device(self):
        Device.init_device(self)
        self.set_change_event("attr1", True, False)
        self.set_change_event("attr2", True, False)
        self._attr1 = 0
        self._attr2 = 0

    def read_attr1(self):
        return self._attr1

    def read_attr2(self):
        return self._attr2
