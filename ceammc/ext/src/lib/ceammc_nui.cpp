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
#include "ceammc_nui.h"

namespace ceammc {
namespace ui {

    bool Style::hasColor(size_t key) const
    {
        return colors.find(key) != colors.end();
    }

    bool Style::getColor(size_t key, HexColor& c) const
    {
        auto it = colors.find(key);
        if (it == colors.end())
            return false;

        c = it->second;
        return true;
    }

    HexColor Style::getColorWithDef(size_t key, HexColor def) const
    {
        getColor(key, def);
        return def;
    }

    bool Style::hasSize(size_t key) const
    {
        return sizes.find(key) == sizes.end();
    }

    bool Style::getSize(size_t key, Size& sz) const
    {
        auto it = sizes.find(key);
        if (it == sizes.end())
            return false;

        sz = it->second;
        return true;
    }

    Size Style::getSizeWithDef(size_t key, const Size& sz) const
    {
        Size res;
        return getSize(key, res) ? res : sz;
    }

    bool Style::hasFont(size_t key) const
    {
        return fonts.find(key) == fonts.end();
    }

    bool Style::getFont(size_t key, Font& ft) const
    {
        auto it = fonts.find(key);
        if (it == fonts.end())
            return false;

        ft = it->second;
        return true;
    }

    bool Style::insertColor(size_t key, HexColor c)
    {
        if (hasColor(key))
            return false;

        colors[key] = c;
        return true;
    }

    bool Style::insertSize(Style::Key key, const Size& sz)
    {
        if (hasSize(key))
            return false;

        sizes[key] = sz;
        return true;
    }

    bool Style::insertFont(Style::Key key, const Font& ft)
    {
        if (hasFont(key))
            return false;

        fonts[key] = ft;
        return true;
    }

}
}
