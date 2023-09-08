/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#include "hoa_3d_scope.h"
#include "ceammc_convert.h"
#include "ceammc_dsp.h"
#include "ceammc_ui.h"

static const int MIN_SIZE = 20;
static const int HOA_DISPLAY_NPOINTS = 65;
static const float HOA_CONTRAST_DARKER = 0.2;
static const float HOA_CONTRAST_LIGHTER = 0.2;

constexpr const int HOA_NROWS_1 = 30;
constexpr const int HOA_NROWS_2 = 20;
constexpr const int HOA_NROWS_3 = 10;

namespace ceammc {
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

}

Hoa3dScope::Hoa3dScope()
    : prop_order(3)
    , nharm_(0)
    , prop_gain(1)
    , prop_refresh(100)
    , clock_([this]() { tick(); })
    , start_clock_(false)
    , harm_layer_(asEBox(), gensym("harm_layer"))
{
}

void Hoa3dScope::init(t_symbol* s, const AtomListView& args, bool usePresets)
{
    UIDspObject::init(s, args, usePresets);

    // first positional argument handling
    if (!args.empty() && args[0].isFloat())
        propSetOrder(clip<t_float>(args[0].asFloat(), HOA_MIN_ORDER, HOA_MAX_2D_ORDER));

    dspSetup(nharm_, 0);
}

void Hoa3dScope::okSize(t_rect* newrect)
{
    newrect->w = pd_clip_min(newrect->w, MIN_SIZE);
    newrect->h = pd_clip_min(newrect->h, MIN_SIZE);
}

void Hoa3dScope::paint()
{
    drawBackground();
    drawHarmonics();
}

t_float Hoa3dScope::propOrder() const
{
    return prop_order;
}

void Hoa3dScope::propSetOrder(t_float v)
{
    auto order = clip<int>(v, HOA_MIN_ORDER, HOA_MAX_3D_ORDER);

    if (!scope_ || (order != scope_->getDecompositionOrder())) {
        dsp::SuspendGuard guard;

        size_t nrow = 0;
        auto bbox = rect();
        if (width() >= 200) {
            nrow = HOA_NROWS_1;
        } else if (width() >= 100) {
            nrow = HOA_NROWS_2;
        } else {
            nrow = HOA_NROWS_3;
        }

        scope_.reset(new Scope3d(order, nrow, nrow * 2));
        prop_order = scope_->getDecompositionOrder();
        nharm_ = scope_->getNumberOfHarmonics();

        in_buf_.resize(nharm_ * HOA_DEFAULT_BLOCK_SIZE);

        eobj_resize_inputs(asEObj(), nharm_);
        canvas_update_dsp();
    }
}

AtomList Hoa3dScope::propView() const
{
    return { prop_view[0], prop_view[1], prop_view[2] };
}

void Hoa3dScope::propSetView(const AtomListView& angles)
{
    if (!angles.empty()) {
        using namespace ceammc::convert;

        dsp::SuspendGuard guard;

        prop_view[0] = angles.floatAt(0, rad2degree(scope_->getViewRotationX()));
        prop_view[1] = angles.floatAt(1, rad2degree(scope_->getViewRotationY()));
        prop_view[2] = angles.floatAt(2, rad2degree(scope_->getViewRotationZ()));

        scope_->setViewRotation(degree2rad(prop_view[0]), degree2rad(prop_view[1]), degree2rad(prop_view[2]));
        scope_->computeRendering();

        harm_layer_.invalidate();
        bg_layer_.invalidate();
        redraw();
    }
}

void Hoa3dScope::tick()
{
    scope_->process(&in_buf_[0]);

    harm_layer_.invalidate();
    redraw();
    if (pd_getdspstate())
        clock_.delay(prop_refresh);
}

void Hoa3dScope::dspOn(double samplerate, long blocksize)
{
    in_buf_.resize(scope_->getNumberOfHarmonics() * blocksize);

    UIDspObject::dspOn(samplerate, blocksize);
    start_clock_ = true;
}

void Hoa3dScope::dspProcess(t_sample** ins, long n_ins, t_sample** outs, long n_outs, long sampleframes)
{
    for (long i = 0; i < n_ins; i++)
        Signal::copy(size_t(sampleframes), ins[i], 1, &in_buf_[i], size_t(n_ins));

    Signal::scale(size_t(n_ins * sampleframes), prop_gain, &in_buf_[0]);

    if (start_clock_) {
        start_clock_ = false;
        clock_.delay(0);
    }
}

void Hoa3dScope::drawBackground()
{
    const auto r = rect();
    UIPainter p = bg_layer_.painter(r);

    if (!p)
        return;

    auto black = rgba_addContrast(prop_color_background, -HOA_CONTRAST_DARKER);
    auto white = rgba_addContrast(prop_color_background, HOA_CONTRAST_LIGHTER);

    const float center = width() / 2;
    const float radius = center * 0.95;

    p.setMatrix({ 1, 0, 0, -1, center, center });

    double angle, x1, x2, y1, y2, cosa, sina;
    for (int i = 0; i < (prop_order * 2 + 2); i++) {
        angle = ((double)(i - 0.5) / (prop_order * 2 + 2) * HOA_2PI);
        cosa = cos(angle);
        sina = sin(angle);
        x1 = cosa * radius * 0.2;
        y1 = sina * radius * 0.2;
        x2 = cosa * radius;
        y2 = sina * radius;

        p.moveTo(x1, y1);
        p.drawLineTo(x2, y2);
        p.setLineWidth(3);
        p.setColor(white);
        p.strokePreserve();

        p.setColor(black);
        p.setLineWidth(1);
        p.stroke();
    }

    for (int i = 5; i > 0; i--) {
        p.setLineWidth(3);
        p.setColor(white);
        p.drawCircle(0, 0, i * 0.2 * radius);
        p.strokePreserve();

        p.setLineWidth(1);
        p.setColor(black);
        p.drawCircle(0, 0, i * 0.2 * radius);
        p.stroke();
    }
}

void Hoa3dScope::drawHarmonics()
{
    const t_rect r = rect();
    UIPainter p = harm_layer_.painter(r);
    if (!p)
        return;

    const float center = width() * 0.5;
    const float radius = center * 0.95;

    p.rotate(HOA_PI);
    p.setLineWidth(1);
    p.setMatrix({ 1, 0, 0, -1, center, center });

    auto black = rgba_addContrast(prop_color_background, -HOA_CONTRAST_DARKER);
    auto white = rgba_addContrast(prop_color_background, HOA_CONTRAST_LIGHTER);

    size_t HALF_ROWS = scope_->getNumberOfRows() / 2;

    for (size_t j = 0; j < HALF_ROWS; j++) {
        // positive harmonics
        char pathLength = 0;
        double constrast = (j - HALF_ROWS) / (double)scope_->getNumberOfRows();
        p.setColor(rgba_addContrast(prop_ph_color, constrast));

        auto elev = scope_->getPointElevation(j);
        for (size_t i = 0; i < scope_->getNumberOfColumns(); i++) {
            auto azim = scope_->getPointAzimuth(i);
            auto value = scope_->getPointValue(j, i);
            if (value >= 0) {
                value *= radius;
                if (!pathLength) {
                    p.moveTo(Math<float>::abscissa(value, azim, elev), Math<float>::ordinate(value, azim, elev));
                    pathLength++;
                } else {
                    p.drawLineTo(Math<float>::abscissa(value, azim, elev), Math<float>::ordinate(value, azim, elev));
                }
            }
        }
        p.closePath();
        if (pathLength)
            p.fill();

        pathLength = 0;
        // negative harmonics
        p.setColor(rgba_addContrast(prop_nh_color, constrast));
        for (size_t i = 0; i < scope_->getNumberOfColumns(); i++) {
            double azim = scope_->getPointAzimuth(i);
            double value = scope_->getPointValue(j, i);
            if (value < 0) {
                value *= -radius;
                if (!pathLength) {
                    p.moveTo(Math<float>::abscissa(value, azim, elev), Math<float>::ordinate(value, azim, elev));
                    pathLength++;
                } else {
                    p.drawLineTo(Math<float>::abscissa(value, azim, elev), Math<float>::ordinate(value, azim, elev));
                }
            }
        }
        p.closePath();
        if (pathLength)
            p.fill();
    }

    double angle, x1, x2, y1, y2, cosa, sina;
    for (int i = 0; i < (prop_order * 2 + 2); i++) {
        angle = ((double)(i - 0.5) / (prop_order * 2 + 2) * HOA_2PI);
        cosa = cos(angle);
        sina = sin(angle);
        x1 = cosa * radius * 0.2;
        y1 = sina * radius * 0.2;
        x2 = cosa * radius;
        y2 = sina * radius;

        p.moveTo(x1, y1);
        p.drawLineTo(x2, y2);
        p.setLineWidth(3);
        p.setColor(white);
        p.strokePreserve();
        p.setColor(black);
        p.setLineWidth(1);
        p.stroke();
    }

    for (int i = 5; i > 0; i--) {
        p.setLineWidth(3);
        p.setColor(white);
        p.drawCircle(0, 0, (double)i * 0.2 * radius);
        p.strokePreserve();
        p.setLineWidth(1);
        p.setColor(black);
        p.stroke();
    }

    for (size_t j = HALF_ROWS; j < scope_->getNumberOfRows(); j++) {
        // positive harmonics
        char pathLength = 0;
        double constrast = (j - HALF_ROWS) / (double)scope_->getNumberOfRows();

        p.setColor(rgba_addContrast(prop_ph_color, constrast));
        auto elev = scope_->getPointElevation(j);
        for (size_t i = 0; i < scope_->getNumberOfColumns(); i++) {
            auto azim = scope_->getPointAzimuth(i);
            auto value = scope_->getPointValue(j, i);
            if (value >= 0) {
                value *= radius;
                if (!pathLength) {
                    p.moveTo(Math<float>::abscissa(value, azim, elev), Math<float>::ordinate(value, azim, elev));
                    pathLength++;
                } else {
                    p.drawLineTo(Math<float>::abscissa(value, azim, elev), Math<float>::ordinate(value, azim, elev));
                }
            }
        }
        p.closePath();
        if (pathLength)
            p.fill();

        pathLength = 0;
        // negative harmonics
        p.setColor(rgba_addContrast(prop_nh_color, constrast));
        for (size_t i = 0; i < scope_->getNumberOfColumns(); i++) {
            auto azim = scope_->getPointAzimuth(i);
            auto value = scope_->getPointValue(j, i);
            if (value < 0) {
                value *= -radius;
                if (!pathLength) {
                    p.moveTo(Math<float>::abscissa(value, azim, elev), Math<float>::ordinate(value, azim, elev));
                    pathLength++;
                } else {
                    p.drawLineTo(Math<float>::abscissa(value, azim, elev), Math<float>::ordinate(value, azim, elev));
                }
            }
        }
        p.closePath();
        if (pathLength)
            p.fill();
    }
}

void Hoa3dScope::setup()
{
    auto SYM_REFRESH = gensym("refresh");
    auto SYM_MSEC = gensym("msec");

    UIObjectFactory<Hoa3dScope> obj("hoa.3d.scope~", EBOX_IGNORELOCKCLICK | EBOX_GROWLINK);
    obj.setDefaultSize(120, 120);

    // hide some properties
    obj.internalProperty("send");
    obj.internalProperty("receive");

    // @order
    obj.addIntProperty("order", _("Ambisonic Order"), HOA_DEFAULT_ORDER, &Hoa3dScope::prop_order, "Ambisonic");
    obj.setPropertyAccessor("order", &Hoa3dScope::propOrder, &Hoa3dScope::propSetOrder);
    obj.setPropertyRange("order", HOA_MIN_ORDER, HOA_MAX_2D_ORDER);

    // @view
    obj.addFloatArrayProperty("view", _("View rotation"), "0 0 0", &Hoa3dScope::prop_view, "Ambisonic");
    obj.setPropertyAccessor("view", &Hoa3dScope::propView, &Hoa3dScope::propSetView);

    // @gain
    obj.addFloatProperty("gain", _("Gain"), 1.f, &Hoa3dScope::prop_gain, "Main");
    obj.setPropertyMin("gain", 0);

    // @refresh
    obj.addIntProperty(SYM_REFRESH->s_name, _("Refresh time (ms)"), 100, &Hoa3dScope::prop_refresh, _("Main"));
    obj.setPropertyRange(SYM_REFRESH->s_name, 20, 1000);
    obj.setPropertyUnits(SYM_REFRESH, SYM_MSEC);

    // @ph_color
    obj.addColorProperty("ph_color", _("Positive Harmonics Color"), "1. 0. 0. 1.", &Hoa3dScope::prop_ph_color);
    obj.addColorProperty("nh_color", _("Negative Harmonics Color"), "0. 0. 1. 1.", &Hoa3dScope::prop_nh_color);
}

void setup_spat_hoa_scope_3d()
{
    Hoa3dScope::setup();
}
