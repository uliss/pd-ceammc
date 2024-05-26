/*****************************************************************************
 * Copyright 2024 Serge Poltavski. All rights reserved.
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
#ifndef HW_PRINT_STRUCT_H
#define HW_PRINT_STRUCT_H

#include <string>
#include <vector>

#include "hw_rust.hpp"

namespace ceammc {

enum class PrintingStatus {
    Ok,
    PrinterNotFound,
    JobError,
};

class PrinterInfo {
public:
    std::string name, system_name, location, driver, uri;
    bool is_default, is_shared;
    ceammc_hw_printer_state state;

    PrinterInfo(const ceammc_hw_printer_info* info)
    {
        name = info->name;
        system_name = info->system_name;
        driver = info->driver_name;
        location = info->location;
        uri = info->uri;
        state = info->state;
        is_default = info->is_default;
        is_shared = info->is_shared;
    }
};

struct ChoosePrinterName {
    std::string name;
};

}

#endif // HW_PRINT_STRUCT_H
