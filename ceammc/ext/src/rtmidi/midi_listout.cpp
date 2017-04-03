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

class PdMidiListOut : public BaseObject {
    const size_t in_count_;
    const size_t out_count_;
    RtMidiOut* midiout;
    

public:
    PdMidiListOut(const PdArgs& a)
        : BaseObject(a)
        , in_count_(0)
        , out_count_(1)
    {

        createOutlet();
        
        try {
            this->midiout = new RtMidiOut();
        } catch (RtMidiError& error) {
            error.printMessage();
            post("MIDI Out error");
        }

        
    }
    
    void onBang()
    {
        AtomList list;
        
        if (this->midiout) {
            int nPorts = midiout->getPortCount();
            
            for (unsigned int i = 0; i < nPorts; i++) {
                // try/catch?
                list.append(Atom(gensym(midiout->getPortName(i).c_str())));
            }
        }
        
        listTo(0, list);
    }
};

extern "C" void setup_midi0x2elistout()
{
    ObjectFactory<PdMidiListOut> obj("midi.listout");
}