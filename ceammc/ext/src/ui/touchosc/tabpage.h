/*****************************************************************************
 * Copyright 2022 Serge Poltavsky. All rights reserved.
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
#ifndef TABPAGE_H
#define TABPAGE_H

#include "control.h"
#include "xmlnode.h"

#include <memory>
#include <vector>

namespace ceammc {
namespace touchosc {

    using ControlPtr = std::unique_ptr<Control>;
    using ControlVec = std::vector<ControlPtr>;

    class TabPage : public XmlNode {
        std::string name_, label_;
        OscAttributes osc_;
        ControlVec controls_;

    public:
        TabPage(const std::string& name);

        const std::string& name() const { return name_; }
        void setName(const std::string& name);
        const std::string& label() const { return label_; }
        void setLabel(const std::string& label);

        OscAttributes& osc() { return osc_; }
        const OscAttributes& osc() const { return osc_; }

        std::ostream& printAttributes(std::ostream& os) const override;
        std::ostream& printContent(std::ostream& os) const override;

        void append(ControlPtr&& ctl);
        const ControlVec& controls() const { return controls_; }
    };

}
}

#endif // TABPAGE_H
