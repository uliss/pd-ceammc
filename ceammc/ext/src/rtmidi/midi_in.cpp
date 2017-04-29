//
//  midi_in.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 24/02/17.
//
//
#include "midi_in.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"

#include <string>

PdMidiIn::PdMidiIn(const PdArgs& a)
    : BaseObject(a)
    , device_(0)
    , callback_(midiCallback)
    , port_(0)
{
    createOutlet();

    try {
        midiin_.reset(new RtMidiIn());

        unsigned int nPorts = midiin_.get()->getPortCount();
        OBJ_DBG << "MIDI In ports (" << nPorts << "):";

        for (unsigned int i = 0; i < nPorts; i++) {
            OBJ_DBG << "    " << i << ": " << midiin_->getPortName(i);
        }
    } catch (RtMidiError& error) {
        error.printMessage();
        OBJ_ERR << "MIDI In error: " << error.what();
    }

    createCbProperty("@port", &PdMidiIn::p_getPort, &PdMidiIn::p_setPort);
    createCbProperty("@dev", &PdMidiIn::p_getDevice, &PdMidiIn::p_setDevice);
}

int PdMidiIn::port() const
{
    return int(port_);
}

const char* PdMidiIn::deviceName() const
{
    return device_->s_name;
}

RtMidiIn::RtMidiCallback PdMidiIn::callback()
{
    return callback_;
}

void PdMidiIn::setCallback(RtMidiIn::RtMidiCallback cb)
{
    callback_ = cb;
}

void PdMidiIn::onFloat(float f)
{
    port_ = f;
    connectByPort();
}

void PdMidiIn::onSymbol(t_symbol* s)
{
    device_ = s;
    connectByDevice();
}

AtomList PdMidiIn::p_getPort() const
{
    return Atom(port_);
}

void PdMidiIn::p_setPort(const AtomList& lst)
{
    if (!checkArgs(lst, ARG_FLOAT)) {
        OBJ_ERR << "MIDI port (int) required.";
        return;
    }

    port_ = lst[0].asFloat(0);
    connectByPort();
}

AtomList PdMidiIn::p_getDevice() const
{
    return device_ ? AtomList(device_) : Atom();
}

void PdMidiIn::p_setDevice(const AtomList& lst)
{
    if (!checkArgs(lst, ARG_SYMBOL)) {
        OBJ_ERR << "device name required";
        return;
    }

    device_ = lst[0].asSymbol();
    connectByDevice();
}

void PdMidiIn::connectByPort()
{
    int n = int(port_);
    unsigned int nPorts = midiin_->getPortCount();
    if (n >= 0 && n < nPorts)
        openPort(n);
    else
        OBJ_ERR << "can't open MIDI port: " << n;
}

void PdMidiIn::connectByDevice()
{
    if (!device_)
        return;

    std::string name(device_->s_name);
    int p = searchPort(name);
    if (p < 0) {
        OBJ_ERR << "can't open MIDI device: " << name;
        return;
    }

    port_ = p;
    connectByPort();
}

void PdMidiIn::midiCallback(double, std::vector<unsigned char>* message, void* userData)
{
    AtomList list;
    for (size_t i = 0; i < message->size(); i++)
        list.append(Atom(float(message->at(i))));

    PdMidiIn* obj = reinterpret_cast<PdMidiIn*>(userData);
    obj->listTo(0, list);
}

void PdMidiIn::openPort(unsigned int n)
{
    if (!midiin_)
        return;

    if (midiin_->isPortOpen())
        midiin_->closePort();

    midiin_->openPort(n);
    midiin_->setCallback(callback_, this);
    midiin_->ignoreTypes(false, false, false);
    device_ = gensym(midiin_->getPortName(n).c_str());
    OBJ_DBG << "open device: [" << n << "] " << device_->s_name;
}

int PdMidiIn::searchPort(const std::string& name)
{
    if (!midiin_)
        return -1;

    unsigned int nPorts = midiin_->getPortCount();
    for (unsigned int idx = 0; idx < nPorts; idx++) {
        if (midiin_->getPortName(idx).substr(0, name.size()) == name) {
            return idx;
        }
    }

    return -1;
}

extern "C" void setup_midi0x2ein()
{
    ObjectFactory<PdMidiIn> obj("midi.in");
}
