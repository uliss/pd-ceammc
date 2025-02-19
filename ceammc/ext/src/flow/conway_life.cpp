/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#include "conway_life.h"

#include <cstring>
#include <iostream>
#include <random>

static std::mt19937 gen;

ConwayLife::ConwayLife()
{
}

bool ConwayLife::random(float density)
{
    if (density < 0 || density > 1)
        return false;

    const CellType thres = std::numeric_limits<CellType>::max() * density;
    auto dist = std::uniform_int_distribution<CellType>(0);
    for (size_t r = 0; r < rows_; r++) {
        for (size_t c = 0; c < cols_; c++) {
            auto v = dist(gen) < thres;
            refStateAt(r, c) = v;
            alive_ += v;
        }
    }

    return true;
}

bool ConwayLife::set(uint16_t rows, uint16_t cols)
{
    auto N = rows * cols;
    if (N > MAXSIZE || N == 0)
        return false;

    rows_ = rows;
    cols_ = cols;
    alive_ = 0;
    clear();
    return true;
}

bool ConwayLife::set(std::initializer_list<bool> l)
{
    if (l.size() != numCells())
        return false;

    alive_ = 0;

    for (size_t i = 0; i < l.size(); i++) {
        auto v = *(l.begin() + i);
        states_[i] = v;
        alive_ += v;
    }

    return true;
}

bool ConwayLife::setAt(uint16_t row, uint16_t col, bool v)
{
    if (row >= rows_ || col >= cols_)
        return false;

    auto& cell = refStateAt(row, col);
    if (cell != v) {
        cell = v;
        alive_ += v ? 1 : -1;
    }

    return true;
}

bool ConwayLife::flipAt(uint16_t row, uint16_t col)
{
    if (row >= rows_ || col >= cols_)
        return false;

    auto& cell = refStateAt(row, col);
    alive_ += cell ? -1 : 1;
    cell = !cell;
    return true;
}

void ConwayLife::clear()
{
    memset(states_, 0, numCells() * sizeof(CellType));
    alive_ = 0;
}
void ConwayLife::clearCounts()
{
    memset(counts_, 0, numCells() * sizeof(CellType));
}

void ConwayLife::next()
{
    clearCounts();
    countNeighbours();

    for (size_t r = 0; r < rows_; r++) {
        for (size_t c = 0; c < cols_; c++) {
            const auto i = idx(r, c);
            const auto n = counts_[i];
            if (states_[i] == 1 && (n < 2 || n > 3)) { // die
                states_[i] = 0;
                alive_--;
            } else if (states_[i] == 0 && n == 3) { // born
                states_[i] = 1;
                alive_++;
            }
        }
    }
}

void ConwayLife::addBlock(uint16_t row, uint16_t col)
{
    setAt(row + 0, col + 0, true)
        && setAt(row + 0, col + 1, true)
        && setAt(row + 1, col + 0, true)
        && setAt(row + 1, col + 1, true);
}

void ConwayLife::addHive(uint16_t row, uint16_t col)
{
    setAt(row + 0, col + 1, true)
        && setAt(row + 0, col + 2, true)
        && setAt(row + 1, col + 0, true)
        && setAt(row + 1, col + 3, true)
        && setAt(row + 2, col + 1, true)
        && setAt(row + 2, col + 2, true);
}

void ConwayLife::addVHive(uint16_t row, uint16_t col)
{
    setAt(row + 0, col + 1, true)
        && setAt(row + 1, col + 0, true)
        && setAt(row + 1, col + 2, true)
        && setAt(row + 2, col + 0, true)
        && setAt(row + 2, col + 2, true)
        && setAt(row + 3, col + 1, true);
}

void ConwayLife::addGlider(uint16_t row, uint16_t col)
{
    setAt(row + 0, col + 2, true)
        && setAt(row + 1, col + 0, true)
        && setAt(row + 1, col + 2, true)
        && setAt(row + 2, col + 1, true)
        && setAt(row + 2, col + 2, true);
}

void ConwayLife::addRPentamino(uint16_t row, uint16_t col)
{
    setAt(row + 0, col + 1, true)
        && setAt(row + 1, col + 1, true)
        && setAt(row + 2, col + 1, true)
        && setAt(row + 0, col + 2, true)
        && setAt(row + 1, col + 0, true);
}

void ConwayLife::addBlinker(uint16_t row, uint16_t col)
{
    setAt(row + 0, col, true)
        && setAt(row + 1, col, true)
        && setAt(row + 2, col, true);
}

void ConwayLife::addOctagon2(uint16_t row, uint16_t col)
{
    addFigure(row, col, { 8, 8 }, {
                                      0, 0, 0, 1, 1, 0, 0, 0, //
                                      0, 0, 1, 0, 0, 1, 0, 0, //
                                      0, 1, 0, 0, 0, 0, 1, 0, //
                                      1, 0, 0, 0, 0, 0, 0, 1, //
                                      1, 0, 0, 0, 0, 0, 0, 1, //
                                      0, 1, 0, 0, 0, 0, 1, 0, //
                                      0, 0, 1, 0, 0, 1, 0, 0, //
                                      0, 0, 0, 1, 1, 0, 0, 0, //
                                  });
}

bool ConwayLife::addFigure(uint16_t row, uint16_t col, std::pair<uint16_t, uint16_t> size, std::initializer_list<bool> l)
{
    const auto data_row = size.first;
    const auto data_col = size.second;

    if (data_row * data_col > l.size())
        return false;

    for (uint16_t r = 0; r < data_row; r++) {
        for (uint16_t c = 0; c < data_col; c++) {
            auto idx = r * data_col + c;
            auto v = l.begin()[idx];
            if (!v)
                continue;

            if (!setAt(row + r, col + c, true))
                break;
        }
    }

    return true;
}



void ConwayLife::countNeighbours()
{
    for (size_t r = 0; r < rows_; r++) {
        for (size_t c = 0; c < cols_; c++) {
            if (at(r, c)) {
                addNeighbour(r - 1, c - 1);
                addNeighbour(r - 1, c + 0);
                addNeighbour(r - 1, c + 1);
                addNeighbour(r + 0, c - 1);
                addNeighbour(r + 0, c + 1);
                addNeighbour(r + 1, c - 1);
                addNeighbour(r + 1, c + 0);
                addNeighbour(r + 1, c + 1);
            }
        }
    }
}

std::ostream& operator<<(std::ostream& os, const ConwayLife& life)
{
    for (size_t r = 0; r < life.rows(); r++) {
        for (size_t c = 0; c < life.cols(); c++)
            os << (life.at(r, c) ? '*' : '_');

        os << '\n';
    }

    return os;
}
