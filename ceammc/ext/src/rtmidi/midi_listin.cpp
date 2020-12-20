//
//  midi_in.cpp
//  pd_ext
//
//  Created by Alex Nadzharov on 24/02/17.
//
//

#include <boost/scoped_ptr.hpp>
#include <stdio.h>

#include "RtMidi.h"

#include "ceammc.h"
#include "ceammc_factory.h"
#include "ceammc_log.h"
#include "ceammc_object.h"

using namespace ceammc;

typedef boost::scoped_ptr<RtMidiIn> SharedMidiIn;

class PdMidiListIn : public BaseObject {
    SharedMidiIn midiin_;

public:
    PdMidiListIn(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();

        try {
            midiin_.reset(new RtMidiIn());
        } catch (RtMidiError& error) {
            error.printMessage();
            OBJ_ERR << "MIDI error: " << error.what();
        }
    }

    void onBang()
    {
        AtomList list;

        if (midiin_) {
            unsigned int nPorts = midiin_->getPortCount();

            for (unsigned int i = 0; i < nPorts; i++)
                list.append(gensym(midiin_->getPortName(i).c_str()));
        }

        listTo(0, list);
    }
};

extern "C" void setup_midi0x2elistin()
{
    ObjectFactory<PdMidiListIn> obj("midi.listin");
}
