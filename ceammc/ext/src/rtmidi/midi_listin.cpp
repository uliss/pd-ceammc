//
//  midi_in.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 24/02/17.
//
//

#include <stdio.h>

#include "RtMidi.h"

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

class PdMidiListIn : public BaseObject {
    const size_t in_count_;
    const size_t out_count_;
    RtMidiIn* midiin;

public:
    PdMidiListIn(const PdArgs& a)
        : BaseObject(a)
        , in_count_(0)
        , out_count_(1)

    {
        createOutlet();
        midiInOutlets.push_back(this->outletAt(0));

        try {
            this->midiin = new RtMidiIn();
        } catch (RtMidiError& error) {
            error.printMessage();
            post("MIDI In error");
        }
    }

    void onBang()
    {
        AtomList list;

        if (this->midiin) {
            int nPorts = midiin->getPortCount();

            for (unsigned int i = 0; i < nPorts; i++) {
                // try/catch?
                list.append(Atom(gensym(midiin->getPortName(i).c_str())));
            }
        }

        listTo(0, list);
    }
};

extern "C" void setup_midi0x2elistin()
{
    ObjectFactory<PdMidiListIn> obj("midi.listin");
}
