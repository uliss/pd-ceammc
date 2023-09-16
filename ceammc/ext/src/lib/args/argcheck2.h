/*****************************************************************************
 * Copyright 2022 Serge Poltavski. All rights reserved.
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
#ifndef ARCHECK2_H
#define ARCHECK2_H

#include "ceammc_atomlist_view.h"

#include <iosfwd>
#include <memory>

#include <boost/container/small_vector.hpp>

namespace ceammc {

class BaseObject;

namespace args {

    class ArgCheckImp;

    using ArgMatchList = boost::container::small_vector<AtomListView, 16>;

    class ArgChecker {
        std::unique_ptr<ArgCheckImp> chk_;

    public:
        ArgChecker(const char* str);
        ~ArgChecker();

        /**
         * Check specified list
         * @param lv - argument list
         * @param obj - pointer to parent object (can be nullptr)
         * @param matches - pointer to write matched items
         * @param printErr - if true, print errors
         * @return true on success, false on error
         */
        bool check(const AtomListView& lv, BaseObject* obj, ArgMatchList* matches = nullptr, bool printErr = true) const;
        void usage(BaseObject* obj = nullptr, t_symbol* m = nullptr) const;
    };

    bool check_args(const char* arg_string, const AtomListView& lv, BaseObject* obj = nullptr, ArgMatchList* matches = nullptr);

}
}

#endif // ARCHECK2_H
