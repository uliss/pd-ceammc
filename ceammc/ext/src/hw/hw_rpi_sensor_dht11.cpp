#include "hw_rpi_sensor_dht11.h"
#include "args/argcheck.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"

HwRpiSensorDht11::HwRpiSensorDht11(const PdArgs& args)
    : RustDispatchedObject<BaseObject>(args)
    , dht_(nullptr, &ceammc_hw_gpio_dht11_free)
    , pin_(nullptr)
{
    pin_ = addGpioPinProperty("@pin");
    pin_->setArgIndex(0);

    connect_ = new BoolProperty("@connect", false);
    connect_->setSuccessFn([this](Property*) {
        gpio_connect(connect_->value());
    });
    addProperty(connect_);

    createOutlet();
}

void HwRpiSensorDht11::onBang()
{
    if (!check_connected(true))
        return;

    ceammc_hw_gpio_dht11_measure(dht_.get());
}

void HwRpiSensorDht11::m_poll(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker args("STATE:b");
    if (!args.check(lv, this))
        return args.usage(this, s);

    if (!check_connected(true))
        return;

    ceammc_hw_gpio_dht11_poll(dht_.get(), lv.boolAt(0, false));
}

bool HwRpiSensorDht11::check_connected(bool print_err)
{
    if (!dht_ && print_err) {
        OBJ_ERR << "device is not connected";
    }

    return dht_.get();
}

bool HwRpiSensorDht11::notify(int /*code*/)
{
    return ceammc_hw_gpio_dht11_process(dht_.get());
}

void HwRpiSensorDht11::gpio_connect(bool state)
{
    if (state) {
        if (pin_->isNone()) {
            OBJ_ERR << "GPIO pin is not set";
            return;
        }

        dht_.reset(
            ceammc_hw_gpio_dht11_new(
                pin_->value(),
                on_notify(), //
                on_message(),
                { this, [](void* user, double temp, double hum) {
                     auto obj = static_cast<HwRpiSensorDht11*>(user);
                     if (!obj)
                         return;

                     AtomArray<2> data;
                     data[0] = temp;
                     data[1] = hum;
                     obj->listTo(0, data.view());
                 } }) //
        );

        if (!dht_)
            OBJ_ERR << "can't connect to device";
    } else {
        dht_.reset();
    }
}

void setup_hw_rpi_sensor_dht11()
{
    ObjectFactory<HwRpiSensorDht11> obj("hw.rpi.sensor.dht11");
    obj.addMethod("poll", &HwRpiSensorDht11::m_poll);
}
