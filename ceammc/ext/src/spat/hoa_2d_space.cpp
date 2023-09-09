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
#include "ceammc_ui.h"
#include "fmt/core.h"
#include "hoa_common.h"

static const int HOA_DISPLAY_NPOINTS = 65;
static const float HOA_CONTRAST_DARKER = 0.2;
static const float HOA_CONTRAST_LIGHTER = 0.2;

#include "Hoa_Math.hpp"

namespace ceammc {

//! The abscissa converter function.
/** This function takes the radius \f$\rho\f$, the azimuth \f$\theta\f$ and the elevation \f$\varphi\f$ of a point and retrieves the abscissa \f$x\f$.
 \f[x = \rho \times cos{(\theta + \frac{\pi}{2})} \times cos{(\varphi)} \f]
 @param     radius		The radius.
 @param     azimuth		The azimuth.
 @param     elevation   The elevation.
 @return    The abscissa.
 */
template <typename T>
static inline T fabscissa(const T radius, const T azimuth, const T elevation = 0.)
{
    return radius * cos(azimuth + HOA_PI2) * cos(elevation);
}

//! The ordinate converter function.
/** This function takes the radius \f$\rho\f$, the azimuth \f$\theta\f$ and the elevation \f$\varphi\f$ of a point and retrieves the ordinate \f$y\f$.
 \f[y = \rho \times sin{(\theta + \frac{\pi}{2})} \times cos{(\varphi)} \f]
 @param     radius		The radius (greather than 0).
 @param     azimuth		The azimuth (between \f$0\f$ and \f$2\pi\f$).
 @param     elevation   The elevation (between \f$-\pi\f$ and \f$\pi\f$).
 @return    The ordinate.
 */
template <typename T>
static inline T fordinate(const T radius, const T azimuth, const T elevation = 0.)
{
    return radius * sin(azimuth + HOA_PI2) * cos(elevation);
}

template <typename T>
class Math {
public:
    //! The clipping function
    /** The function clips a number between boundaries. \n
     If \f$x < min\f$, \f[f(x) = min\f]
     else if \f$x > max\f$, \f[f(x) = max\f]
     else \f[f(x) = x\f]
     @param     n   The value to clip.
     @param     lower     The low boundary.
     @param     upper     The high boundary.
     @return    The function return the clipped value.
     */
    static inline T clip(const T& n, const T& lower, const T& upper)
    {
        return std::max(lower, std::min(n, upper));
    }

    //! The wrapping function between  \f$0\f$ and \f$2\pi\f$.
    /** The function wraps a number between \f$0\f$ and \f$2\pi\f$.
     \f[f(x) = x \% 2\pi \f]
     @param     value   The value to wrap.
     @return    The function return the wrapped value.
     */
    static inline T wrap_twopi(const T& value)
    {
        T new_value = value;
        while (new_value < 0.) {
            new_value += (T)HOA_2PI;
        }
        while (new_value >= (T)HOA_2PI) {
            new_value -= (T)HOA_2PI;
        }
        return new_value;
    }

    //! The wrapping function between \f$-\pi\f$ and \f$\pi\f$.
    /** The function wraps a number between \f$-\pi\f$ and \f$\pi\f$.
      \f[f(x) = x \% \pi \f]
     @param     value   The value to wrap.
     @return    The function return the wrapped value.
     */
    static inline T wrap_pi(const T& value)
    {
        T new_value = value;
        while (new_value < -HOA_PI) {
            new_value += (T)HOA_2PI;
        }
        while (new_value >= HOA_PI) {
            new_value -= (T)HOA_2PI;
        }
        return new_value;
    }

    //! The abscissa converter function.
    /** This function takes the radius \f$\rho\f$, the azimuth \f$\theta\f$ and the elevation \f$\varphi\f$ of a point and retrieves the abscissa \f$x\f$.
     \f[x = \rho \times cos{(\theta + \frac{\pi}{2})} \times cos{(\varphi)} \f]
     @param     radius		The radius.
     @param     azimuth		The azimuth.
     @param     elevation   The elevation.
     @return    The abscissa.
     */
    static inline T abscissa(const T radius, const T azimuth, const T elevation = 0.)
    {
        return radius * cos(azimuth + HOA_PI2) * cos(elevation);
    }

    //! The ordinate converter function.
    /** This function takes the radius \f$\rho\f$, the azimuth \f$\theta\f$ and the elevation \f$\varphi\f$ of a point and retrieves the ordinate \f$y\f$.
     \f[y = \rho \times sin{(\theta + \frac{\pi}{2})} \times cos{(\varphi)} \f]
     @param     radius		The radius (greather than 0).
     @param     azimuth		The azimuth (between \f$0\f$ and \f$2\pi\f$).
     @param     elevation   The elevation (between \f$-\pi\f$ and \f$\pi\f$).
     @return    The ordinate.
     */
    static inline T ordinate(const T radius, const T azimuth, const T elevation = 0.)
    {
        return radius * sin(azimuth + HOA_PI2) * cos(elevation);
    }

    //! The height converter function.
    /** This function takes the radius \f$\rho\f$, the azimuth \f$\theta\f$ and the elevation \f$\varphi\f$ of a point and retrieves the height \f$h\f$.
     \f[h = \rho \times sin{(\varphi)} \f]
     @param     radius		The radius.
     @param     azimuth		The azimuth.
     @param     elevation   The elevation.
     @return    The height.
     */
    static inline T height(const T radius, const T azimuth, const T elevation = 0.)
    {
        return radius * sin(elevation);
    }

    //! The radius converter function.
    /** This function takes the abscissa \f$x\f$, the ordinate \f$y\f$ and the height \f$z\f$ of a point and retrieves the radius \f$\rho\f$.
     \f[\rho = \sqrt{x^2 + y^2 +z^2} \f]
     @param     x		The abscissa.
     @param     y		The ordinate.
     @param     z		The height.
     @return    The radius.
     */
    static inline T radius(const T x, const T y, const T z = 0.)
    {
        return sqrt(x * x + y * y + z * z);
    }

    //! The azimuth converter function.
    /** This function takes the abscissa \f$x\f$, the ordinate \f$y\f$ and the height \f$z\f$ of a point and retrieves the azimuth \f$\theta\f$.
      \f[\theta = \arctan{(y / x)} - \frac{\pi}{2} \f]
     @param     x		The abscissa.
     @param     y		The ordinate.
     @param     z		The height.
     @return    The azimuth.
     */
    static inline T azimuth(const T x, const T y, const T z = 0.)
    {
        if (x == 0 && y == 0)
            return 0;
        return atan2(y, x) - HOA_PI2;
    }

    //! The elevation converter function.
    /** This function takes the abscissa \f$x\f$, the ordinate \f$y\f$ and the height \f$z\f$ of a point and retrieves the elevation \f$\varphi\f$.
     \f[\varphi = \arcsin{(\frac{z}{\sqrt{x^2 + y^2 +z^2}})} \f]
     @param     x		The abscissa.
     @param     y		The ordinate.
     @param     z		The height.
     @return    The elevation.
     */
    static inline T elevation(const T x, const T y, const T z = 0.)
    {
        if (z == 0)
            return 0;
        return asin(z / sqrt(x * x + y * y + z * z));
    }

    //! The factorial
    /** The function computes the factorial, the product of all positive integers less than or equal to an integer.
     \f[n! = \prod_{1 \leq i \leq n} i = 1 \times 2 \times {...} \times (n - 1) \times n \f]
     @param     n     The interger.
     @return    The function return the factorial of n.
     */
    static inline long double factorial(long n)
    {
        long double result = n;
        if (n == 0)
            return 1;
        while (--n > 0)
            result *= n;

        return result;
    }
};

constexpr int MIN_SIZE = 20;

Hoa2dSpace::Hoa2dSpace()
    : prop_nchan(4)
    , clock_([this]() {})
    , start_clock_(false)
    , space_layer_(asEBox(), gensym("space_layer"))
    , points_layer_(asEBox(), gensym("points_layer"))
{
    createOutlet();
}

void Hoa2dSpace::init(t_symbol* s, const AtomListView& args, bool usePresets)
{
    UIObject::init(s, args, usePresets);

    //     first positional argument handling
    //    if (!args.empty() && args[0].isFloat())
    //        propSetOrder(clip<t_float>(args[0].asFloat(), HOA_MIN_ORDER, HOA_MAX_2D_ORDER));
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

    for (int i = 0; i < prop_nchan; i++) {
        auto angle = (i - 0.5 - ((prop_nchan & 1) * 0.25)) / prop_nchan * HOA_2PI;

        auto rx = cos(angle) * radius;
        auto ry = sin(angle) * radius;
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

    auto abscissa = ceammc::fabscissa<float>(chan_radius_[0], 0);
    auto ordinate = ceammc::fordinate<float>(chan_radius_[0], 0);
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
        abscissa = ceammc::fabscissa<float>(radius, angle);
        ordinate = ceammc::fordinate<float>(radius, angle);
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
        auto angle = (double)(i + 1.) / (double)prop_nchan * HOA_2PI;
        angle -= HOA_2PI / (double)prop_nchan;
        auto abscissa = fabscissa(radius, angle);
        auto ordinate = fordinate(radius, angle);
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
    const auto center = width() / 2;

    t_pt mouse;
    mouse.x = pt.x - center;
    mouse.y = center * 2. - pt.y - center;

    if (modifiers == EMOD_ALT) { // alt : rotation
        mode_ = EDIT_ROTATE;
        angle_ref_ = Math<float>::wrap_twopi(Math<float>::azimuth(mouse.x, mouse.y) - HOA_PI + (HOA_PI / (double)prop_nchan));
        std::copy(chan_refs_.begin(), chan_refs_.begin() + prop_nchan, chan_values_.end());
    } else if (modifiers == EMOD_SHIFT) { // shift : gain
        mode_ = EDIT_ZOOM;
        auto radius = Math<float>::radius(mouse.x, mouse.y);
        value_ref_ = convert::lin2lin_clip<float>(radius, center * 0.2, center * 0.95, prop_min, prop_max);
        std::copy(chan_values_.begin(), chan_values_.begin() + prop_nchan, chan_refs_.begin());
    } else {
        mode_ = EDIT_DEFAULT;
        onMouseDrag(view, pt, modifiers);
    }
}

void Hoa2dSpace::onMouseDrag(t_object* view, const t_pt& pt, long modifiers)
{
    const auto center = width() / 2;

    t_pt mouse; // distance from widget center
    mouse.x = pt.x - center;
    mouse.y = center - pt.y;

    if (mode_ == EDIT_ROTATE) { // alt : rotation
        auto angle = Math<float>::wrap_twopi(Math<float>::azimuth(mouse.x, mouse.y) - HOA_PI + (HOA_PI / (double)prop_nchan));
        auto inc = angle_ref_ - angle;
        for (int i = 0; i < prop_nchan; i++) {
            auto angle = Math<float>::wrap_twopi((double)i / (double)prop_nchan * HOA_2PI + inc);
            int index = angle / HOA_2PI * prop_nchan;
            int index2 = index + 1;
            if (index2 >= prop_nchan)
                index2 = 0;

            auto mu = (double)index / (double)prop_nchan * (double)HOA_2PI;
            mu = (double)(angle - mu) / ((double)HOA_2PI / (double)prop_nchan);
            auto value = cosine_interpolation(chan_refs_[index], chan_refs_[index2], mu);
            chan_values_[i] = pd_clip_minmax(value, prop_min, prop_max);
        }
    } else if (mode_ == EDIT_ZOOM) { // shift : gain
        auto radius = Math<float>::radius(mouse.x, mouse.y);
        auto inc = convert::lin2lin_clip<float>(radius, center * 0.2, center * 0.95, prop_min, prop_max) - value_ref_;
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

void Hoa2dSpace::setup()
{
    UIObjectFactory<Hoa2dSpace> obj("hoa.space", EBOX_GROWLINK);
    obj.addAlias("hoa.2d.space");
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
