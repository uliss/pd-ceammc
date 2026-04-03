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
#ifndef PIXEL_LINE_H
#define PIXEL_LINE_H

#include "ceammc_object.h"

namespace ceammc {

enum class PixelLineError {
    NoError,
    InvalidStep,
    EmptyLine,
    PixelIndexOutOfRange,
    DivisionByZero,
};

struct PixelRgba {
    std::uint8_t r { 0 }, g { 0 }, b { 0 }, a { 255 };

    PixelRgba& operator*=(float v);
    PixelRgba& operator+=(std::uint8_t v);
    PixelRgba& operator+=(const PixelRgba& c);
    PixelRgba& operator-=(const PixelRgba& v);
    PixelRgba& operator-=(std::uint8_t v);
    PixelRgba& operator/=(float v);

    PixelRgba blend_normal(const PixelRgba& c) const;
    PixelRgba brighten(float k) const;
    PixelRgba darker(float k) const;
    PixelRgba grayscale() const;
    PixelRgba mix(const PixelRgba& c, float k) const;
    PixelRgba negative() const;
    PixelRgba rotate(float k) const;
    PixelRgba saturate(float k) const;
    PixelRgba swap_channels(int ch0, int ch1) const;

    std::uint32_t get_int() const;

public:
    PixelRgba(const PixelRgba&) = default;
    PixelRgba& operator=(const PixelRgba&) = default;

    static PixelRgba black();
    static PixelRgba white();
    static PixelRgba none();
};

struct PixelLength {
    std::uint16_t length;
    explicit PixelLength(std::uint16_t l)
        : length(l)
    {
    }
};

struct PixelPos {
    std::int16_t pos;
    explicit PixelPos(std::int16_t p)
        : pos { p }
    {
    }

    bool is_valid(size_t N) const;
    PixelLineError to_index(size_t N, size_t& idx) const;
};

class PixelAbsSlice;
using PixelStep = std::uint16_t;
using PixelFunc = std::function<void(size_t, PixelRgba&)>;

class PixelRelClosedRange {
    PixelPos from_;
    PixelPos to_;
    PixelStep step_;

    PixelRelClosedRange(PixelPos from, PixelPos to, PixelStep step = 1)
        : from_ { from }
        , to_ { to }
        , step_ { step }
    {
    }

    explicit PixelRelClosedRange(PixelPos from)
        : from_ { from }
        , to_ { -1 }
        , step_ { 1 }
    {
    }

public:
    PixelRelClosedRange()
        : from_ { 0 }
        , to_ { -1 }
        , step_ { 1 }
    {
    }

    static PixelRelClosedRange all() { return PixelRelClosedRange {}; }
    static PixelRelClosedRange range(PixelPos from) { return PixelRelClosedRange { from }; }
    static PixelRelClosedRange range(PixelPos from, PixelPos to);
    static PixelRelClosedRange slice(PixelPos from, PixelPos to, PixelStep step);

    PixelLineError get_slice(size_t N, PixelAbsSlice& slice) const;
};

class PixelAbsSlice {
    size_t begin_, end_;
    PixelStep step_;

public:
    friend class PixelRelClosedRange;

public:
    size_t length() const { return (end_ - begin_) / step_; }
    bool get_nth_index(size_t n, size_t& idx) const;

    template <class T>
    void each(T& data, const PixelFunc& fn) const
    {
        for (size_t i = begin_; i < std::min(end_, data.size()); i += step_) {
            fn(i, data[i]);
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const PixelAbsSlice& s);
};

std::ostream& operator<<(std::ostream& os, const PixelAbsSlice& slice);

class PixelLineLayer {
    std::vector<PixelRgba> data_;

public:
    using const_iterator = std::vector<PixelRgba>::const_iterator;

public:
    explicit PixelLineLayer(size_t size, const PixelRgba& color = {});

    PixelLineError pixelAt(const PixelPos& pos, PixelRgba& color) const;
    PixelLineError pixelAt(size_t idx, PixelRgba& color) const;
    size_t size() const { return data_.size(); }
    void resize(size_t size, const PixelRgba& color = {});
    const_iterator begin() const { return data_.begin(); }
    const_iterator end() const { return data_.end(); }

    void add(const PixelRgba& c, const PixelAbsSlice& slice);
    void sub(const PixelRgba& c, const PixelAbsSlice& slice);
    void mul(float k, const PixelAbsSlice& slice);
    PixelLineError div(float k, const PixelAbsSlice& slice);

    void apply(const PixelAbsSlice& slice, const PixelFunc& fn);
    void copy(const PixelAbsSlice& from, const PixelAbsSlice& to);
    void fill(const PixelRgba& color, const PixelAbsSlice& slice);
    void negative(const PixelAbsSlice& slice);
    void darken(float k, const PixelAbsSlice& slice);
    void grayscale(const PixelAbsSlice& slice);
    void swap_channels(int c0, int c1, const PixelAbsSlice& slice);

    void set(const PixelRgba& color, size_t idx);
    void shift_right(size_t steps);
    void shift_left(size_t steps);
};

class PixelLine : public BaseObject {
    std::vector<PixelLineLayer> layers_;
    AtomList output_;

    IntProperty* size_;
    IntProperty* num_layers_;

public:
    explicit PixelLine(const PdArgs& args);

    void initDone() final;
    void dump() const override;
    void onBang() final;

    void m_clear(t_symbol* s, const AtomListView& lv);
    void m_darken(t_symbol* s, const AtomListView& lv);
    void m_fill(t_symbol* s, const AtomListView& lv);
    void m_grayscale(t_symbol* s, const AtomListView& lv);
    void m_invert(t_symbol* s, const AtomListView& lv);
    void m_set(t_symbol* s, const AtomListView& lv);
    void m_shift(t_symbol* s, const AtomListView& lv);

private:
    void syncLayers();
    PixelLineLayer* getAbsSliceFromRelRange(t_symbol* s, t_int layer, PixelAbsSlice& slice, t_int first, t_int last, t_int step);
};

void setup_base_pixel_line();

} // namespace ceammc

#endif // PIXEL_LINE_H
