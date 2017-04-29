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
    AtomProperty* port_name_;

public:
    PdMidiIn(const PdArgs& a)
        : BaseObject(a)
        , port_name_(0)
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

        port_name_ = new AtomProperty("@port", Atom(0.f));
        createProperty(port_name_);
        parseArguments();

        if (midiin_) {
            if (port_name_->value().isFloat())
                onFloat(port_name_->value().asFloat(0));

            if (port_name_->value().isSymbol())
                onSymbol(port_name_->value().asSymbol());
        }
    }

    void onFloat(float f)
    {
        int p = int(f);
        unsigned int nPorts = midiin_->getPortCount();
        if (p >= 0 && p < nPorts)
            openPort(p);
        else
            OBJ_ERR << "can't open MIDI port: " << p;
    }

    void onSymbol(t_symbol* s)
    {
        int p = searchPort(s);
        if (p >= 0)
            openPort(p);
        else
            OBJ_ERR << "can't open MIDI port: " << p;
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
        OBJ_DBG << "open device: [" << n << "] " << midiin_->getPortName(n);
    }

    int searchPort(t_symbol* s)
    {
        if (!s)
            return -1;

        if (midiin_) {
            unsigned int nPorts = midiin_->getPortCount();
            for (unsigned int idx = 0; idx < nPorts; idx++) {
                if (midiin_->getPortName(idx) == s->s_name) {
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
