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

#ifdef WITH_CUPS
#include "hw_print_cups.h"
#define IMPL_NS ceammc::cups
#else
#endif

#include "args/argcheck2.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
#include "ceammc_platform.h"
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
        PrinterList lst;

        IMPL_NS::get_printers([this, &lst](const ceammc::PrinterInfo& info) {
            lst.push_back(info);
        });

        workerThreadDebug(fmt::format("{} printers found", lst.size()));

        addReply(lst);

    } else if (t == typeid(PrintFile)) {
        auto& job = boost::get<PrintFile>(req);
        auto title = fmt::format("PureData print '{}'", platform::basename(job.file_path.c_str()));

        auto id = IMPL_NS::print_file(job.file_path, job.printer_name, title, job.opts);
        if (id.first != PrintingStatus::Ok) {
            workerThreadError("printing failed");
        } else {
            workerThreadDebug(fmt::format("print job: {}", id.second));
        }
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
            di->insert("driver_name", gensym(p.driver.c_str()));
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
    static const args::ArgChecker chk("FILE:s OPTS:a*");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    auto fname = lv.symbolAt(0, &s_)->s_name;
    auto pname = lv.symbolAt(1, &s_)->s_name;
    PrintOptions opts;

    list::foreachProperty(lv.subView(1), [&opts](const t_symbol* k, const AtomListView& lv) {
        switch (crc32_hash(k)) {
        case "@landscape"_hash:
            opts.landscape = lv.asBool(true);
            break;
        default:
            break;
        }
    });

    auto path = findInStdPaths(fname);
    if (path.empty()) {
        OBJ_ERR << fmt::format("file not found: '{}'", fname);
        return;
    }

    addRequest(PrintFile { pname, path, opts });
}

void setup_hw_printer()
{
#ifdef WITH_CUPS
    LIB_DBG << fmt::format("CUPS version: {}", CUPS_VERSION);
#endif

    ObjectFactory<HwPrinter> obj("hw.printer");
    obj.addMethod("devices", &HwPrinter::m_devices);
    obj.addMethod("print", &HwPrinter::m_print);
}

#endif
