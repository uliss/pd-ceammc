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

#include <boost/variant.hpp>
#include <string>
#include <vector>

namespace ceammc {

enum class PrinterState {
    Unknown,
    Ready,
    Paused,
    Printing
};

enum class PrintingStatus {
    Ok,
    PrinterNotFound,
    JobError,
};

using JobStatus = std::pair<PrintingStatus, int>;

struct PrintOptions {
    bool landscape;
};

using PrinterOptionAttr = boost::variant<boost::blank, std::string, bool, int>;

struct PrinterOption {
    std::string name;
    PrinterOptionAttr ready;
    PrinterOptionAttr def;
    std::vector<PrinterOptionAttr> supported;
};

class PrinterInfo {
public:
    std::string name, system_name, location, driver, uri;
    bool is_default, is_shared;
    PrinterState state;
    std::vector<PrinterOption> options;
};

}

#endif // HW_PRINT_STRUCT_H
