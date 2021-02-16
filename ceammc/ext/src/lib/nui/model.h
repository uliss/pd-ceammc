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
#ifndef MODEL_H
#define MODEL_H

#include <cstdint>

#include "m_pd.h"

#include "nui/color.h"
#include "nui/font.h"

namespace ceammc {
namespace ui {

    struct EmptyData {
    };

    using PropId = int16_t;
    constexpr PropId PROP_ID_ALL = -1;
    constexpr PropId PROP_ID_FRAME = -2;

    template <typename Props>
    class ModelBase {
    public:
        virtual bool hasProp(PropId id) = 0;
        virtual const Props& getProp(PropId id) const = 0;
    };

    struct FrameProps {
        HexColor bd_color { colors::st_border };
        HexColor sel_color { colors::blue };
        float padding { 2 };
        bool selected { true };

        FrameProps()
        {
        }
    };

    struct SliderProps {
        float value { 0 },
            min { 0 },
            max { 1 };

        HexColor bd_color { colors::st_backgr },
            bg_color { colors::st_backgr },
            kn_color { colors::black };

        int8_t style_idx { 0 };
        bool log_scale { false };

        SliderProps() { }
        SliderProps(int8_t style);
    };

    struct LabelProps {
        t_symbol* text;
        t_symbol* tooltip;
        Font font;
        HexColor bd_color { colors::st_border },
            bg_color { colors::st_backgr },
            txt_color { colors::st_active };

        int8_t style_idx { 0 };

        LabelProps(int8_t style);
    };

    class FrameModel : public ModelBase<FrameProps> {
    };

    class SliderModel : public ModelBase<SliderProps> {
    };

    class LabelModel : public ModelBase<LabelProps> {
    };
}
}

#endif // MODEL_H
