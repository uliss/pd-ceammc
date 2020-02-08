/*
// Copyright (c) 2012-2016 CICM - Universite Paris 8 - Labex Arts H2H.
// Authors :
// 2012: Pierre Guillot, Eliott Paris & Julien Colafrancesco.
// 2012-2015: Pierre Guillot & Eliott Paris.
// 2015: Pierre Guillot & Eliott Paris & Thomas Le Meur (Light version)
// 2016: Pierre Guillot & Eliott Paris.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef DEF_HOA_SCOPE_LIGHT
#define DEF_HOA_SCOPE_LIGHT

#define hoa_noexcept noexcept
#define hoa_override override

#include "Hoa_Encoder.hpp"
#include "Hoa_Planewaves.hpp"

namespace hoa
{
    //! The scope class offers a representation a the sound field in the harmonics domain.
    /** The scope discretize a circle for the 2d or a sphere for the 3d by a set of point and uses a decoder to project the  harmonics on it. This class should be used for graphical interfaces outside the digital signal processing. If the number of points for the discretization is very large, then you should prefer to record snapshot of the harmonics and to call the process method at an interval adapted to a graphical rendering.
     */
    template <Dimension D, typename T> class Scope : public ProcessorHarmonics<D, T>
    {
    public:

        //! The scope constructor.
        /**	The scope constructor allocates and initialize the member values to computes harmonics projection depending on a order of decomposition and a number of points. The order must be at least 1.
         @param     order            The order.
         @param     numberOfPoints   The number of points.
         */
        Scope(size_t order, size_t numberOfPoints);

        //! The Scope destructor.
        /**	The Scope destructor free the memory.
         */
        virtual ~Scope() hoa_noexcept = 0;

        //! Set the offset.
        /**	Set the rotation of the spherical harmonics in radian.
         */
        virtual inline void setViewRotation(const T x_axe, const T y_axe, const T z_axe) hoa_noexcept = 0;

        //! Compute the values of the summation of every harmonic to the representation of the sound field
        /** Compute the values of the summation of every harmonic to the representation of the sound field
         */
        virtual void computeRendering() hoa_noexcept = 0;

        //! This method performs the spherical/circular harmonics projection with single precision.
        /**	You should use this method to compute the projection of the spherical/circular harmonics over an ambisonic sphere. The inputs array contains the spherical/circular harmonics samples and the minimum size must be the number of harmonics.
         @param     inputs   The inputs array.
         @param     outputs  The dummy outputs array (null).
         */
        virtual inline void process(const T* inputs, T* outputs) hoa_noexcept hoa_override = 0;

        //! This method performs the spherical harmonics projection with single precision.
        /**	You should use this method to compute the projection of the spherical harmonics over an ambisonic sphere. The inputs array contains the spherical harmonics samples and the minimum size must be the number of harmonics.

         @param     inputs   The inputs array.
         */
        virtual inline void process(const T* inputs) hoa_noexcept = 0;

    };

#ifndef DOXYGEN_SHOULD_SKIP_THIS

    template <typename T> class Scope<Hoa2d, T> : public Encoder<Hoa2d, T>, protected ProcessorPlanewaves<Hoa2d, T>
    {
    private:
        T*  m_matrix;
        T*  m_vector;
        T   m_maximum;
    public:

        //! The scope constructor.
        /**	The scope constructor allocates and initialize the member values to computes circular harmonics projection on a circle depending on a order of decomposition and a circle discretization. The circle is discretized by the number of points. The order must be at least 1. The number of points and column should be at least 3 (but it's very low).
         @param     order            The order.
         @param     numberOfPoints   The number of points.
         */
        Scope(size_t order, size_t numberOfPoints) hoa_noexcept :
        Encoder<Hoa2d, T>(order),
        ProcessorPlanewaves<Hoa2d, T>(numberOfPoints)
        {
            m_matrix = Signal<T>::alloc(ProcessorPlanewaves<Hoa2d, T>::getNumberOfPlanewaves() * Encoder<Hoa2d, T>::getNumberOfHarmonics());
            m_vector = Signal<T>::alloc(ProcessorPlanewaves<Hoa2d, T>::getNumberOfPlanewaves());
            computeRendering();
        }

        //! The scope destructor.
        /**	The scope destructor free the memory.
         */
        ~Scope() hoa_noexcept
        {
            Signal<T>::free(m_matrix);
            Signal<T>::free(m_vector);
        }

        //! Set the offset.
        /**	Set the rotation of the spherical harmonics in radian.
         */
        inline void setViewRotation(const T x_axe, const T y_axe, const T z_axe) hoa_noexcept
        {
            ProcessorPlanewaves<Hoa2d, T>::setPlanewavesRotation(x_axe, y_axe, z_axe);
        }

        //! Get the value of the z rotation.
        /** Get the value of the z rotation. The value is in radian, between 0 and 2π.
         */
        inline T getViewRotationZ() const hoa_noexcept
        {
            return ProcessorPlanewaves<Hoa2d, T>::getPlanewavesRotationZ();
        }

        //! Compute the values of the summation of every harmonic to the representation of the sound field
        /** Compute the values of the summation of every harmonic to the representation of the sound field
         */
        void computeRendering() hoa_noexcept
        {
            const T factor = 1. / (T)(Encoder<Hoa2d, T>::getDecompositionOrder() + 1.);
            for(size_t i = 0; i < ProcessorPlanewaves<Hoa2d, T>::getNumberOfPlanewaves(); i++)
            {
                Encoder<Hoa2d, T>::setAzimuth(ProcessorPlanewaves<Hoa2d, T>::getPlanewaveAzimuth(i));
                Encoder<Hoa2d, T>::process(&factor, m_matrix + i * Encoder<Hoa2d, T>::getNumberOfHarmonics());
                m_matrix[i * Encoder<Hoa2d, T>::getNumberOfHarmonics()] = factor * 0.5;
            }
            for(size_t i = 0; i < ProcessorPlanewaves<Hoa2d, T>::getNumberOfPlanewaves(); i++)
            {
                m_vector[i] = 0.;
            }
            m_maximum = 0;
        }

        //! Retrieve the number of points.
        /**	Retrieve the number of points used to discretize the ambisonic circle.
         @return     This method returns the number of points used to discretize the circle.
         */
        inline size_t getNumberOfPoints() const hoa_noexcept
        {
            return ProcessorPlanewaves<Hoa2d, T>::getNumberOfPlanewaves();
        }

        //! Retrieve the value of a point of the circular harmonics projection.
        /**	Retrieve the result value of the circular harmonics projection for a given point defined by an index. The absolute of the value can be used as the radius of the point for a 2 dimentionnal representation. For the index, 0 is the 0 azimtuh of the circle. The maximum index must be the number of points - 1.
         @param     index   The point index of the point.
         @return    This method returns the value of a point of the ambisonic circle.
         */
        inline T getPointValue(const size_t index) const hoa_noexcept
        {
            return m_vector[index];
        }

        //! Retrieve the radius of a point of the circular harmonics projection.
        /**	Retrieve the radius of the circular harmonics projection for a given point defined by an index. This the absolute of the result of the projection. For the index, 0 is the 0 azimtuh of the circle. The maximum index must be the number of points - 1.
         @param     pointIndex   The point index of the point.
         @return    This method returns the radius of a point of the ambisonic circle.
         */
        inline T getPointRadius(const size_t index) const hoa_noexcept
        {
            return fabs(m_vector[index]);
        }

        //! Retrieve the azimuth of a point of the circular harmonics projection.
        /**	Retrieve the azimuth of the circular harmonics projection for a given point defined by an index.The maximum index must be the number of points - 1.
         @param     pointIndex   The point index of the point.
         @return    This method returns the azimuth of a point of the ambisonic circle.
         */
        inline T getPointAzimuth(const size_t index) const hoa_noexcept
        {
            return ProcessorPlanewaves<Hoa2d, T>::getPlanewaveAzimuth(index);
        }

        //! Retrieve the abscissa of a point of the circular harmonics projection.
        /**	Retrieve the abscissa of the circular harmonics projection for a given point defined by an index.The maximum index must be the number of points - 1.

         @param     pointIndex   The point index of the point.
         @return    This method returns the abscissa of a point of the ambisonic circle.

         @see       getOrdinate
         */
        inline double getPointAbscissa(const size_t index) const hoa_noexcept
        {
            return fabs(m_vector[index]) * ProcessorPlanewaves<Hoa2d, T>::getPlanewaveAbscissa(index);
        }

        //! Retrieve the ordinate of a point of the circular harmonics projection.
        /**	Retrieve the ordinate of the circular harmonics projection for a given point defined by an index.The maximum index must be the number of points - 1.

         @param     pointIndex   The point index of the point.
         @return    This method returns the ordinate of a point of the ambisonic circle.

         @see       getAbscissa
         */
        inline double getPointOrdinate(const size_t index) const hoa_noexcept
        {
            return fabs(m_vector[index]) * ProcessorPlanewaves<Hoa2d, T>::getPlanewaveOrdinate(index);
        }

        //! This method performs the circular harmonics projection.
        /**	You should use this method to compute the projection of the circular harmonics over an ambisonics circle. The inputs array contains the circular harmonics samples and the minimum size must be the number of harmonics.
         @param     inputs   The inputs array.
         */
        inline void process(const T* inputs, T* outputs) hoa_noexcept hoa_override
        {
            Signal<T>::mul(Encoder<Hoa2d, T>::getNumberOfHarmonics(), ProcessorPlanewaves<Hoa2d, T>::getNumberOfPlanewaves(), inputs, m_matrix, m_vector);
            m_maximum = fabs(Signal<T>::max(ProcessorPlanewaves<Hoa2d, T>::getNumberOfPlanewaves(), m_vector));
            if(m_maximum > 1.)
            {
                Signal<T>::scale(ProcessorPlanewaves<Hoa2d, T>::getNumberOfPlanewaves(), (1. / m_maximum), m_vector);
            }
        }

        //! This method performs the circular harmonics projection.
        /**	You should use this method to compute the projection of the circular harmonics over an ambisonics circle. The inputs array contains the circular harmonics samples and the minimum size must be the number of harmonics.
         @param     inputs   The inputs array.
         */
        inline void process(const T* inputs) hoa_noexcept
        {
            Signal<T>::mul(Encoder<Hoa2d, T>::getNumberOfHarmonics(), ProcessorPlanewaves<Hoa2d, T>::getNumberOfPlanewaves(), inputs, m_matrix, m_vector);
            m_maximum = fabs(Signal<T>::max(ProcessorPlanewaves<Hoa2d, T>::getNumberOfPlanewaves(), m_vector));
            if(m_maximum > 1.)
            {
                Signal<T>::scale(ProcessorPlanewaves<Hoa2d, T>::getNumberOfPlanewaves(), (1. / m_maximum), m_vector);
            }
        }
    };

    template <typename T> class Scope<Hoa3d, T> : public Encoder<Hoa3d, T>, protected ProcessorPlanewaves<Hoa3d, T>
    {
    private:
        const size_t m_number_of_rows;
        const size_t m_number_of_columns;
        T*  m_matrix;
        T*  m_vector;
        T   m_maximum;
    public:

        //! The Scope constructor.
        /**	The Scope constructor allocates and initialize the member values to computes spherical harmonics projection on a sphere depending on a order of decomposition and a sphere discretization. The sphere discretization is done by a set of points defined by rows and columns then the precision will be lower at the elevation center (0 radian) than at the top (1/2 Pi) or the bottom (-1/2 Pi) of the sphere. The number of row discretize the elevation then it set how many points are used between the bottom and the top. The number of column discretize the azimuth circle then it set how many points are used to make the turn from the front (O radian). Then the sphere is discretized by number of rows * number of columns points. The order must be at least 1. The number of rows and column should be at least 3 (but it's very low).

         @param     order            The order.
         @param     numberOfRow      The number of rows.
         @param     numberOfColumn	The number of columns.
         */
        Scope(size_t order, size_t numberOfRow, size_t numberOfColumn) hoa_noexcept :
        Encoder<Hoa3d, T>(order),
        ProcessorPlanewaves<Hoa3d, T>(numberOfRow * numberOfColumn),
        m_number_of_rows(numberOfRow),
        m_number_of_columns(numberOfColumn)
        {
            for(size_t i = 0; i < m_number_of_rows; i++)
            {
                const T elevation = (T)i  * HOA_PI / (T)(m_number_of_rows - 1) - HOA_PI2;
                for(size_t j = 0; j < m_number_of_columns; j++)
                {
                    ProcessorPlanewaves<Hoa3d, T>::setPlanewaveAzimuth(i * m_number_of_columns + j, (T)j * HOA_2PI / (T)m_number_of_columns);
                    ProcessorPlanewaves<Hoa3d, T>::setPlanewaveElevation(i * m_number_of_columns + j, elevation);
                }
            }

            m_matrix = Signal<T>::alloc(ProcessorPlanewaves<Hoa3d, T>::getNumberOfPlanewaves() * Encoder<Hoa3d, T>::getNumberOfHarmonics());
            m_vector = Signal<T>::alloc(ProcessorPlanewaves<Hoa3d, T>::getNumberOfPlanewaves());
            computeRendering();
        }

        //! The scope destructor.
        /**	The scope destructor free the memory.
         */
        ~Scope() hoa_noexcept
        {
            Signal<T>::free(m_matrix);
            Signal<T>::free(m_vector);
        }

        //! Retrieve the number of rows.
        /**	Retrieve the number of rows used to discretize the ambisonic sphere.

         @return     This method returns the number of rows used to discretize the sphere.
         */
        inline size_t getNumberOfRows() const hoa_noexcept
        {
            return m_number_of_rows;
        }

        //! Retrieve the number of column.
        /**	Retrieve the number of column used to discretize the ambisonic sphere.

         @return     This method returns the number of column used to discretize the sphere.
         */
        inline size_t getNumberOfColumns() const hoa_noexcept
        {
            return m_number_of_columns;
        }

        //! Retrieve the value of a point of the spherical harmonics projection.
        /**	Retrieve the result value of the spherical harmonics projection for a given point defined by a row index and a column index. The absolute of the value can be used as the radius of the point for a 3 dimensional representation. For the row index, 0 is the bottom of the sphere, number of rows / 2 is at the center of the elevation and number of rows - 1 is at the top of the sphere. For the column index, 0 is the front (0 radian) and number of columns / 2 is the rear of the sphere. The maximum row index must be the number of row - 1 and the maximum column index must be the number of columns - 1.

         @param     rowIndex     The row index of the point.
         @param     columnIndex  The column index of the point.
         @return    This method returns the value of a point of the ambisonic sphere.
         @see       getradius
         @see       getAzimuth
         @see       getElevation
         */
        inline T getPointValue(const size_t rowIndex, const size_t columnIndex) const hoa_noexcept
        {
            return m_vector[rowIndex * m_number_of_columns + columnIndex];
        }

        //! Retrieve the radius of a point of the spherical harmonics projection.
        /**	Retrieve the radius of the spherical harmonics projection for a given point defined by a row index and a column index. This the absolute of the result of the projection. For the row index, 0 is the bottom of the sphere, number of rows / 2 is at the center of the elevation and number of rows - 1 is at the top of the sphere. For the column index, 0 is the front (0 radian) and number of columns / 2 is the rear of the sphere. The maximum row index must be the number of row - 1 and the maximum column index must be the number of columns - 1.

         @param     rowIndex     The row index of the point.
         @param     columnIndex  The column index of the point.
         @return    This method returns the radius of a point of the ambisonic sphere.
         @see       getAzimuth
         @see       getElevation
         @see       getValue
         */
        inline T getPointRadius(const size_t rowIndex, const size_t columnIndex) const hoa_noexcept
        {
            return fabs(m_vector[rowIndex * m_number_of_columns + columnIndex]);
        }

        //! Retrieve the azimuth of a point of the spherical harmonics projection.
        /**	Retrieve the azimuth of the spherical harmonics projection for a given point defined by a row index and a column index. For the column index, 0 is the front (0 radian) and number of columns / 2 is the rear of the sphere. The maximum column index must be the number of columns - 1.

         @param     rowIndex     The row index of the point.
         @param     columnIndex  The column index of the point.
         @return    This method returns the azimuth of a point of the ambisonic sphere.
         @see       getValue
         @see       getRadius
         @see       getElevation
         */
        inline T getPointAzimuth(const size_t columnIndex) const hoa_noexcept
        {
            return (T)columnIndex * HOA_2PI / (T)m_number_of_columns;
        }

        //! Retrieve the elevation of a point of the spherical harmonics projection.
        /**	Retrieve the elevation of the spherical harmonics projection for a given point defined by a row index. For the row index, 0 is the bottom of the sphere, number of rows / 2 is at the center of the elevation and number of rows - 1 is at the top of the sphere. The maximum row index must be the number of row - 1.

         @param     rowIndex     The row index of the point.
         @param     columnIndex  The column index of the point.
         @return    This method returns the elevation of a point of the ambisonic sphere.
         @see       getValue
         @see       getRadius
         @see       getAzimuth
         */
        inline T getPointElevation(const size_t rowIndex) const hoa_noexcept
        {
            return (T)rowIndex * HOA_PI / (T)(m_number_of_rows - 1) - HOA_PI2;
        }

        //! Set the offset.
        /**	Set the rotation of the spherical harmonics in radian.
         */
        inline void setViewRotation(const T x_axe, const T y_axe, const T z_axe) hoa_noexcept
        {
            ProcessorPlanewaves<Hoa3d, T>::setPlanewavesRotation(x_axe, y_axe, z_axe);
        }

        //! Get the value of the x rotation.
        /** Get the value of the x rotation. The value is in radian, between 0 and 2π.
         */
        inline T getViewRotationX() const hoa_noexcept
        {
            return ProcessorPlanewaves<Hoa3d, T>::getPlanewavesRotationX();
        }

        //! Get the value of the y rotation.
        /** Get the value of the y rotation. The value is in radian, between 0 and 2π.
         */
        inline T getViewRotationY() const hoa_noexcept
        {
            return ProcessorPlanewaves<Hoa3d, T>::getPlanewavesRotationY();
        }

        //! Get the value of the z rotation.
        /** Get the value of the z rotation. The value is in radian, between 0 and 2π.
         */
        inline T getViewRotationZ() const hoa_noexcept
        {
            return ProcessorPlanewaves<Hoa3d, T>::getPlanewavesRotationZ();
        }

        //! Compute the values of the summation of every harmonic to the representation of the sound field
        /** Compute the values of the summation of every harmonic to the representation of the sound field
         */
        void computeRendering() hoa_noexcept
        {
            const T factor = 12.5 / (T)(Encoder<Hoa3d, T>::getNumberOfHarmonics());
            for(size_t i = 0; i < ProcessorPlanewaves<Hoa3d, T>::getNumberOfPlanewaves(); i++)
            {
                Encoder<Hoa3d, T>::setAzimuth(ProcessorPlanewaves<Hoa3d, T>::getPlanewaveAzimuth(i));
                Encoder<Hoa3d, T>::setElevation(ProcessorPlanewaves<Hoa3d, T>::getPlanewaveElevation(i));
                Encoder<Hoa3d, T>::process(&factor, m_matrix + i * Encoder<Hoa3d, T>::getNumberOfHarmonics());
                for(size_t j = 0; j < Encoder<Hoa3d, T>::getNumberOfHarmonics(); j++)
                {
                    const size_t l = Encoder<Hoa3d, T>::getHarmonicDegree(j);
                    if(Encoder<Hoa3d, T>::getHarmonicOrder(j) == 0)
                    {
                        m_matrix[i * Encoder<Hoa3d, T>::getNumberOfHarmonics() + j] *= (2. * l + 1.);
                    }
                    else
                    {
                        m_matrix[i * Encoder<Hoa3d, T>::getNumberOfHarmonics() + j] *= T(2. * l + 1.) * 4. * HOA_PI;
                    }
                }
            }
            for(size_t i = 0; i < ProcessorPlanewaves<Hoa3d, T>::getNumberOfPlanewaves(); i++)
            {
                m_vector[i] = 0.;
            }
            m_maximum = 0;
        }

        //! This method performs the spherical harmonics projection with single precision.
        /**	You should use this method to compute the projection of the spherical harmonics over an ambisonic sphere. The inputs array contains the spherical harmonics samples and the minimum size must be the number of harmonics.

         @param     inputs   The inputs array.
         */
        inline void process(const T* inputs, T* outputs) hoa_noexcept hoa_override
        {
            Signal<T>::mul(Encoder<Hoa3d, T>::getNumberOfHarmonics(), ProcessorPlanewaves<Hoa3d, T>::getNumberOfPlanewaves(), inputs, m_matrix, m_vector);
            m_maximum = fabs(Signal<T>::max(ProcessorPlanewaves<Hoa3d, T>::getNumberOfPlanewaves(), m_vector));
            if(m_maximum > 1.)
            {
                Signal<T>::scale(ProcessorPlanewaves<Hoa3d, T>::getNumberOfPlanewaves(), (1. / m_maximum), m_vector);
            }
        }

        //! This method performs the spherical harmonics projection with single precision.
        /**	You should use this method to compute the projection of the spherical harmonics over an ambisonic sphere. The inputs array contains the spherical harmonics samples and the minimum size must be the number of harmonics.

         @param     inputs   The inputs array.
         */
        inline void process(const T* inputs) hoa_noexcept
        {
            Signal<T>::mul(Encoder<Hoa3d, T>::getNumberOfHarmonics(), ProcessorPlanewaves<Hoa3d, T>::getNumberOfPlanewaves(), inputs, m_matrix, m_vector);
            m_maximum = fabs(Signal<T>::max(ProcessorPlanewaves<Hoa3d, T>::getNumberOfPlanewaves(), m_vector));
            if(m_maximum > 1.)
            {
                Signal<T>::scale(ProcessorPlanewaves<Hoa3d, T>::getNumberOfPlanewaves(), (1. / m_maximum), m_vector);
            }
        }
    };

#endif

}

#endif



