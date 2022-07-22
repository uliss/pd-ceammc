#include "firmata_pin.h"

#include <iomanip>

namespace firmata {

Pin::Pin(PinN n)
    : n_(n)
    , analog_n_(-1)
    , mode_(PROTO_PIN_MODE_INPUT)
    , value_(0)
    , pullup_(false)
    , state_(0)
{
}

PinN Pin::number() const
{
    return n_;
}

void Pin::setNumber(PinN n)
{
    n_ = n;
}

bool Pin::isAnalog() const
{
    return analog_n_ >= 0;
}

int Pin::analogNumber() const
{
    return analog_n_;
}

void Pin::setAnalogNumber(AnalogPinN n)
{
    analog_n_ = n;
}

PinCapabilityList& Pin::capabilities()
{
    return caps_;
}

const PinCapabilityList& Pin::capabilities() const
{
    return caps_;
}

void Pin::addCapability(FirmataPinCapability c)
{
    caps_.push_back(c);
}

int Pin::value() const
{
    return value_;
}

void Pin::setValue(int v)
{
    value_ = v;
}

FirmataPinMode Pin::mode() const
{
    return mode_;
}

bool Pin::setMode(FirmataPinMode m)
{
    if (!isSupportedMode(m))
        return false;

    mode_ = m;
    return true;
}

bool Pin::isSupportedMode(FirmataPinMode m) const
{
    for (const FirmataPinCapability& c : caps_) {
        if (c.mode == m)
            return true;
    }

    return false;
}

int Pin::bitResolution() const
{
    for (const FirmataPinCapability& c : caps_) {
        if (c.mode == mode_)
            return c.resolution;
    }

    return 0;
}

long Pin::state() const
{
    return state_;
}

void Pin::setState(long v)
{
    state_ = v;
}

bool Pin::pullUp() const
{
    return pullup_;
}

void Pin::setPullUp(bool v)
{
    pullup_ = v;
}

std::ostream& operator<<(std::ostream& os, const Pin& p)
{
    os << "pin #" << int(p.number());

    if (p.isAnalog())
        os << " (A" << int(p.analogNumber()) << ")";

    os << ":\n";
    os << "  supported modes:\n";

    for (const FirmataPinCapability& c : p.capabilities()) {
        os << "  - " << std::left << std::setw(8) << firmata_pin_mode_str(c.mode)
           << ": " << (int)c.resolution << "\n";
    }

    return os;
}

} // namespace firmata
