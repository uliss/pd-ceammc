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
#ifndef COMMON_H
#define COMMON_H

#include <array>
#include <cstdint>
#include <string>
#include <type_traits>

#include "m_pd.h"

namespace ceammc {
namespace ui {
    enum UIFactoryFlags : uint16_t {
        UI_FACTORY_NONE = 0,
        UI_FACTORY_FLAG_DISPLACE = 1,
        UI_FACTORY_FLAG_SELECT = (1 << 1),
        UI_FACTORY_FLAG_ACTIVATE = (1 << 2),
        UI_FACTORY_FLAG_DELETE = (1 << 3),
        UI_FACTORY_FLAG_VIS = (1 << 4),
        UI_FACTORY_FLAG_CLICK = (1 << 5),
        UI_FACTORY_FLAG_MOUSE_ENTER = (1 << 6),
        UI_FACTORY_FLAG_MOUSE_LEAVE = (1 << 7),
        UI_FACTORY_FLAG_MOUSE_MOVE = (1 << 8),
        UI_FACTORY_FLAG_MOUSE_DOWN = (1 << 9),
        UI_FACTORY_FLAG_MOUSE_UP = (1 << 10),
        UI_FACTORY_FLAG_MOUSE_RIGHT = (1 << 11),
        UI_FACTORY_FLAG_DEFAULT = UI_FACTORY_FLAG_VIS
            | UI_FACTORY_FLAG_DISPLACE
            | UI_FACTORY_FLAG_SELECT
            | UI_FACTORY_FLAG_DELETE
    };

    enum KeyModFlags {
        KEY_MOD_NONE = 0,
        KEY_MOD_SHIFT = 1,
        KEY_MOD_MAJ = 2,
        KEY_MOD_CTRL = 4,
        KEY_MOD_CMD = 8,
        KEY_MOD_ALT = 16,
        KEY_MOD_RIGHT = 32
    };

    enum CursorFlags {
        CURSOR_LEFT_PTR = 0,
        CURSOR_CENTER_PTR = 1, /*!< The center_ptr string. */
        CURSOR_PLUS = 2, /*!< The plus string. */
        CURSOR_HAND = 3, /*!< The hand2 string. */
        CURSOR_CIRCLE = 4, /*!< The circle string. */
        CURSOR_X = 5, /*!< The X_cursor string. */
        CURSOR_BOTTOM = 6, /*!< The bottom_side string. */
        CURSOR_RIGHT_CORNER = 7, /*!< The bottom_right_corner string. */
        CURSOR_RIGHT_SIDE = 8, /*!< The right_side string. */
        CURSOR_LEFT_SIDE = 9, /*!< The left_side string. */
        CURSOR_EXCHANGE = 10, /*!< The exchange string. */
        CURSOR_XTERM = 11, /*!< The xterm string. */
        CURSOR_MOVE = 12, /*!< The move cursor */
        CURSOR_VDOUBLE_ARROW = 13, /*!< The sb_v_double_arrow string. */
        CURSOR_HDOUBLE_ARROW = 14, /*!< The sb_h_double_arrow string. */
        CURSOR_MAX_COUNT,
    };

    enum SelectionType {
        SELECT_NONE = 0,
        SELECT_OBJ = 1,
        SELECT_INLET = 2,
        SELECT_OUTLET = 3,
        SELECT_BOTTOM = 4,
        SELECT_CORNER = 5,
        SELECT_RIGHT = 6
    };

    enum ResizeMode {
        RESIZE_NONE = 0,
        RESIZE_LINKED,
        RESIZE_WIDTH,
        RESIZE_HEIGHT,
        RESIZE_BOTH
    };

    template <class>
    struct hasher;
    template <>
    struct hasher<std::string> {
        std::size_t constexpr operator()(char const* input) const
        {
            return *input ? static_cast<unsigned int>(*input) + 33 * (*this)(input + 1) : 5381;
        }
        std::size_t operator()(const std::string& str) const
        {
            return (*this)(str.c_str());
        }
    };

    template <typename T>
    std::size_t constexpr hash(T&& t)
    {
        return hasher<typename std::decay<T>::type>()(std::forward<T>(t));
    }

    inline namespace literals {
        std::size_t constexpr operator"" _hash(const char* s, size_t)
        {
            return hasher<std::string>()(s);
        }
    }

    enum TextAlign {
        TEXT_ALIGN_LEFT,
        TEXT_ALIGN_RIGHT,
        TEXT_ALIGN_CENTER,
        TEXT_ALIGN_JUSTIFY,
    };

    enum AnchorPosition {
        ANCHOR_CORNER_LEFT_TOP,
        ANCHOR_CORNER_LEFT_BOTTOM,
        ANCHOR_CORNER_RIGHT_TOP,
        ANCHOR_CORNER_RIGHT_BOTTON,
        ANCHOR_SIDE_LEFT_CENTER,
        ANCHOR_SIDE_RIGHT_CENTER,
        ANCHOR_SIDE_TOP_CENTER,
        ANCHOR_SIDE_BOTTOM_CENTER,
        ANCHOR_CENTER,
    };

    enum EventType {
        EVENT_MOUSE_DOWN,
        EVENT_MOUSE_UP,
        EVENT_MOUSE_DRAG,
        EVENT_MOUSE_ENTER,
        EVENT_MOUSE_LEAVE,
        EVENT_MOUSE_DBL_CLICK,
        EVENT_POPUP,
        EVENT_KEY_DOWN,
        EVENT_KEY_UP
    };

    enum EventStatus {
        EVENT_STATUS_ACCEPT,
        EVENT_STATUS_IGNORE,
        EVENT_STATUS_CONTINUE
    };

    enum XletType : char {
        XLET_CONTROL = '_',
        XLET_AUDIO = '~',
    };

    constexpr size_t UI_MAX_XLET_NUMBER = 30;

    class Xlets {
        std::array<char, UI_MAX_XLET_NUMBER + 1> xlets_;
        uint8_t size_;

    public:
        Xlets();

        static Xlets fromInlets(t_object* x);
        static Xlets fromOutlets(t_object* x);

        bool append(XletType t);
        size_t size() const { return size_; }

        const char* asString() const { return xlets_.data(); }
    };
}
}

#endif // COMMON_H
