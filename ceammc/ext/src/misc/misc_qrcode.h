/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#ifndef MISC_QRCODE_H
#define MISC_QRCODE_H

#include "ceammc_object.h"
#include "ceammc_property_enum.h"

#include <memory>

using namespace ceammc;

namespace qrcodegen {
class QrCode;
}

using QrPointer = std::unique_ptr<qrcodegen::QrCode>;

class MiscQrCode : public BaseObject {
    SymbolProperty* value_;
    SymbolEnumProperty* quality_;
    QrPointer qrcode_;
    AtomList data_;

public:
    MiscQrCode(const PdArgs& args);

    void onBang() override;
    void onSymbol(t_symbol* s) override;

private:
    void output();
};

void setup_misc_qrcode();

#endif // MISC_QRCODE_H
