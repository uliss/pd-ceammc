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
#include "nui.h"

namespace ceammc {
namespace ui {

    bool Style::hasColor(StyleKey key) const
    {
        return colors.find(key) != colors.end();
    }

    bool Style::getColor(StyleKey key, HexColor& c) const
    {
        auto it = colors.find(key);
        if (it == colors.end())
            return false;

        c = it->second;
        return true;
    }

    HexColor Style::getColorWithDef(StyleKey key, HexColor def) const
    {
        getColor(key, def);
        return def;
    }

    bool Style::hasSize(StyleKey key) const
    {
        return sizes.find(key) == sizes.end();
    }

    bool Style::getSize(StyleKey key, Size& sz) const
    {
        auto it = sizes.find(key);
        if (it == sizes.end())
            return false;

        sz = it->second;
        return true;
    }

    Size Style::getSizeWithDef(StyleKey key, const Size& sz) const
    {
        Size res;
        return getSize(key, res) ? res : sz;
    }

    bool Style::hasFont(StyleKey key) const
    {
        return fonts.find(key) == fonts.end();
    }

    bool Style::getFont(StyleKey key, Font& ft) const
    {
        auto it = fonts.find(key);
        if (it == fonts.end())
            return false;

        ft = it->second;
        return true;
    }

    Font Style::getFontWithDef(StyleKey key, const Font& ft) const
    {
        Font res;
        return getFont(key, res) ? res : ft;
    }

    bool Style::insertColor(StyleKey key, HexColor c)
    {
        if (hasColor(key))
            return false;

        colors[key] = c;
        return true;
    }

    bool Style::insertSize(StyleKey key, const Size& sz)
    {
        if (hasSize(key))
            return false;

        sizes[key] = sz;
        return true;
    }

    bool Style::insertFont(StyleKey key, const Font& ft)
    {
        if (hasFont(key))
            return false;

        fonts[key] = ft;
        return true;
    }

    StyleCollection::StyleCollection()
    {
    }

    StyleCollection& StyleCollection::instance()
    {
        static StyleCollection instance_;
        return instance_;
    }

    const Style* StyleCollection::style(int idx)
    {
        return instance().getStyle(idx);
    }

    HexColor StyleCollection::color(int styleIdx, StyleKey key, HexColor def)
    {
        return instance().getColor(styleIdx, key, def);
    }

    Size StyleCollection::size(int styleIdx, StyleKey key, const Size& def)
    {
        return instance().getSize(styleIdx, key, def);
    }

    Font StyleCollection::font(int styleIdx, StyleKey key, const Font& def)
    {
        return instance().getFont(styleIdx, key, def);
    }

    const Style* StyleCollection::getStyle(int idx) const
    {
        if (idx < 0)
            return 0;

        idx--;
        return (idx < 0 || idx >= styles_.size())
            ? &default_
            : &styles_[idx];
    }

    HexColor StyleCollection::getColor(int styleIdx, StyleKey key, HexColor def) const
    {
        auto st = getStyle(styleIdx);
        if (!st)
            return def;
        else
            return st->getColorWithDef(key, def);
    }

    Size StyleCollection::getSize(int styleIdx, StyleKey key, const Size& def) const
    {
        auto st = getStyle(styleIdx);
        if (!st)
            return def;
        else
            return st->getSizeWithDef(key, def);
    }

    Font StyleCollection::getFont(int styleIdx, StyleKey key, const Font& def) const
    {
        auto st = getStyle(styleIdx);
        if (!st)
            return def;
        else
            return st->getFontWithDef(key, def);
    }
}
}
