/*****************************************************************************
 * Copyright 2026 Serge Poltavski. All rights reserved.
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
#include "pixel_line.h"
#include "ceammc_convert.h"
#include "ceammc_factory.h"
#include "colorm/colorm.h"
#include "datatype_color.h"
#include "fmt/core.h"
#include "pixel_line_args.hpp"

constexpr t_int PIXEL_LINE_SIZE_DEF = 16;
constexpr t_int PIXEL_LINE_SIZE_MIN = 1;
constexpr t_int PIXEL_LINE_SIZE_MAX = 1024;
constexpr t_int PIXEL_LINE_NUM_LAYERS_DEF = 1;
constexpr t_int PIXEL_LINE_NUM_LAYERS_MIN = 1;
constexpr t_int PIXEL_LINE_NUM_LAYERS_MAX = 4;

namespace ceammc {

static colorm::Rgb to_colorm(const PixelRgba& c)
{
    return colorm::Rgb(c.r, c.g, c.b, c.a / 255.0);
}

static PixelRgba from_colorm(const colorm::Rgb& c)
{
    return PixelRgba {
        static_cast<std::uint8_t>(c.red8()),
        static_cast<std::uint8_t>(c.green8()),
        static_cast<std::uint8_t>(c.blue8()),
        static_cast<std::uint8_t>(c.alpha8()),
    };
}

static PixelRgba from_datatype(const DataTypeColor& c)
{
    return PixelRgba {
        static_cast<std::uint8_t>(c.red8()),
        static_cast<std::uint8_t>(c.green8()),
        static_cast<std::uint8_t>(c.blue8()),
        static_cast<std::uint8_t>(c.alpha8()),
    };
}

static t_float color_to_float(const PixelRgba& c)
{
    return (c.r << 16) | (c.g << 8) | c.b;
}

static bool is_error(PixelLineError err, const BaseObject* obj = nullptr)
{
    switch (err) {
    case PixelLineError::InvalidStep:
        Error(obj) << "invalid slice step";
        return true;
    case PixelLineError::EmptyLine:
        Error(obj) << "empty line";
        return true;
    case PixelLineError::PixelIndexOutOfRange:
        Error(obj) << "pixel is out of range";
        return true;
    case PixelLineError::DivisionByZero:
        Error(obj) << "division by zero";
        return true;
    case PixelLineError::NoError:
        return false;
    default:
        Error(obj) << "unknown error: " << static_cast<int>(err);
        return true;
    }
}

bool PixelAbsSlice::get_nth_index(size_t n, size_t& idx) const
{
    auto i = begin_ + (n * step_);
    if (i < end_) {
        idx = i;
        return true;
    } else {
        return false;
    }
}

std::ostream& operator<<(std::ostream& os, const PixelAbsSlice& slice)
{
    os << "slice: [" << slice.begin_ << ", " << slice.end_ << ") step=" << slice.step_;
    return os;
}

PixelLineLayer::PixelLineLayer(size_t size, const PixelRgba& color)
    : data_(size, color)
{
}

PixelLineError PixelLineLayer::pixelAt(const PixelPos& pos, PixelRgba& color) const
{
    size_t idx = 0;
    auto err = pos.to_index(data_.size(), idx);
    if (err != PixelLineError::NoError)
        return err;

    return pixelAt(idx, color);
}

PixelLineError PixelLineLayer::pixelAt(size_t idx, PixelRgba& color) const
{
    if (idx < data_.size()) {
        color = data_[idx];
        return PixelLineError::NoError;
    } else
        return PixelLineError::PixelIndexOutOfRange;
}

void PixelLineLayer::set(const PixelRgba& color, size_t idx)
{
    if (idx < data_.size())
        data_[idx] = color;
}

void PixelLineLayer::fill(const PixelRgba& color, const PixelAbsSlice& slice)
{
    apply(slice, [color](size_t, PixelRgba& c) { c = color; });
}

void PixelLineLayer::resize(size_t size, const PixelRgba& color)
{
    data_.resize(size, color);
}

void PixelLineLayer::add(const PixelRgba& c, const PixelAbsSlice& slice)
{
    apply(slice, [c](size_t, PixelRgba& color) { color += c; });
}

void PixelLineLayer::sub(const PixelRgba& c, const PixelAbsSlice& slice)
{
    apply(slice, [c](size_t, PixelRgba& color) { color -= c; });
}

void PixelLineLayer::mul(float k, const PixelAbsSlice& slice)
{
    apply(slice, [k](size_t, PixelRgba& color) { color *= k; });
}

PixelLineError PixelLineLayer::div(float k, const PixelAbsSlice& slice)
{
    if (k == 0) {
        return PixelLineError::DivisionByZero;
    } else {
        apply(slice, [k](size_t, PixelRgba& color) { color /= k; });
        return PixelLineError::NoError;
    }
}

void PixelLineLayer::negative(const PixelAbsSlice& slice)
{
    apply(slice, [](size_t, PixelRgba& color) { color = color.negative(); });
}

void PixelLineLayer::darken(float k, const PixelAbsSlice& slice)
{
    apply(slice, [k](size_t, PixelRgba& color) { color = color.darker(k); });
}

void PixelLineLayer::grayscale(const PixelAbsSlice& slice)
{
    apply(slice, [](size_t, PixelRgba& color) { color = color.grayscale(); });
}

void PixelLineLayer::swap_channels(int c0, int c1, const PixelAbsSlice& slice)
{
    apply(slice, [c0, c1](size_t, PixelRgba& color) { color = color.swap_channels(c0, c1); });
}

void PixelLineLayer::shift_right(size_t steps)
{
    if (data_.empty())
        return;

    steps = steps % data_.size();
    std::rotate(data_.begin(), data_.begin() + steps, data_.end());
}

void PixelLineLayer::shift_left(size_t steps)
{
    auto N = data_.size();
    if (N == 0)
        return;

    steps = steps % N;
    std::rotate(data_.begin(), data_.begin() + (N - steps), data_.end());
}

void PixelLineLayer::apply(const PixelAbsSlice& slice, const PixelFunc& fn)
{
    slice.each(data_, fn);
}

void PixelLineLayer::copy(const PixelAbsSlice& from, const PixelAbsSlice& to)
{
    auto min_length = std::min(from.length(), to.length());
    size_t isrc { 0 }, idest { 0 };
    for (size_t i = 0; i < min_length; i++) {
        if (!from.get_nth_index(i, isrc))
            break;

        if (!to.get_nth_index(i, idest))
            break;

        data_[isrc] = data_[idest];
    }
}

PixelRelClosedRange PixelRelClosedRange::range(PixelPos from, PixelPos to)
{
    return PixelRelClosedRange { from, to };
}

PixelRelClosedRange PixelRelClosedRange::slice(PixelPos from, PixelPos to, PixelStep step)
{
    return PixelRelClosedRange { from, to, step };
}

PixelLineError PixelRelClosedRange::get_slice(size_t N, PixelAbsSlice& slice) const
{
    if (step_ < 1)
        return PixelLineError::InvalidStep;

    size_t a { 0 }, b { 0 };
    auto err = from_.to_index(N, a);
    if (err != PixelLineError::NoError)
        return err;

    err = to_.to_index(N, b);
    if (err != PixelLineError::NoError)
        return err;

    auto mm = std::minmax(a, b);

    slice.begin_ = mm.first;
    slice.end_ = mm.second + 1;
    slice.step_ = step_;

    return PixelLineError::NoError;
}

bool PixelPos::is_valid(size_t N) const
{
    if (0 <= pos && pos < N)
        return true;
    else if (pos < 0 && (-pos) <= N)
        return true;
    else
        return true;
}

PixelLineError PixelPos::to_index(size_t N, size_t& idx) const
{
    if (N == 0)
        return PixelLineError::EmptyLine;
    else if (pos >= 0)
        idx = std::min<size_t>(pos, N - 1);
    else
        idx = N - std::min(N, static_cast<size_t>(-pos));

    return PixelLineError::NoError;
}

PixelRgba& PixelRgba::operator+=(const PixelRgba& c)
{
    r = clip_max<int>(r + c.r, 255);
    g = clip_max<int>(g + c.g, 255);
    b = clip_max<int>(b + c.g, 255);
    return *this;
}

PixelRgba& PixelRgba::operator+=(std::uint8_t v)
{
    return this->operator+=(PixelRgba { v, v, v, 255 });
}

PixelRgba& PixelRgba::operator-=(const PixelRgba& c)
{
    r = clip<int>(r - c.r, 0, 255);
    g = clip<int>(g - c.g, 0, 255);
    b = clip<int>(b - c.b, 0, 255);
    return *this;
}

PixelRgba& PixelRgba::operator-=(std::uint8_t v)
{
    return this->operator-=(PixelRgba { v, v, v, 255 });
}

PixelRgba PixelRgba::swap_channels(int ch0, int ch1) const
{
    auto mm = std::minmax(ch0, ch1);
    auto res = *this;

    if (mm.first == 0 && mm.second == 1)
        std::swap(res.r, res.g);
    else if (mm.first == 0 && mm.second == 2)
        std::swap(res.r, res.b);
    else if (mm.first == 1 && mm.second == 2)
        std::swap(res.g, res.b);

    return res;
}

std::uint32_t PixelRgba::get_int() const
{
    return r << 24 | g << 16 | b << 8 | a;
}

PixelRgba PixelRgba::negative() const
{
    return PixelRgba {
        static_cast<std::uint8_t>(255 - r),
        static_cast<std::uint8_t>(255 - g),
        static_cast<std::uint8_t>(255 - b),
        a,
    };
}

PixelRgba& PixelRgba::operator*=(float v)
{
    r = clip<int>(std::round(r * v), 0, 255);
    g = clip<int>(std::round(g * v), 0, 255);
    b = clip<int>(std::round(b * v), 0, 255);
    return *this;
}

PixelRgba& PixelRgba::operator/=(float v)
{
    if (v == 0)
        return *this;

    r = clip<int>(std::round(r / v), 0, 255);
    g = clip<int>(std::round(g / v), 0, 255);
    b = clip<int>(std::round(b / v), 0, 255);
    return *this;
}

PixelRgba PixelRgba::saturate(float k) const
{
    return from_colorm(to_colorm(*this).saturate(k));
}

PixelRgba PixelRgba::grayscale() const
{
    return from_colorm(to_colorm(*this).grayscale());
}

PixelRgba PixelRgba::mix(const PixelRgba& c, float k) const
{
    return from_colorm(to_colorm(*this).mix(to_colorm(c), k));
}

PixelRgba PixelRgba::brighten(float k) const
{
    return from_colorm(to_colorm(*this).brighten(k));
}

PixelRgba PixelRgba::darker(float k) const
{
    return from_colorm(to_colorm(*this).darken(k));
}

PixelRgba PixelRgba::rotate(float k) const
{
    return from_colorm(to_colorm(*this).rotate(k));
}

PixelRgba PixelRgba::blend_normal(const PixelRgba& c) const
{
    return from_colorm(to_colorm(*this).blendNormal(to_colorm(c)));
}

PixelRgba PixelRgba::black()
{
    return PixelRgba { 0, 0, 0, 255 };
}

PixelRgba PixelRgba::white()
{
    return PixelRgba { 0, 0, 0, 255 };
}

PixelRgba PixelRgba::none()
{
    return PixelRgba { 0, 0, 0, 0 };
}

PixelLine::PixelLine(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();

    num_layers_ = new IntProperty("@layers", PIXEL_LINE_NUM_LAYERS_DEF);
    num_layers_->checkClosedRange(PIXEL_LINE_NUM_LAYERS_MIN, PIXEL_LINE_NUM_LAYERS_MAX);
    num_layers_->setInitOnly();
    addProperty(num_layers_);

    size_ = new IntProperty("@size", PIXEL_LINE_SIZE_DEF);
    size_->checkClosedRange(PIXEL_LINE_SIZE_MIN, PIXEL_LINE_SIZE_MAX);
    addProperty(size_);
}

void PixelLine::initDone()
{
    BaseObject::initDone();

    size_t N = size_->value();
    for (int i = 0; i < num_layers_->value(); i++) {
        layers_.emplace_back(N, PixelRgba::black());
    }
    output_.resizeClip(N);
}

void PixelLine::dump() const
{
    BaseObject::dump();
    int i = 0;
    for (auto& l : layers_) {
        Post post(this);
        post << "layer #" << i++ << " [" << l.size() << "]";
        for (auto& a : l) {
            post << fmt::format(" #{:08x}", a.get_int());
        }
    }
}

void PixelLine::onBang()
{
    syncLayers();
    listTo(0, output_.view());
}

/// @function "clear the pixels of the layer (by filling them with a transparent color)" {
///     #layer int?    "layer index" { default: 0 check: >=0 }
///     @range? "pixel range" {
///         #from int  "first element"            { }
///         #to   int? "last element (including)" { default: -1 }
///         #step int? "step"                     { default: 1 check: >0 }
///     }
///     @flush? "output buffer" {}
/// }
void PixelLine::m_clear(t_symbol* s, const AtomListView& lv)
{
    m_clear_args args;
    if (!args.parse_args(lv, this))
        return;

    auto layer = args.layer;
    if (layer >= layers_.size()) {
        METHOD_ERR(s) << "invalid layer index: " << layer;
        return;
    }

    PixelAbsSlice slice;
    if (is_error(PixelRelClosedRange::slice(
                     PixelPos(args.prop_range.from),
                     PixelPos(args.prop_range.to),
                     args.prop_range.step)
                     .get_slice(layers_[layer].size(), slice),
            this))
        return;

    layers_[layer].fill(PixelRgba::none(), slice);

    if (args.prop_flush._count)
        onBang();
}

/// @function "fill the layer pixels by specified color" {
///     #layer int     "layer index" { check: >=0 }
///     #color color   "fill color"  {}
///     @range? "pixel range" {
///         #from int  "first element"            { }
///         #to   int? "last element (including)" { default: -1 }
///         #step int? "step"                     { default: 1 check: >0 }
///     }
///     @flush? "output buffer" {}
/// }
void PixelLine::m_fill(t_symbol* s, const AtomListView& lv)
{
    m_fill_args args;
    if (!args.parse_args(lv, this))
        return;

    auto layer = args.layer;
    if (layer >= layers_.size()) {
        METHOD_ERR(s) << "invalid layer index: " << layer;
        return;
    }

    PixelAbsSlice slice;
    if (is_error(PixelRelClosedRange::slice(
                     PixelPos(args.prop_range.from),
                     PixelPos(args.prop_range.to),
                     args.prop_range.step)
                     .get_slice(layers_[layer].size(), slice),
            this))
        return;

    layers_[layer].fill(from_datatype(args.color), slice);

    if (args.prop_flush._count)
        onBang();
}

/// @function "convert the layer pixels to grayscale" {
///     #layer int     "layer index" { check: >=0 }
///     @range? "pixel range" {
///         #from int  "first element"            { }
///         #to   int? "last element (including)" { default: -1 }
///         #step int? "step"                     { default: 1 check: >0 }
///     }
///     @flush?        "output buffer" {}
/// }
void PixelLine::m_grayscale(t_symbol* s, const AtomListView& lv)
{
    m_grayscale_args args;
    if (!args.parse_args(lv, this))
        return;

    auto layer = args.layer;
    if (layer >= layers_.size()) {
        METHOD_ERR(s) << "invalid layer index: " << layer;
        return;
    }

    PixelAbsSlice slice;
    if (is_error(PixelRelClosedRange::slice(
                     PixelPos(args.prop_range.from),
                     PixelPos(args.prop_range.to),
                     args.prop_range.step)
                     .get_slice(layers_[layer].size(), slice),
            this))
        return;

    layers_[layer].grayscale(slice);

    if (args.prop_flush._count)
        onBang();
}

void PixelLine::m_invert(t_symbol* s, const AtomListView& lv)
{
    auto layer = lv.intAt(0, 0);
    if (layer < 0 || layer >= layers_.size()) {
        METHOD_ERR(s) << "invalid layer index: " << layer;
        return;
    }

    PixelRelClosedRange range;
    PixelAbsSlice slice;
    auto err = range.get_slice(layers_[layer].size(), slice);
    if (PixelLineError::NoError != err) {
        METHOD_ERR(s) << "err: " << static_cast<int>(err);
        return;
    }
    layers_[layer].negative(slice);
    syncLayers();
}

void PixelLine::m_shift(t_symbol* s, const AtomListView& lv)
{
}

void PixelLine::m_darken(t_symbol* s, const AtomListView& lv)
{
    auto layer = lv.intAt(0, 0);
    if (layer < 0 || layer >= layers_.size()) {
        METHOD_ERR(s) << "invalid layer index: " << layer;
        return;
    }

    PixelRelClosedRange range;
    PixelAbsSlice slice;
    auto err = range.get_slice(layers_[layer].size(), slice);
    if (PixelLineError::NoError != err) {
        METHOD_ERR(s) << "err: " << static_cast<int>(err);
        return;
    }

    auto k = lv.floatAt(1, 0.125);
    layers_[layer].darken(k, slice);
    syncLayers();
}

void PixelLine::syncLayers()
{
    auto it = std::max_element(layers_.begin(), layers_.end(),
        [](const PixelLineLayer& l0, const PixelLineLayer& l1) {
            return l0.size() < l1.size();
        });

    if (it == layers_.end()) {
        OBJ_ERR << "no layers";
        return;
    }

    auto N = it->size();
    OBJ_DBG << "max size: " << N;
    output_.resizePad(N, Atom(0.));
    for (size_t i = 0; i < N; i++) {
        auto pixel_prev = PixelRgba::black();

        for (auto& l : layers_) {
            auto pixel_color = PixelRgba::black();
            if (is_error(l.pixelAt(i, pixel_color), this))
                continue;

            pixel_prev = pixel_color.blend_normal(pixel_prev);
        }

        output_[i] = color_to_float(pixel_prev);
    }
}

void setup_base_pixel_line()
{
    ObjectFactory<PixelLine> obj("pix.line");

    obj.addMethod("clear", &PixelLine::m_clear);
    obj.addMethod("darken", &PixelLine::m_darken);
    obj.addMethod("fill", &PixelLine::m_fill);
    obj.addMethod("grayscale", &PixelLine::m_grayscale);
    obj.addMethod("invert", &PixelLine::m_invert);
    obj.addMethod("shift", &PixelLine::m_shift);
}

} // namespace ceammc
