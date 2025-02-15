#include "hw_sensors_dht11.h"
#include "args/argcheck.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

HwSensorDht11::HwSensorDht11(const PdArgs& args)
    : HwDht11Base(args)
    , pin_(nullptr)
{
    pin_ = new IntProperty("@pin", 4, PropValueAccess::INITONLY);
    pin_->checkClosedRange(0, 255);
    pin_->setArgIndex(0);
    addProperty(pin_);

    createOutlet();
}

HwSensorDht11::~HwSensorDht11()
{
    ceammc_hw_gpio_dht11_free(dht_);
}

void HwSensorDht11::initDone()
{
    dht_ = ceammc_hw_gpio_dht11_new(pin_->value(),
        { size_t(subscriberId()), [](size_t id) { Dispatcher::instance().send(NotifyMessage { id, 0 }); } }, //
        { this, [](void* data, const char* msg) {
             auto obj = static_cast<HwSensorDht11*>(data);
             if (!obj)
                 return;

             Error(obj) << msg;
         } },
        { this, [](void* user, double temp, double hum) {
             auto obj = static_cast<HwSensorDht11*>(user);
             if (!obj)
                 return;

             AtomArray<2> data;
             data[0] = temp;
             data[1] = hum;
             obj->listTo(0, data.view());
         } });
}

void HwSensorDht11::onBang()
{
    ceammc_hw_gpio_dht11_measure(dht_);
}

void HwSensorDht11::m_poll(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker args("STATE:b");
    if (!args.check(lv, this))
        return args.usage(this, s);

    ceammc_hw_gpio_dht11_poll(dht_, lv.boolAt(0, false));
}

bool HwSensorDht11::notify(int /*code*/)
{
    return ceammc_hw_gpio_dht11_process(dht_);
}

void setup_hw_sensor_dht11()
{
    ObjectFactory<HwSensorDht11> obj("hw.sensor.dht11");
    obj.addMethod("poll", &HwSensorDht11::m_poll);
}
