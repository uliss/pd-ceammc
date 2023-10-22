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
#ifndef HOA_MATH_COMPAT_H
#define HOA_MATH_COMPAT_H

#include "Hoa_Defs.hpp"
#include <algorithm>

namespace ceammc {
namespace hoa {
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
}

#endif // HOA_MATH_COMPAT_H
