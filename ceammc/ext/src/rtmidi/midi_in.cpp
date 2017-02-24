//
//  midi_in.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 24/02/17.
//
//

#include <stdio.h>


//todo proper build
#define __MACOSX_CORE__
#include "RtMidi.cpp"

#include "ceammc.hpp"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

class PdMidiIn : public BaseObject {
    const size_t in_count_;
    const size_t out_count_;
    RtMidiIn* midiin;
    RtMidiIn* midiout;

public:
    PdMidiIn(const PdArgs& a)
        : BaseObject(a)
        , in_count_(0)
        , out_count_(1)
    {
        try {
            this->midiin = new RtMidiIn();
        } catch (RtMidiError& error) {
            
            error.printMessage();
            post("MIDI In error");
        }
        
        try {
            this->midiout = new RtMidiIn();
        } catch (RtMidiError& error) {
            
            error.printMessage();
            post("MIDI Out error");
        }
        
        
        
        if (this->midiin)
        {
            int nPorts = midiin->getPortCount();
            
            post("MIDI In ports (%i) :", nPorts);
            
            for ( unsigned int i=0; i<nPorts; i++ )
            {
                // try/catch?
                post(midiin->getPortName(i).c_str());
            }
        }
        
        if (this->midiout)
        {
            int nPorts = midiout->getPortCount();
            
            post("MIDI Out ports (%i) :", nPorts);
            
            for ( unsigned int i=0; i<nPorts; i++ )
            {
                // try/catch?
                post(midiout->getPortName(i).c_str());
            }
        }
        
    }
};

extern "C" void setup_midi0x2ein()
{
    ObjectFactory<PdMidiIn> obj("midi.in");
}