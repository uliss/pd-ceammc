/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef FILE_SIZE_H
#define FILE_SIZE_H

#include "ceammc_data.h"
#include "datatype_string.h"
#include "path_async_base.h"
using namespace ceammc;

class FileSize : public PathAsyncBase<int> {
    std::string file_;

public:
    FileSize(const PdArgs& args);
    void onSymbol(t_symbol* s) override;
    void onDataT(const StringAtom& s);

private:
    void processResult() override;
    FutureResult createTask() override;
};

void setup_file_size();

#endif // FILE_SIZE_H
