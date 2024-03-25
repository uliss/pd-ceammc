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
CONTROL_OBJECT_STUB(HwPrinter, 1, 1, "compiled without printer support");
OBJECT_STUB_SETUP(HwPrinter, hw_printer, "hw.printer");
#else

#include "args/argcheck2.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"
#include "ceammc_fn_list.h"
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

        auto n = ceammc_hw_get_printers({
            this,
            [](void* user, const ceammc_hw_printer_info* info) {
                auto this_ = static_cast<HwPrinter*>(user);
                if (this_)
                    this_->addReply(PrinterInfo(info));
            } //
        });

        workerThreadDebug(fmt::format("{} printers found", n));
    } else if (t == typeid(PrintFile)) {
        auto& job = boost::get<PrintFile>(req);

        auto job_id = ceammc_hw_print_file(
            job.printer_name.c_str(),
            job.file_path.c_str(),
            &job.opts,
            {
                this,
                [](void* user, const char* msg) {
                    auto this_ = static_cast<HwPrinter*>(user);
                    if (this_)
                        this_->workerThreadError(msg);
                } //
            });

        if (job_id == ceammc_JOB_ERROR) {
            workerThreadError("printing failed");
        } else {
            workerThreadDebug(fmt::format("print job: {}", job_id));
        }
    }
}

void HwPrinter::processResult(const printer::Reply& res)
{
    auto& t = res.type();
    if (t == typeid(PrinterInfo)) {
        auto& info = boost::get<PrinterInfo>(res);

        DictAtom di;
        di->insert("name", gensym(info.name.c_str()));
        di->insert("system_name", gensym(info.system_name.c_str()));
        di->insert("driver_name", gensym(info.driver.c_str()));
        di->insert("uri", gensym(info.uri.c_str()));
        di->insert("location", gensym(info.location.c_str()));
        di->insert("is_default", info.is_default);
        di->insert("is_shared", info.is_shared);

        atomTo(0, di);
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
    ceammc_hw_print_options opts;

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
    ObjectFactory<HwPrinter> obj("hw.printer");
    obj.addMethod("devices", &HwPrinter::m_devices);
    obj.addMethod("print", &HwPrinter::m_print);
}

#endif
