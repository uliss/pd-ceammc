//
//  midi_in.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 24/02/17.
//
//

#include <boost/scoped_ptr.hpp>

#include "RtMidi.h"

#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

typedef boost::scoped_ptr<RtMidiIn> SharedMidiIn;

class PdMidiIn : public BaseObject {
    SharedMidiIn midiin_;
    float port_;
    t_symbol* device_;

public:
    PdMidiIn(const PdArgs& a)
        : BaseObject(a)
        , port_(0)
        , device_(0)
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
        createCbProperty("@device", &PdMidiIn::p_getDevice, &PdMidiIn::p_setDevice);
        parseArguments();
    }

    void onFloat(float f)
    {
        port_ = f;
        connectByPort();
    }

    void onSymbol(t_symbol* s)
    {
        device_ = s;
        connectByDevice();
    }

    AtomList p_getPort() const
    {
        return Atom(port_);
    }

    void p_setPort(const AtomList& lst)
    {
        if (!checkArgs(lst, ARG_FLOAT)) {
            OBJ_ERR << "MIDI port (int) required.";
            return;
        }

        port_ = lst[0].asFloat(0);
        connectByPort();
    }

    AtomList p_getDevice() const
    {
        return device_ ? AtomList(device_) : Atom();
    }

    void p_setDevice(const AtomList& lst)
    {
        if (!checkArgs(lst, ARG_SYMBOL)) {
            OBJ_ERR << "device name required";
            return;
        }

        device_ = lst[0].asSymbol();
        connectByDevice();
    }

    void connectByPort()
    {
        int n = int(port_);
        unsigned int nPorts = midiin_->getPortCount();
        if (n >= 0 && n < nPorts)
            openPort(n);
        else
            OBJ_ERR << "can't open MIDI port: " << n;
    }

    void connectByDevice()
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

public:
    static void midiCallback(double /*deltatime*/, std::vector<unsigned char>* message, void* userData)
    {
        AtomList list;
        for (size_t i = 0; i < message->size(); i++)
            list.append(Atom(float(message->at(i))));

        PdMidiIn* obj = reinterpret_cast<PdMidiIn*>(userData);
        obj->listTo(0, list);
    }

private:
    void openPort(unsigned int n)
    {
        if (midiin_->isPortOpen())
            midiin_->closePort();

        midiin_->openPort(n);
        midiin_->setCallback(&midiCallback, this);
        midiin_->ignoreTypes(false, false, false);
        device_ = gensym(midiin_->getPortName(n).c_str());
        OBJ_DBG << "open device: [" << n << "] " << device_->s_name;
    }

    int searchPort(const std::string& name)
    {
        if (midiin_) {
            unsigned int nPorts = midiin_->getPortCount();
            for (unsigned int idx = 0; idx < nPorts; idx++) {
                if (midiin_->getPortName(idx).substr(0, name.size()) == name) {
                    return idx;
                }
            }
        }

        return -1;
    }
};

extern "C" void setup_midi0x2ein()
{
    ObjectFactory<PdMidiIn> obj("midi.in");
}
