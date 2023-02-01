#include "hw_arduino.h"
#include "../base/function.h"
#include "arduino/arduino.h"
#include "ceammc_factory.h"

#include <cmath>
#include <cstdint>
#include <sstream>

using namespace ceammc::hw;

static bool is_byte(t_float v)
{
    return v >= 0 && v <= 255;
}

static void read_tick(ArduinoExternal* a)
{
    a->tick();
}

ArduinoExternal::ArduinoExternal(const PdArgs& args)
    : BaseObject(args)
    , port_(0)
    , on_connect_(0)
    , serial_(0)
    , vid_(0)
    , pid_(0)
    , baud_rate_(0)
    , reconnect_(0)
    , read_clock_(0)
    , ready_(false)
{
    createOutlet();

    createCbBoolProperty("@connected", [this]() -> bool { return arduino_ && arduino_->isConnected(); });

    port_ = new SymbolProperty("@port", &s_);
    port_->setArgIndex(0);
    port_->setInitOnly();
    addProperty(port_);

    on_connect_ = new SymbolProperty("@on_connect", &s_);
    addProperty(on_connect_);

    baud_rate_ = new IntEnumProperty("@rate", {
                                                  57600,
                                                  110,
                                                  300,
                                                  600,
                                                  1200,
                                                  2400,
                                                  4800,
                                                  9600,
                                                  14400,
                                                  19200,
                                                  38400,
                                                  56000,
                                                  115200,
                                                  128000,
                                                  256000,
                                              });
    baud_rate_->setArgIndex(1);
    addProperty(baud_rate_);

    serial_ = new SymbolProperty("@serial", &s_);
    addProperty(serial_);

    vid_ = new IntProperty("@vendor_id", 0);
    addProperty(vid_);

    pid_ = new IntProperty("@product_id", 0);
    addProperty(pid_);

    reconnect_ = new FlagProperty("@reconnect");
    addProperty(reconnect_);

    createCbListProperty("@devices", [this]() -> AtomList {
        AtomList res;

        if (arduino_) {
            for (auto& s : arduino_->allDevices())
                res.append(gensym(s.c_str()));
        }

        return res;
    });
}

ArduinoExternal::~ArduinoExternal()
{
    clock_free(read_clock_);
    processMessages();
}

void ArduinoExternal::onFloat(t_float f)
{
    if (!is_byte(f)) {
        OBJ_ERR << "invalid input value: " << f << "; integers in byte range [0-255] expected";
        return;
    }

    uint8_t byte = static_cast<uint8_t>(roundf(f));

    if (!arduino_->write(byte))
        OBJ_ERR << "can't send data: device is not connected";
}

void ArduinoExternal::onList(const AtomListView& lv)
{
    std::string data;
    data.reserve(lv.size());

    for (size_t i = 0; i < lv.size(); i++) {
        const Atom& a = lv[i];

        t_float f = 0;
        if (!a.getFloat(&f)) {
            OBJ_ERR << "non float data in list: " << a;
            return;
        }

        if (!is_byte(f)) {
            OBJ_ERR << "invalid byte value in list: " << f;
            return;
        }

        data.push_back(f);
    }

    if (!arduino_->write(data))
        OBJ_ERR << "can't send data: device is not connected";
}

void ArduinoExternal::initDone()
{
    BaseObject::initDone();

    arduino_.reset(new Arduino(port_->value()->s_name, baud_rate_->value()));
    arduino_->setReconnect(reconnect_->value());
    arduino_->setVendorId(vid_->value());
    arduino_->setProductId(pid_->value());
    arduino_->setUsbSerial(serial_->value()->s_name);

    read_clock_ = clock_new(this, (t_method)read_tick);
    clock_set(read_clock_, 3000);

    arduino_->start();
}

void ArduinoExternal::tick()
{
    clock_delay(read_clock_, 10);

    // do it BEFORE connection check
    processMessages();

    if (!arduino_->isConnected())
        return;

    // do once
    if (!ready_)
        ready_ = true;

    if (arduino_->hasInputData()) {
        ArduinoData data;
        arduino_->read(data);

        for (size_t i = 0; i < data.size(); i++)
            floatTo(0, data[i]);
    }
}

void ArduinoExternal::m_connect(t_symbol*, const AtomListView& args)
{
    if (args.toT<bool>(true))
        arduino_->start();
    else
        arduino_->stop();
}

void ArduinoExternal::m_disconnect(t_symbol*, const AtomListView&)
{
    arduino_->stop();
}

void ArduinoExternal::onConnect()
{
    port_->setValue(gensym(arduino_->port().c_str()));
    vid_->setValue(arduino_->vendorId());
    pid_->setValue(arduino_->productId());
    serial_->setValue(gensym(arduino_->usbSerial().c_str()));

    // if @on_connect is not empty
    if (on_connect_->value() != &s_) {
        if (!Function::exists(on_connect_->value())) {
            OBJ_ERR << "@on_connect function not exists: " << on_connect_->get();
        } else {
            Function* fn = Function::function(on_connect_->value());
            fn->onBang();
        }
    }
}

void ArduinoExternal::processMessages()
{
    if (arduino_->hasMessages()) {
        Messages lst = arduino_->popMessages();
        Messages::iterator it = lst.begin();

        for (; it != lst.end(); ++it) {
            if (it->first == MSG_ERROR)
                OBJ_ERR << it->second;
            else if (it->first == MSG_DEBUG)
                OBJ_DBG << it->second;
            else if (it->first == MSG_CONNECT)
                onConnect();
        }
    }
}

void hw_setup_arduino()
{
    ObjectFactory<ArduinoExternal> obj("hw.arduino");
    obj.addMethod("connect", &ArduinoExternal::m_connect);
    obj.addMethod("disconnect", &ArduinoExternal::m_disconnect);
}
