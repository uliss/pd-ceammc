#ifndef _IFACE_MATRIX_H
#define _IFACE_MATRIX_H

#include <armadillo>
#include <sstream>

#include "ceammc_fn_list.h"
#include "data_iface.h"

using namespace arma;

/**
 * @library matrix
 * @version 0.1
 * @license GPLv3
 * @author Serge Poltavski
 */
static void wrapper_init()
{
    arma_rng::set_seed_random();
}

/**
 * @brief MatrixTilde data type
 * @keywords algebra linear matrix
 * @since 0.1
 * @constructor NROWS NCOLS ...
 */
class MatrixTilde : public DataIFace {
    using ValueType = t_float;
    using MatT = Mat<ValueType>;
    enum {
        DIR_COL = 0,
        DIR_ROW = 1
    };

    MatT m_;

    MatrixTilde(const MatT& m)
        : m_(m)
    {
    }

public:
    /**
     * @brief MatrixTilde data type
     * @param m - number of rows
     * @param n - number of columns
     * @since 0.1
     * @keywords matrix algebra
     * @example
     * [B] [3 4 10 20 30(
     * |   |
     * |   |     [set 3 2 6 5 4 3 2 1(
     * |   |     |
     * [matrix.new 2 2 1 2 3 4]
     * |  |
     * |  [set $1( [B]
     * |  |        |
     * |  [matrix.new]
     * |  |
     * [ui.display @display_type 1]
     */
    MatrixTilde(unsigned int m = 1, unsigned int n = 1);
    bool operator==(const MatrixTilde& m) const;

    /**
     * @brief return MatrixTilde size (rows, columns)
     * @return list (rows, columns)
     * @since 0.1
     * @see matrix.nrows matrix.ncols
     * @keywords size dimensions
     * @example
     * [bang( [2 2 1 2 3 4(
     * |      |
     * [matrix.new 10 16]
     * |
     * [matrix.size]
     * |
     * [ui.display @display_type 1]
     */
    std::vector<int> size() const;

    /**
     * @brief return number of rows in MatrixTilde
     * @return number of rows
     * @since 0.1
     * @see matrix.ncols matrix.size
     * @keywords size
     * @example
     * [bang( [10 16(
     * |      |
     * [matrix.new 3 8]
     * |
     * [matrix.nrows]
     * |
     * [F]
     */
    int nrows() const;

    /**
     * @brief return number of columns in MatrixTilde
     * @return number of columns
     * @since 0.1
     * @see matrix.nrows matrix.size
     * @keywords size
     * @example
     * [bang( [4 2(
     * |      |
     * [matrix.new 3 8]
     * |
     * [matrix.ncols]
     * |
     * [F]
     */
    int ncols() const;

    /**
     * @brief get matrix diagonals
     * @param n - number of diagonal (0 - means main diagonal)
     * @return diagonal as column
     * @keywords diagonal
     * @since 0.1
     * @example
     * [3 3 1 2 3 4 5 6 7 8 9(
     * |
     * [list->matrix] [F]
     * |              |.
     * [matrix.diag   0]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde diag(int n) const;

    /**
     * @brief create MatrixTilde with zeros
     * @param m - number of rows
     * @param n - number of columns
     * @since 0.1
     * @keywords fill zeros
     * @see matrix.ones matrix.full
     * @return matrix filled with zeros
     * @example
     * [B] [F]  [2 2(  [F]
     * |   |    |      |.
     * [matrix.zeros 2 3]
     * |
     * [ui.display @display_type 1]
     */
    static MatrixTilde zeros(unsigned int m, unsigned int n);

    /**
     * @brief create MatrixTilde filled with zeros with the same shape as input
     * @since 0.1
     * @keywords fill zeros
     * @see matrix.zeros
     * @return new matrix filled with zeros
     * @example
     * [3 3(
     * |
     * [matrix.randu] [2 2(
     * |              |
     * [matrix.zeros_like]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde zeros_like() const;

    /**
     * @brief create MatrixTilde with ones
     * @since 0.1
     * @param m - number of rows
     * @param n - number of columns
     * @keywords fill ones
     * @see matrix.zeros matrix.ones_like matrix.full
     * @return matrix filled with ones
     * @example
     * [B] [F]  [2 2(  [F]
     * |   |    |      |.
     * [matrix.ones  2 3]
     * |
     * [ui.display @display_type 1]
     */
    static MatrixTilde ones(unsigned int m, unsigned int n);

    /**
     * @brief create MatrixTilde filled with ones with the same shape as input
     * @since 0.1
     * @keywords fill ones
     * @see matrix.ones
     * @return new matrix filled with ones
     * @example
     * [3 3(
     * |
     * [matrix.randu] [2 2(
     * |              |
     * [matrix.ones_like]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde ones_like() const;

    /**
     * @brief create identity MatrixTilde
     * @since 0.1
     * @param n - number of rows and columns
     * @keywords fill indentity
     * @see matrix.zeros matrix.ones
     * @return identity matrix
     * @pd_alias matrix.id
     * @example
     * [B] [3( [F]
     * |   |   |
     * [matrix.identity 2]
     * |
     * [ui.display @display_type 1]
     */
    static MatrixTilde identity(unsigned int n);

    /**
     * @brief create MatrixTilde filled with specified value
     * @since 0.1
     * @param m - number of rows
     * @param n - number of columns
     * @param v - fill value
     * @keywords fill
     * @see matrix.fill
     * @return new matrix
     * @example
     * [B] [2 2 4(      [F]
     * |   |            |..
     * [matrix.full 2 3 -5]
     * |
     * [ui.display @display_type 1]
     */
    static MatrixTilde full(unsigned int m, unsigned int n, t_float v);

    /**
     * @brief fill input MatrixTilde with value
     * @param v - fill value
     * @since 0.1
     * @keywords fill
     * @see matrix.identity
     * @return new matrix
     * @example
     * [2 4(
     * |
     * [matrix.randu]
     * |
     * [matrix.fill 0.1]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde fill(t_float v) const;

    /**
     * @brief generates MatrixTilde with uniform distribution [0-1]
     * @return new matrix
     * @since 0.1
     * @param m - number of rows
     * @param n - number of columns
     * @keywords random generate
     * @see matrix.randn
     * @example
     * [B] [3 3( [F]  [F]
     * |   |     |    |.
     * [matrix.randu 2 2]
     * |
     * [ui.display @display_type 1]
     */
    static MatrixTilde randu(unsigned int m, unsigned int n);

    /**
     * @brief generates MatrixTilde using Gaussian distribution with zero mean and unit variance
     * @return new matrix
     * @param m - number of rows
     * @param n - number of columns
     * @since 0.1
     * @keywords random generate gauss
     * @see matrix.randu
     * @example
     * [B] [3 3( [F]  [F]
     * |   |     |    |.
     * [matrix.randn 2 2]
     * |
     * [ui.display @display_type 1]
     */
    static MatrixTilde randn(unsigned int m, unsigned int n);

    /**
     * @brief replace specific elements with a new value
     * @param old_value - old value
     * @param new_value - new value
     * @return new matrix
     * @since 0.1
     * @keywords replace
     * @example
     * [3 3(
     * |
     * [matrix.identity] [F]   [F]
     * |                 |.    |..
     * [matrix.replace      0  0.1]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde replace(t_float old_value, t_float new_value) const;

    /**
     * @brief change size while keeping elements
     * @param nrows - new number of rows
     * @param ncols - new number of columns
     * @return new matrix
     * @since 0.1
     * @keywords reshape
     * @example
     * [2 3 1 2 3 4 5 6(
     * |
     * [list->matrix] [F]   [F]
     * |              |.    |..
     * [matrix.reshape     3 2]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde reshape(int nrows, int ncols) const;

    /**
     * @brief change size while keeping elements and preserving layout
     * @param nrows - new number of rows
     * @param ncols - new number of columns
     * @return new matrix
     * @since 0.1
     * @keywords size
     * @see matrix.size
     * @example
     * [2 2 1 2 3 4(
     * |
     * [list->matrix] [F]  [F]
     * |              |.   |..
     * [matrix.resize       3 3]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde resize(int nrows, int ncols) const;

    /**
     * @brief get specified column
     * @since 0.1
     * @keywords row col
     * @see matrix.row_at
     * @param n - column number
     * @return matrix column
     * @example
     * [2 3 1 2 3 4 5 6(  [F]
     * |                  |.
     * [matrix.col_at 1    ]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde col_at(unsigned int n) const;

    /**
     * @brief get specified row
     * @since 0.1
     * @keywords row col
     * @see matrix.row_at
     * @param n - row number
     * @return matrix row
     * @example
     * [3 3 1 2 3 4 5 6 7 8 9(  [F]
     * |                        |.
     * [matrix.row_at 1           ]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde row_at(unsigned int n) const;

    /**
     * @brief insert vector at specified matrix row
     * @param n - row number
     * @param r - row to insert
     * @return new matrix
     * @since 0.1
     * @keywords insert
     * @see matrix.row
     * @example
     * [3 3(                  [B]
     * |                      |
     * [matrix.zeros]  [F]    [matrix.row 10 20 30]
     * |               |.     |..
     * [matrix.insert_row     1 1 3 1 2 3]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde insert_row(unsigned int n, const MatrixTilde& r) const;

    /**
     * @brief insert vector at specified matrix column
     * @param n - col number
     * @param r - col to insert
     * @return new matrix
     * @since 0.1
     * @keywords insert
     * @see matrix.col
     * @example
     * [3 3(                [B]
     * |                    |
     * [matrix.zeros] [F]   [matrix.col 10 20 30]
     * |              |.    |..
     * [matrix.insert_col    1 3 1 2 4 6]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde insert_col(unsigned int n, const MatrixTilde& r) const;

    /**
     * @brief remove specified matrix row
     * @param n - row number
     * @return new matrix
     * @since 0.1
     * @keywords remove
     * @see matrix.insert_row
     * @example
     * [3 3 1 2 3 4 5 6 7 8 9(  [F]
     * |                        |.
     * [matrix.remove_row        0]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde remove_row(unsigned int n) const;

    /**
     * @brief remove specified matrix column
     * @param n - column number
     * @return new matrix
     * @since 0.1
     * @keywords remove
     * @see matrix.insert_col
     * @example
     * [3 3 1 2 3 4 5 6 7 8 9(  [F]
     * |                        |.
     * [matrix.remove_col        0]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde remove_col(unsigned int n) const;

    /**
     * @brief swaps specified columns in matrix
     * @param a - first column index
     * @param b - second column index
     * @since 0.1
     * @return new matrix
     * @keywords swap
     * @see matrix.swap_rows
     * @example
     * [3 3 1 2 3 4 5 6 7 8 9(
     * |
     * [matrix.swap_cols 0 2]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde swap_cols(unsigned int a, unsigned int b) const;

    /**
     * @brief swaps specified rows in matrix
     * @param a - first row index
     * @param b - second row index
     * @since 0.1
     * @return new matrix
     * @keywords swap
     * @see matrix.swap_cols
     * @example
     * [3 3 1 2 3 4 5 6 7 8 9(
     * |
     * [matrix.swap_rows 1 2]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde swap_rows(unsigned int a, unsigned int b) const;

    /**
     * @brief returns minimum value in MatrixTilde
     * @return minimum element
     * @since 0.1
     * @see matrix.max
     * @example
     * [2 3(
     * |
     * [matrix.randu]
     * |
     * [matrix.min]
     * |
     * [F digits=8]
     */
    t_float min() const;

    /**
     * @brief returns maximum in MatrixTilde
     * @return maximum element
     * @since 0.1
     * @see matrix.min
     * @example
     * [2 3(
     * |
     * [matrix.randu]
     * |
     * [matrix.max]
     * |
     * [F digits=8]
     */
    t_float max() const;

    /**
     * @brief transpose MatrixTilde
     * @pd_alias matrix.t
     * @keywords transpose
     * @since 0.1
     * @return transposed matrix
     * @example
     * [3 3 1 2 3 4 5 6 7 8 9(
     * |
     * [matrix.transpose]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde transpose() const;

    /**
     * @brief inverse square matrix
     * @return new matrix
     * @since 0.1
     * @pd_alias matrix.-1
     * @keywords inverse solve
     * @example
     * [2 2 1 2 3 4(
     * |
     * [matrix.inverse]
     * |             |
     * |             [ui.display @display_type 1]
     * |
     * [matrix.inverse]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde inverse() const;

    /**
     * @brief rank of the matrix
     * @return rank
     * @since 0.1
     * @example
     * [2 2 1 1 0 1( [2 2 0 1 0 0( [2 2 0 0 0 0(
     * |             |             |
     * |             |             |
     * |             |             |
     * [matrix.new                 ]
     * |             |
     * [matrix.rank] [ui.display @display_type 1]
     * |
     * [F]
     */
    int rank() const;

    /**
     * @brief element-wise matrix addition
     * @param v - float to add
     * @return result MatrixTilde
     * @see matrix.sub
     * @since 0.1
     * @pd_alias matrix.+
     * @example
     * [2 3(         [2 3(
     * |             |
     * [matrix.ones] [matrix.randu]
     * |             |.
     * [matrix.add  10]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde add(t_float v) const;
    /**
     * @param m - matrix to add
     */
    MatrixTilde add(const MatrixTilde& m) const;

    /**
     * @brief element-wise matrix subtraction
     * @param m - subtrahend matrix
     * @return result MatrixTilde
     * @see matrix.add
     * @since 0.1
     * @pd_alias matrix.-
     * @example
     * [2 4(         [2 4(
     * |             |
     * [matrix.ones] [matrix.randu]
     * |             |.
     * [matrix.add   1]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde sub(const MatrixTilde& m) const;
    /**
     * @param v - float to add to all matrix elements
     */
    MatrixTilde sub(t_float v) const;

    /**
     * @brief element-wise matrix multiplication
     * @param b - matrix to multiply element-wise
     * @return result matrix
     * @see matrix.add matrix.dot
     * @pd_alias matrix.*
     * @example
     * [2 2 1 2 3 4(   [2 2 4 3 2 1(
     * |               |.
     * [matrix.mul    10]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde mul(const MatrixTilde& b) const;
    /**
     * @param v - float to multiply all elements
     */
    MatrixTilde mul(t_float v) const;

    /**
     * @brief element-wise matrix division
     * @param m - divisor matrix
     * @return result MatrixTilde
     * @see matrix.mul
     * @since 0.1
     * @pd_alias matrix./
     * @example
     * [2 4(
     * |
     * [matrix.ones]  [2 4 1 2 3 4 5 6 7 8(
     * |              |.
     * [matrix.div    4]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde div(const MatrixTilde& m) const;
    /**
     * @param v - float to divide all elements
     */
    MatrixTilde div(t_float v) const;

    /**
     * @brief matrix product
     * @param m - MatrixTilde to multiply
     * @return result MatrixTilde
     * @pd_alias matrix.dot
     * @since 0.1
     * @see matrix.mul
     * @example
     * [2 2(              [2 4(
     * |                  |
     * [matrix.fill 2]    [matrix.randu] [2 3 1 2 3 4 5 6(
     * |                  |.             |.
     * [matrix.dot MatrixTilde       2 2 1 0 0 1]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde matmul(const MatrixTilde& m) const;

    /**
     * @brief shift MatrixTilde elements by N positions in each column
     * @param n - shift value (can be negative)
     * @return new matrix
     * @since 0.1
     * @keywords shift
     * @see shift_row
     * @example
     * [3 3 1 2 3 4 5 6 7 8 9(
     * |
     * [list->matrix] [F]
     * |              |.
     * [matrix.shift_col 1]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde shift_col(int n) const;

    /**
     * @brief shift MatrixTilde elements by N positions in each row
     * @param n - shift value (can be negative)
     * @return new matrix
     * @since 0.1
     * @keywords shift
     * @see shift_col
     * @pd_alias matrix.>>
     * @example
     * [3 3 1 2 3 4 5 6 7 8 9(
     * |
     * [list->matrix] [F]
     * |              |.
     * [matrix.shift_row 1]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde shift_row(int n) const;

    /**
     * @brief convert MatrixTilde to list
     * @return elements list
     * @return matrix size
     * @since 0.1
     * @pd_name to_list
     * @pd_alias matrix->list
     * @see matrix.from_list
     * @example
     * [2 2 1 2 3 4(
     * |
     * [list->matrix]
     * |
     * [matrix.to_list      ]
     * |                   ^|
     * [ui.display]      [ui.display]
     */
    std::tuple<std::vector<t_float>, std::vector<t_float>> toList() const;

    /**
     * @brief Solve a system of linear equations, A*X = B, where X is unknown
     * @param b - can be a vector or matrix
     * @return result X matrix
     * @since 0.1
     * @example
     * [2 2 1 2 3 4(
     * |
     * [matrix.solve 2 2 1 2 3 4]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde solve(const MatrixTilde& b) const;

    /**
     * @brief 2D matrix convolution
     * @param b - convolution kernel
     * @return new matrix of same size
     * @since 0.1
     * @keywords convolution
     * @example
     * [T]
     * |
     * [metro 200]
     * |
     * [get list(
     * |
     * [ui.matrix @size=113x113 @cols=16 @rows=16 #d]
     * |
     * |
     * |
     * |
     * |                    [loadbang]
     * |                    |
     * |                    [16 16(
     * |                    |
     * [route list]         [matrix.randu]
     * |                    |
     * [msg 16 16]          [matrix.> 0.6]
     * |                    |
     * [matrix.new        #a]
     * |
     * [t a a]
     * |  ^|
     * |  [matrix.conv2 3 3 1 1 1 1 0 1 1 1 1]
     * |  |
     * |  [t a a         ]
     * |  |             ^|
     * |  [matrix.== 2] [matrix.== 3]
     * |  |.            |
     * [matrix.*]       |
     * |                |.
     * [matrix.+          ]
     * |              |
     * [matrix->list] [msg set #b]
     * |
     * [msg set list #c]
     *
     * [X b->a]
     * [X c->d]
     */
    MatrixTilde conv2(const MatrixTilde& b) const;

    /**
     * @brief element-wise greater then comparasion
     * @param b - matrix to compare
     * @pd_alias matrix.>
     * @see matrix.== matrix.>=
     * @since 0.1
     * @return compare result - matrix with 1 and 0
     * @example
     * [2 2 1 2 3 4(             [F]
     * |                         |.
     * [matrix.> MatrixTilde 2 2 4 3 2 1]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde gt(const MatrixTilde& b) const;
    /**
     * @param v - float to compare
     */
    MatrixTilde gt(t_float v) const;

    /**
     * @brief element-wise greater equal comparasion
     * @param b - matrix to compare
     * @pd_alias matrix.>=
     * @see matrix.>
     * @since 0.1
     * @return compare result - matrix with 1 and 0
     * @example
     * [2 2 1 2 3 4(             [F]
     * |                         |.
     * [matrix.>= MatrixTilde 2 2 4 3 2 1]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde ge(const MatrixTilde& b) const;
    /**
     * @param v - float to compare
     */
    MatrixTilde ge(t_float v) const;

    /**
     * @brief element-wise less equal comparasion
     * @param b - matrix to compare
     * @pd_alias matrix.<=
     * @see matrix.<
     * @since 0.1
     * @return compare result - matrix with 1 and 0
     * @example
     * [2 2 1 2 3 4(              [F]
     * |                          |.
     * [matrix.<= MatrixTilde 2 2 4 3 2 1]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde le(const MatrixTilde& b) const;
    /**
     * @param v - float to compare
     */
    MatrixTilde le(t_float v) const;

    /**
     * @brief element-wise less then comparasion
     * @param b - matrix to compare
     * @pd_alias matrix.<
     * @see matrix.<=
     * @since 0.1
     * @return compare result - matrix with 1 and 0
     * @example
     * [2 2 1 2 3 4(         [F]
     * |                     |.
     * [matrix.< MatrixTilde 2 2 4 3 2 1]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde lt(const MatrixTilde& b) const;
    /**
     * @param v - float to compare
     */
    MatrixTilde lt(t_float v) const;

    /**
     * @brief element-wise matrix equal comparasion
     * @param v - float to compare
     * @return compare result - matrix with 1 and 0
     * @since 0.1
     * @see matrix.> matrix.<
     * @pd_alias matrix.==
     * @example
     * [2 2 1 2 3 4(        [F]   [2 2 1 4 3 6(
     * |                    |.    |.
     * [matrix.== MatrixTilde 2 2 1 2 3 4]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde eq(t_float v) const;
    /**
     * @param b - matrix to compare
     */
    MatrixTilde eq(const MatrixTilde& b) const;

    /**
     * @brief element-wise matrix not equal comparasion
     * @param v - float to compare
     * @return compare result - matrix with 1 and 0
     * @since 0.1
     * @see matrix.==
     * @pd_alias matrix.!=
     * @example
     * [2 2 1 2 3 4(         [F]   [2 2 1 4 3 6(
     * |                     |.    |.
     * [matrix.!= MatrixTilde 2 2 1 2 3 4]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde ne(t_float v) const;
    /**
     * @param b - matrix to compare
     */
    MatrixTilde ne(const MatrixTilde& b) const;

    /**
     * @brief load matrix from CSV format
     * @param fname - filename to load
     * @return loaded matrix
     * @since 0.1
     * @see matrix.save
     * @example
     * [2 4(
     * |
     * [matrix.randu]         [symbol test_matrix2.csv(
     * |                      |.
     * [matrix.save test_matrix.csv]
     * |
     * [ui.display @display_type 1]
     *
     *
     *
     *
     *
     * [test_matrix.csv(      [test_matrix2.csv(
     * |                      |
     * [symbol                ]
     * |
     * [matrix.load]
     * |
     * [ui.display @display_type 1]
     */
    static MatrixTilde load(const std::string& fname);

    /**
     * @brief save matrix in CSV format
     * @param fname - filename to save
     * @return original matrix
     * @since 0.1
     * @see matrix.load
     * @example
     * [2 4(
     * |
     * [matrix.randu]         [symbol test_matrix2.csv(
     * |                      |.
     * [matrix.save test_matrix.csv]
     * |
     * [ui.display @display_type 1]
     *
     *
     *
     *
     *
     * [test_matrix.csv(      [test_matrix2.csv(
     * |                      |
     * |                      |
     * [symbol                ]
     * |
     * [matrix.load]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde save(const std::string& fname) const;

    /**
     * @brief create MatrixTilde from list values
     * @param v - element values
     * @return new matrix
     * @since 0.1
     * @see matrix.to_list
     * @pd_name from_list
     * @pd_alias list->matrix
     * @example
     * [2 2 10 20 30 40(
     * |
     * [matrix.from_list]
     * |
     * [ui.display @display_type 1]
     */
    static MatrixTilde fromList(const std::vector<t_float>& v);

    /**
     * @brief create MatrixTilde column with specified values
     * @param v - column values
     * @return column
     * @since 0.1
     * @see matrix.row
     * @example
     * [B] [1 2 3 4(
     * |   |
     * [matrix.col 0.2345 1.2345 3.1415]
     * |       |
     * [print] [ui.display @display_type 1]
     */
    static MatrixTilde col(const std::vector<t_float> &v);

    /**
     * @brief create MatrixTilde row with specified values
     * @param v - row values
     * @return row
     * @see matrix.col
     * @since 0.1
     * @example
     * [B] [1 2 3 4(
     * |   |
     * [matrix.row 2 2 2]
     * |       |
     * [print] [ui.display @display_type 1]
     */
    static MatrixTilde row(const std::vector<t_float>& v);

    /**
     * @brief generate row with linearly spaced N elements
     * @param start - start element value
     * @param end - including end element value
     * @param N - number of element, if 0, means default value 100 elements
     * @return matrix row
     * @since 0.1
     * @keywords linspace
     * @see matrix.logspace
     * @example
     * [B] [0 10 5(
     * |   |
     * [matrix.linspace 1 2 3]
     * |
     * [ui.display @display_type 1]
     */
    static MatrixTilde linspace(t_float start, t_float end, unsigned int N);

    /**
     * @brief generate row with N elements spaced logarithmically
     * @param A - start from 10^A
     * @param B - end and including 10^B
     * @param N - number of element, if 0, means default value 50 elements
     * @return matrix row
     * @since 0.1
     * @keywords logspace
     * @see matrix.linspace
     * @example
     * [B] [1 3 5( [1 3 20(
     * |   |       |
     * [matrix.logspace 1 2 10]
     * |
     * [ui.display @display_type 1]
     */
    static MatrixTilde logspace(t_float A, t_float B, unsigned int N);

    /**
     * @brief calc abs value for each matrix element
     * @return new matrix
     * @since 0.1
     * @keywords abs magnitude
     * @example
     * [2 2 -1 -2 -3 -4(
     * |
     * [matrix.abs]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde abs() const;

    /**
     * @brief accumulate (sum) all elements of matrix
     * @return sum all elements of matrix
     * @since 0.1
     * @keywords accumulate sum
     * @see matrix.sum
     * @example
     * [2( [4(           [B]
     * |   |             |
     * [matrix.identity] [matrix.linspace 1 10 100]
     * |                 |
     * [matrix.accu      ]
     * |
     * [F]
     */
    t_float accu() const;

    /**
     * @brief check if all elements of the matrix are non-zero
     * @return 1 or 0
     * @since 0.1
     * @see matrix.any
     * @example
     * [2 2(         [3 3(          [4(
     * |             |              |
     * [matrix.ones] [matrix.zeros] [matrix.id]
     * |             |              |
     * |             |              |
     * |             |              |
     * [matrix.all                  ]
     * |
     * [F]
     */
    bool all() const;

    /**
     * @brief row with each element (0 or 1) indicating whether the corresponding column has all non-zero elements
     * @return row vector
     * @since 0.1
     * @see matrix.all matrix.all_row
     * @example
     * [2 2 1 2 3 4( [2 2 1 2 0 3( [2 2 1 0 0 1(
     * |             |             |
     * |             |             |
     * |             |             |
     * [matrix.all_col             ]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde all_col() const;

    /**
     * @brief column with each element (0 or 1) indicating whether the corresponding row has all non-zero elements
     * @return row vector
     * @since 0.1
     * @see matrix.all matrix.all_col
     * @example
     * [2 2 1 2 3 4( [2 2 1 2 0 3( [2 2 1 0 0 1(
     * |             |             |
     * |             |             |
     * |             |             |
     * [matrix.all_row             ]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde all_row() const;

    /**
     * @brief check if any elements of the matrix are non-zero
     * @return 1 or 0
     * @since 0.1
     * @see matrix.all
     * @example
     * [2 2(         [3 3(          [4(
     * |             |              |
     * [matrix.ones] [matrix.zeros] [matrix.id]
     * |             |              |
     * |             |              |
     * |             |              |
     * [matrix.any                  ]
     * |
     * [F]
     */
    bool any() const;

    /**
     * @brief row with each element (0 or 1) indicating whether the corresponding column has any non-zero elements
     * @return row vector
     * @since 0.1
     * @see matrix.any matrix.any_row
     * @example
     * [2 2 1 2 3 4( [2 2 1 2 0 3( [2 2 1 0 0 0(
     * |             |             |
     * |             |             |
     * |             |             |
     * [matrix.any_col             ]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde any_col() const;

    /**
     * @brief column with each element (0 or 1) indicating whether the corresponding row has any non-zero elements
     * @return row vector
     * @since 0.1
     * @see matrix.any matrix.any_col
     * @example
     * [2 2 1 2 3 4( [2 2 1 2 0 3( [2 2 1 0 0 0(
     * |             |             |
     * |             |             |
     * |             |             |
     * [matrix.any_row             ]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde any_row() const;

    /**
     * @brief clip matrix value to range [min, max]
     * @param min - minimal value
     * @param max - maximum value
     * @return new matrix
     * @since 0.1
     * @keywords clip min max
     * @see matrix.min matrix.max
     * @example
     * [3 3(
     * |
     * [matrix.randn]
     * |
     * [matrix.clip 0 1]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde clip(t_float min, t_float max) const;

    /**
     * @brief determinant of square matrix
     * @return value
     * @since 0.1
     * @example
     * [3 3(
     * |
     * [matrix.randu]  [3 5(
     * |               |
     * [matrix.det     ]
     * |
     * [F digits=8]
     */
    t_float det() const;

    /**
     * @brief mean matrix value
     * @return mean value
     * @since 0.1
     * @see matrix.mean_row matrix.mean_col
     * @example
     * [3 5(
     * |
     * [matrix.randu]
     * |
     * [matrix.mean]
     * |
     * [F digits=8]
     */
    t_float mean() const;

    /**
     * @brief mean value for each matrix column
     * @return row vector with mean value for each column
     * @since 0.1
     * @see matrix.mean matrix.mean_row
     * @example
     * [3 4(
     * |
     * [matrix.randu]
     * |
     * [matrix.mean_col]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde mean_col() const;

    /**
     * @brief mean value for each matrix row
     * @return column vector with mean value for each row
     * @since 0.1
     * @see matrix.mean matrix.mean_col
     * @example
     * [3 4(
     * |
     * [matrix.randu]
     * |
     * [matrix.mean_row]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde mean_row() const;

    /**
     * @brief median matrix value
     * @return median value
     * @since 0.1
     * @see matrix.median_row matrix.median_col
     * @example
     * [1 9 1 1 1 2 2 3 3 6 10(
     * |               |
     * [matrix.median] [matrix.mean]
     * |               |
     * [F digits=8]    [F digits=8]
     */
    t_float median() const;

    /**
     * @brief median matrix value for each column
     * @return median value row
     * @since 0.1
     * @see matrix.median_row matrix.median
     * @example
     * [3 3 1 2 3 4 5 6 7 8 9(
     * |
     * [matrix.median_col]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde median_col() const;

    /**
     * @brief median matrix value for each row
     * @return median value column
     * @since 0.1
     * @see matrix.median_col matrix.median
     * @example
     * [3 3 1 2 3 4 5 6 7 8 9(
     * |
     * [matrix.median_row]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde median_row() const;

    /**
     * @brief standart deviation for matrix values
     * @return standart deviation
     * @param norm_type - if 0 given, N-1 normalization used, if 1 - N
     * @since 0.1
     * @see matrix.stddev_col matrix.stddev_row
     * @example
     * [5 5(          [5 5(
     * |              |
     * [matrix.randu] [matrix.randn]
     * |              |
     * [matrix.stddev ]
     * |
     * [F digits=8]
     */
    t_float stddev(bool norm_type) const;

    /**
     * @brief standart deviation matrix value for each column
     * @return standart deviation value row
     * @since 0.1
     * @param norm_type - if 0 given, N-1 normalization used, if 1 - N
     * @see matrix.stddev matrix.stddev_row
     * @example
     * [3 3 3 3 3 3 4 5 3 2 6(
     * |
     * [matrix.stddev_col]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde stddev_col(bool norm_type) const;

    /**
     * @brief standart deviation matrix value for each row
     * @return standart deviation value col
     * @param norm_type - if 0 given, N-1 normalization used, if 1 - N
     * @since 0.1
     * @see matrix.stddev matrix.stddev_col
     * @example
     * [3 3 3 3 3 3 4 5 3 2 6(
     * |
     * [matrix.stddev_row]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde stddev_row(bool norm_type) const;

    /**
     * @brief matrix range (difference between max and min)
     * @return range value
     * @since 0.1
     * @see matrix.range_col matrix.range_row
     * @example
     * [3 3 1 2 3 4 5 6 7 8 9(
     * |
     * [matrix.range]
     * |
     * [F]
     */
    t_float range() const;

    /**
     * @brief matrix column range (difference between max and min)
     * @return range value row
     * @since 0.1
     * @see matrix.range matrix.range_row
     * @example
     * [3 3 1 2 3 4 5 6 7 8 9(
     * |
     * [matrix.range_col]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde range_col() const;

    /**
     * @brief matrix row range (difference between max and min)
     * @return range value column
     * @since 0.1
     * @see matrix.range matrix.range_col
     * @example
     * [3 3 1 2 3 4 5 6 7 8 9(
     * |
     * [matrix.range_row]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde range_row() const;

    /**
     * @brief produce a matrix containing column histogram counts
     * @param n_bins - number of bins
     * @return hist matrix
     * @since 0.1
     * @example
     * [100 1( [1 100000(  [A A_HIST0 size=20 yr=0..1]
     * |       |
     * |       |
     * |       |
     * [matrix.randn]
     * |
     * |
     * |
     * |
     * [matrix.hist 30]
     * |
     * [matrix->list]
     * |
     * [array.set A_HIST0 @normalize]
     */
    MatrixTilde hist(unsigned int n_bins) const;

    /**
     * @brief element-wise base-e exponent: e^x
     * @return new matrix
     * @since 0.1
     * @keywords exponent
     * @example
     * [2 2 1 2 3 4(
     * |
     * [matrix.exp]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde exp() const;

    /**
     * @brief element-wise base-2 exponent: 2^x
     * @return new matrix
     * @since 0.1
     * @keywords exponent
     * @example
     * [2 2 1 2 3 4(
     * |
     * [matrix.exp2]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde exp2() const;

    /**
     * @brief element-wise base-10 exponent: 10^x
     * @return new matrix
     * @since 0.1
     * @keywords exponent
     * @example
     * [2 2 0 1 2 3(
     * |
     * [matrix.exp10]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde exp10() const;

    /**
     * @brief element-wise base-e exponent, truncated to avoid infinity
     * @return new matrix
     * @since 0.1
     * @keywords exponent
     * @example
     * [2 2 10 50 100 150(      [2 2 10 50 100 150(
     * |                        |
     * [matrix.trunc_exp]       [matrix.exp]
     * |                        |
     * [ui.display @display_type 1]
     */
    MatrixTilde trunc_exp() const;

    /**
     * @brief element-wise signum function
     * @return new matrix
     * @since 0.1
     * @keywords exponent
     * @example
     * [2 4(
     * |
     * [matrix.randn]
     * |
     * [matrix.sign]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde sign() const;

    /**
     * @brief element-wise sine
     * @return new matrix
     * @since 0.1
     * @see matrix.asin
     * @keywords sin trigonometric
     * @example
     * [0 6.4 100(         [A ARR_SIN size=100 w=260 h=130 yr=-1..1]
     * |
     * [matrix.linspace]
     * |
     * [matrix.sin]
     * |
     * [matrix->list]
     * |
     * [array.set ARR_SIN]
     */
    MatrixTilde sin() const;

    /**
     * @brief element-wise acrsine
     * @return new matrix
     * @since 0.1
     * @see matrix.sin
     * @keywords arcsine trigonometric
     * @example
     * [-1 1 100(         [A ARR_ASIN size=100 w=260 h=130 yr=-2..2]
     * |
     * [matrix.linspace]
     * |
     * [matrix.asin]
     * |
     * [matrix->list]
     * |
     * [array.set ARR_ASIN]
     */
    MatrixTilde asin() const;

    /**
     * @brief element-wise hyperbolic sine
     * @return new matrix
     * @since 0.1
     * @see matrix.asinh
     * @keywords sinh trigonometric
     * @example
     * [-2 2 100(         [A ARR_SINH size=100 w=260 h=130 yr=-4..4]
     * |
     * [matrix.linspace]
     * |
     * [matrix.sinh]
     * |
     * [matrix->list]
     * |
     * [array.set ARR_SINH]
     */
    MatrixTilde sinh() const;

    /**
     * @brief element-wise hyperbolic acrsine
     * @return new matrix
     * @since 0.1
     * @see matrix.sinh
     * @keywords acrsinh trigonometric
     * @example
     * [-3 3 100(         [A ARR_ASINH size=100 w=260 h=130 yr=-2..2]
     * |
     * [matrix.linspace]
     * |
     * [matrix.asinh]
     * |
     * [matrix->list]
     * |
     * [array.set ARR_ASINH]
     */
    MatrixTilde asinh() const;

    /**
     * @brief element-wise cosine
     * @return new matrix
     * @since 0.1
     * @see matrix.acos
     * @keywords cosine trigonometric
     * @example
     * [0 6.4 100(         [A ARR_COS size=100 w=260 h=130 yr=-1..1]
     * |
     * [matrix.linspace]
     * |
     * [matrix.cos]
     * |
     * [matrix->list]
     * |
     * [array.set ARR_COS]
     */
    MatrixTilde cos() const;

    /**
     * @brief element-wise acrcosine
     * @return new matrix
     * @since 0.1
     * @see matrix.cos
     * @keywords acrcosine trigonometric
     * @example
     * [-1 1 100(         [A ARR_ACOS size=100 w=260 h=130 yr=0..3]
     * |
     * [matrix.linspace]
     * |
     * [matrix.acos]
     * |
     * [matrix->list]
     * |
     * [array.set ARR_ACOS]
     */
    MatrixTilde acos() const;

    /**
     * @brief element-wise hyperbolic cosine
     * @return new matrix
     * @since 0.1
     * @see matrix.acosh
     * @keywords hyperbolic cosine trigonometric
     * @example
     * [-2 2 100(         [A ARR_COSH size=100 w=260 h=130 yr=0..4]
     * |
     * [matrix.linspace]
     * |
     * [matrix.cosh]
     * |
     * [matrix->list]
     * |
     * [array.set ARR_COSH]
     */
    MatrixTilde cosh() const;

    /**
     * @brief element-wise hyperbolic acrcosine
     * @return new matrix
     * @since 0.1
     * @see matrix.cosh
     * @keywords hyperbolic acrcosine trigonometric
     * @example
     * [0 4 100(         [A ARR_ACOSH size=100 w=260 h=130 yr=0..2]
     * |
     * [matrix.linspace]
     * |
     * [matrix.acosh]
     * |
     * [matrix->list]
     * |
     * [array.set ARR_ACOSH]
     */
    MatrixTilde acosh() const;

    /**
     * @brief element-wise tangent
     * @return new matrix
     * @since 0.1
     * @see matrix.atan matrix.sin
     * @keywords tangent trigonometric
     * @example
     * [-10 10 100(         [A ARR_TAN size=100 w=260 h=130 yr=-10..10]
     * |
     * [matrix.linspace]
     * |
     * [matrix.tan]
     * |
     * [matrix->list]
     * |
     * [array.set ARR_TAN]
     */
    MatrixTilde tan() const;

    /**
     * @brief element-wise arctangent
     * @return new matrix
     * @since 0.1
     * @see matrix.tan matrix.sin
     * @keywords arctangent trigonometric
     * @example
     * [-2 2 100(         [A ARR_ATAN size=100 w=260 h=130 yr=-1..1]
     * |
     * [matrix.linspace]
     * |
     * [matrix.atan]
     * |
     * [matrix->list]
     * |
     * [array.set ARR_ATAN]
     */
    MatrixTilde atan() const;

    /**
     * @brief element-wise hyperbolic tangent
     * @return new matrix
     * @since 0.1
     * @see matrix.atanh
     * @keywords tangent trigonometric hyperbolic
     * @example
     * [-3 3 100(         [A ARR_TANH size=100 w=260 h=130 yr=-1..1]
     * |
     * [matrix.linspace]
     * |
     * [matrix.tanh]
     * |
     * [matrix->list]
     * |
     * [array.set ARR_TANH]
     */
    MatrixTilde tanh() const;

    /**
     * @brief element-wise hyperbolic acrtangent
     * @return new matrix
     * @since 0.1
     * @see matrix.tanh
     * @keywords tangent trigonometric hyperbolic
     * @example
     * [-1 1 100(         [A ARR_ATANH size=100 w=260 h=130 yr=-3..3]
     * |
     * [matrix.linspace]
     * |
     * [matrix.atanh]
     * |
     * [matrix->list]
     * |
     * [array.set ARR_TANH]
     */
    MatrixTilde atanh() const;

    /**
     * @brief generate a column vector or row vector from matrix
     * @param dim - if == 0, generates column, if 1 - row
     * @return column or row
     * @since 0.1
     * @example
     * [2 2 1 2 3 4(  [T]
     * |              |.
     * [matrix.vectorise 1]
     * |
     * [ui.display @display_type 1]
     */
    MatrixTilde vectorise(bool dim) const;

    /**
     * @brief select matrix element at specified position
     * @param m - row number
     * @param n - column number
     * @since 0.1
     * @see matrix.col_at
     * @return float element
     * @example
     * [0 64 65(
     * |
     * [matrix.linspace]
     * |
     * [matrix.reshape 8 8]
     * |
     * |           [hradio]
     * |           |
     * |           |   [hradio]
     * |           |.  |..
     * [matrix.at     1 1]
     * |
     * [F]
     */
    t_float at(unsigned int m, unsigned int n) const;

public:
    std::string toString() const
    {
        std::ostringstream buf;
        m_.print(buf);
        return buf.str();
    }

    static const char* typeName()
    {
        return "Matrix";
    }

    Result setFromList(const ceammc::AtomListView& lv) override
    {
        try {
            m_ = fromList(ceammc::list::extractByType<t_float>(lv)).m_;
        } catch (std::exception& e) {
            return error(e.what());
        }

        return ok();
    }
};

#endif

MatrixTilde::MatrixTilde(unsigned int m, unsigned int n)
    : m_(m, n, fill::zeros)
{
}

bool MatrixTilde::operator==(const MatrixTilde& m) const
{
    return approx_equal(m_, m.m_, "absdiff", 0.00001);
}

MatrixTilde MatrixTilde::zeros(unsigned int m, unsigned int n)
{
    return MatrixTilde(MatT(m, n, fill::zeros));
}

MatrixTilde MatrixTilde::zeros_like() const
{
    return MatrixTilde(MatT(m_.n_rows, m_.n_rows, fill::zeros));
}

MatrixTilde MatrixTilde::ones(unsigned int m, unsigned int n)
{
    return MatrixTilde(MatT(m, n, fill::ones));
}

MatrixTilde MatrixTilde::ones_like() const
{
    return MatrixTilde(MatT(m_.n_rows, m_.n_rows, fill::ones));
}

MatrixTilde MatrixTilde::identity(unsigned int n)
{
    return MatrixTilde(MatT(n, n, fill::eye));
}

MatrixTilde MatrixTilde::full(unsigned int m, unsigned int n, t_float v)
{
    MatT mtx(m, n);
    return MatrixTilde(mtx.fill(v));
}

MatrixTilde MatrixTilde::fill(t_float v) const
{
    MatT m(m_);
    return MatrixTilde(m.fill(v));
}

MatrixTilde MatrixTilde::randu(unsigned int m, unsigned int n)
{
    return MatrixTilde(MatT(m, n, fill::randu));
}

MatrixTilde MatrixTilde::randn(unsigned int m, unsigned int n)
{
    return MatrixTilde(MatT(m, n, fill::randn));
}

MatrixTilde MatrixTilde::replace(t_float old_value, t_float new_value) const
{
    MatT m(m_);
    return MatrixTilde(m.replace(old_value, new_value));
}

MatrixTilde MatrixTilde::reshape(int nrows, int ncols) const
{
    if (nrows < 1 || ncols < 1)
        throw std::runtime_error("invalid matrix size");

    MatT m(m_);
    m.reshape(nrows, ncols);
    return MatrixTilde(m);
}

MatrixTilde MatrixTilde::resize(int nrows, int ncols) const
{
    if (nrows < 1 || ncols < 1) {
        std::ostringstream s;
        s << "invalid matrix size: " << nrows << 'x' << ncols;
        throw std::runtime_error(s.str());
    }

    MatT m(m_);
    m.resize(nrows, ncols);
    return MatrixTilde(m);
}

MatrixTilde MatrixTilde::col_at(unsigned int n) const
{
    if (n >= m_.n_cols) {
        std::ostringstream s;
        s << "invalid column number: " << n;
        throw std::runtime_error(s.str());
    }

    return MatrixTilde(m_.col(n));
}

MatrixTilde MatrixTilde::row_at(unsigned int n) const
{
    if (n >= m_.n_rows) {
        std::ostringstream s;
        s << "invalid row number: " << n;
        throw std::runtime_error(s.str());
    }

    return MatrixTilde(m_.row(n));
}

MatrixTilde MatrixTilde::insert_row(unsigned int n, const MatrixTilde& row) const
{
    if (n > m_.n_rows) {
        std::ostringstream s;
        s << "invalid row number: " << n;
        throw std::runtime_error(s.str());
    }

    if (row.nrows() > 1)
        throw std::runtime_error("not a row given");

    MatT m(m_);
    m.insert_rows(n, row.m_);
    return MatrixTilde(m);
}

MatrixTilde MatrixTilde::insert_col(unsigned int n, const MatrixTilde& col) const
{
    if (n > m_.n_cols) {
        std::ostringstream s;
        s << "invalid column number: " << n;
        throw std::runtime_error(s.str());
    }

    if (col.ncols() > 1)
        throw std::runtime_error("not a column given");

    MatT m(m_);
    m.insert_cols(n, col.m_);
    return MatrixTilde(m);
}

MatrixTilde MatrixTilde::remove_row(unsigned int n) const
{
    if (n >= m_.n_rows) {
        std::ostringstream s;
        s << "invalid row number: " << n;
        throw std::runtime_error(s.str());
    }

    MatT m(m_);
    m.shed_row(n);
    return MatrixTilde(m);
}

MatrixTilde MatrixTilde::remove_col(unsigned int n) const
{
    if (n >= m_.n_cols) {
        std::ostringstream s;
        s << "invalid column number: " << n;
        throw std::runtime_error(s.str());
    }

    MatT m(m_);
    m.shed_col(n);
    return MatrixTilde(m);
}

MatrixTilde MatrixTilde::swap_cols(unsigned int a, unsigned int b) const
{
    if (a >= m_.n_cols) {
        std::ostringstream s;
        s << "invalid column index: " << a;
        throw std::runtime_error(s.str());
    }

    if (b >= m_.n_cols) {
        std::ostringstream s;
        s << "invalid column index: " << b;
        throw std::runtime_error(s.str());
    }

    if (a == b)
        return *this;

    MatT m(m_);
    m.swap_cols(a, b);
    return MatrixTilde(m);
}

MatrixTilde MatrixTilde::swap_rows(unsigned int a, unsigned int b) const
{
    if (a >= m_.n_rows) {
        std::ostringstream s;
        s << "invalid row index: " << a;
        throw std::runtime_error(s.str());
    }

    if (b >= m_.n_rows) {
        std::ostringstream s;
        s << "invalid row index: " << b;
        throw std::runtime_error(s.str());
    }

    if (a == b)
        return *this;

    MatT m(m_);
    m.swap_rows(a, b);
    return MatrixTilde(m);
}

t_float MatrixTilde::min() const
{
    return m_.min();
}

t_float MatrixTilde::max() const
{
    return m_.max();
}

std::vector<int> MatrixTilde::size() const
{
    return { int(m_.n_rows), int(m_.n_cols) };
}

int MatrixTilde::nrows() const
{
    return m_.n_rows;
}

int MatrixTilde::ncols() const
{
    return m_.n_cols;
}

MatrixTilde MatrixTilde::diag(int n) const
{
    return MatrixTilde(m_.diag(n));
}

MatrixTilde MatrixTilde::transpose() const
{
    return MatrixTilde(trans(m_));
}

MatrixTilde MatrixTilde::inverse() const
{
    return MatrixTilde(m_.i());
}

int MatrixTilde::rank() const
{
    return arma::rank(m_);
}

MatrixTilde MatrixTilde::add(const MatrixTilde& m) const
{
    return MatrixTilde(m_ + m.m_);
}

MatrixTilde MatrixTilde::add(t_float v) const
{
    return MatrixTilde(m_ + v);
}

MatrixTilde MatrixTilde::sub(const MatrixTilde& m) const
{
    return MatrixTilde(m_ - m.m_);
}

MatrixTilde MatrixTilde::sub(t_float v) const
{
    return MatrixTilde(m_ - v);
}

MatrixTilde MatrixTilde::matmul(const MatrixTilde& m) const
{
    return MatrixTilde(m_ * m.m_);
}

MatrixTilde MatrixTilde::mul(t_float v) const
{
    return MatrixTilde(m_ * v);
}

MatrixTilde MatrixTilde::load(const std::string& fname)
{
    MatT m;

    if (!m.load(DataIFace::findFileName(fname), arma::csv_ascii)) {
        std::ostringstream ss;
        ss << "can't load matrix from: \"" << fname << '"';
        throw std::runtime_error(ss.str());
    }

    return MatrixTilde(m);
}

MatrixTilde MatrixTilde::save(const std::string& fname) const
{
    std::string fname2 = DataIFace::makeNewFileName(fname);
    if (!m_.save(fname2, arma::csv_ascii)) {
        std::ostringstream ss;
        ss << "can't save matrix to: \"" << fname2 << '"';
        throw std::runtime_error(ss.str());
    }

    return *this;
}

MatrixTilde MatrixTilde::div(const MatrixTilde& m) const
{
    return MatrixTilde(m_ / m.m_);
}

MatrixTilde MatrixTilde::div(t_float v) const
{
    return MatrixTilde(m_ / v);
}

MatrixTilde MatrixTilde::shift_col(int n) const
{
    return MatrixTilde(shift(m_, n, 0));
}

MatrixTilde MatrixTilde::shift_row(int n) const
{
    return MatrixTilde(shift(m_, n, 1));
}

std::tuple<std::vector<t_float>, std::vector<t_float>> MatrixTilde::toList() const
{
    std::tuple<std::vector<t_float>, std::vector<t_float>> res;

    for (int i = 0; i < m_.n_rows; i++) {
        for (int j = 0; j < m_.n_cols; j++)
            std::get<0>(res).push_back(m_.at(i, j));
    }

    std::get<1>(res) = { float(m_.n_rows), float(m_.n_cols) };

    return res;
}

MatrixTilde MatrixTilde::solve(const MatrixTilde& b) const
{
    return MatrixTilde(arma::solve(m_, b.m_));
}

MatrixTilde MatrixTilde::conv2(const MatrixTilde& b) const
{
    return MatrixTilde(arma::conv2(m_, b.m_, "same"));
}

MatrixTilde MatrixTilde::gt(const MatrixTilde& b) const
{
    return MatrixTilde(arma::conv_to<MatT>::from(m_ > b.m_));
}

MatrixTilde MatrixTilde::gt(t_float v) const
{
    return MatrixTilde(arma::conv_to<MatT>::from(m_ > MatT(arma::size(m_)).fill(v)));
}

MatrixTilde MatrixTilde::ge(const MatrixTilde& b) const
{
    return MatrixTilde(arma::conv_to<MatT>::from(m_ >= b.m_));
}

MatrixTilde MatrixTilde::ge(t_float v) const
{
    return MatrixTilde(arma::conv_to<MatT>::from(m_ >= MatT(arma::size(m_)).fill(v)));
}

MatrixTilde MatrixTilde::le(const MatrixTilde& b) const
{
    return MatrixTilde(arma::conv_to<MatT>::from(m_ <= b.m_));
}

MatrixTilde MatrixTilde::le(t_float v) const
{
    return MatrixTilde(arma::conv_to<MatT>::from(m_ <= MatT(arma::size(m_)).fill(v)));
}

MatrixTilde MatrixTilde::lt(t_float v) const
{
    return MatrixTilde(arma::conv_to<MatT>::from(m_ < MatT(arma::size(m_)).fill(v)));
}

MatrixTilde MatrixTilde::lt(const MatrixTilde& b) const
{
    return MatrixTilde(arma::conv_to<MatT>::from(m_ < b.m_));
}

MatrixTilde MatrixTilde::eq(const MatrixTilde& b) const
{
    return MatrixTilde(arma::conv_to<MatT>::from(m_ == b.m_));
}

MatrixTilde MatrixTilde::ne(t_float v) const
{
    return MatrixTilde(arma::conv_to<MatT>::from(m_ != MatT(arma::size(m_)).fill(v)));
}

MatrixTilde MatrixTilde::ne(const MatrixTilde& b) const
{
    return MatrixTilde(arma::conv_to<MatT>::from(m_ != b.m_));
}

MatrixTilde MatrixTilde::eq(t_float v) const
{
    return MatrixTilde(arma::conv_to<MatT>::from(m_ == MatT(arma::size(m_)).fill(v)));
}

MatrixTilde MatrixTilde::mul(const MatrixTilde& b) const
{
    return MatrixTilde(m_ % b.m_);
}

MatrixTilde MatrixTilde::fromList(const std::vector<t_float> &v)
{
    if (v.size() < 2)
        throw std::runtime_error("expected MROWS NCOLS [values...]");

    int m = v[0];
    int n = v[1];

    if (m < 1 || n < 1) {
        std::ostringstream os;
        os << "invalid matrix size: " << m << 'x' << n;
        throw std::runtime_error(os.str());
    }

    size_t total = m * n;
    if (v.size() > total + 2)
        throw std::runtime_error("extra values in list");

    MatT res(m, n, fill::zeros);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            auto idx = i * n + j;
            if (idx >= v.size())
                break;

            res.at(i, j) = v[idx + 2];
        }
    }

    return MatrixTilde(res);
}

MatrixTilde MatrixTilde::col(const std::vector<t_float>& v)
{
    return MatrixTilde(Col<ValueType>(v));
}

MatrixTilde MatrixTilde::row(const std::vector<t_float> &v)
{
    return MatrixTilde(Row<ValueType>(v));
}

MatrixTilde MatrixTilde::linspace(t_float start, t_float end, unsigned int N)
{
    return MatrixTilde(arma::linspace<Row<ValueType>>(start, end, (N == 0) ? 100 : N));
}

MatrixTilde MatrixTilde::logspace(t_float A, t_float B, unsigned int N)
{
    return MatrixTilde(arma::logspace<Row<ValueType>>(A, B, (N == 0) ? 50 : N));
}

MatrixTilde MatrixTilde::abs() const
{
    return MatrixTilde(arma::abs(m_));
}

t_float MatrixTilde::accu() const
{
    return arma::accu(m_);
}

bool MatrixTilde::all() const
{
    return arma::all(arma::vectorise(m_));
}

MatrixTilde MatrixTilde::all_col() const
{
    return MatrixTilde(arma::conv_to<MatT>::from(arma::all(m_, 0)));
}

MatrixTilde MatrixTilde::all_row() const
{
    return MatrixTilde(arma::conv_to<MatT>::from(arma::all(m_, 1)));
}

bool MatrixTilde::any() const
{
    return arma::any(arma::vectorise(m_));
}

MatrixTilde MatrixTilde::any_col() const
{
    return MatrixTilde(arma::conv_to<MatT>::from(arma::any(m_, 0)));
}

MatrixTilde MatrixTilde::any_row() const
{
    return MatrixTilde(arma::conv_to<MatT>::from(arma::any(m_, 1)));
}

MatrixTilde MatrixTilde::clip(t_float min, t_float max) const
{
    return MatrixTilde(arma::clamp(m_, min, max));
}

t_float MatrixTilde::det() const
{
    return arma::det(m_);
}

t_float MatrixTilde::mean() const
{
    return arma::mean(arma::mean(m_));
}

MatrixTilde MatrixTilde::mean_col() const
{
    return MatrixTilde(arma::mean(m_, DIR_COL));
}

MatrixTilde MatrixTilde::mean_row() const
{
    return MatrixTilde(arma::mean(m_, DIR_ROW));
}

t_float MatrixTilde::median() const
{
    return arma::median(arma::vectorise(m_));
}

MatrixTilde MatrixTilde::median_col() const
{
    return MatrixTilde(arma::median(m_, DIR_COL));
}

MatrixTilde MatrixTilde::median_row() const
{
    return MatrixTilde(arma::median(m_, DIR_ROW));
}

t_float MatrixTilde::stddev(bool norm_type) const
{
    return arma::stddev(arma::vectorise(m_), norm_type ? 1 : 0);
}

MatrixTilde MatrixTilde::stddev_col(bool norm_type) const
{
    return MatrixTilde(arma::stddev(m_, norm_type ? 1 : 0, DIR_COL));
}

MatrixTilde MatrixTilde::stddev_row(bool norm_type) const
{
    return MatrixTilde(arma::stddev(m_, norm_type ? 1 : 0, DIR_ROW));
}

t_float MatrixTilde::range() const
{
    return arma::range(arma::vectorise(m_));
}

MatrixTilde MatrixTilde::range_col() const
{
    return MatrixTilde(arma::range(m_, DIR_COL));
}

MatrixTilde MatrixTilde::range_row() const
{
    return MatrixTilde(arma::range(m_, DIR_ROW));
}

MatrixTilde MatrixTilde::hist(unsigned int n_bins) const
{
    if (m_.is_colvec())
        return MatrixTilde(arma::conv_to<MatT>::from(arma::hist(arma::conv_to<fcolvec>::from(m_), n_bins)));
    else if (m_.is_rowvec())
        return MatrixTilde(arma::conv_to<MatT>::from(arma::hist(arma::conv_to<frowvec>::from(m_), n_bins)));
    else
        throw std::runtime_error("not a vector given");
}

MatrixTilde MatrixTilde::exp() const
{
    return MatrixTilde(arma::exp(m_));
}

MatrixTilde MatrixTilde::exp2() const
{
    return MatrixTilde(arma::exp2(m_));
}

MatrixTilde MatrixTilde::exp10() const
{
    return MatrixTilde(arma::exp10(m_));
}

MatrixTilde MatrixTilde::trunc_exp() const
{
    return MatrixTilde(arma::trunc_exp(m_));
}

MatrixTilde MatrixTilde::sign() const
{
    return MatrixTilde(arma::sign(m_));
}

MatrixTilde MatrixTilde::sin() const
{
    return MatrixTilde(arma::sin(m_));
}

MatrixTilde MatrixTilde::asin() const
{
    return MatrixTilde(arma::asin(m_));
}

MatrixTilde MatrixTilde::sinh() const
{
    return MatrixTilde(arma::sinh(m_));
}

MatrixTilde MatrixTilde::asinh() const
{
    return MatrixTilde(arma::asinh(m_));
}

MatrixTilde MatrixTilde::cos() const
{
    return MatrixTilde(arma::cos(m_));
}

MatrixTilde MatrixTilde::acos() const
{
    return MatrixTilde(arma::acos(m_));
}

MatrixTilde MatrixTilde::cosh() const
{
    return MatrixTilde(arma::cosh(m_));
}

MatrixTilde MatrixTilde::acosh() const
{
    return MatrixTilde(arma::acosh(m_));
}

MatrixTilde MatrixTilde::tan() const
{
    return MatrixTilde(arma::tan(m_));
}

MatrixTilde MatrixTilde::atan() const
{
    return MatrixTilde(arma::atan(m_));
}

MatrixTilde MatrixTilde::tanh() const
{
    return MatrixTilde(arma::tanh(m_));
}

MatrixTilde MatrixTilde::atanh() const
{
    return MatrixTilde(arma::atanh(m_));
}

MatrixTilde MatrixTilde::vectorise(bool dim) const
{
    return MatrixTilde(arma::vectorise(m_, dim ? DIR_ROW : DIR_COL));
}

t_float MatrixTilde::at(unsigned int m, unsigned int n) const
{
    return m_(m, n);
}
