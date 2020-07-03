//
//  midi_in.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 24/02/17.
//
//

#include <stdio.h>

#include "RtMidi.h"

#include "ceammc.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

class PdMidiOut : public BaseObject {
    const size_t in_count_;
    const size_t out_count_;

    RtMidiOut* midiout;

public:
    PdMidiOut(const PdArgs& a)
        : BaseObject(a)
        , in_count_(0)
        , out_count_(1)
    {

        try {
            this->midiout = new RtMidiOut();
        } catch (RtMidiError& error) {
            error.printMessage();
            post("MIDI Out error");
        }

        if (this->midiout) {
            int nPorts = midiout->getPortCount();

            post("MIDI Out ports (%i) :", nPorts);

            for (unsigned int i = 0; i < nPorts; i++) {
                // try/catch?
                post(midiout->getPortName(i).c_str());
            }

            if (nPorts) {
                midiout->openPort(0);
            }
        }
    }

    void onList(const AtomList& l)
    {
        std::vector<unsigned char> msg;
        for (int i = 0; i < l.size(); i++) {
            msg.push_back(int(l.at(i).asFloat()));
        }

        midiout->sendMessage(&msg);
    }

    void onFloat(float f)
    {
        int i = int(f);

        int nPorts = midiout->getPortCount();
        if (i < nPorts) {
            if (midiout->isPortOpen())
                midiout->closePort();
            midiout->openPort(i);
        }
    }

    void onSymbol(t_symbol* s)
    {
        int p = searchPort(s);
        if (p) {
            if (midiout->isPortOpen())
                midiout->closePort();
            midiout->openPort(p);
        }
    }

private:
    int searchPort(t_symbol* s)
    {
        if (!s)
            return -1;

        if (this->midiout) {
            int nPorts = midiout->getPortCount();
            for (unsigned int i = 0; i < nPorts; i++) {
                // try/catch?
                if (midiout->getPortName(i) == s->s_name) {
                    return i;
                }
            }
        }

        return -1;
    };
};

extern "C" void setup_midi0x2eout()
{
    ObjectFactory<PdMidiOut> obj("midi.out");
}
