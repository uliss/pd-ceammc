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
#ifndef CEAMMC_FAUST_UI_H
#define CEAMMC_FAUST_UI_H

#include "ceammc_datatypes.h"
#include "ceammc_property_info.h"
#include "faust_common_ui.h"

namespace ceammc {
namespace faust {

    enum UIElementType : std::uint8_t {
        UI_BUTTON,
        UI_CHECK_BUTTON,
        UI_V_SLIDER,
        UI_H_SLIDER,
        UI_NUM_ENTRY,
        UI_V_BARGRAPH,
        UI_H_BARGRAPH,
        UI_END_GROUP,
        UI_V_GROUP,
        UI_H_GROUP,
        UI_T_GROUP
    };

    class UIElement {
        UIElementType type_;
        t_symbol* label_;
        FAUSTFLOAT init_, min_, max_, step_;
        FAUSTFLOAT* vptr_;
        t_symbol* set_prop_symbol_;
        t_symbol* get_prop_symbol_;
        PropertyInfo pinfo_;

    public:
        UIElement(UIElementType t, const char* label);
        UIElement(UIElementType t, t_symbol* label);
        const t_symbol* label() const { return label_; }
        t_symbol* typeSymbol() const;
        UIElementType type() const { return type_; }
        FAUSTFLOAT init() const { return init_; }
        FAUSTFLOAT min() const { return min_; }
        FAUSTFLOAT max() const { return max_; }
        FAUSTFLOAT step() const { return step_; }

        t_symbol* getPropertySym() { return get_prop_symbol_; }
        t_symbol* setPropertySym() { return set_prop_symbol_; }

        FAUSTFLOAT value(FAUSTFLOAT def = 0.f) const;
        void setValue(FAUSTFLOAT v, bool clip = false);

        const FAUSTFLOAT* valuePtr() const { return vptr_; }
        void setValuePtr(FAUSTFLOAT* vPtr)
        {
            vptr_ = vPtr;
            *vptr_ = init_;
        }
        void setContraints(FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);

        const PropertyInfo& propInfo() const { return pinfo_; }
        void setUnits(PropValueUnits u) { pinfo_.setUnits(u); }
        void setType(PropValueType t) { pinfo_.setType(t); }

    private:
        void initProperty();
    };

    // typedefs
    using UIElementPtr = std::unique_ptr<UIElement>;
    using OscSegmentList = std::vector<const t_symbol*>;
    using UIList = std::vector<UIElementPtr>;

    class UIEnumEntry {
        const ceammc::BoostStaticString<10> name_;
        const std::uint32_t hash_;

    public:
        explicit UIEnumEntry(const char* name);
        bool operator==(std::uint32_t hash) const { return hash_ == hash; }
        bool operator==(const char* name) const;
        const char* name() const { return name_.c_str(); }
        std::uint32_t hash() const { return hash_; }
    };

    using UIEnumList = std::vector<UIEnumEntry>;

    // functions
    PropValueUnits to_units(const char* u);
    PropValueType to_value_type(const char* v);

    class DeclareValue {
        enum UpdateMode : std::uint8_t {
            UPDATE_NONE = 0,
            UPDATE_TYPE = 1,
            UPDATE_UNITS = 1 << 1,
            UPDATE_STYLE = 1 << 1,
        };

        const FAUSTFLOAT* value_ { 0 };
        PropValueType type_ { PropValueType::FLOAT };
        PropValueUnits units_ { PropValueUnits::NONE };
        std::uint8_t update_flags_ { UPDATE_NONE };

    public:
        void reset();
        void update(const FAUSTFLOAT* value, UIElementPtr& e);
        void setValue(const FAUSTFLOAT* v);
        void setType(PropValueType t);
        void setUnits(PropValueUnits u);
        void setStyle(const char* str);
        const void* value() const { return value_; }
    };

    class PdUI : public UI {
        UIList ui_elements_;
        OscSegmentList osc_segs_;
        const t_symbol* name_;
        DeclareValue last_declare_;

    public:
        PdUI(const char* name);
        const t_symbol* name() const { return name_; }
        const OscSegmentList& oscSegments() const { return osc_segs_; }

        const UIList& elements() const { return ui_elements_; }
        UIElement* uiAt(size_t pos);
        const UIElement* uiAt(size_t pos) const;
        size_t uiCount() const;
        void addSoundfile(const char* /*label*/, const char* /*filename*/, Soundfile** /*sf_zone*/);

    protected:
        void add_elem(UIElementType type, t_symbol* label, FAUSTFLOAT* zone);
        void add_elem(UIElementType type, t_symbol* label, FAUSTFLOAT* zone,
            FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);
        void add_elem(UIElementType type, t_symbol* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max);

    public:
        virtual void addButton(const char* label, FAUSTFLOAT* zone);
        virtual void addCheckButton(const char* label, FAUSTFLOAT* zone);
        virtual void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);
        virtual void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);
        virtual void addNumEntry(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step);

        virtual void addHorizontalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max);
        virtual void addVerticalBargraph(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT min, FAUSTFLOAT max);

        virtual void openTabBox(const char* label);
        virtual void openHorizontalBox(const char* label);
        virtual void openVerticalBox(const char* label);
        virtual void closeBox();

        virtual void declare(FAUSTFLOAT* v, const char* name, const char* value);

    public:
        std::vector<FAUSTFLOAT> uiValues() const;
        void setUIValues(const std::vector<FAUSTFLOAT>& v);
    };

}
}

#endif // CEAMMC_FAUST_UI_H
