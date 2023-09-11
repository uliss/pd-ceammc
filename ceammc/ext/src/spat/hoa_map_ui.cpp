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
#include "hoa_map_ui.h"
#include "args/argcheck2.h"
#include "ceammc_canvas.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_crc32.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"
#include "fmt/core.h"
#include <forward_list>

namespace ceammc {

CEAMMC_DEFINE_SYM_HASH(abscissa);
CEAMMC_DEFINE_SYM_HASH(azimuth);
CEAMMC_DEFINE_SYM_HASH(car);
CEAMMC_DEFINE_SYM_HASH(cartesian);
CEAMMC_DEFINE_SYM_HASH(description);
CEAMMC_DEFINE_SYM_HASH(elevation);
CEAMMC_DEFINE_SYM_HASH(group);
CEAMMC_DEFINE_SYM_HASH(height);
CEAMMC_DEFINE_SYM_HASH(index);
CEAMMC_DEFINE_SYM_HASH(modified);
CEAMMC_DEFINE_SYM_HASH(mute);
CEAMMC_DEFINE_SYM_HASH(ordinate);
CEAMMC_DEFINE_SYM_HASH(pol);
CEAMMC_DEFINE_SYM_HASH(polar);
CEAMMC_DEFINE_SYM_HASH(radius);
CEAMMC_DEFINE_SYM_HASH(relazimuth);
CEAMMC_DEFINE_SYM_HASH(relelevation);
CEAMMC_DEFINE_SYM_HASH(relpolar);
CEAMMC_DEFINE_SYM_HASH(relradius);
CEAMMC_DEFINE_SYM_HASH(remove);
CEAMMC_DEFINE_SYM_HASH(set);
CEAMMC_DEFINE_SYM_HASH(source);
CEAMMC_DEFINE_SYM_HASH(xy);
CEAMMC_DEFINE_SYM_HASH(xz);
CEAMMC_DEFINE_SYM_HASH(yz);

constexpr int MIN_SIZE = 20;
constexpr float MAX_ZOOM = 1.;
constexpr float MIN_ZOOM = 0.01;
constexpr size_t SOURCE_OUTLET = 0;
constexpr size_t GROUP_OUTLET = 1;
constexpr size_t INFO_OUTLET = 2;
constexpr const char* ODD_BINDING_SUFFIX = "map1572";

enum BindingMapMsgFlag {
    BMAP_REDRAW = 0x01,
    BMAP_NOTIFY = 0x02,
    BMAP_OUTPUT = 0x04
};

using ulong = unsigned long;

static inline double hoa_pd_distance(double x1, double y1, double x2, double y2, double z1 = 0., double z2 = 0.)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
}

using SubscriberList = std::forward_list<HoaMapUI*>;
struct HoaManagerData {
    hoa::Source::Manager manager;
    SubscriberList subscribers;

    void subscribe(HoaMapUI* p)
    {
        auto it = std::find(subscribers.begin(), subscribers.end(), p);
        if (it == subscribers.end())
            subscribers.push_front(p);
    }

    void unsubscribe(HoaMapUI* p)
    {
        subscribers.remove(p);
    }

    std::shared_ptr<HoaManagerData> clone() const
    {
        return std::shared_ptr<HoaManagerData> { new HoaManagerData { manager, SubscriberList() } };
    }
};

HoaMapUI::HoaMapUI()
    : selection_(asEBox(), gensym("selections"))
    , sources_(asEBox(), gensym("sources"))
    , groups_(asEBox(), gensym("groups"))
{
    f_binding_name = s_null;
    f_manager.reset(new HoaManagerData { { 1. / (double)MIN_ZOOM - 5. }, SubscriberList { this } });

    createOutlet();
    createOutlet();
    createOutlet();
}

HoaMapUI::~HoaMapUI()
{
    f_manager->unsubscribe(this);
    f_manager.reset();
}

void HoaMapUI::okSize(t_rect* newrect)
{
    newrect->w = pd_clip_min(newrect->w, MIN_SIZE);
    newrect->h = pd_clip_min(newrect->h, MIN_SIZE);
}

void HoaMapUI::paint()
{
    drawBackground();
    drawSelection();
    drawSources();
    drawGroups();
}

void HoaMapUI::onBang()
{
    output();
}

void HoaMapUI::drawBackground()
{
    const auto rect = this->rect();
    auto p = bg_layer_.painter(rect);

    if (!p)
        return;

    p.setColor(prop_color_background);
    p.drawCircle(rect.w / 2., rect.w / 2., (rect.w / 2.) * (1. / MIN_ZOOM * prop_zoom) - 1.);
    p.fill();

    const t_rgba black = rgba_addContrast(prop_color_background, -0.14);

    /* Circles */
    double radius = prop_zoom * rect.w / 10.;
    for (int i = 5; i > 0; i--) {
        p.setLineWidth(2);
        p.setColor(prop_color_background);
        p.drawCircle(rect.w / 2 - 0.5, rect.w / 2 - 0.5, (double)i * radius - 1.);
        p.stroke();

        p.setLineWidth(1);
        p.setColor(black);
        p.drawCircle(rect.w / 2, rect.w / 2, (double)i * radius - 1.);
        p.stroke();
    }

    double ecart = prop_zoom * rect.w / 2.;
    if (ecart < 10 && ecart >= 5)
        ecart *= 2;
    else if (ecart < 5 && ecart > 2.5)
        ecart *= 4;
    else if (ecart < 2.5)
        ecart *= 8;

    ecart = (int)ecart;
    double i = 0;
    while (i < rect.w / 2.) {
        p.setLineWidth(2);
        p.setColor(prop_color_background);
        p.moveTo(0. - 0.5, rect.w / 2. - i - 0.5);
        p.drawLineTo(rect.w - 0.5, rect.w / 2. - i - 0.5);
        p.stroke();
        p.moveTo(0. - 0.5, rect.w / 2. + i - 0.5);
        p.drawLineTo(rect.w - 0.5, rect.w / 2. + i - 0.5);
        p.stroke();
        p.moveTo(rect.w / 2. - i - 0.5, 0. - 0.5);
        p.drawLineTo(rect.w / 2. - i - 0.5, rect.w - 0.5);
        p.stroke();
        p.moveTo(rect.w / 2. + i - 0.5, 0. - 0.5);
        p.drawLineTo(rect.w / 2. + i - 0.5, rect.w - 0.5);
        p.stroke();

        p.setLineWidth(1);
        p.setColor(black);
        p.moveTo(0., rect.w / 2. - i);
        p.drawLineTo(rect.w, rect.w / 2. - i);
        p.stroke();
        p.moveTo(0., rect.w / 2. + i);
        p.drawLineTo(rect.w, rect.w / 2. + i);
        p.stroke();
        p.moveTo(rect.w / 2. - i, 0.);
        p.drawLineTo(rect.w / 2. - i, rect.w);
        p.stroke();
        p.moveTo(rect.w / 2. + i, 0.);
        p.drawLineTo(rect.w / 2. + i, rect.w);
        p.stroke();
        i += ecart;
    }
}

void HoaMapUI::drawSelection()
{
    const auto r = rect();
    auto p = selection_.painter(r);

    if (!p)
        return;

    const t_rgba color_sel = rgba_addContrast(prop_color_background, -0.14);

    if (f_rect_selection_exist) {
        p.setLineWidth(1);
        p.setColor(color_sel);
        p.drawRect(f_rect_selection);
        p.stroke();
    }
}

void HoaMapUI::drawSources()
{
    const auto r = rect();
    auto p = sources_.painter(r);

    if (!p)
        return;

    const t_pt center = { r.w * 0.5f, r.h * 0.5f };

    const t_rgba color_sel = rgba_addContrast(prop_color_background, -0.14);
    const auto font_size = ebox_getfontsize(asEBox());
    const float source_size = font_size / 2.5;

    p.setLineWidth(1);

    std::vector<UITextLayout> jtl;
    jtl.reserve(f_manager->manager.getNumberOfSources());

    for (auto it = f_manager->manager.getFirstSource(); it != f_manager->manager.getLastSource(); it++) {
        t_pt src_pos;
        if (f_coord_view == sym_xy()) {
            src_pos.x = (it->second->getAbscissa() * prop_zoom + 1.) * center.x;
            src_pos.y = (-it->second->getOrdinate() * prop_zoom + 1.) * center.y;
        } else if (f_coord_view == sym_xz()) {
            src_pos.x = (it->second->getAbscissa() * prop_zoom + 1.) * center.x;
            src_pos.y = (-it->second->getHeight() * prop_zoom + 1.) * center.y;
        } else {
            src_pos.x = (it->second->getOrdinate() * prop_zoom + 1.) * center.x;
            src_pos.y = (-it->second->getHeight() * prop_zoom + 1.) * center.y;
        }

        // post("%f : (%f %f %f)", float(it->second->getElevation()), float(it->second->getAbscissa()), float(it->second->getOrdinate()), float(it->second->getHeight()));

        t_rgba src_color;
        src_color.red = it->second->getColor()[0];
        src_color.green = it->second->getColor()[1];
        src_color.blue = it->second->getColor()[2];
        src_color.alpha = it->second->getColor()[3];

        char description[250];
        if (it->second->getDescription().c_str()[0])
            sprintf(description, "%i : %s", (int)it->first, it->second->getDescription().c_str());
        else
            sprintf(description, "%i", (int)it->first);

        t_pt txt_pos;
        txt_pos.x = src_pos.x - 2. * source_size;
        txt_pos.y = src_pos.y - source_size - font_size - 1.;

        jtl.push_back(UITextLayout(&b_font));
        jtl.back().setColor(src_color);
        jtl.back().set(description, txt_pos.x, txt_pos.y, font_size * 10., font_size * 2.);
        jtl.back().setWrap(false);
        jtl.back().setAnchor(ETEXT_LEFT);
        jtl.back().setJustify(ETEXT_JCENTER);
        p.drawText(jtl.back());

        if (f_selected_source && f_selected_source->getIndex() == it->first) {
            p.setColor(color_sel);
            p.drawCircle(src_pos.x, src_pos.y, source_size * 1.5);
            p.fill();

            for (const auto& g : it->second->getGroups()) {
                t_pt group_pos;
                p.moveTo(src_pos.x, src_pos.y);

                if (f_coord_view == sym_xy()) {
                    group_pos.x = (g.second->getAbscissa() * prop_zoom + 1.) * center.x;
                    group_pos.y = (-g.second->getOrdinate() * prop_zoom + 1.) * center.y;
                } else if (f_coord_view == sym_xz()) {
                    group_pos.x = (g.second->getAbscissa() * prop_zoom + 1.) * center.x;
                    group_pos.y = (-g.second->getHeight() * prop_zoom + 1.) * center.y;
                } else {
                    group_pos.x = (g.second->getOrdinate() * prop_zoom + 1.) * center.x;
                    group_pos.y = (-g.second->getHeight() * prop_zoom + 1.) * center.y;
                }

                p.drawLineTo(group_pos.x, group_pos.y);
                p.stroke();
            }
        }

        using mf = hoa::Math<float>;

        if (!it->second->getMute()) {
            p.setColor(src_color);
            p.drawCircle(src_pos.x, src_pos.y, source_size);
            p.fill();
        } else {
            p.setColor(src_color);
            p.drawCircle(src_pos.x, src_pos.y, source_size);
            p.fill();
            p.setColor(rgba_red);
            p.drawCircle(src_pos.x, src_pos.y, source_size);
            p.stroke();
            p.moveTo(src_pos.x + mf::abscissa(source_size * 1., HOA_PI2 / 2.), src_pos.y + mf::ordinate(source_size * 1., HOA_PI2 / 2.));
            p.drawLineTo(src_pos.x + mf::abscissa(source_size * 1., HOA_PI2 * 5. / 2.), src_pos.y + mf::ordinate(source_size * 1., HOA_PI * 5. / 4.));
            p.stroke();
        }
    }
}

void HoaMapUI::drawGroups()
{
    const auto rect = this->rect();
    auto p = groups_.painter(rect);

    if (!p)
        return;

    t_pt center = { rect.w * 0.5f, rect.h * 0.5f };

    const auto color_sel = rgba_addContrast(prop_color_background, -0.14);
    const auto font_size = fontSize();
    const auto source_size = font_size / 2.f;

    std::vector<UITextLayout> jtl;
    jtl.reserve(f_manager->manager.getNumberOfGroups());

    p.setLineWidth(2);

    for (auto it = f_manager->manager.getFirstGroup(); it != f_manager->manager.getLastGroup(); it++) {
        t_pt src_pos;
        if (f_coord_view == sym_xy()) {
            src_pos.x = (it->second->getAbscissa() * prop_zoom + 1.) * center.x;
            src_pos.y = (-it->second->getOrdinate() * prop_zoom + 1.) * center.y;
        } else if (f_coord_view == sym_xz()) {
            src_pos.x = (it->second->getAbscissa() * prop_zoom + 1.) * center.x;
            src_pos.y = (-it->second->getHeight() * prop_zoom + 1.) * center.y;
        } else {
            src_pos.x = (it->second->getOrdinate() * prop_zoom + 1.) * center.x;
            src_pos.y = (-it->second->getHeight() * prop_zoom + 1.) * center.y;
        }

        t_rgba src_color;
        src_color.red = it->second->getColor()[0];
        src_color.green = it->second->getColor()[1];
        src_color.blue = it->second->getColor()[2];
        src_color.alpha = it->second->getColor()[3];

        char description[250] = { 0 };
        if (it->second->getDescription().c_str()[0])
            sprintf(description, "%i : %s", (int)it->first, it->second->getDescription().c_str());
        else
            sprintf(description, "%i", (int)it->first);

        t_pt txt_pos;
        txt_pos.x = src_pos.x - 2. * source_size;
        txt_pos.y = src_pos.y - source_size - font_size - 1.;

        jtl.push_back(UITextLayout(&b_font));
        jtl.back().setColor(src_color);
        jtl.back().set(description, txt_pos.x, txt_pos.y, font_size * 10., font_size * 2.);
        jtl.back().setWrap(false);
        jtl.back().setAnchor(ETEXT_LEFT);
        jtl.back().setJustify(ETEXT_JLEFT);
        p.drawText(jtl.back());

        if (f_selected_group && f_selected_group->getIndex() == it->first) {
            p.setColor(color_sel);
            p.drawCircle(src_pos.x, src_pos.y, source_size * 1.5);
            p.fill();

            for (auto& g : it->second->getSources()) {
                p.moveTo(src_pos.x, src_pos.y);

                t_pt groupDisplayPos;
                if (f_coord_view == sym_xy()) {
                    groupDisplayPos.x = (g.second->getAbscissa() * prop_zoom + 1.) * center.x;
                    groupDisplayPos.y = (-g.second->getOrdinate() * prop_zoom + 1.) * center.y;
                } else if (f_coord_view == sym_xz()) {
                    groupDisplayPos.x = (g.second->getAbscissa() * prop_zoom + 1.) * center.x;
                    groupDisplayPos.y = (-g.second->getHeight() * prop_zoom + 1.) * center.y;
                } else {
                    groupDisplayPos.x = (g.second->getOrdinate() * prop_zoom + 1.) * center.x;
                    groupDisplayPos.y = (-g.second->getHeight() * prop_zoom + 1.) * center.y;
                }

                p.drawLineTo(groupDisplayPos.x, groupDisplayPos.y);
                p.stroke();
            }
        }

        p.setColor(src_color);

        if (!it->second->getMute()) {
            p.setColor(src_color);
            p.drawCircle(src_pos.x, src_pos.y, source_size * 1.);
            p.stroke();
            p.drawText(jtl.back());
        } else {
            p.setColor(rgba_red);
            p.drawCircle(src_pos.x, src_pos.y, source_size);
            p.stroke();
            for (int j = 0; j < 2; j++) {
                using mf = hoa::Math<float>;
                p.moveTo(src_pos.x, src_pos.y);
                p.drawLineTo(src_pos.x + mf::abscissa(source_size * 1., HOA_2PI * j / 2. + HOA_PI2 / 2.), src_pos.y + mf::ordinate(source_size * 1., HOA_2PI * j / 2. + HOA_PI2 / 2.));
                p.stroke();
            }
        }
    }
}

void HoaMapUI::output()
{
    if (!output_enabled_)
        return;

    // output group mute state
    for (auto it = f_manager->manager.getFirstGroup(); it != f_manager->manager.getLastGroup(); it++) {
        AtomArray<3> data {
            it->first,
            sym_mute(),
            it->second->getMute()
        };
        listTo(GROUP_OUTLET, data.view());
    }
    // output source mute state
    for (auto it = f_manager->manager.getFirstSource(); it != f_manager->manager.getLastSource(); it++) {
        AtomArray<3> data {
            it->first,
            sym_mute(),
            it->second->getMute()
        };
        listTo(SOURCE_OUTLET, data.view());
    }
    if (f_output_mode == sym_polar()) {
        for (auto it = f_manager->manager.getFirstGroup(); it != f_manager->manager.getLastGroup(); it++) {
            AtomArray<5> data {
                it->first,
                sym_polar(),
                it->second->getRadius(),
                it->second->getAzimuth(),
                it->second->getElevation(),
            };
            listTo(GROUP_OUTLET, data.view());
        }
        for (auto it = f_manager->manager.getFirstSource(); it != f_manager->manager.getLastSource(); it++) {
            AtomArray<5> data {
                it->first,
                sym_polar(),
                it->second->getRadius(),
                it->second->getAzimuth(),
                it->second->getElevation(),
            };
            listTo(SOURCE_OUTLET, data.view());
        }
    } else {
        for (auto it = f_manager->manager.getFirstGroup(); it != f_manager->manager.getLastGroup(); it++) {
            AtomArray<5> data {
                it->first,
                sym_cartesian(),
                it->second->getAbscissa(),
                it->second->getOrdinate(),
                it->second->getHeight(),
            };
            listTo(GROUP_OUTLET, data.view());
        }
        for (auto it = f_manager->manager.getFirstSource(); it != f_manager->manager.getLastSource(); it++) {
            AtomArray<5> data {
                it->first,
                sym_cartesian(),
                it->second->getAbscissa(),
                it->second->getOrdinate(),
                it->second->getHeight(),
            };
            listTo(SOURCE_OUTLET, data.view());
        }
    }
}

void HoaMapUI::sendBindedMapUpdate(long flags)
{
    for (auto x : f_manager->subscribers) {
        if (x != this) {
            if (flags & BMAP_REDRAW) {
                x->sources_.invalidate();
                x->groups_.invalidate();
                x->redraw();
            }
            if (flags & BMAP_NOTIFY)
                ebox_notify(x->asEBox(), sym_modified());

            if (flags & BMAP_OUTPUT && output_enabled_)
                x->bangTo(0);
        }
    }
}

void HoaMapUI::selectElement(const t_pt& pt)
{
    const auto r = rect();

    t_pt cursor;
    cursor.x = ((pt.x / r.w * 2.) - 1.) / prop_zoom;
    cursor.y = ((-pt.y / r.h * 2.) + 1.) / prop_zoom;
    double distanceSelected = fontSize() / (prop_zoom * 2. * r.w);

    f_cursor_position.x = cursor.x;
    f_cursor_position.y = cursor.y;

    f_selected_source = NULL;
    f_selected_group = NULL;

    for (auto it = f_manager->manager.getFirstSource(); it != f_manager->manager.getLastSource(); it++) {
        t_pt displayed_coords;
        if (f_coord_view == sym_xy()) {
            displayed_coords.x = it->second->getAbscissa();
            displayed_coords.y = it->second->getOrdinate();
        } else if (f_coord_view == sym_xz()) {
            displayed_coords.x = it->second->getAbscissa();
            displayed_coords.y = it->second->getHeight();
        } else {
            displayed_coords.x = it->second->getOrdinate();
            displayed_coords.y = it->second->getHeight();
        }
        auto distanceSelected_test = hoa_pd_distance(displayed_coords.x, displayed_coords.y, cursor.x, cursor.y);

        if (distanceSelected_test <= distanceSelected) {
            distanceSelected = distanceSelected_test;
            f_selected_source = it->second;
        }
    }

    if (!f_selected_source) {
        for (auto it = f_manager->manager.getFirstGroup(); it != f_manager->manager.getLastGroup(); it++) {
            t_pt displayed_coords;
            if (f_coord_view == sym_xy()) {
                displayed_coords.x = it->second->getAbscissa();
                displayed_coords.y = it->second->getOrdinate();
            } else if (f_coord_view == sym_xz()) {
                displayed_coords.x = it->second->getAbscissa();
                displayed_coords.y = it->second->getHeight();
            } else {
                displayed_coords.x = it->second->getOrdinate();
                displayed_coords.y = it->second->getHeight();
            }
            auto distanceSelected_test = hoa_pd_distance(displayed_coords.x, displayed_coords.y, cursor.x, cursor.y);

            if (distanceSelected_test <= distanceSelected) {
                distanceSelected = distanceSelected_test;
                f_selected_group = it->second;
            }
        }
    }
}

void HoaMapUI::m_source(const AtomListView& lv)
{
    static const args::ArgChecker chk("INDEX:i PARAM:s OPTS:a*");
    if (!chk.check(lv, nullptr))
        return chk.usage();

    auto index = lv.intAt(0, 0);
    auto param = lv.symbolAt(1, &s_);

    int causeOutput = 1;
    if (index > 0) {
        auto tmp = f_manager->manager.newSource(ulong(index));

        if (param == sym_polar() || param == sym_pol()) {
            if (lv.size() >= 5 && lv[2].isFloat() && lv[3].isFloat() && lv[4].isFloat())
                tmp->setCoordinatesPolar(lv.floatAt(2, 0), lv.floatAt(3, 0), lv.floatAt(4, 0));
            else if (lv.size() >= 4 && lv[2].isFloat() && lv[3].isFloat())
                tmp->setCoordinatesPolar(index, lv.floatAt(2, 0), lv.floatAt(3, 0));
        } else if (param == sym_radius())
            tmp->setRadius(lv.floatAt(2, 0));
        else if (param == sym_azimuth())
            tmp->setAzimuth(lv.floatAt(2, 0));
        else if (param == sym_elevation())
            tmp->setElevation(lv.floatAt(2, 0));
        else if (param == sym_cartesian() || param == sym_car()) {
            if (lv.size() >= 5 && lv[2].isFloat() && lv[3].isFloat() && lv[4].isFloat())
                tmp->setCoordinatesCartesian(lv.floatAt(2, 0), lv.floatAt(3, 0), lv.floatAt(4, 0));
            else if (lv.size() >= 4 && lv[2].isFloat() && lv[3].isFloat())
                tmp->setCoordinatesCartesian(lv.floatAt(2, 0), lv.floatAt(3, 0));
        } else if (param == sym_abscissa())
            tmp->setAbscissa(lv.floatAt(2, 0));
        else if (param == sym_ordinate())
            tmp->setOrdinate(lv.floatAt(2, 0));
        else if (param == sym_height())
            tmp->setHeight(lv.floatAt(2, 0));
        else if (param == sym_remove()) {
            f_manager->manager.removeSource(ulong(index));
            AtomArray<3> data;
            data[0] = index;
            data[1] = sym_mute();
            data[2] = 1;
            listTo(0, data.view());
        } else if (param == sym_mute())
            tmp->setMute(lv.floatAt(2, 0));
        else if (param == sym_description()) {
            causeOutput = 0;
            char description[250];
            char number[250];
            if (lv.size() > 2 && lv[2].isSymbol()) {
                strcpy(description, lv[2].asSymbol()->s_name);
                strcat(description, " ");
                if (lv[2].asSymbol() == sym_remove()) {
                    tmp->setDescription("");
                    ebox_notify(asEBox(), sym_modified());
                    sources_.invalidate();
                    groups_.invalidate();
                    redraw();
                    return;
                }
            }
            for (int i = 3; i < lv.size(); i++) {
                if (lv[i].isSymbol()) {
                    strcat(description, lv[i].asSymbol()->s_name);
                    strcat(description, " ");
                } else if (lv[i].isInteger()) {
                    sprintf(number, "%d ", lv[i].asInt());
                    strcat(description, number);
                } else if (lv[i].isFloat()) {
                    sprintf(number, "%f ", lv[i].asFloat());
                    strcat(description, number);
                }
            }
            tmp->setDescription(description);
        } else if (param == sym_color() && lv.size() >= 5) {
            tmp->setColor(lv.floatAt(2, 0), lv.floatAt(3, 0), lv.floatAt(4, 0), lv.floatAt(5, 0));
            causeOutput = 0;
        } else {
            causeOutput = 0;
        }

        ebox_notify(asEBox(), sym_modified());
        sources_.invalidate();
        groups_.invalidate();
        redraw();

        if (causeOutput) {
            output();
            sendBindedMapUpdate(BMAP_OUTPUT);
        }

        sendBindedMapUpdate(BMAP_REDRAW | BMAP_NOTIFY);
    }
}

void HoaMapUI::m_group(const AtomListView& lv)
{
    using namespace hoa;

    static const args::ArgChecker chk("INDEX:i PARAM:s DATA:a*");
    if (!chk.check(lv, nullptr))
        return chk.usage();

    auto index = lv.intAt(0, 0);
    auto param = lv.symbolAt(1, &s_);

    int causeOutput = 1;
    if (index > 0) {
        bool newGroupCreated = false;
        hoa::Source::Group* tmp = f_manager->manager.getGroup(index);
        if (!tmp) {
            tmp = f_manager->manager.createGroup(index);
            newGroupCreated = true;
        }

        if (param == sym_set()) {
            for (int i = 2; i < lv.size(); i++) {
                ulong ind = ulong(lv.intAt(i, 0));
                if (ind > 0) {
                    auto* src = f_manager->manager.newSource(ind);
                    tmp->addSource(src);
                }
            }
        } else if (param == sym_polar() || param == sym_pol()) {
            if (lv.size() >= 5 && lv[2].isFloat() && lv[3].isFloat() && lv[4].isFloat())
                tmp->setCoordinatesPolar(lv.floatAt(2, 0), lv.floatAt(3, 0), lv.floatAt(4, 0));
            else if (lv.size() >= 4 && lv[2].isFloat() && lv[3].isFloat())
                tmp->setCoordinatesPolar(lv.floatAt(2, 0), lv.floatAt(3, 0));
        } else if (param == sym_azimuth())
            tmp->setAzimuth(lv.floatAt(2, 0));
        else if (param == sym_elevation())
            tmp->setElevation(lv.floatAt(2, 0));
        else if (param == sym_cartesian() || param == sym_car()) {
            if (lv.size() >= 5 && lv[2].isFloat() && lv[3].isFloat() && lv[4].isFloat())
                tmp->setCoordinatesCartesian(lv.floatAt(2, 0), lv.floatAt(3, 0), lv.floatAt(4, 0));
            else if (lv.size() >= 4 && lv[2].isFloat() && lv[3].isFloat())
                tmp->setCoordinatesCartesian(lv.floatAt(2, 0), lv.floatAt(3, 0));
        } else if (param == sym_abscissa())
            tmp->setAbscissa(lv.floatAt(2, 0));
        else if (param == sym_ordinate())
            tmp->setOrdinate(lv.floatAt(2, 0));
        else if (param == sym_height())
            tmp->setHeight(lv.floatAt(2, 0));
        else if (param == sym_relpolar()) {
            if (lv.size() >= 5 && lv[2].isFloat() && lv[3].isFloat() && lv[4].isFloat())
                tmp->setRelativeCoordinatesPolar(lv.floatAt(2, 0), lv.floatAt(3, 0), lv.floatAt(4, 0));
            else if (lv.size() >= 4 && lv[2].isFloat() && lv[3].isFloat())
                tmp->setRelativeCoordinatesPolar(lv.floatAt(2, 0), lv.floatAt(3, 0));
        } else if (param == sym_relradius()) {
            tmp->setRelativeRadius(lv.floatAt(2, 0) + tmp->getRadius());
        } else if (param == sym_relazimuth()) {
            if (f_coord_view == sym_xy()) {
                tmp->setRelativeAzimuth(lv.floatAt(2, 0) + tmp->getAzimuth());
            } else if (f_coord_view == sym_xz()) {
                t_pt source_display;
                double source_radius, source_azimuth, aAngleOffset = lv.floatAt(2, 0);
                std::map<ulong, Source*>& sourcesOfGroup = tmp->getSources();
                for (auto it = sourcesOfGroup.begin(); it != sourcesOfGroup.end(); it++) {
                    source_display.x = it->second->getAbscissa();
                    source_display.y = it->second->getHeight();
                    source_radius = Math<float>::radius(source_display.x, source_display.y);
                    source_azimuth = Math<float>::azimuth(source_display.x, source_display.y);
                    source_azimuth += aAngleOffset;

                    it->second->setAbscissa(Math<float>::abscissa(source_radius, source_azimuth));
                    it->second->setHeight(Math<float>::ordinate(source_radius, source_azimuth));
                }
            } else {
                t_pt source_display;
                double source_radius, source_azimuth, aAngleOffset = lv.floatAt(2, 0);
                std::map<ulong, Source*>& sourcesOfGroup = tmp->getSources();
                for (auto it = sourcesOfGroup.begin(); it != sourcesOfGroup.end(); it++) {
                    source_display.x = it->second->getOrdinate();
                    source_display.y = it->second->getHeight();
                    source_radius = Math<float>::radius(source_display.x, source_display.y);
                    source_azimuth = Math<float>::azimuth(source_display.x, source_display.y);
                    source_azimuth += aAngleOffset;

                    it->second->setOrdinate(Math<float>::abscissa(source_radius, source_azimuth));
                    it->second->setHeight(Math<float>::ordinate(source_radius, source_azimuth));
                }
            }
        } else if (param == sym_relelevation()) {
            tmp->setRelativeElevation(lv.floatAt(2, 0));
        } else if (param == sym_mute()) {
            tmp->setMute(lv.intAt(2, 0));
        } else if (param == sym_remove()) {
            f_manager->manager.removeGroup(index);
            AtomArray<3> data;
            data[0] = index;
            data[1] = sym_mute();
            data[2] = 1;
            listTo(0, data.view());
        } else if (param == sym_description()) {
            causeOutput = 0;
            char description[250];
            char number[250];
            if (lv.size() > 2 && lv[2].isSymbol()) {
                strcpy(description, lv.symbolAt(2, &s_)->s_name);
                strcat(description, " ");
                if (lv.symbolAt(2, &s_) == sym_remove()) {
                    tmp->setDescription("");
                    ebox_notify(asEBox(), sym_modified());
                    groups_.invalidate();
                    redraw();
                    return;
                }
            }
            for (int i = 3; i < lv.size(); i++) {
                if (lv[i].isSymbol()) {
                    strcat(description, lv.symbolAt(i, &s_)->s_name);
                    strcat(description, " ");
                } else if (lv[i].isInteger()) {
                    sprintf(number, "%d ", lv[i].asInt());
                    strcat(description, number);
                } else if (lv.isFloat()) {
                    sprintf(number, "%f ", lv[i].asFloat());
                    strcat(description, number);
                }
            }
            tmp->setDescription(description);
        } else if (param == sym_color() && lv.size() >= 6) {
            causeOutput = 0;
            tmp->setColor(lv.floatAt(2, 0), lv.floatAt(3, 0), lv.floatAt(4, 0), lv.floatAt(5, 0));
        } else {
            causeOutput = 0;
        }

        if (newGroupCreated) {
            if (!f_manager->manager.addGroup(tmp)) {
                delete tmp;
            }
        }
    }

    ebox_notify(asEBox(), sym_modified());
    groups_.invalidate();
    sources_.invalidate();
    redraw();

    if (causeOutput) {
        output();
        sendBindedMapUpdate(BMAP_OUTPUT);
    }
    sendBindedMapUpdate(BMAP_REDRAW | BMAP_NOTIFY);
}

void HoaMapUI::m_info()
{
    // Sources
    const auto num_src = f_manager->manager.getNumberOfSources();

    { // number
        AtomArray<3> data { sym_source(), sym_number(), num_src };
        listTo(INFO_OUTLET, data.view());
    }

    { // indexes
        AtomList res;
        res.fill(Atom(0.), num_src + 2);
        res[0] = sym_source();
        res[1] = sym_index();

        int j = 2;
        for (auto it = f_manager->manager.getFirstSource(); it != f_manager->manager.getLastSource(); it++) {
            res[j++] = it->first;
        }

        listTo(INFO_OUTLET, res);
    }

    { // mutes
        AtomArray<4> data { sym_source(), sym_mute(), 0., 0. };
        for (auto it = f_manager->manager.getFirstSource(); it != f_manager->manager.getLastSource(); it++) {
            data[2] = it->first;
            data[3] = it->second->getMute();
            listTo(INFO_OUTLET, data.view());
        }
    }

    // Groups
    auto num_groups = f_manager->manager.getNumberOfGroups();
    { // number
        AtomArray<3> data { sym_group(), sym_number(), num_groups };
        listTo(INFO_OUTLET, data.view());
    }

    { // indexes
        AtomList res;
        res.fill(Atom(0.), num_groups + 2);
        res[0] = sym_group();
        res[1] = sym_index();

        int j = 2;
        for (auto it = f_manager->manager.getFirstGroup(); it != f_manager->manager.getLastGroup(); it++) {
            res[j++] = it->first;
        }

        listTo(INFO_OUTLET, res);
    }

    AtomList res;
    for (auto it = f_manager->manager.getFirstGroup(); it != f_manager->manager.getLastGroup(); it++) {
        auto N = it->second->getNumberOfSources() + 3;
        res.resizePad(N, Atom(0.));

        res[0] = sym_group();
        res[1] = sym_source();
        res[2] = it->first;

        int j = 3;
        for (auto& kv : it->second->getSources())
            res[j++] = kv.first;

        listTo(INFO_OUTLET, res);
    }

    { // mutes
        AtomArray<4> data { sym_group(), sym_mute(), 0., 0. };
        for (auto it = f_manager->manager.getFirstGroup(); it != f_manager->manager.getLastGroup(); it++) {
            data[2] = it->first;
            data[3] = it->second->getMute();
            listTo(INFO_OUTLET, data.view());
        }
    }
}

void HoaMapUI::updateAllAndOutput()
{
    sources_.invalidate();
    groups_.invalidate();
    ebox_notify(asEBox(), sym_modified());
    redraw();
    output();
    sendBindedMapUpdate(BMAP_REDRAW | BMAP_OUTPUT | BMAP_NOTIFY);
}

t_symbol* HoaMapUI::makeBindSymbol(t_symbol* sym) const
{
    auto cnv = canvas_root(canvas());
    if (!cnv || !sym || sym == &s_ || sym == s_null)
        return nullptr;

    char buf[MAXPDSTRING];
    auto end = fmt::format_to(buf, "p{}_{}_{}", static_cast<const void*>(cnv), sym->s_name, ODD_BINDING_SUFFIX);
    *end = '\0';
    return gensym(buf);
}

void HoaMapUI::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    f_rect_selection_exist = false;
    f_rect_selection.w = f_rect_selection.h = 0.;

    selectElement(pt);

    if (!f_selected_source && !f_selected_group) {
        f_rect_selection.x = pt.x;
        f_rect_selection.y = pt.y;
        f_rect_selection_exist = true;
    }
}

void HoaMapUI::onMouseUp(t_object* view, const t_pt& pt, long modifiers)
{
    using namespace hoa;
    const auto r = rect();

    mouse_was_dragging_ = false;

    int causeOutput = 0;
    int causeRedraw = 0;
    int causeNotify = 0;

    if (f_rect_selection_exist) {
        ulong indexOfNewGroup = 1;
        for (auto it = f_manager->manager.getFirstGroup(); it != f_manager->manager.getLastGroup(); it++) {
            if (it->first != indexOfNewGroup)
                break;

            indexOfNewGroup++;
        }

        double x1 = ((f_rect_selection.x / r.w * 2.) - 1.) / prop_zoom;
        double x2 = (((f_rect_selection.x + f_rect_selection.w) / r.w * 2.) - 1.) / prop_zoom;
        double y1 = ((-f_rect_selection.y / r.h * 2.) + 1.) / prop_zoom;
        double y2 = (((-f_rect_selection.y - f_rect_selection.h) / r.h * 2.) + 1.) / prop_zoom;

        bool newGroupCreated = false;
        auto tmp = f_manager->manager.getGroup(indexOfNewGroup);
        if (!tmp) {
            tmp = f_manager->manager.createGroup(indexOfNewGroup);
            newGroupCreated = true;
        }
        for (auto it = f_manager->manager.getFirstSource(); it != f_manager->manager.getLastSource(); it++) {
            t_pt screen_source_coord;
            if (f_coord_view == sym_xy()) {
                screen_source_coord.x = it->second->getAbscissa();
                screen_source_coord.y = it->second->getOrdinate();
            } else if (f_coord_view == sym_xz()) {
                screen_source_coord.x = it->second->getAbscissa();
                screen_source_coord.y = it->second->getHeight();
            } else {
                screen_source_coord.x = it->second->getOrdinate();
                screen_source_coord.y = it->second->getHeight();
            }

            if (((screen_source_coord.x > x1 && screen_source_coord.x < x2)
                    || (screen_source_coord.x < x1 && screen_source_coord.x > x2))
                && ((screen_source_coord.y > y1 && screen_source_coord.y < y2)
                    || (screen_source_coord.y < y1 && screen_source_coord.y > y2))) {
                tmp->addSource(it->second);
                causeOutput = causeRedraw = causeNotify = 1;
            }
        }

        if (newGroupCreated) {
            if (!f_manager->manager.addGroup(tmp)) {
                delete tmp;
            }
        }
    }

    f_rect_selection_exist = false;
    f_rect_selection.w = f_rect_selection.h = 0;

    selection_.invalidate();
    redraw();

    if (causeNotify) {
        ebox_notify(asEBox(), sym_modified());
        sendBindedMapUpdate(BMAP_NOTIFY);
    }

    if (causeRedraw) {
        sources_.invalidate();
        groups_.invalidate();
        redraw();
        sendBindedMapUpdate(BMAP_REDRAW);
    }

    if (causeOutput) {
        output();
        sendBindedMapUpdate(BMAP_OUTPUT);
    }
}

void HoaMapUI::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
    using namespace hoa;
    using mf = Math<float>;
    const auto r = rect();

    t_pt cursor;
    cursor.x = ((pt.x / r.w * 2.) - 1.) / prop_zoom;
    cursor.y = ((-pt.y / r.h * 2.) + 1.) / prop_zoom;

    int causeOutput = 0;
    int causeRedraw = 0;
    int causeNotify = 0;

    if (f_selected_source) {
        const bool mode_set_radius = modifiers & EMOD_ALT;
        const bool mode_set_abscissa = (modifiers & EMOD_CTRL) && !(modifiers & EMOD_SHIFT);
        const bool mode_set_ordinate = (modifiers & EMOD_CTRL) && (modifiers & EMOD_SHIFT);
        const bool mode_set_both = (modifiers & EMOD_SHIFT) && !(modifiers & EMOD_CTRL);

        if (mode_set_both) {
            if (f_coord_view == sym_xy()) {
                f_selected_source->setAzimuth(mf::azimuth(cursor.x, cursor.y));
            } else if (f_coord_view == sym_xz()) {
                auto source_radius = mf::radius(f_selected_source->getAbscissa(), f_selected_source->getHeight());
                auto mouse_azimuth = mf::wrap_twopi(mf::azimuth(cursor.x, cursor.y));

                f_selected_source->setAbscissa(mf::abscissa(source_radius, mouse_azimuth));
                f_selected_source->setHeight(mf::ordinate(source_radius, mouse_azimuth));
            } else {
                auto source_radius = mf::radius(f_selected_source->getOrdinate(), f_selected_source->getHeight());
                auto mouse_azimuth = mf::wrap_twopi(mf::azimuth(cursor.x, cursor.y));

                f_selected_source->setOrdinate(mf::abscissa(source_radius, mouse_azimuth));
                f_selected_source->setHeight(mf::ordinate(source_radius, mouse_azimuth));
            }

            causeOutput = causeRedraw = causeNotify = 1;
        } else if (mode_set_radius) {
            f_selected_source->setRadius(mf::radius(cursor.x, cursor.y));
            causeOutput = causeRedraw = causeNotify = 1;
        } else if (mode_set_abscissa) {
            if (f_coord_view == sym_xy() || f_coord_view == sym_xz())
                f_selected_source->setAbscissa(cursor.x);
            else
                f_selected_source->setOrdinate(cursor.x);
            causeOutput = causeRedraw = causeNotify = 1;
        } else if (mode_set_ordinate) {
            if (f_coord_view == sym_xy())
                f_selected_source->setOrdinate(cursor.y);
            else
                f_selected_source->setHeight(cursor.y);
            causeOutput = causeRedraw = causeNotify = 1;
        } else {
            if (f_coord_view == sym_xy()) {
                f_selected_source->setCoordinatesCartesian(cursor.x, cursor.y);
            } else if (f_coord_view == sym_xz()) {
                f_selected_source->setCoordinatesCartesian(cursor.x, f_selected_source->getOrdinate(), cursor.y);
            } else {
                f_selected_source->setCoordinatesCartesian(f_selected_source->getAbscissa(), cursor.x, cursor.y);
            }
            causeOutput = causeRedraw = causeNotify = 1;
        }
    } else if (f_selected_group) {
        if ((modifiers & EMOD_SHIFT) && !(modifiers & EMOD_ALT) && !(modifiers & EMOD_CTRL)) {
            if (f_coord_view == sym_xy()) {
                f_selected_group->setRelativeAzimuth(mf::azimuth(cursor.x, cursor.y));
            } else if (f_coord_view == sym_xz()) {
                if (mouse_was_dragging_) {
                    t_pt source_display;
                    float source_radius, source_azimuth, mouse_azimuth, mouse_azimuth_prev;
                    mouse_azimuth = mf::wrap_twopi(mf::azimuth(cursor.x, cursor.y));
                    mouse_azimuth_prev = mf::wrap_twopi(mf::azimuth(f_cursor_position.x, f_cursor_position.y));

                    auto& sourcesOfGroup = f_selected_group->getSources();
                    for (auto it = sourcesOfGroup.begin(); it != sourcesOfGroup.end(); it++) {
                        source_display.x = it->second->getAbscissa();
                        source_display.y = it->second->getHeight();
                        source_radius = mf::radius(source_display.x, source_display.y);
                        source_azimuth = mf::azimuth(source_display.x, source_display.y);
                        source_azimuth += mouse_azimuth - mouse_azimuth_prev;

                        it->second->setAbscissa(mf::abscissa(source_radius, source_azimuth));
                        it->second->setHeight(mf::ordinate(source_radius, source_azimuth));
                    }
                }
            } else {
                if (mouse_was_dragging_) {
                    t_pt source_display;
                    float source_radius, source_azimuth, mouse_azimuth, mouse_azimuth_prev;
                    mouse_azimuth = mf::wrap_twopi(mf::azimuth(cursor.x, cursor.y));
                    mouse_azimuth_prev = mf::wrap_twopi(mf::azimuth(f_cursor_position.x, f_cursor_position.y));

                    auto& sourcesOfGroup = f_selected_group->getSources();
                    for (auto it = sourcesOfGroup.begin(); it != sourcesOfGroup.end(); it++) {
                        source_display.x = it->second->getOrdinate();
                        source_display.y = it->second->getHeight();
                        source_radius = mf::radius(source_display.x, source_display.y);
                        source_azimuth = mf::azimuth(source_display.x, source_display.y);
                        source_azimuth += mouse_azimuth - mouse_azimuth_prev;

                        it->second->setOrdinate(mf::abscissa(source_radius, source_azimuth));
                        it->second->setHeight(mf::ordinate(source_radius, source_azimuth));
                    }
                }
            }
            causeOutput = causeRedraw = causeNotify = 1;
        } else if ((modifiers & EMOD_ALT) && !(modifiers & EMOD_SHIFT)) {
            f_selected_group->setRelativeRadius(mf::radius(cursor.x, cursor.y));
            causeOutput = causeRedraw = causeNotify = 1;
        } else if ((modifiers & EMOD_ALT) && (modifiers & EMOD_SHIFT)) {
            if (f_coord_view == sym_xy()) {
                f_selected_group->setRelativeRadius(mf::radius(cursor.x, cursor.y));
                f_selected_group->setRelativeAzimuth(mf::azimuth(cursor.x, cursor.y));
            } else if (f_coord_view == sym_xz()) {
                if (mouse_was_dragging_) {
                    t_pt source_display;
                    double source_radius, source_azimuth, mouse_azimuth, mouse_azimuth_prev, mouse_radius, mouse_radius_prev;
                    mouse_radius = pd_clip_min(mf::radius(cursor.x, cursor.y), 0);
                    mouse_radius_prev = pd_clip_min(mf::radius(f_cursor_position.x, f_cursor_position.y), 0);
                    mouse_azimuth = mf::wrap_twopi(mf::azimuth(cursor.x, cursor.y));
                    mouse_azimuth_prev = mf::wrap_twopi(mf::azimuth(f_cursor_position.x, f_cursor_position.y));

                    auto& sourcesOfGroup = f_selected_group->getSources();
                    for (auto it = sourcesOfGroup.begin(); it != sourcesOfGroup.end(); it++) {
                        source_display.x = it->second->getAbscissa();
                        source_display.y = it->second->getHeight();
                        source_radius = mf::radius(source_display.x, source_display.y);
                        source_radius += mouse_radius - mouse_radius_prev;
                        source_azimuth = mf::azimuth(source_display.x, source_display.y);
                        source_azimuth += mouse_azimuth - mouse_azimuth_prev;

                        it->second->setAbscissa(mf::abscissa(source_radius, source_azimuth));
                        it->second->setHeight(mf::ordinate(source_radius, source_azimuth));
                    }
                }
            } else {
                if (mouse_was_dragging_) {
                    t_pt source_display;
                    double source_radius, source_azimuth, mouse_azimuth, mouse_azimuth_prev, mouse_radius, mouse_radius_prev;
                    mouse_radius = pd_clip_min(mf::radius(cursor.x, cursor.y), 0);
                    mouse_radius_prev = pd_clip_min(mf::radius(f_cursor_position.x, f_cursor_position.y), 0);
                    mouse_azimuth = mf::wrap_twopi(mf::azimuth(cursor.x, cursor.y));
                    mouse_azimuth_prev = mf::wrap_twopi(mf::azimuth(f_cursor_position.x, f_cursor_position.y));

                    auto& sourcesOfGroup = f_selected_group->getSources();
                    for (auto it = sourcesOfGroup.begin(); it != sourcesOfGroup.end(); it++) {
                        source_display.x = it->second->getOrdinate();
                        source_display.y = it->second->getHeight();
                        source_radius = mf::radius(source_display.x, source_display.y);
                        source_radius += mouse_radius - mouse_radius_prev;
                        source_azimuth = mf::azimuth(source_display.x, source_display.y);
                        source_azimuth += mouse_azimuth - mouse_azimuth_prev;

                        it->second->setOrdinate(mf::abscissa(source_radius, source_azimuth));
                        it->second->setHeight(mf::ordinate(source_radius, source_azimuth));
                    }
                }
            }
            causeOutput = causeRedraw = causeNotify = 1;
        } else if ((modifiers & EMOD_CTRL) && !(modifiers & EMOD_SHIFT)) {
            if (f_coord_view == sym_xy() || f_coord_view == sym_xz())
                f_selected_group->setAbscissa(cursor.x);
            else
                f_selected_group->setOrdinate(cursor.x);
            causeOutput = causeRedraw = causeNotify = 1;
        } else if ((modifiers & EMOD_CTRL) && (modifiers & EMOD_SHIFT)) {
            if (f_coord_view == sym_xy())
                f_selected_group->setOrdinate(cursor.y);
            else
                f_selected_group->setHeight(cursor.y);
            causeOutput = causeRedraw = causeNotify = 1;
        } else {
            if (f_coord_view == sym_xy()) {
                f_selected_group->setCoordinatesCartesian(cursor.x, cursor.y);
            } else if (f_coord_view == sym_xz()) {
                f_selected_group->setAbscissa(cursor.x);
                f_selected_group->setHeight(cursor.y);
            } else {
                f_selected_group->setOrdinate(cursor.x);
                f_selected_group->setHeight(cursor.y);
            }
            causeOutput = causeRedraw = causeNotify = 1;
        }
    } else {
        f_rect_selection.w = pt.x - f_rect_selection.x;
        f_rect_selection.h = pt.y - f_rect_selection.y;

        selection_.invalidate();
        redraw();
        causeOutput = causeRedraw = causeNotify = 0;
    }

    f_cursor_position.x = cursor.x;
    f_cursor_position.y = cursor.y;
    mouse_was_dragging_ = true;

    if (causeNotify) {
        ebox_notify(asEBox(), sym_modified());
        sendBindedMapUpdate(BMAP_NOTIFY);
    }

    if (causeRedraw) {
        sources_.invalidate();
        groups_.invalidate();
        redraw();
        sendBindedMapUpdate(BMAP_REDRAW);
    }

    if (causeOutput) {
        output();
        sendBindedMapUpdate(BMAP_OUTPUT);
    }
}

void HoaMapUI::onMouseMove(t_object* view, const t_pt& pt, long modifiers)
{
    selectElement(pt);

    if (f_selected_source || f_selected_group)
        setCursor(t_cursor::ECURSOR_HAND);
    else
        setCursor(t_cursor::ECURSOR_CENTER_PTR);

    sources_.invalidate();
    groups_.invalidate();
    redraw();
}

void HoaMapUI::onMouseWheel(const t_pt& pt, long modifiers, float delta)
{
    if (modifiers == EMOD_ALT) {
        prop_zoom = clip<float>(prop_zoom + delta / 100., MIN_ZOOM, MAX_ZOOM);

        bg_layer_.invalidate();
        sources_.invalidate();
        groups_.invalidate();
        redraw();
    }
}

void HoaMapUI::showPopup(const t_pt& pt, const t_pt& abs_pt)
{
    if (f_selected_group) {
        popup_menu_list_.clear();
        PopupMenuCallbacks menu("group");
        menu.addItem(_("Group menu"), {});
        menu.addSeparator();
        menu.addItem(_("Remove group"), [this](const t_pt&) {
            AtomArray<3> data { f_selected_group->getIndex(), sym_mute(), 1 };
            listTo(GROUP_OUTLET, data.view());
            f_manager->manager.removeGroup(f_selected_group->getIndex());
            updateAllAndOutput();
            return true;
        });
        menu.addItem(_("Remove group and sources"), [this](const t_pt&) {
            f_selected_group->setMute(true);
            output();
            sendBindedMapUpdate(BMAP_OUTPUT);
            f_manager->manager.removeGroupWithSources(f_selected_group->getIndex());
            updateAllAndOutput();
            return true;
        });
        menu.addItem(_("Mute group"), [this](const t_pt&) {
            f_selected_group->setMute(true);
            updateAllAndOutput();
            return true;
        });
        if (f_selected_group->getSubMute())
            menu.addItem(_("Unmute group"), [this](const t_pt&) {
                f_selected_group->setMute(false);
                updateAllAndOutput();
                return true;
            });

        popup_menu_list_.push_back(menu);
        showDefaultPopupMenu(pt, abs_pt);

    } else if (f_selected_source) {
        popup_menu_list_.clear();
        PopupMenuCallbacks menu("source");
        menu.addItem(_("Source menu"), {});
        menu.addSeparator();
        menu.addItem(_("Remove source"), [this](const t_pt& pos) {
            AtomArray<3> data { f_selected_source->getIndex(), sym_mute(), 1 };
            listTo(SOURCE_OUTLET, data.view());
            f_manager->manager.removeSource(f_selected_source->getIndex());
            updateAllAndOutput();
            return true;
        });

        auto mute_cb = [this](const t_pt& pos) {
            f_selected_source->setMute(!f_selected_source->getMute());
            updateAllAndOutput();
            return true;
        };

        if (f_selected_source->getMute())
            menu.addItem(_("Unmute source"), mute_cb);
        else
            menu.addItem(_("Mute source"), mute_cb);

        popup_menu_list_.push_back(menu);
        showDefaultPopupMenu(pt, abs_pt);
    } else {
        popup_menu_list_.clear();
        PopupMenuCallbacks menu("default");
        menu.addItem(_("Main menu"), {});
        menu.addSeparator();
        menu.addItem(_("Add source"), [this](const t_pt&) {
            ulong index = 1;
            for (auto it = f_manager->manager.getFirstSource(); it != f_manager->manager.getLastSource(); it++) {
                if (it->first != index)
                    break;
                index++;
            }

            f_manager->manager.newSource(index);

            updateAllAndOutput();
            return true;
        });
        menu.addItem(_("Clear all"), [this](const t_pt& pt) {
            m_clear_all({});
            return true;
        });

        popup_menu_list_.push_back(menu);
        showDefaultPopupMenu(pt, abs_pt);
    }
}

void HoaMapUI::loadPreset(size_t idx)
{
    auto lv = PresetStorage::instance().listValueAt(presetId(), idx);
    onList(lv);
}

void HoaMapUI::storePreset(size_t idx)
{
    //    StaticAtomList<HOA_MAX_PLANEWAVES> lv;
    //    for (int i = 0; i < prop_nchan; i++)
    //        lv.push_back(chan_values_[i]);

    //    PresetStorage::instance().setListValueAt(presetId(), idx, lv.view());
}

void HoaMapUI::interpPreset(t_float idx)
{
    //    StaticAtomList<HOA_MAX_PLANEWAVES> def;
    //    for (int i = 0; i < prop_nchan; i++)
    //        def.push_back(chan_values_[i]);

    //    onList(PresetStorage::instance().interListValue(presetId(), idx, def.view()));
}

void HoaMapUI::m_clear_all(const AtomListView& lv)
{
    // mute all source and output before clearing them to notify hoa.#.map~
    for (auto it = f_manager->manager.getFirstSource(); it != f_manager->manager.getLastSource(); it++)
        it->second->setMute(true);

    output();
    sendBindedMapUpdate(BMAP_OUTPUT);

    // now we can clear, then notify, output and redraw all maps
    f_manager->manager.clear();

    ebox_notify(asEBox(), sym_modified());
    sources_.invalidate();
    groups_.invalidate();
    redraw();
    output();
    sendBindedMapUpdate(BMAP_REDRAW | BMAP_OUTPUT | BMAP_NOTIFY);
}

void HoaMapUI::m_set(const AtomListView& lv)
{
    output_enabled_ = false;
    if (lv.size() > 0) {
        t_symbol* msgtype = lv[0].asSymbol();
        if (msgtype == sym_source())
            m_source(lv.subView(1));
        else if (msgtype == sym_group())
            m_group(lv.subView(1));
    }
    output_enabled_ = true;
}

void HoaMapUI::m_set_zoom(const AtomListView& lv)
{
    if (!lv.isFloat())
        return;

    prop_zoom = pd_clip_minmax(lv.floatAt(0, 0), MIN_ZOOM, MAX_ZOOM);

    bg_layer_.invalidate();
    sources_.invalidate();
    groups_.invalidate();
    redraw();
}

void HoaMapUI::m_set_bind(const AtomListView& lv)
{
    using SharedManagerWeak = std::weak_ptr<HoaManagerData>;

    static std::unordered_map<t_symbol*, SharedManagerWeak> map_;

    if (!lv.isSymbol()) {
        auto bind_sym = makeBindSymbol(f_binding_name);
        if (bind_sym) {
            auto it = map_.find(bind_sym);
            if (it != map_.end()) {
                if (!it->second.expired()) { // make clone
                    f_manager->unsubscribe(this);
                    f_manager = f_manager->clone();
                    f_manager->subscribe(this);
                }
            }
        }

        f_binding_name = s_null;
        return;
    }

    auto new_binding_name = lv.asSymbol();

    if (new_binding_name != f_binding_name) {
        auto bind_sym = makeBindSymbol(f_binding_name);
        if (bind_sym) { // detach from old
            auto it = map_.find(bind_sym);
            if (it != map_.end()) {
                if (!it->second.expired()) { // make clone
                    f_manager->unsubscribe(this);
                    f_manager = f_manager->clone();
                    f_manager->subscribe(this);
                }
            }
        }

        f_binding_name = s_null;

        bind_sym = makeBindSymbol(new_binding_name);
        if (bind_sym) { // bind to new
            auto it = map_.find(bind_sym);
            if (it == map_.end() || it->second.expired()) {
                map_[bind_sym] = f_manager;
            } else {
                f_manager = it->second.lock();
                f_manager->subscribe(this);
            }

            f_binding_name = new_binding_name;

            sources_.invalidate();
            groups_.invalidate();
            redraw();
            output();
        }
    }
}

void HoaMapUI::setup()
{
    UIObjectFactory<HoaMapUI> obj("hoa.map.ui", EBOX_GROWLINK);
    obj.setDefaultSize(225, 225);

    obj.useBang();
    obj.usePresets();
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_DRAG | UI_MOUSE_UP | UI_MOUSE_MOVE | UI_MOUSE_WHEEL);
    obj.usePopup();

    obj.addMethod("clear_all", &HoaMapUI::m_clear_all);
    obj.addMethod("group", &HoaMapUI::m_group);
    obj.addMethod("info", &HoaMapUI::m_info);
    obj.addMethod("set", &HoaMapUI::m_set);
    obj.addMethod("source", &HoaMapUI::m_source);

    obj.addMenuProperty("view", _("Coordinate View"), "xy", &HoaMapUI::f_coord_view, "xy xz yz", _("Main"));
    obj.addMenuProperty("outputmode", _("Output Mode"), "polar", &HoaMapUI::f_output_mode, "polar cartesian", _("Behavior"));

    obj.addFloatProperty("zoom", _("Zoom"), 0.35, &HoaMapUI::prop_zoom, _("Behavior"));
    obj.setPropertyAccessor("zoom", &HoaMapUI::m_get_zoom, &HoaMapUI::m_set_zoom);

    obj.addSymbolProperty("mapname", _("Map Name"), "(null)", &HoaMapUI::f_binding_name, _("Main"));
    obj.setPropertyAccessor("mapname", &HoaMapUI::m_get_bind, &HoaMapUI::m_set_bind);

    //    eclass_addmethod(c, (method) hoa_map_preset,        "preset",         A_CANT,  0);
    //    eclass_addmethod(c, (method) hoa_map_interpolate,   "interpolate",    A_CANT,  0);
    //    eclass_addmethod(c, (method) hoa_map_sourcesPreset, "sources_preset",  A_GIMME, 0);
}
}

void setup_spat_hoa_map_ui()
{
    ceammc::HoaMapUI::setup();
}
