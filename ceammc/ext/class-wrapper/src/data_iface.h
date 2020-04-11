/*
 * The MIT License (MIT)
 *
 * Copyright (C) 2018-2019 Alex Nadzharov, Serge Poltavsky
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#ifndef DATA_IFACE_H
#define DATA_IFACE_H

#include "ceammc_atomlist.h"
#include "ceammc_format.h"

#include <boost/optional.hpp>
#include <string>

/**
 * @brief Result of operator class: ok or error
 */
class Result : public boost::optional<std::string> {
    int rc_;

public:
    /**
     * @brief Result constructor
     * @param errMsg - error message
     * @param code error code
     */
    Result(const std::string& errMsg, int code = 1);
    Result();

    operator bool() const
    {
        return rc_ == 0;
    }

    /**
     * check for success
     * @return true if no error
     */
    bool isOk() const;

    /**
     * check for error
     * @param msg - pointer to error message to write to
     * @param code - pointer to code, to write to
     * @return true if error
     */
    bool error(std::string* msg = nullptr, int* code = nullptr) const;

    /**
     * result code
     */
    int code() const;
};

/**
 * @brief Base class for user wrapped data
 *
 * Wrapped data class can contains member functions and static member functions.
 * This method arguments and return types are supported now:
 *     - bool
 *     - int
 *     - unsigned int
 *     - long
 *     - unsigned long
 *     - float
 *     - double
 *     - std::string
 *     - std::vector<float>
 *     - std::vector<int>
 *     - std::vector<std::string>
 *     - t_symbol* (Pd data type)
 *     - ceammc::Atom
 *     - ceammc::AtomList
 *
 * Also type std::tuple<...> is supported with all listed above arguments only for output result.
 * In such case each tuple value is outputted to separate outlet.
 * For example wrapper pd object for method: std::tuple<int, int> MyData::foo(int i) const
 * will have two inlet (first for MyData type, second for @b i argument) and
 * two outlets for two result ints in tuple.
 *
 * Output rules (with pd types):
 *     - bool -> 1.f or 0.f t_float
 *     - int, long float, double etc. -> t_float
 *     - std::string -> t_symbol*
 *     - std::vector<...> -> list
 *
 * Input rules:
 *     - pd -> bool: 1.f, 0.f, "true" or "false" are accepted as bool values
 *     - pd -> int: floats converted to int (long, etc) simple as int(1.0f)
 *
 * Subclass MyData of DataIFace should implement at least this methods:
 *     -  MyData(int v = 0) - empty constructor or constructor with all default values
 *     -  bool operator==(const MyData& m) const - equal check operator
 *     -  std::string toString() const - convertion to string
 *     -  static const char* typeName() - data type name
 *
 * Also you can override this functions:
 *     - Result setFromFloat(t_float f)
 *     - Result setFromSymbol(t_symbol* s)
 *     - Result setFromList(const ceammc::AtomList& l)
 *
 * to initialize your data from float, symbol or list respectively
 *
 * Documentation
 *
 * This doxygen tags are used for generating pddoc documentation:
 *     - "@brief" - object description
 *     - "@see" - cross reference to other objects, space separated list
 *     - "@return" - description of object output (repeat for several outlets)
 *     - "@since" - version of library from which this function became available
 *     - "@pd_name" - change Pd name
 *     - "@pd_alias" add Pd object alias
 *     - "@keywords" - space separated list of keywords
 *     - "@version" - wrapper library version (used in first class declaration)
 *     - "@library" - wrapper library name (used in first class declaration)
 *     - "@license" - wrapper library license (used in first class declaration)
 *     - "@author" - library author (can be repeated several times)
 *     - "@example" - usage example in ascii pddoc format
 */
class DataIFace {
public:
    enum SetStatusCode {
        OK = 0,
        ERROR,
        NOT_SUPPORTED
    };

public:
    DataIFace();
    virtual ~DataIFace();

    /**
     * @brief set data from float. You should override it for usage
     * @param f - input float
     * @return operation result
     */
    virtual Result setFromFloat(t_float f)
    {
        return Result(std::string("unexpected float: ") + std::to_string(f), NOT_SUPPORTED);
    }

    /**
     * @brief set data from symbol
     * @param s - input symbol
     * @return opration result
     */
    virtual Result setFromSymbol(t_symbol* s)
    {
        return Result(std::string("unexpected symbol: ") + s->s_name, NOT_SUPPORTED);
    }

    /**
     * @brief set data from list. You should override it for usage
     * @param l - input list
     * @return operation result
     */
    virtual Result setFromList(const ceammc::AtomList& l)
    {
        return Result(std::string("unexpected list: ") + ceammc::to_string(l), NOT_SUPPORTED);
    }

    /**
     * @brief set data from any message (Pd). You should override it for usage
     * @param s - message selector
     * @param l - message content
     * @return operation result
     */
    virtual Result setFromAny(t_symbol* s, const ceammc::AtomList& l)
    {
        return Result("unexpected message", NOT_SUPPORTED);
    }

    /**
     * @brief set data value from PureData list
     * @param lst - input list
     * @return operation result
     */
    Result setFromPd(const ceammc::AtomList& lst);

    /**
     * @brief toJsonString
     */
    virtual std::string toJsonString() const { return ""; }

public:
    /**
     * helper ok function
     */
    static Result ok();

    /**
     * @brief helper error function
     * @param msg - error message
     */
    static Result error(const std::string& msg);

    /**
     * @brief helper function to make absolute filenames
     * if absolute filename given it returned unchanged, if realtive filename is given:
     * - trying to create filename relative current canvas path, if no current canvas (new unsaved document)
     * - creates filename realtive to Pd user directory
     * @param path - file path
     * @return new path
     * @note no read/write check performed for result path
     */
    static std::string makeNewFileName(const std::string& path);

    /**
     * @brief helper function for making absolute filenames
     * if absolute filename given it returned unchanged, if realtive filename is given:
     * - trying to find filename relative current canvas path, if no current canvas (new unsaved document)
     * - creates filename realtive to Pd user directory
     * @param path
     * @return new path
     * @note no file existance check performed for result path
     */
    static std::string findFileName(const std::string& path);
};

#endif // DATA_IFACE_H
