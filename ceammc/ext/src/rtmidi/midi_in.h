/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef MIDI_IN_H
#define MIDI_IN_H

#include <boost/scoped_ptr.hpp>

#include "RtMidi.h"
#include "ceammc_object.h"

using namespace ceammc;

typedef boost::scoped_ptr<RtMidiIn> SharedMidiIn;

class PdMidiIn : public BaseObject {
    SharedMidiIn midiin_;
    t_symbol* device_;
    RtMidiIn::RtMidiCallback callback_;
    float port_;

public:
    PdMidiIn(const PdArgs& a);

    int port() const;
    const char* deviceName() const;

    RtMidiIn::RtMidiCallback callback();
    void setCallback(RtMidiIn::RtMidiCallback cb);

    /** pd */
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;

    /** props */
    AtomList p_getPort() const;
    void p_setPort(const AtomList& lst);
    AtomList p_getDevice() const;
    void p_setDevice(const AtomList& lst);

private:
    void connectByPort();
    void connectByDevice();
    void openPort(unsigned int n);
    int searchPort(const std::string& name);

private:
    static void midiCallback(double /*deltatime*/, std::vector<unsigned char>* message, void* userData);
};

#endif // MIDI_IN_H
