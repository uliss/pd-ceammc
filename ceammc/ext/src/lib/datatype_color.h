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
#ifndef DATATYPE_COLOR_H
#define DATATYPE_COLOR_H

#include "ceammc_abstractdata.h"
#include "ceammc_atomlist.h"
#include "ceammc_data.h"

#include <cstdint>
#include <iosfwd>

namespace ceammc {

class DataTypeColor;
using ColorAtom = DataAtom<DataTypeColor>;

class DataTypeColor : public AbstractData {
private:
    float data_[4];

public:
    DataTypeColor();
    DataTypeColor(std::uint32_t value);
    explicit DataTypeColor(const AtomListView& lv);
    DataTypeColor(const DataTypeColor& c);

    float dataAt(size_t n) const { return data_[n]; }
    float red() const { return data_[0]; }
    float green() const { return data_[1]; }
    float blue() const { return data_[2]; }
    float alpha() const { return data_[3]; };
    std::uint8_t red8() const { return std::round(data_[0] * 255); }
    std::uint8_t green8() const { return std::round(data_[1] * 255); }
    std::uint8_t blue8() const { return std::round(data_[2] * 255); }
    std::uint8_t alpha8() const { return std::round(data_[3] * 255); }

    /**
     * set RGB color with integers values in [0,255] range
     * @param r - red [0,255]
     * @param g - green [0,255]
     * @param b - blue [0,255]
     * @param a - alpha [0,255]
     */
    DataTypeColor& setRgb8(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 255);

    /**
     * set RGB color with float values in [0,1] range
     * @param r - red [0,1]
     * @param g - green [0,1]
     * @param b - blue [0,1]
     * @param a - alpha [0,1]
     */
    DataTypeColor& setRgbf(float r, float g, float b, float a = 1);

    /**
     * set HSL color
     * @param h - hue [0,360]
     * @param s - saturation [0,1]
     * @param l - luminosity [0,1]
     * @param a - alpha [0,1]
     */
    DataTypeColor& setHsl(float h, float s, float l, float a = 1);

    /**
     * set HWB color
     * @param h - hue [0,360]
     * @param w - whiteness [0,1]
     * @param b - blackness [0,1]
     * @param a - alpha [0,1]
     */
    DataTypeColor& setHwb(float h, float w, float b, float a = 1);

    /**
     * set OkLab color
     * @param l - lightness [0,1]
     * @param a [-1,+1]
     * @param b [-1,+1]
     * @param alpha - alpha [0,1]
     */
    DataTypeColor& setOkLab(float l, float a, float b, float alpha = 1);

    /**
     * Set color from hex string: #RGB, #RGBA, #RRGGBB, #RRGGBBAA
     */
    bool setHex(const char* str);

    /**
     * set color brighter
     * @param v - [0-1] range
     */
    DataTypeColor& brighten(float v);

    /**
     * set color darker
     * @param v - [0-1] range
     */
    DataTypeColor& darken(float v);

    /**
     * set color more saturate
     * @param v - [0-1] range
     */
    DataTypeColor& saturate(float v);

    /**
     * set color less saturate
     * @param v - [0-1] range
     */
    DataTypeColor& desaturate(float v);

    /**
     * rotate color hue
     * @param v: [0-360] range
     */
    DataTypeColor& rotate(float v);

    /**
     * flip color hue
     * @param v
     */
    DataTypeColor& flip(float v);

    /**
     * convert to grayscale
     */
    DataTypeColor& grayscale();

    /**
     * set color in the sRGB where one of the red, green, or blue values is about 255
     */
    DataTypeColor& maximizeLightness();

    /**
     * returns a mixture of this color and another color
     */
    DataTypeColor mix(const DataTypeColor& c, float f = 0.5) const;

    /**
     * invert color
     */
    DataTypeColor& invert();

    /**
     * return the WCAG contrast ratio of this color to another color
     * @see https://www.w3.org/TR/WCAG20-TECHS/G18.html
     */
    float contrast(const DataTypeColor& c) const;

    /**
     * returns the luminance of this color
     */
    float calculateLuminance() const;

    /** pure virtual implementations*/
    DataTypeId type() const noexcept;
    bool isEqual(const AbstractData* d) const noexcept;
    AtomList toList(bool sorted = false) const;
    std::string toString() const noexcept final;
    AbstractData* clone() const;
    std::string toListStringContent() const noexcept final;
    std::string toDictStringContent() const noexcept final;
    bool set(const AbstractData* d) noexcept;

    bool operator==(const DataTypeColor& c) const noexcept;
    bool operator!=(const DataTypeColor& c) const noexcept { return !operator==(c); }

    /**
     * Assign color
     */
    DataTypeColor& operator=(const DataTypeColor& c);

public:
    static bool looksLikeCtor(const AtomListView& lv) noexcept;

public:
    static const DataTypeId dataType;
};

std::ostream& operator<<(std::ostream& os, const DataTypeColor& color);

}

#endif // DATATYPE_COLOR_H
