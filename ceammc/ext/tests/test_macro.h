/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef TEST_MACRO_H
#define TEST_MACRO_H

// Accept any number of args >= N, but expand to just the Nth one.
// Here, N == 6.
#define _GET_NTH_ARG(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N

// Define some macros to help us create overrides based on the
// arity of a for-each-style macro.
#define _fe_0(_call, ...)
#define _fe_1(_call, x) _call(x)
#define _fe_2(_call, x, ...) _call(x), _fe_1(_call, __VA_ARGS__)
#define _fe_3(_call, x, ...) _call(x), _fe_2(_call, __VA_ARGS__)
#define _fe_4(_call, x, ...) _call(x), _fe_3(_call, __VA_ARGS__)
#define _fe_5(_call, x, ...) _call(x), _fe_4(_call, __VA_ARGS__)
#define _fe_6(_call, x, ...) _call(x), _fe_5(_call, __VA_ARGS__)
#define _fe_7(_call, x, ...) _call(x), _fe_6(_call, __VA_ARGS__)
#define _fe_8(_call, x, ...) _call(x), _fe_7(_call, __VA_ARGS__)
#define _fe_9(_call, x, ...) _call(x), _fe_8(_call, __VA_ARGS__)
#define _fe_10(_call, x, ...) _call(x), _fe_9(_call, __VA_ARGS__)
#define _fe_11(_call, x, ...) _call(x), _fe_10(_call, __VA_ARGS__)
#define _fe_12(_call, x, ...) _call(x), _fe_11(_call, __VA_ARGS__)
#define _fe_13(_call, x, ...) _call(x), _fe_12(_call, __VA_ARGS__)
#define _fe_14(_call, x, ...) _call(x), _fe_13(_call, __VA_ARGS__)
#define _fe_15(_call, x, ...) _call(x), _fe_14(_call, __VA_ARGS__)

/**
 * Provide a for-each construct for variadic macros. Supports up
 * to 4 args.
 *
 * Example usage1:
 *     #define FWD_DECLARE_CLASS(cls) class cls;
 *     CALL_MACRO_X_FOR_EACH(FWD_DECLARE_CLASS, Foo, Bar)
 *
 * Example usage 2:
 *     #define START_NS(ns) namespace ns {
 *     #define END_NS(ns) }
 *     #define MY_NAMESPACES System, Net, Http
 *     CALL_MACRO_X_FOR_EACH(START_NS, MY_NAMESPACES)
 *     typedef foo int;
 *     CALL_MACRO_X_FOR_EACH(END_NS, MY_NAMESPACES)
 */

#define CALL_MACRO_X_FOR_EACH(x, ...)                                                             \
    _GET_NTH_ARG("ignored", ##__VA_ARGS__, _fe_15, _fe_14, _fe_13, _fe_12, _fe_11, _fe_10, _fe_9, \
        _fe_8, _fe_7, _fe_6, _fe_5, _fe_4, _fe_3, _fe_2, _fe_1, _fe_0)                            \
    (x, ##__VA_ARGS__)

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

#ifndef TEST_BIN_DIR
#define TEST_BIN_DIR "."
#endif

// simple wrappers
#define S(v) Atom(gensym(v))
#define F(v) Atom(t_float(v))
#define A(v) test_atom_wrap(v)
#define SYM(txt) gensym(txt)

/**
 * @brief empty list
 */
#define L() AtomList()

/**
 * @brief variadic list of atoms (mixed)
 */
#define _TEST_ATOM_WRAP(elem) test_atom_wrap(elem)
#define LA(...) AtomList({ CALL_MACRO_X_FOR_EACH(_TEST_ATOM_WRAP, __VA_ARGS__) })

/**
 * @brief create atomlist from parsed string (as Pd does for object arguments
 */
#define LP(s) AtomList::parseString(s)

/**
 * @brief variadic list of floats (only)
 */
#define LF(...) AtomList({ __VA_ARGS__ })

/**
 * @brief variadic list of float aprrox compare
 */
#define LX(...) ListApprox({ __VA_ARGS__ })

/**
 * @brief variadic list of float aprrox compare
 */
#define LAX(...) AtomListApprox(LA(__VA_ARGS__))

/**
 * @brief variadic list of atoms (mixed)
 */
#define LD(...) DataAtomList({ CALL_MACRO_X_FOR_EACH(_TEST_ATOM_WRAP, __VA_ARGS__) })

#endif
