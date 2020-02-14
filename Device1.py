import tango
from tango.server import Device, attribute


class Device1(Device):

    attr1 = attribute()

    def read_attr1(self):
        return 0.0