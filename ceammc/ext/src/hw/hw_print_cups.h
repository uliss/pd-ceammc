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
#ifndef HW_PRINT_CUPS_H
#define HW_PRINT_CUPS_H

#include "ceammc_crc32.h"
#include "hw_print_struct.h"

#include <cups/cups.h>
#include <functional>

namespace {
inline std::string from_cstr(const char* s)
{
    return s ? s : std::string {};
}

inline bool to_bool(const char* s)
{
    return !s ? false : (std::strcmp(s, "true") == 0);
}

}

namespace ceammc {
namespace cups {

    inline PrinterState to_state(const char* s)
    {
        if (!s)
            return PrinterState::Unknown;

        switch (ceammc::crc32_hash(s)) {
        case "3"_hash:
            return PrinterState::Ready;
            break;
        case "4"_hash:
            return PrinterState::Printing;
            break;
        case "5"_hash:
            return PrinterState::Paused;
        default:
            return PrinterState::Unknown;
        }
    }

    static std::vector<PrinterOptionAttr> get_ipp_attr(ipp_attribute_t* attr)
    {
        std::vector<PrinterOptionAttr> res;

        auto n = ippGetCount(attr);
        auto tag = ippGetValueTag(attr);
        switch (tag) {
        case IPP_TAG_BOOLEAN: {
            for (int i = 0; i < n; i++) {
                auto v = ippGetBoolean(attr, i);
                if (v < 0)
                    break;

                res.push_back(bool(v != 0));
            }
        } break;
        case IPP_TAG_ENUM:
        case IPP_TAG_INTEGER: {
            for (int i = 0; i < n; i++) {
                auto v = ippGetInteger(attr, i);
                if (v <= 0)
                    break;

                res.push_back(v);
            }
        } break;
        case IPP_TAG_STRING:
        case IPP_TAG_KEYWORD: {
            for (int i = 0; i < n; i++) {
                auto v = ippGetString(attr, i, nullptr);
                if (!v)
                    break;

                res.push_back(std::string(v));
            }
        } break;
        default:
            break;
        }

        return res;
    }

    static PrinterOption get_printer_option(cups_dest_t& dest, const char* name)
    {
        auto info = cupsCopyDestInfo(CUPS_HTTP_DEFAULT, &dest);
        if (!info)
            return {};

        PrinterOption res;

        auto supported = cupsFindDestSupported(CUPS_HTTP_DEFAULT, &dest, info, name);
        if (supported) {
            auto name = from_cstr(ippGetName(supported));
            res.supported = get_ipp_attr(supported);
        }

        auto def = cupsFindDestDefault(CUPS_HTTP_DEFAULT, &dest, info, name);
        if (def) {
            auto attr = get_ipp_attr(def);
            if (attr.size() > 0)
                res.def = attr.front();
            else
                res.def = boost::blank();
        }
        auto ready = cupsFindDestReady(CUPS_HTTP_DEFAULT, &dest, info, name);
        if (ready) {
            auto attr = get_ipp_attr(ready);
            if (attr.size() > 0)
                res.ready = attr.front();
            else
                res.ready = boost::blank();
        }

        return res;
    }

    static PrinterInfo get_printer_info(cups_dest_t& dest)
    {
        PrinterInfo res;
        res.system_name = dest.name;
        res.is_default = dest.is_default;

        for (int i = 0; i < dest.num_options; i++) {
            auto& opt = dest.options[i];
            if (!opt.name)
                continue;

            auto hash = crc32_hash(opt.name);
            switch (hash) {
            case "printer-info"_hash:
                res.name = from_cstr(opt.value);
                break;
            case "printer-location"_hash:
                res.location = from_cstr(opt.value);
                break;
            case "device-uri"_hash:
                res.uri = from_cstr(opt.value);
                break;
            case "printer-make-and-model"_hash:
                res.driver = from_cstr(opt.value);
                break;
            case "printer-is-shared"_hash:
                res.is_shared = to_bool(opt.value);
                break;
            case "printer-state"_hash:
                res.state = to_state(opt.value);
                break;
            }
        }

        //        if(get_printer_option(dest, CUPS_PRINT_COLOR_MODE).) {

        //        }

        //        res.options.push_back(get_printer_option(dest, CUPS_COPIES));
        //        res.options.push_back(get_printer_option(dest, CUPS_MEDIA));
        //        res.options.push_back(get_printer_option(dest, CUPS_ORIENTATION));
        //        res.options.push_back(get_printer_option(dest, CUPS_PRINT_QUALITY));
        //        res.options.push_back(get_printer_option(dest, CUPS_MEDIA_SOURCE));
        //        res.options.push_back(get_printer_option(dest, CUPS_MEDIA_TYPE));
        //        res.options.push_back();
        //        res.options.push_back(get_printer_option(dest, CUPS_NUMBER_UP));

        return res;
    }

    static void get_printers(std::function<void(const PrinterInfo&)> fn)
    {
        cups_dest_t* dests = nullptr;
        auto num = cupsGetDests(&dests);

        for (int i = 0; i < num; i++) {
            if (!dests[i].name)
                continue;

            fn(get_printer_info(dests[i]));
        }

        cupsFreeDests(num, dests);
    }

    static JobStatus print_file(const std::string& path, const std::string& printer_name, const std::string& job_title, const PrintOptions& opts)
    {
        std::string pname = printer_name;
        if (pname.empty()) {
            pname = cupsGetDefault2(CUPS_HTTP_DEFAULT);
            if (pname.empty())
                return { PrintingStatus::PrinterNotFound, 0 };
        }

        int num_cups_opts = 0;
        cups_option_t* cups_opts = nullptr;

        if (opts.landscape) {
            num_cups_opts = cupsAddOption(CUPS_ORIENTATION, CUPS_ORIENTATION_LANDSCAPE,
                num_cups_opts, &cups_opts);
        }

        auto job_id = cupsPrintFile2(CUPS_HTTP_DEFAULT,
            cupsGetDefault(),
            path.c_str(),
            job_title.c_str(),
            num_cups_opts,
            cups_opts);

        cupsFreeOptions(num_cups_opts, cups_opts);

        if (job_id == 0) {
            cupsLastErrorString();
            return { PrintingStatus::JobError, 0 };
        } else {
            return { PrintingStatus::Ok, job_id };
        }
    }
}
}

#endif // HW_PRINT_CUPS_H
