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
#include "hoa_2d_space.h"
#include "ceammc_containers.h"
#include "ceammc_convert.h"
#include "ceammc_preset.h"
#include "ceammc_ui.h"
#include "hoa_common.h"
#include "hoa_math_compat.h"

static const int HOA_DISPLAY_NPOINTS = 65;
static const float HOA_CONTRAST_DARKER = 0.2;
static const float HOA_CONTRAST_LIGHTER = 0.2;

namespace ceammc {

constexpr int MIN_SIZE = 20;

Hoa2dSpace::Hoa2dSpace()
    : prop_nchan(4)
    , space_layer_(asEBox(), gensym("space_layer"))
    , points_layer_(asEBox(), gensym("points_layer"))
{
    createOutlet();
}

void Hoa2dSpace::init(t_symbol* s, const AtomListView& args, bool usePresets)
{
    UIObject::init(s, args, usePresets);

    // first positional argument handling
    if (!args.empty() && args[0].isFloat())
        propSetNumChan(clip<t_float>(args[0].asFloat(), HOA_MIN_PLANEWAVES, HOA_MAX_PLANEWAVES));
}

void Hoa2dSpace::okSize(t_rect* newrect)
{
    newrect->w = pd_clip_min(newrect->w, MIN_SIZE);
    newrect->h = pd_clip_min(newrect->h, MIN_SIZE);
}

void Hoa2dSpace::paint()
{
    drawBackground();
    drawSpace();
    drawPoints();
}

void Hoa2dSpace::drawBackground()
{
    t_matrix transform;
    t_rgba black = rgba_addContrast(prop_color_border, -HOA_CONTRAST_DARKER);
    t_rgba white = rgba_addContrast(prop_color_background, HOA_CONTRAST_LIGHTER);

    auto r = rect();
    auto p = bg_layer_.painter(r);

    if (!p)
        return;

    auto center = width() / 2;
    auto radius = center * 0.95;
    egraphics_matrix_init(&transform, 1, 0, 0, -1, center, center);
    p.setMatrix(transform);

    // draw beams
    for (int i = 0; i < prop_nchan; i++) {
        auto angle = ((float(i) - 0.5) / prop_nchan) * HOA_2PI;

        auto rx = hoa::Math<float>::abscissa(radius, angle);
        auto ry = hoa::Math<float>::ordinate(radius, angle);
        auto x1 = rx * 0.2;
        auto y1 = ry * 0.2;

        p.moveTo(x1, y1);
        p.drawLineTo(rx, ry);
        p.setLineWidth(3);
        p.setColor(white);
        p.strokePreserve();

        p.setColor(black);
        p.setLineWidth(1);
        p.stroke();
    }

    // draw circles
    for (int i = 5; i > 0; i--) {
        p.drawCircle(0, 0, (double)i * 0.2 * radius);
        p.setLineWidth(3);
        p.setColor(white);
        p.strokePreserve();
        p.setColor(black);
        p.setLineWidth(1);
        p.stroke();
    }
}

static double cosine_interpolation(double y1, double y2, float mu)
{
    double mu2 = (1 - cos(mu * HOA_PI)) / 2;
    return (y1 * (1 - mu2) + y2 * mu2);
}

void Hoa2dSpace::drawSpace()
{
    auto r = rect();
    auto p = space_layer_.painter(r);

    if (!p)
        return;

    float center = width() / 2;
    float radius = center * 0.95;
    t_matrix transform;
    egraphics_matrix_init(&transform, 1, 0, 0, -1, center, center);
    p.setMatrix(transform);
    p.setLineWidth(2);
    p.setColor(prop_space_color);

    auto diff = prop_max - prop_min;
    auto ratio = radius / 5.;
    for (int i = 0; i < prop_nchan; i++) {
        chan_radius_[i] = (chan_values_[i] - prop_min) / diff * 4 * ratio + ratio;
    }

    auto abscissa = hoa::Math<float>::abscissa(chan_radius_[0], 0);
    auto ordinate = hoa::Math<float>::ordinate(chan_radius_[0], 0);
    p.moveTo(abscissa, ordinate);
    for (int i = 1; i < HOA_DISPLAY_NPOINTS; i++) {
        int index1 = (double)i / (double)HOA_DISPLAY_NPOINTS * prop_nchan;
        auto index2 = index1 + 1;

        auto mu = (double)index1 / (double)prop_nchan * (double)HOA_DISPLAY_NPOINTS;
        mu = (double)(i - mu) / ((double)HOA_DISPLAY_NPOINTS / (double)prop_nchan);
        if (index2 >= prop_nchan)
            index2 = 0;

        radius = cosine_interpolation(chan_radius_[index1], chan_radius_[index2], mu);
        auto angle = (double)i / (double)HOA_DISPLAY_NPOINTS * HOA_2PI;
        abscissa = hoa::Math<float>::abscissa(radius, angle);
        ordinate = hoa::Math<float>::ordinate(radius, angle);
        p.drawLineTo(abscissa, ordinate);
    }

    p.closePath();
    p.fillPreserve();
    p.stroke();
}

void Hoa2dSpace::drawPoints()
{
    auto r = rect();
    auto p = points_layer_.painter(r);

    if (!p)
        return;

    float center = width() / 2;
    float radius = center * 0.95;

    t_matrix transform;
    egraphics_matrix_init(&transform, 1, 0, 0, -1, center, center);
    p.setMatrix(transform);
    p.setColor(prop_pt_color);

    for (int i = 0; i < prop_nchan; i++) {
        auto radius = chan_radius_[i] - 3.5;
        auto angle = (float(i) / prop_nchan) * HOA_2PI;
        auto abscissa = hoa::Math<float>::abscissa(radius, angle);
        auto ordinate = hoa::Math<float>::ordinate(radius, angle);
        p.drawCircle(abscissa, ordinate, 3.);
        p.fill();
    }
}

void Hoa2dSpace::output()
{
    StaticAtomList<HOA_MAX_PLANEWAVES> v;
    for (int i = 0; i < prop_nchan; i++)
        v.push_back(chan_values_[i]);

    listTo(0, v.view());
    send(v.view());
}

t_float Hoa2dSpace::propNumChan() const
{
    return prop_nchan;
}

void Hoa2dSpace::propSetNumChan(t_float v)
{
    auto new_val = clip<int, HOA_MIN_PLANEWAVES, HOA_MAX_PLANEWAVES>(v);
    if (new_val != prop_nchan) {
        prop_nchan = new_val;
        std::fill(chan_values_.begin(), chan_values_.begin() + new_val, 0);
        bg_layer_.invalidate();
        space_layer_.invalidate();
        points_layer_.invalidate();
        redraw();
    }
}

void Hoa2dSpace::onList(const AtomListView& lv)
{
    auto N = std::min<size_t>(lv.size(), prop_nchan);
    for (size_t i = 0; i < N; i++) {
        chan_values_[i] = clip<float>(lv.floatAt(i, 0), prop_min, prop_max);
    }

    output();

    space_layer_.invalidate();
    points_layer_.invalidate();
    redraw();
}

void Hoa2dSpace::onMouseDown(t_object* view, const t_pt& pt, const t_pt& abs_pt, long modifiers)
{
    using namespace ceammc::hoa;
    const auto center = width() / 2;

    t_pt mouse;
    mouse.x = pt.x - center;
    mouse.y = center * 2. - pt.y - center;

    if (modifiers == EMOD_ALT) { // alt : rotation
        mode_ = EDIT_ROTATE;
        angle_ref_ = Math<float>::wrap_twopi(Math<float>::azimuth(mouse.x, mouse.y) - HOA_PI + (HOA_PI / prop_nchan));
        std::copy(chan_values_.begin(), chan_values_.begin() + prop_nchan, chan_refs_.begin()); // save ref values
    } else if (modifiers == EMOD_SHIFT) { // shift : gain
        mode_ = EDIT_ZOOM;
        auto radius = Math<float>::radius(mouse.x, mouse.y);
        value_ref_ = convert::lin2lin_clip<float>(radius, center * 0.2, center * 0.95, prop_min, prop_max);
        std::copy(chan_values_.begin(), chan_values_.begin() + prop_nchan, chan_refs_.begin()); // save ref values
    } else {
        mode_ = EDIT_DEFAULT;
        onMouseDrag(view, pt, modifiers);
    }
}

void Hoa2dSpace::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
    using namespace ceammc::hoa;
    const auto center = width() / 2;

    t_pt mouse; // distance from widget center
    mouse.x = pt.x - center;
    mouse.y = center - pt.y;

    if (mode_ == EDIT_ROTATE) { // alt : rotation
        auto angle = Math<float>::wrap_twopi(Math<float>::azimuth(mouse.x, mouse.y) - HOA_PI + (HOA_PI / prop_nchan));
        auto inc = angle_ref_ - angle;
        for (int i = 0; i < prop_nchan; i++) {
            auto angle = Math<float>::wrap_twopi((double)i / (double)prop_nchan * HOA_2PI + inc);
            int idx1 = angle / HOA_2PI * prop_nchan;
            int idx2 = (idx1 + 1) % prop_nchan;

            auto mu = float(idx1) / prop_nchan * HOA_2PI;
            mu = (angle - mu) / (HOA_2PI / prop_nchan);
            auto value = cosine_interpolation(chan_refs_[idx1], chan_refs_[idx2], mu);
            chan_values_[i] = clip<float>(value, prop_min, prop_max);
        }
    } else if (mode_ == EDIT_ZOOM) { // shift : gain
        auto radius = Math<float>::radius(mouse.x, mouse.y);
        auto inc = convert::lin2lin<float>(radius, center * 0.2, center * 0.95, prop_min, prop_max) - value_ref_;
        for (int i = 0; i < prop_nchan; i++)
            chan_values_[i] = clip<float>(chan_refs_[i] + inc, prop_min, prop_max);
    } else {
        auto angle = Math<float>::wrap_twopi(Math<float>::azimuth(mouse.x, mouse.y) + (HOA_PI / (double)prop_nchan));
        auto radius = Math<float>::radius(mouse.x, mouse.y);
        auto value = convert::lin2lin_clip<float>(radius, center * 0.2, center * 0.95, prop_min, prop_max);
        int index = angle / HOA_2PI * prop_nchan;
        if (index < chan_values_.size())
            chan_values_[index] = value;
    }

    space_layer_.invalidate();
    points_layer_.invalidate();
    redraw();
    output();
}

void Hoa2dSpace::loadPreset(size_t idx)
{
    auto lv = PresetStorage::instance().listValueAt(presetId(), idx);
    onList(lv);
}

void Hoa2dSpace::storePreset(size_t idx)
{
    StaticAtomList<HOA_MAX_PLANEWAVES> lv;
    for (int i = 0; i < prop_nchan; i++)
        lv.push_back(chan_values_[i]);

    PresetStorage::instance().setListValueAt(presetId(), idx, lv.view());
}

void Hoa2dSpace::interpPreset(t_float idx)
{
    StaticAtomList<HOA_MAX_PLANEWAVES> def;
    for (int i = 0; i < prop_nchan; i++)
        def.push_back(chan_values_[i]);

    onList(PresetStorage::instance().interListValue(presetId(), idx, def.view()));
}

void Hoa2dSpace::setup()
{
    UIObjectFactory<Hoa2dSpace> obj("hoa.2d.space", EBOX_GROWLINK);
    obj.addAlias("hoa.space");
    obj.setDefaultSize(120, 120);

    obj.usePresets();
    obj.useMouseEvents(UI_MOUSE_DOWN | UI_MOUSE_DRAG);

    // @nch
    obj.addIntProperty("nch", _("Number of Channels"), 4, &Hoa2dSpace::prop_nchan, _("Main"));
    obj.setPropertyAccessor("nch", &Hoa2dSpace::propNumChan, &Hoa2dSpace::propSetNumChan);
    obj.setPropertyRange("nch", HOA_MIN_PLANEWAVES, HOA_MAX_PLANEWAVES);

    obj.addFloatProperty("min", _("Minimum Value"), 0, &Hoa2dSpace::prop_min, "Bounds");
    obj.addFloatProperty("max", _("Maximum Value"), 1, &Hoa2dSpace::prop_max, "Bounds");

    // colors
    obj.addColorProperty("sp_color", _("Space Color"), "0.27 0.43 0.54 1", &Hoa2dSpace::prop_space_color);
    obj.addColorProperty("pt_color", _("Virtuals Microphones Color"), "0. 0. 0. 1.", &Hoa2dSpace::prop_pt_color);
}

}

void setup_spat_hoa_2d_space()
{
    ceammc::Hoa2dSpace::setup();
}
