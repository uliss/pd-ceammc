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

#ifndef WITH_PRINTER
#include "ceammc_stub.h"
CONTROL_OBJECT_STUB(HwPrinter, 1, 3, "compiled without printer support");
OBJECT_STUB_SETUP(HwPrinter, hw_printer, "hw.printer");
#else

#include "ceammc_factory.h"
#include "datatype_dict.h"
#include "fmt/core.h"
#include "hw_printer.h"

using namespace ceammc::printer;
using namespace ceammc::printer::req;

HwPrinter::HwPrinter(const PdArgs& args)
    : HwPrinterBase(args)
{
    createOutlet();
}

void HwPrinter::processRequest(const Request& req, ResultCallback cb)
{
    auto& t = req.type();
    if (t == typeid(ListPrinters)) {
        PrinterList info;
        int rc = ceammc_hw_get_printers(
            { &info,
                [](void* user, const ceammc_hw_printer_info* info) {
                    auto data = static_cast<PrinterList*>(user);
                    if (!data || !info)
                        return;

                    PrinterInfo pi;
                    pi.name = info->name;
                    pi.system_name = info->system_name;
                    pi.driver_name = info->driver_name;
                    pi.uri = info->uri;
                    pi.location = info->location;
                    pi.is_default = info->is_default;
                    pi.is_shared = info->is_shared;
                    pi.state = info->state;
                    data->push_back(pi);
                } });

        if (rc >= 0)
            workerThreadDebug(fmt::format("{} printers found", rc));

        cb(info);
    } else if (t == typeid(PrintFile)) {
        auto& job = boost::get<PrintFile>(req);
        if (!ceammc_hw_print_file(job.printer_name.c_str(), job.path.c_str()))
            workerThreadError("printing failed");
    }
}

void HwPrinter::processResult(const printer::Reply& res)
{
    auto& t = res.type();
    if (t == typeid(PrinterList)) {
        AtomList lst;

        for (auto& p : boost::get<PrinterList>(res)) {
            DictAtom di;
            di->insert("name", gensym(p.name.c_str()));
            di->insert("system_name", gensym(p.system_name.c_str()));
            di->insert("driver_name", gensym(p.driver_name.c_str()));
            di->insert("uri", gensym(p.uri.c_str()));
            di->insert("location", gensym(p.location.c_str()));
            di->insert("is_default", p.is_default);
            di->insert("is_shared", p.is_shared);
            lst.append(di);
        }

        listTo(0, lst);
    }
}

void HwPrinter::m_devices(t_symbol* s, const AtomListView& lv)
{
    addRequest(ListPrinters {});
}

void HwPrinter::m_print(t_symbol* s, const AtomListView& lv)
{
    if (!checkArgs(lv, ARG_SYMBOL, ARG_SYMBOL, s))
        return;

    auto pname = lv.symbolAt(0, &s_)->s_name;
    auto fname = lv.symbolAt(1, &s_)->s_name;
    auto path = findInStdPaths(fname);
    if (path.empty()) {
        OBJ_ERR << fmt::format("file not found: '{}'", fname);
        return;
    }

    addRequest(PrintFile { pname, path });
}

void setup_hw_printer()
{
    ObjectFactory<HwPrinter> obj("hw.printer");
    obj.addMethod("devices", &HwPrinter::m_devices);
    obj.addMethod("print", &HwPrinter::m_print);
}

#endif
