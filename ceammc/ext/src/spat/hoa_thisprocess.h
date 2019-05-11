/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#ifndef HOA_THISPROCESS_H
#define HOA_THISPROCESS_H

#include "hoa_common.h"

#include <unordered_map>

class ProcessInstance;

class HoaThisProcess : public BaseObject {
    const ProcessInstance* process_;
    std::unordered_map<t_symbol*, AtomList> props_;

public:
    HoaThisProcess(const PdArgs& args);

    void onBang() override;
    void parseProperties() override;

    const ProcessInstance* process() const { return process_; }
    void setProcess(const ProcessInstance* p) { process_ = p; }

public:
    static HoaThisProcess* fromObject(void* obj);
    static bool isA(void* obj);
};

void setup_spat_hoa_thisprocess();

#endif // HOA_THISPROCESS_H
