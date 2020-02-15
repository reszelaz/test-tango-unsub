#include <tango.h>
#include <unistd.h>

using namespace Tango;

class EventCallBack: public Tango::CallBack {
    public: 
    void push_event(Tango::EventData* event) {
        if (event->err) {
            Tango::Except::print_error_stack(event->errors);
        } else {
            std::cout << "event: " << event->attr_name << " " << event->event << std::endl; 
        }

    };
};

class CB: public Tango::CallBack {
    public: 
    CB(DeviceProxy* dev, int id):
        dev(dev), id(id) 
    {};

    void push_event(Tango::EventData* event) {
        if (event->err) {
            Tango::Except::print_error_stack(event->errors);
        } else {
            dev->unsubscribe_event(id);
            std::cout << "unsubscribed " << id << std::endl;
        }

    };
    int id;
    Tango::DeviceProxy * dev;
};

int main(unsigned int argc, char **argv)
{

    Tango::DeviceProxy *dev1 = new Tango::DeviceProxy("test/device1/1");
    Tango::DeviceProxy *dev2 = new Tango::DeviceProxy("test/device2/1");

    sleep(1);
    int id = dev1->subscribe_event(
        "attr1", Tango::EventType::ATTR_CONF_EVENT, new EventCallBack()
    );
    sleep(1);

    dev2->subscribe_event(
        "attr1", Tango::EventType::CHANGE_EVENT, new EventCallBack()
    );

    CB * cb = new CB(dev1, id);
    dev2->subscribe_event(
        "attr2", Tango::EventType::CHANGE_EVENT, cb
    );

    sleep(30);
};
