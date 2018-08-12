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

#ifndef BOOST_PP_VARIADICS
  #define BOOST_PP_VARIADICS
#endif

#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/tuple/rem.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#ifndef TEST_DATA_DIR
#define TEST_DATA_DIR "."
#endif

// simple wrappers
#define S(v) Atom(gensym(v))
#define F(v) Atom(float(v))
#define A(v) test_atom_wrap(v)
#define SYM(txt) gensym(txt)

// helper macroses
#define _TEST_ATOM_WRAP(s, data, elem) test_atom_wrap(elem)
#define _TEST_ATOM_SEQ(...) BOOST_PP_SEQ_TRANSFORM(_TEST_ATOM_WRAP, 1, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))
#define _TEST_ATOM_TUPLE(...) BOOST_PP_SEQ_TO_TUPLE(_TEST_ATOM_SEQ(__VA_ARGS__))

/**
 * @brief empty list
 */
#define L() AtomList()

/**
 * @brief variadic list of atoms (mixed)
 */
#define LA(...) AtomList({ BOOST_PP_TUPLE_REM_CTOR(_TEST_ATOM_TUPLE(__VA_ARGS__)) })

/**
 * @brief variadic list of floats (only)
 */
#define LF(...) AtomList({ __VA_ARGS__ })

/**
 * @brief variadic list of float aprrox compare
 */
#define LX(...) ListApprox({ __VA_ARGS__ })

/**
 * @brief variadic list of atoms (mixed)
 */
#define LD(...) DataAtomList({ BOOST_PP_TUPLE_REM_CTOR(_TEST_ATOM_TUPLE(__VA_ARGS__)) })

#endif
