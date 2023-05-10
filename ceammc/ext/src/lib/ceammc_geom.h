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
#ifndef CEAMMC_GEOM_H
#define CEAMMC_GEOM_H

#include <cstdint>

namespace ceammc {

/**
 * @enum epath_types
 * @brief The types of path.
 * @details It define all the path type.
 */
enum epath_types : std::int8_t {
    E_PATH_MOVE = 0, /*!< This type is move. */
    E_PATH_LINE = 1, /*!< This type is line. */
    E_PATH_CURVE = 2, /*!< This type is curve. */
    E_PATH_CLOSE = 3, /*!< This type is close. */
    E_PATH_CIRCLE = 4
};

/**
 * @enum eshape_types
 * @brief The types of shape.
 * @details It define soem of the shape type.
 */
enum eshape_types : std::int8_t {
    E_SHAPE_OVAL = 0, /*!< This shape is oval. */
    E_SHAPE_ARC = 1, /*!< This shape is arc. */
    E_SHAPE_IMAGE = 2, /*!< This shape is image. */
    E_SHAPE_RECT = 3 /*!< This shape is rectangle. */
};

/**
 * @struct t_pt
 * @brief A point structure.
 * @details It contains the members x and y for abscissa and ordinate.
 */
struct t_pt {
    float x; /*!< The abscissa coordiante. */
    float y; /*!< The ordiante coordiante. */

    constexpr t_pt()
        : x(0)
        , y(0)
    {
    }

    constexpr t_pt(float x_, float y_)
        : x(x_)
        , y(y_)
    {
    }

    constexpr t_pt(epath_types path, float y_)
        : x(path)
        , y(y_)
    {
    }

    constexpr t_pt(eshape_types shape, float y_)
        : x(shape)
        , y(y_)
    {
    }

    constexpr t_pt(double x_, double y_)
        : x(x_)
        , y(y_)
    {
    }

    bool operator==(const t_pt& pt) const { return x == pt.x && y == pt.y; }
    bool operator!=(const t_pt& pt) const { return !this->operator==(pt); }
};

struct t_rect {
    float x, y, w, h;
    t_rect(int x_, int y_, int w_, int h_)
        : x(x_)
        , y(y_)
        , w(w_)
        , h(h_)
    {
    }

    t_rect(float x_, float y_, float w_, float h_)
        : x(x_)
        , y(y_)
        , w(w_)
        , h(h_)
    {
    }

    t_rect()
        : x(0)
        , y(0)
        , w(0)
        , h(0)
    {
    }

    bool operator==(const t_rect& r) const { return x == r.x && y == r.y && w == r.w && h == r.h; }
    bool operator!=(const t_rect& r) const { return !operator==(r); }

    t_pt left_top() const { return { x, y }; };
    t_pt right_bottom() const { return { x + w, y + h }; };
};

}

#endif // CEAMMC_GEOM_H
