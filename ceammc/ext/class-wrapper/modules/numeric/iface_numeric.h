#ifndef _IFACE_NUMERIC_H
#define _IFACE_NUMERIC_H

#include <boost/rational.hpp>
#include <complex>
#include <regex>
#include <stdexcept>
#include <tuple>

#include "data_iface.h"

/**
 * @author Serge Poltavski
 * @version 0.1
 * @library numeric
 * @license GPL3
 */
static void wrapper_init()
{
}

/**
 * @brief Complex data
 * @since 0.1
 * @keywords complex
 * @constructor REAL IMAG
 */
class Complex : public DataIFace {
    std::complex<float> v_;

public:
    /**
     * @brief creates Complex number in polar coordinates
     * @param r - magnitude
     * @param theta - phase angle in radians
     * @return Complex number
     * @since 0.1
     * @see complex.new
     * @keywords polar
     * @example
     * [bang( [1 3.1415926( [1 6.2831852(
     * |      |             |
     * [complex.polar 1 1.5707963]
     * |
     * [ui.display @display_type 1]
     */
    static Complex polar(float r = 0, float theta = 0);

public:
    /**
     * @brief Complex data
     * @param re - real part
     * @param im - imaginary part
     * @example
     * [B] [1 2( [set 3 4(
     * |   |     |
     * [complex.new 4 5]
     * |
     * [ui.display @display_type 1]
     */
    Complex(float re = 0, float im = 0);
    bool operator==(const Complex& c) const;
    void operator=(const Complex& c);

    /**
     * @brief equal check
     * @pd_name ==
     * @keywords compare
     * @param c - complex number to compare
     * @return 1 if numbers are equal, otherwise 0
     * @see complex.!=
     * @since 0.1
     * @example
     * [2 4(  [3 6(  [B]
     * |      |      |
     * |      |      [complex.new 3 6]
     * |      |      |.
     * [complex.==  2 4]
     * |
     * [F]
     */
    bool eq(const Complex& c) const;
    /**
     * @param f - float number to compare
     */
    bool eq(float f) const;

    /**
     * @brief not equal check
     * @since 0.1
     * @pd_name !=
     * @keywords compare
     * @param c - complex number to compare
     * @return 1 if numbers are not equal, otherwise 0
     * @see complex.==
     * @example
     * [2 4(  [3 6(  [B]
     * |      |      |
     * |      |      [complex.new 2 4]
     * |      |      |.
     * [complex.!=  3 6]
     * |
     * [F]
     */
    bool neq(const Complex& c) const;
    /**
     * @param f - float to compare
     */
    bool neq(float f) const;

    /**
     * @brief add two complex numbers
     * @pd_alias complex.+
     * @since 0.1
     * @keywords arithmetic math
     * @param c - complex number to add
     * @return addition result
     * @see complex.sub
     * @example
     * [B]
     * |
     * [complex.new 2 4] [3 5(
     * |                 |.
     * [complex.add 3     ]
     * |
     * [ui.display @display_type 1]
     */
    Complex add(const Complex& c) const;
    /**
     * @param f - float to add
     */
    Complex add(float f) const;

    /**
     * @brief substruct complex numbers
     * @pd_alias complex.-
     * @since 0.1
     * @keywords arithmetic math
     * @param c - complex number to substract
     * @return substraction result
     * @see complex.add
     * @example
     * [B]
     * |
     * [complex.new 2 4] [3 6(
     * |                 |.
     * [complex.sub    1  ]
     * |
     * [ui.display @display_type 1]
     */
    Complex sub(const Complex& c) const;
    /**
     * @param f - float to substract
     */
    Complex sub(float f) const;

    /**
     * @brief multiply complex numbers
     * @pd_alias complex.*
     * @since 0.1
     * @keywords arithmetic math
     * @param c - complex number multiplier
     * @return multiplication result
     * @see complex.div
     * @example
     * [B]
     * |
     * [complex.new 2 4] [4 5(
     * |                 |.
     * [complex.mul     3  ]
     * |
     * [ui.display @display_type 1]
     */
    Complex mul(const Complex& c) const;
    /**
     * @param v - float multiplier
     */
    Complex mul(float v) const;

    /**
     * @brief divide complex numbers
     * @pd_alias complex./
     * @since 0.1
     * @keywords arithmetic math
     * @param c - complex number divisor
     * @return division result
     * @see complex.mul
     * @example
     * [B]               [B]
     * |                 |
     * [complex.new 2 3] [complex.new 4 5]
     * |                 |.
     * [complex.div       2]
     * |
     * [ui.display @display_type 1]
     */
    Complex div(const Complex& c) const;
    /**
     * @param f - float divisor
     */
    Complex div(float f) const;

    /**
     * @brief the complex conjugate
     * @since 0.1
     * @return result
     * @example
     * [B]
     * |
     * [complex.new 10 20]
     * |
     * [complex.conj]
     * |
     * [ui.display @display_type 1]
     */
    Complex conj() const;

    /**
     * @brief the magnitude of a complex number
     * @since 0.1
     * @return the magnitude of a complex number
     * @see complex.norm complex.arg
     * @example
     * [1 1( [2 6( [3 4(
     * |     |     |
     * [complex.abs]
     * |
     * [F]
     */
    float abs() const;

    /**
     * @brief the phase angle of the complex number x, expressed in radians
     * @since 0.1
     * @return phase angle in radians
     * @example
     * [1 0( [0 1( [-1 0( [-1 -1(
     * |     |     |      |
     * |     |     |      |
     * [complex.arg       ]
     * |
     * [F]
     */
    float arg() const;

    /**
     * @brief the squared magnitude
     * @since 0.1
     * @return squared magnitude
     * @see complex.abs
     * @example
     * [1 2( [3 4(
     * |     |
     * |     |
     * [complex.norm]
     * |
     * [F]
     */
    float norm() const;

    /**
     * @brief get real part of complex number
     * @since 0.1
     * @return real part of complex number
     * @see complex.imag
     * @example
     * [1 2( [3 4( [5 6(
     * |     |     |
     * [complex.real]
     * |
     * [F]
     */
    float real() const;

    /**
     * @brief get imaginary part of complex number
     * @since 0.1
     * @return imaginary part of complex number
     * @see complex.real
     * @example
     * [1 2( [3 4( [5 6(
     * |     |     |
     * [complex.imag]
     * |
     * [F]
     */
    float imag() const;

    /**
     * @brief complex base e exponential
     * @since 0.1
     * @return result
     * @example
     * [0 0( [1 0( [2 2(
     * |     |     |
     * [complex.exp]
     * |
     * [ui.display @display_type 1]
     */
    Complex exp() const;

    /**
     * @brief complex natural logarithm with the branch cuts along the negative real axis
     * @pd_alias ln
     * @since 0.1
     * @return result
     * @example
     * [0 0( [1 0( [2 2(
     * |     |     |
     * [complex.log]
     * |
     * [ui.display @display_type 1]
     */
    Complex log() const;

    /**
     * @brief complex common logarithm with the branch cuts along the negative real axis
     * @since 0.1
     * @return result
     * @example
     * [10 0( [1 0( [100 2(
     * |      |     |
     * [complex.log10]
     * |
     * [ui.display @display_type 1]
     */
    Complex log10() const;

    /**
     * @brief complex power
     * @since 0.1
     * @return result
     * @example
     * [B]               [loadbang]
     * |                 |
     * [complex.new 2 4] [complex.new 3 6]
     * |                 |.
     * [complex.pow       ]
     * |
     * [ui.display @display_type 1]
     * @param p - complex power
     */
    Complex pow(const Complex& p) const;

    /**
     * @brief complex square root in the range of the right half-plane
     * @since 0.1
     * @return result
     * @example
     * [0 0( [1 0( [2 2(
     * |     |     |
     * [complex.sqrt]
     * |
     * [ui.display @display_type 1]
     */
    Complex sqrt() const;

    /**
     * @brief sine of complex number
     * @since 0.1
     * @return result
     * @see complex.cos
     * @example
     * [0 0( [1 0( [2 2(
     * |     |     |
     * [complex.sin]
     * |
     * [ui.display @display_type 1]
     */
    Complex sin() const;

    /**
     * @brief cosine of complex number
     * @since 0.1
     * @see complex.sin
     * @return result
     * @example
     * [0 0( [1 0( [2 2(
     * |     |     |
     * [complex.cos]
     * |
     * [ui.display @display_type 1]
     */
    Complex cos() const;

    /**
     * @brief tangent of complex number
     * @since 0.1
     * @see complex.sin complex.cos
     * @return result
     * @example
     * [0 0( [1 0( [2 2(
     * |     |     |
     * [complex.tan]
     * |
     * [ui.display @display_type 1]
     */
    Complex tan() const;

    /**
     * @brief unpacks complex number to real and imaginary parts
     * @pd_name unpack
     * @pd_alias complex->tuple
     * @since 0.1
     * @return real part
     * @return imaginary part
     * @example
     * [0 0( [1 0( [2 2(
     * |     |     |
     * [complex->tuple]
     * |       ^|
     * [F]     [F]
     */
    std::tuple<float, float> toTuple() const
    {
        return std::make_tuple(v_.real(), v_.imag());
    }

    /**
     * @brief real and imaginary part as list
     * @pd_name to_list
     * @pd_alias complex->list
     * @since 0.1
     * @return list of real and imaginary part
     * @example
     * [0 0( [1 0( [2 2(
     * |     |     |
     * [complex->list]
     * |
     * [ui.display @display_type 1]
     */
    std::vector<float> toList() const
    {
        return { v_.real(), v_.imag() };
    }

public:
    std::string toString() const
    {
        return std::string("Complex ")
            + std::to_string(v_.real()) + " " + std::to_string(v_.imag());
    }

    static const char* typeName()
    {
        return "Complex";
    }

    Result setFromFloat(t_float f) override
    {
        v_ = std::complex<float>(f, 0);
        return ok();
    }

    Result setFromList(const ceammc::AtomListView& lv) override
    {
        if (lv.size() != 2)
            return error("IMAG REAL pair expected");

        v_ = std::complex<float>(lv[0].asFloat(), lv[1].asFloat());
        return ok();
    }
};

Complex Complex::polar(float r, float theta)
{
    Complex res;
    res.v_ = std::polar(r, theta);
    return res;
}

Complex::Complex(float re, float im)
    : v_(re, im)
{
}

bool Complex::operator==(const Complex& c) const
{
    return v_ == c.v_;
}

void Complex::operator=(const Complex& c)
{
    v_ = c.v_;
}

bool Complex::eq(const Complex& c) const
{
    return *this == c;
}

bool Complex::eq(float f) const
{
    return v_.real() == f && v_.imag() == 0;
}

bool Complex::neq(const Complex& c) const
{
    return !eq(c);
}

bool Complex::neq(float f) const
{
    return !eq(f);
}

Complex Complex::add(const Complex& c) const
{
    auto v = v_ + c.v_;
    return Complex(v.real(), v.imag());
}

Complex Complex::add(float f) const
{
    return Complex(v_.real() + f, v_.imag());
}

Complex Complex::sub(const Complex& c) const
{
    auto v = v_ - c.v_;
    return Complex(v.real(), v.imag());
}

Complex Complex::sub(float f) const
{
    return Complex(v_.real() - f, v_.imag());
}

Complex Complex::mul(const Complex& c) const
{
    auto v = v_ * c.v_;
    return Complex(v.real(), v.imag());
}

Complex Complex::mul(float v) const
{
    return Complex(v_.real() * v, v_.imag() * v);
}

Complex Complex::div(const Complex& c) const
{
    if (c.v_ == std::complex<float>(0, 0))
        throw std::runtime_error("division by zero");

    auto v = v_ / c.v_;
    return Complex(v.real(), v.imag());
}

Complex Complex::div(float f) const
{
    if (f == 0)
        throw std::runtime_error("division by zero");

    return Complex(v_.real() / f, v_.imag() / f);
}

Complex Complex::conj() const
{
    auto v = std::conj(v_);
    return Complex(v.real(), v.imag());
}

float Complex::abs() const
{
    return std::abs(v_);
}

float Complex::arg() const
{
    return std::arg(v_);
}

float Complex::norm() const
{
    return std::norm(v_);
}

float Complex::real() const
{
    return std::real(v_);
}

float Complex::imag() const
{
    return std::imag(v_);
}

Complex Complex::exp() const
{
    auto v = std::exp(v_);
    return Complex(v.real(), v.imag());
}

Complex Complex::log() const
{
    auto v = std::log(v_);
    return Complex(v.real(), v.imag());
}

Complex Complex::log10() const
{
    auto v = std::log10(v_);
    return Complex(v.real(), v.imag());
}

Complex Complex::pow(const Complex& p) const
{
    auto v = std::pow(v_, p.v_);
    return Complex(v.real(), v.imag());
}

Complex Complex::sqrt() const
{
    auto v = std::sqrt(v_);
    return Complex(v.real(), v.imag());
}

Complex Complex::sin() const
{
    auto v = std::sin(v_);
    return Complex(v.real(), v.imag());
}

Complex Complex::cos() const
{
    auto v = std::cos(v_);
    return Complex(v.real(), v.imag());
}

Complex Complex::tan() const
{
    auto v = std::tan(v_);
    return Complex(v.real(), v.imag());
}

/**
 * @brief The Rational class
 * @since 0.1
 * @keywords rational math
 * @constructor NUM DEN
 */
class Rational : public DataIFace {
    using RationalImpl = boost::rational<int>;

private:
    RationalImpl r_;

    Rational(const RationalImpl& r)
        : r_(r)
    {
    }

public:
    /**
     * @brief Rational number
     * @param num - numerator
     * @param den - denominator
     * @example
     * [B] [1 2( [set 3 4( [9/8( [F]   [9 / 8(
     * |   |     |         |     |     |
     * |   |     |         |     |     |
     * |   |     |         |     |     |
     * [rational.new 4                 ]
     * |
     * [ui.display @display_type 1]
     */
    Rational(int num = 0, int den = 1);
    bool operator==(const Rational& c) const;
    void operator=(const Rational& c);

    /**
     * @brief less than compare operator
     * @param r - rational number to compare
     * @pd_alias rational.<
     * @keywords compare
     * @return  1 if left number is less then right
     * @since 0.1
     * @see rational.>
     * @example
     * [1 3(    [1 3( [1 4( [F]
     * |        |.    |.    |.
     * [rational.<      2 3  ]
     * |
     * [F]
     */
    bool lt(const Rational& r) const;
    /**
     * @param f - float to compare
     */
    bool lt(float f) const;

    /**
     * @brief greater than compare operator
     * @param r - rational number to compare
     * @pd_alias rational.>
     * @keywords compare
     * @return  1 if left number is greater then right
     * @since 0.1
     * @see rational.<
     * @example
     * [1 3(    [1 3( [1 4( [F]
     * |        |.    |.    |.
     * [rational.>        1 5]
     * |
     * [F]
     */
    bool gt(const Rational& r) const;
    /**
     * @param f - float to compare
     */
    bool gt(float f) const;

    /**
     * @brief less equal compare operator
     * @param r - rational number to compare
     * @pd_alias rational.<=
     * @keywords compare
     * @return  1 if left number is less equal then right
     * @since 0.1
     * @see rational.<
     * @example
     * [1 3(    [1 3( [1 4( [F]
     * |        |.    |.    |.
     * [rational.<=       2 3]
     * |
     * [F]
     */
    bool le(const Rational& r) const;
    /**
     * @param f - float to compare
     */
    bool le(float f) const;

    /**
     * @brief equal check
     * @pd_name ==
     * @keywords compare
     * @param r - rational number to compare
     * @return 1 if numbers are equal, otherwise 0
     * @see rational.!=
     * @since 0.1
     * @example
     * [2 1(  [3 6(
     * |      |
     * |      |      [0.5(
     * |      |      |.
     * [rational.==  2 1]
     * |
     * [F]
     */
    bool eq(const Rational& r) const;
    /**
     * @param f - float to compare
     */
    bool eq(float f) const;

    /**
     * @brief greater equal compare operator
     * @param r - rational number to compare
     * @pd_alias rational.>=
     * @keywords compare
     * @return  1 if left number is greater equal then right
     * @since 0.1
     * @see rational.>
     * @example
     * [1 3(    [1 3( [1 4( [F]
     * |        |.    |.    |.
     * [rational.>=        1 5]
     * |
     * [F]
     */
    bool ge(const Rational& r) const;
    /**
     * @param f - float to compare
     */
    bool ge(float f) const;

    /**
     * @brief not equal check
     * @since 0.1
     * @pd_name !=
     * @keywords compare
     * @param c - rational number to compare
     * @return 1 if numbers are not equal, otherwise 0
     * @see rational.==
     * @example
     * [2 3(  [3 6(
     * |      |
     * |      |      [1 2(  [0.5(
     * |      |      |.     |.
     * [rational.!=  2 3     ]
     * |
     * [F]
     */
    bool neq(const Rational& c) const;
    /**
     * @param f - float to compare
     */
    bool neq(float f) const;

    /**
     * @brief add two rational numbers
     * @pd_alias rational.+
     * @since 0.1
     * @keywords arithmetic math
     * @param c - rational number to add
     * @return addition result
     * @see rational.sub
     * @example
     * [B]                [B]
     * |                  |
     * [rational.new 2 4] [rational.new 3 6] [F]
     * |                  |.                 |.
     * [rational.add                     10 -10]
     * |
     * [ui.display @display_type 1]
     */
    Rational add(const Rational& c) const;
    /**
     * @param v - integer to add
     */
    Rational add(int v) const;

    /**
     * @brief substruct rational numbers
     * @pd_alias rational.-
     * @since 0.1
     * @keywords arithmetic math
     * @param c - rational number to substract
     * @return substraction result
     * @see rational.add
     * @example
     * [B]                [B]
     * |                  |
     * [rational.new 2 4] [rational.new 3 6]
     * |                  |.
     * [rational.sub  1    ]
     * |
     * [ui.display @display_type 1]
     */
    Rational sub(const Rational& c) const;
    /**
     * @param v - integer to substract
     */
    Rational sub(int v) const;

    /**
     * @brief multiply rational numbers
     * @pd_alias rational.*
     * @since 0.1
     * @keywords arithmetic math
     * @param c - rational number multiplier
     * @return multiplication result
     * @see rational.div
     * [B]                [B]
     * |                  |
     * [rational.new 2 4] [rational.new 3 0]
     * |                  |.
     * [rational.mul     10 ]
     * |
     * [ui.display @display_type 1]
     */
    Rational mul(const Rational& c) const;
    /**
     * @param v - integer muptiplier
     */
    Rational mul(int v) const;

    /**
     * @brief divide rational numbers
     * @pd_alias rational./
     * @since 0.1
     * @keywords arithmetic math
     * @param c - rational number divisor
     * @return division result
     * @see rational.mul
     * @example
     * [B]                [B]
     * |                  |
     * [rational.new 2 3] [rational.new 4 5]
     * |                  |.
     * [rational.div       3]
     * |
     * [ui.display @display_type 1]
     */
    Rational div(const Rational& c) const;
    /**
     * @param v - integer divisor
     */
    Rational div(int v) const;

    /**
     * @brief unpacks rational number to numerator and denominator parts
     * @pd_name unpack
     * @pd_alias rational->tuple
     * @since 0.1
     * @return numerator part
     * @return demonimator part
     * @example
     * [0 0( [1 0( [2 2(
     * |     |     |
     * [rational->tuple]
     * |       ^|
     * [F]     [F]
     */
    std::tuple<float, float> toTuple() const;

    /**
     * @brief numerator and denominator part as list
     * @pd_name to_list
     * @pd_alias rational->list
     * @since 0.1
     * @return list of numerator and denominator parts
     * @example
     * [0 0( [1 0( [2 2(
     * |     |     |
     * [rational->list]
     * |
     * [ui.display @display_type 1]
     */
    std::vector<float> toList() const;

    /**
     * @brief convert to float value
     * @pd_name to_float
     * @pd_alias rational->float
     * @since 0.1
     * @return float value
     * @example
     * [1 3(
     * |
     * [rational->float]
     * |
     * [F]
     */
    float toFloat() const;

    /**
     * @brief rational power
     * @param n - exponent
     * @return result value
     * @since 0.1
     * @example
     * [1/2(         [F]
     * |             |.
     * [rational.pow 10]
     * |
     * [ui.display @display_type 1]
     */
    Rational pow(int n) const;

    /**
     * @brief abs absolute value
     * @return result value
     * @since 0.1
     * @example
     * [-11/4( [1/4(
     * |       |
     * [rational.abs]
     * |
     * [ui.display @display_type 1]
     */
    Rational abs() const;

    /**
     * @brief reciprocal rational number
     * @pd_alias rational.1/x
     * @since 0.1
     * @return reciprocal number
     * @example
     * [1/2(
     * |
     * [rational.reciprocal  ]
     * |                     |
     * [rational.reciprocal] [ui.display @display_type 1]
     * |
     * [ui.display @display_type 1]
     */
    Rational reciprocal() const;

    /**
     * @brief converts input float to rational with specified precision
     * @return rational number
     * @since 0.1
     * @param f - input float
     * @param max_den - max denominator value
     * @pd_name from_float
     * @pd_alias float->rational
     * @example
     * [3.14159265(
     * |
     * |   [1( [16( [256( [4096(
     * |   |.  |.   |.    |.
     * [float->rational   1]
     * |
     * [ui.display @display_type 1]
     */
    static Rational floatToRational(float f, int max_den);

public:
    std::string toString() const
    {
        return std::to_string(r_.numerator()) + "/" + std::to_string(r_.denominator());
    }

    static const char* typeName()
    {
        return "Rational";
    }

    /**
     * @ignore
     */
    Result setFromString(const std::string& str)
    {
        static std::regex re("((\\+|-)?\\d+)/(\\d+)");
        std::smatch match;
        if (!std::regex_match(str, match, re)) {
            std::ostringstream ss;
            ss << "invalid string: " << str;
            return error(ss.str());
        }

        r_.assign(std::stoi(match.str(1)), std::stoi(match.str(3)));
        return ok();
    }

    Result setFromFloat(t_float f) override
    {
        r_.assign(int(f), 1);
        return ok();
    }

    Result setFromSymbol(t_symbol* s) override
    {
        return setFromString(s->s_name);
    }

    Result setFromList(const ceammc::AtomListView& lv) override
    {
        if (lv.size() != 2) {
            if (lv.size() == 3)
                return setFromString(to_string(lv, ""));

            return error("NUM DEM pair expected");
        }

        if (!lv[0].isInteger() || !lv[1].isInteger()) {
            std::ostringstream ss;
            ss << "natural values expected: " << lv;
            return error(ss.str());
        }

        if (lv[1].asInt() == 0) {
            std::ostringstream ss;
            ss << "invalid denominator value: " << lv[1];
            return error(ss.str());
        }

        r_.assign(lv[0].asInt(), lv[1].asInt());
        return ok();
    }

    Result setFromAny(t_symbol* s, const AtomListView& lv) override
    {
        std::string str(s->s_name);
        str += to_string(lv, "");
        return setFromString(str);
    }
};

Rational::Rational(int n, int d)
    : r_(n, d)
{
}

bool Rational::operator==(const Rational& r) const
{
    return r_ == r.r_;
}

void Rational::operator=(const Rational& r)
{
    r_ = r.r_;
}

bool Rational::lt(const Rational& r) const
{
    return r_ < r.r_;
}

bool Rational::lt(float f) const
{
    return boost::rational_cast<float>(r_) < f;
}

bool Rational::gt(const Rational& r) const
{
    return r_ > r.r_;
}

bool Rational::gt(float f) const
{
    return boost::rational_cast<float>(r_) > f;
}

bool Rational::le(const Rational& r) const
{
    return r_ <= r.r_;
}

bool Rational::le(float f) const
{
    return boost::rational_cast<float>(r_) <= f;
}

bool Rational::eq(const Rational& c) const
{
    return *this == c;
}

bool Rational::eq(float f) const
{
    return boost::rational_cast<float>(r_) == f;
}

bool Rational::ge(const Rational& r) const
{
    return r_ >= r.r_;
}

bool Rational::ge(float f) const
{
    return boost::rational_cast<float>(r_) >= f;
}

bool Rational::neq(const Rational& c) const
{
    return !(*this == c);
}

bool Rational::neq(float f) const
{
    return boost::rational_cast<float>(r_) != f;
}

Rational Rational::add(const Rational& c) const
{
    return Rational(r_ + c.r_);
}

Rational Rational::add(int v) const
{
    return add(Rational(v, 1));
}

Rational Rational::sub(const Rational& c) const
{
    return Rational(r_ - c.r_);
}

Rational Rational::sub(int v) const
{
    return sub(Rational(v, 1));
}

Rational Rational::mul(const Rational& c) const
{
    return Rational(r_ * c.r_);
}

Rational Rational::mul(int v) const
{
    return mul(Rational(v, 1));
}

Rational Rational::div(const Rational& c) const
{
    if (c.r_.numerator() == 0)
        throw std::runtime_error("division by zero");

    return Rational(r_ / c.r_);
}

Rational Rational::div(int v) const
{
    return div(Rational(v, 1));
}

std::tuple<float, float> Rational::toTuple() const
{
    return std::make_tuple(float(r_.numerator()), float(r_.denominator()));
}

std::vector<float> Rational::toList() const
{
    return { float(r_.numerator()), float(r_.denominator()) };
}

float Rational::toFloat() const
{
    return float(r_.numerator()) / float(r_.denominator());
}

Rational Rational::pow(int n) const
{
    if (n == 0)
        return Rational(1, 1);
    else {
        // not fast and naive, but prevents overflow
        auto res = r_;
        int p = std::abs(n);
        while (--p)
            res *= r_;

        if (n > 0)
            return Rational(res.numerator(), res.denominator());
        else
            return Rational(res.denominator(), res.numerator());
    }
}

Rational Rational::abs() const
{
    if (r_ < 0)
        return Rational(-1 * r_);

    return r_;
}

Rational Rational::reciprocal() const
{
    return Rational(r_.denominator(), r_.numerator());
}

Rational Rational::floatToRational(float f, int max_den)
{
    int num = 0;
    int denom = 1;

    int neg = 0;
    int n = 1;

    if (max_den <= 1)
        return Rational((int)f, 1);

    if (f < 0) {
        neg = 1;
        f = -f;
    }

    while (f != floor(f)) {
        n <<= 1;
        f *= 2;
    }

    int d = f;
    int h[3] = { 0, 1, 0 }, k[3] = { 1, 0, 0 };

    /* continued fraction and check denominator each step */
    for (int i = 0; i < 64; i++) {
        int a = n ? d / n : 0;
        if (i && !a)
            break;

        int x = d;
        d = n;
        n = x % n;

        x = a;
        if (k[1] * a + k[0] >= max_den) {
            x = (max_den - k[0]) / k[1];
            if (x * 2 >= a || k[1] >= max_den)
                i = 65;
            else
                break;
        }

        h[2] = x * h[1] + h[0];
        h[0] = h[1];
        h[1] = h[2];
        k[2] = x * k[1] + k[0];
        k[0] = k[1];
        k[1] = k[2];
    }

    denom = k[1];
    num = neg ? -h[1] : h[1];

    return Rational(num, denom);
}

#endif
