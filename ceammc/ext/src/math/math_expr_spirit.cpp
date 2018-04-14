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
#include "math_expr_spirit.h"
#include "m_pd.h"

/*=============================================================================
    Copyright (c) 2001-2003 Dan Nuffer
    Copyright (c) 2001-2003 Joel de Guzman
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
///////////////////////////////////////////////////////////////////////////////
//
//  Full calculator example with variables
//  [ JDG 9/18/2002 ]
//
///////////////////////////////////////////////////////////////////////////////
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_symbols.hpp>
#include <functional>
#include <iostream>
#include <stack>
#include <string>

using namespace std;
using namespace BOOST_SPIRIT_CLASSIC_NS;

///////////////////////////////////////////////////////////////////////////////
//
//  Semantic actions
//
///////////////////////////////////////////////////////////////////////////////
struct push_num {
    push_num(stack<double>& eval_)
        : eval(eval_)
    {
    }

    void operator()(double n) const
    {
        eval.push(n);
        cout << "push\t" << n << endl;
    }

    stack<double>& eval;
};

template <typename op>
struct do_op {
    do_op(op const& the_op, stack<double>& eval_)
        : m_op(the_op)
        , eval(eval_)
    {
    }

    void operator()(char const*, char const*) const
    {
        double rhs = eval.top();
        eval.pop();
        double lhs = eval.top();
        eval.pop();

        cout << "popped " << lhs << " and " << rhs << " from the stack. ";
        cout << "pushing " << m_op(lhs, rhs) << " onto the stack.\n";
        eval.push(m_op(lhs, rhs));
    }

    op m_op;
    stack<double>& eval;
};

template <class op> do_op<op>
make_op(op const& the_op, stack<double>& eval)
{
    return do_op<op>(the_op, eval);
}

struct do_negate {
    do_negate(stack<double>& eval_)
        : eval(eval_)
    {
    }

    void operator()(char const*, char const*) const
    {
        double lhs = eval.top();
        eval.pop();

        cout << "popped " << lhs << " from the stack. ";
        cout << "pushing " << -lhs << " onto the stack.\n";
        eval.push(-lhs);
    }

    stack<double>& eval;
};

struct get_var {
    get_var(stack<double>& eval_)
        : eval(eval_)
    {
    }

    void operator()(double n) const
    {
        eval.push(n);
        cout << "push\t" << n << endl;
    }

    stack<double>& eval;
};

struct set_var {
    set_var(double*& var_)
        : var(var_)
    {
    }

    void operator()(double& n) const
    {
        var = &n;
    }

    double*& var;
};

struct redecl_var {
    void operator()(double& /*n*/) const
    {
        cout << "Warning. You are attempting to re-declare a var.\n";
    }
};

struct do_assign {
    do_assign(double*& var_, stack<double>& eval_)
        : var(var_)
        , eval(eval_)
    {
    }

    void operator()(char const*, char const*) const
    {
        if (var != 0) {
            *var = eval.top();
            cout << "assigning\n";
        }
    }

    double*& var;
    stack<double>& eval;
};

///////////////////////////////////////////////////////////////////////////////
//
//  Our calculator grammar
//
///////////////////////////////////////////////////////////////////////////////
struct calculator : public grammar<calculator> {
    calculator(stack<double>& eval_)
        : eval(eval_)
    {
    }

    template <typename ScannerT>
    struct definition {
        definition(calculator const& self)
        {
            factor = real_p[push_num(self.eval)]
                | vars[get_var(self.eval)]
                | '(' >> expression >> ')'
                | ('-' >> factor)[do_negate(self.eval)];

            term = factor
                >> *(('*' >> factor)[make_op(multiplies<double>(), self.eval)]
                       | ('^' >> factor)[make_op(multiplies<double>(), self.eval)]
                       | ('/' >> factor)[make_op(divides<double>(), self.eval)]);

            expression = term
                >> *(('+' >> term)[make_op(plus<double>(), self.eval)]
                       | ('-' >> term)[make_op(minus<double>(), self.eval)]);

            //                        assignment = vars[set_var(self.var)]
            //                            >> '=' >> expression[do_assign(self.var, self.eval)];

            //            var_decl = lexeme_d
            //                [((alpha_p >> *(alnum_p | '_'))
            //                    - vars[redecl_var()])[vars.add]];

            //            declaration = lexeme_d["var" >> space_p] >> var_decl >> *(',' >> var_decl);

            statement = expression;
        }

        symbols<double> vars;
        rule<ScannerT> statement, declaration, var_decl,
            assignment, expression, term, factor;

        rule<ScannerT> const&
        start() const { return statement; }
    };

    mutable double* var;
    stack<double>& eval;
};

///////////////////////////////////////////////////////////////////////////////
//
//  Main program
//
///////////////////////////////////////////////////////////////////////////////
OptDouble parse(const char* s)
{
    post("parse: %s", s);

    cout << "/////////////////////////////////////////////////////////\n\n";
    cout << "\t\tThe calculator with variables...\n\n";
    cout << "/////////////////////////////////////////////////////////\n\n";
    cout << "Variables may be declared:\t\tExample: var i, j, k\n";
    cout << "Assigning to a variable:\t\tExample: i = 10 * j\n";
    cout << "To evaluate an expression:\t\tExample: i * 3.33E-3\n\n";

    stack<double> eval;
    calculator calc(eval); //  Our parser

    string str(s);

    parse_info<> info = parse(str.c_str(), calc, space_p);

    if (info.full) {
        cout << "-------------------------\n";
        cout << "Parsing succeeded\n";
        cout << "-------------------------\n";
    } else {
        cout << "-------------------------\n";
        cout << "Parsing failed\n";
        cout << "stopped at: \": " << info.stop << "\"\n";
        cout << "-------------------------\n";
    }

    if (!eval.empty())
        return eval.top();

    return boost::none;
}

/*

/ Symbol table for storing variables.
typedef std::map<std::string, double> symtab_t;

struct calculator : boost::spirit::classic::grammar<calculator> {
    // The parser object is copied a lot, so instead of keeping its own table
    // of variables, it keeps track of a reference to a common table.
    calculator(symtab_t& variables)
        : variables(variables)
    {
    }

    // A production can have an associated closure, to store information
    // for that production.
    struct value_closure : boost::spirit::classic::closure<value_closure, double> {
        member1 value;
    };

    struct assignment_closure : boost::spirit::classic::closure<assignment_closure, std::string, double> {
        member1 name;
        member2 value;
    };

    struct string_closure : boost::spirit::classic::closure<string_closure, std::string> {
        member1 name;
    };

    // Following is the grammar definition.
    template <typename ScannerT>
    struct definition {
        definition(calculator const& self)
        {
            using namespace boost::spirit::classic;
            using namespace phoenix;
            //            using namespace boost::phoenix;

            // The commands are linked to functors or member functions,
            // to demonstrate both styles. In real code, you should choose
            // one style and use it uniformly.
            command
                = as_lower_d["help"][do_help()];

            // The lexeme_d directive tells the scanner to treat white space as
            // significant. Thus, an identifier cannot have internal white space.
            // The alpha_p and alnum_p parsers are built-in.
            // Notice how the semantic action uses a Phoenix lambda function
            // that constructs a std::string. The arg1 and arg2 placeholders are
            // are bound at runtime to the iterator range that matches this rule.
            identifier
                = lexeme_d
                    [(alpha_p | '_')
                        >> *(alnum_p | '_')][identifier.name = construct_<std::string>(arg1, arg2)];

            group
                = '('
                >> expression[group.value = arg1]
                >> ')';

            // An assignment statement must store the variable name and value.
            // The name and the value are stored in the closure, then the define
            // function is called to store the definition. Notice how a rule can
            // have multiple semantic actions.
            assignment
                = identifier[assignment.name = arg1]
                >> '='
                >> expression[assignment.value = arg1]
                             [bind(&calculator::define, assignment.name, assignment.value)];

            // A statement can end at the end of the line, or with a semicolon.
            statement
                = (command
                      | assignment
                      | expression[boost::bind(&calculator::do_print, arg1)])
                >> (end_p | ';');

            // The longest_d directive is built-in to tell the parser to make
            // the longest match it can. Thus "1.23" matches real_p rather than
            // int_p followed by ".23".
            literal
                = longest_d
                    [int_p[literal.value = arg1]
                        | real_p[literal.value = arg1]];

            // A variable name must be looked up. This is a straightforward
            // Phoenix binding.
            factor
                = literal[factor.value = arg1]
                | group[factor.value = arg1]
                | identifier[factor.value = boost::bind(&calculator::lookup, arg1)];

            term
                = factor[term.value = arg1]
                >> *(('*' >> factor[term.value *= arg1])
                       | ('/' >> factor[term.value /= arg1]));

            expression
                = term[expression.value = arg1]
                >> *(('+' >> term[expression.value += arg1])
                       | ('-' >> term[expression.value -= arg1]));
        }

        // The start symbol is returned from start().
        boost::spirit::classic::rule<ScannerT> const&
        start() const { return statement; }

        // Each rule must be declared, optionally with an associated closure.
        boost::spirit::classic::rule<ScannerT> command, statement;
        boost::spirit::classic::rule<ScannerT, assignment_closure::context_t> assignment;
        boost::spirit::classic::rule<ScannerT, string_closure::context_t> identifier;
        boost::spirit::classic::rule<ScannerT, value_closure::context_t> expression, factor,
            group, literal, term;
    };

    // Member functions that are called in semantic actions.
    void define(const std::string& name, double value) const
    {
        variables[name] = value;
    }

    double lookup(const std::string& name) const
    {
        symtab_t::iterator it = variables.find(name);
        if (it == variables.end()) {
            std::cerr << "undefined name: " << name << '\n';
            return 0.0;
        } else
            return (*it).second;
    }

    void do_print(double x) const
    {
        std::cout << x << '\n';
    }

private:
    symtab_t& variables;
};

void parse(const char* str)
{
    using namespace boost::spirit::classic;
    using namespace std;
    symtab_t variables;
    calculator calc(variables);

    variables["pi"] = 3.141592653589792;

    // Read one line of text and parse it. If the parser does not consume
    // the entire string, keep parsing the same string until an error occurs
    // or the string is consumed. Then go back and read another string.
    std::string line(str);
    string::iterator first = line.begin();
    parse_info<string::iterator> info;
    do {
        info = parse(first, line.end(), calc, space_p);
        if (!info.hit)
            // Display a caret that points to the position where the error
            // was detected.
            cerr << setw(info.stop - line.begin()) << " "
                 << "^ error\n";
        else if (!info.full)
            // Keep track of where to start parsing the next statement.
            first = info.stop;
    } while (!info.full && info.hit);
}
*/
