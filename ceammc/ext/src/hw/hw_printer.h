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
#ifndef HW_PRINTER_H
#define HW_PRINTER_H

#include <boost/variant.hpp>

#include "ceammc_pollthread_spsc.h"
#include "hw_rust_print.hpp"

namespace ceammc {
namespace printer {

    namespace req {
        struct ListPrinters { };
        struct ChoosePrinter {
            int idx;
        };
        struct PrintFile {
            std::string printer_name;
            std::string file_path;
            ceammc_hw_print_options opts;
        };
    }

    namespace reply {
    }

    using Request = boost::variant<req::ListPrinters, req::ChoosePrinter, req::PrintFile>;
    using Reply = boost::variant<PrinterInfo, ChoosePrinterName>;
}
}

using namespace ceammc;

using HwPrinterBase = FixedSPSCObject<printer::Request, printer::Reply>;

class HwPrinter : public HwPrinterBase {
    SymbolProperty* name_ { nullptr };

public:
    HwPrinter(const PdArgs& args);

    void processRequest(const printer::Request& req, ResultCallback cb) final;
    void processResult(const printer::Reply& res) final;

    void m_choose(t_symbol* s, const AtomListView& lv);
    void m_devices(t_symbol* s, const AtomListView& lv);
    void m_print(t_symbol* s, const AtomListView& lv);
};

void setup_hw_printer();

#endif // HW_PRINTER_H
