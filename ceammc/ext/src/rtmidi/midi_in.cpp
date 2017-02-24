//
//  midi_in.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 24/02/17.
//
//

#include <stdio.h>

//TODO proper build
#define __MACOSX_CORE__
#include "RtMidi.cpp"

#include "ceammc.hpp"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

std::vector<t_outlet*> midiInOutlets;

void mycallback(double deltatime, std::vector<unsigned char>* message, void* userData)
{
    size_t nBytes = message->size();

    AtomList list;
    for (unsigned int i = 0; i < nBytes; i++)
        list.append(Atom(float(message->at(i))));

    for (std::vector<t_outlet*>::iterator it = midiInOutlets.begin(); it != midiInOutlets.end(); ++it) {
        list.output(*it);
    }
}

class PdMidiIn : public BaseObject {
    const size_t in_count_;
    const size_t out_count_;
    RtMidiIn* midiin;

    AtomProperty portName;

public:
    PdMidiIn(const PdArgs& a)
        : BaseObject(a)
        , in_count_(0)
        , out_count_(1)
        , portName("port", Atom(gensym("")), 0)
    {
        createOutlet();
        midiInOutlets.push_back(this->outletAt(0));

        try {
            this->midiin = new RtMidiIn();
        } catch (RtMidiError& error) {
            error.printMessage();
            post("MIDI In error");
        }

        if (this->midiin) {
            int nPorts = midiin->getPortCount();

            post("MIDI In ports (%i) :", nPorts);

            for (unsigned int i = 0; i < nPorts; i++) {
                // try/catch?
                post(midiin->getPortName(i).c_str());
            }

            if (nPorts) {
                midiin->openPort(0);
                midiin->setCallback(&mycallback);
                midiin->ignoreTypes(false, false, false);
            }
        }

        createProperty(&this->portName);
    }

    void onFloat(float f)
    {
        int i = int(f);
        
        int nPorts = midiin->getPortCount();
        if (i<nPorts)
        {
            if (midiin->isPortOpen())
                midiin->closePort();
            midiin->openPort(i);
        }
            
            
    }

    void onSymbol(t_symbol* s)
    {
        int p = searchPort(s);
        if (p) {
            if (midiin->isPortOpen())
                midiin->closePort();
            midiin->openPort(p);
        }
    }

private:
    int searchPort(t_symbol* s)
    {
        if (!s) return -1;
        
        if (this->midiin) {
            int nPorts = midiin->getPortCount();
            for (unsigned int i = 0; i < nPorts; i++) {
                // try/catch?
                if(midiin->getPortName(i) == s->s_name)
                {return i;}
            }
            
        }

        return -1;
    };
};

extern "C" void setup_midi0x2ein()
{
    ObjectFactory<PdMidiIn> obj("midi.in");
}