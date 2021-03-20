/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef HW_SERIAL_H
#define HW_SERIAL_H

#include "ceammc_property_enum.h"
#include "ceammc_thread.h"

#include <future>

namespace ceammc {

class SerialPort;
class SerialTask;

typedef std::vector<std::string> DeviceList;

class SerialPort : public ThreadExternal {
    SymbolProperty* port_;
    IntEnumProperty* baud_rate_;
    std::unique_ptr<thread::Pipe> pipe_in_;
    std::unique_ptr<thread::Pipe> pipe_out_;

public:
    SerialPort(const PdArgs& args);
    ~SerialPort();

    void onFloat(t_float f) override;
    void onList(const AtomList& l) override;

    void onThreadDone(int rc) override;
    bool onThreadCommand(int code) override;

    void m_open(t_symbol* s, const AtomListView& l);
    void m_close(t_symbol* s, const AtomListView&);

    AtomList propDevices() const;

private:
    SerialTask* serial();
};

}

void hw_setup_serial();

#endif // HW_SERIAL_H
