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
#ifndef CEAMMC_PROPERTY_MESSAGE_H
#define CEAMMC_PROPERTY_MESSAGE_H

#include "ceammc_message.h"
#include "ceammc_property.h"

namespace ceammc {

class MessageProperty : public Property {
    Message msg_;

public:
    MessageProperty(const char* name, const Message& init = Message(), PropValueAccess access = PropValueAccess::READWRITE);

    AtomList get() const override;
    bool setList(const AtomListView& lv) override;
    bool getList(AtomList& l) const override;

    inline const Message& value() const { return msg_; }
    inline Message& value() { return msg_; }
    bool setValue(const Message& m);
    const AtomList& defaultValue() const;

public:
    using value_type = Message;
};

}

#endif // CEAMMC_PROPERTY_MESSAGE_H
