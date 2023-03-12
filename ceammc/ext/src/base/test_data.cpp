#include <algorithm>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <cassert>
#include <exception>
#include <iostream>
#include <m_pd.h>
#include <sstream>
#include <string>
#include <vector>

typedef boost::variant<t_float, std::string> test_atom;
typedef std::vector<test_atom> test_atom_lst;
typedef std::string test_condition;
typedef boost::property_tree::ptree::iterator iterator;
typedef boost::property_tree::ptree ptree;

struct single_test {
    single_test() { }
    single_test(const test_atom_lst& in, const test_condition& cond, const test_atom_lst& exp)
        : input(in)
        , expect(exp)
        , condition(cond)
    {
    }

    test_atom_lst input;
    test_atom_lst expect;
    test_condition condition;
};

struct atom_array {
    atom_array(size_t n)
        : argc(n)
        , argv(0)
    {
        argv = reinterpret_cast<t_atom*>(getbytes(sizeof(t_atom) * argc));
    }
    ~atom_array() { freebytes(argv, argc); }
    inline t_atom* at(size_t n) { return &argv[n]; }
    int argc;
    t_atom* argv;

private:
    atom_array(atom_array&);
    atom_array& operator=(atom_array&);
};

class atom_visitor : public boost::static_visitor<> {
    t_atom* a_;

public:
    atom_visitor(t_atom* a)
        : a_(a)
    {
    }
    void operator()(const t_float& f) const { SETFLOAT(a_, f); }
    void operator()(const std::string& str) const { SETSYMBOL(a_, gensym(str.c_str())); }
};

typedef std::vector<single_test> all_tests;
all_tests* create_all_tests()
{
    return new all_tests;
}

void free_all_tests(all_tests* t)
{
    delete t;
}

void add_single_check(all_tests* t, const test_atom_lst& input,
    const test_condition& cond,
    const test_atom_lst& expect)
{
    if (!t)
        return;
    t->push_back(single_test(input, cond, expect));
}

t_class* test_data_class;
typedef struct test_data {
    t_object x_obj;
    t_outlet* out_source;
    all_tests* data;
} t_test_data;

static void output_atoms(t_outlet* x, t_symbol* s, const test_atom_lst& atoms)
{
    atom_array a(atoms.size());
    for (size_t i = 0; i < atoms.size(); i++) {
        atom_visitor v(a.at(i));
        boost::apply_visitor(v, atoms[i]);
    }
    outlet_anything(x, s, a.argc, a.argv);
}

static void output_single_test(t_test_data* x, const single_test& t)
{
    output_atoms(x->out_source, &s_list, t.input);
    output_atoms(x->x_obj.te_outlet, gensym(t.condition.c_str()), t.expect);
}

static void test_data_bang(t_test_data* x)
{
    if (!x->data)
        return;

    for (size_t i = 0; i < x->data->size(); i++) {
        output_single_test(x, x->data->at(i));
    }
}

static void test_data_load(t_test_data* x, t_symbol* s)
{
    x->data->clear();
    try {
        ptree pt;
        boost::property_tree::read_json(s->s_name, pt);

        // tests iterate
        iterator test_iter = pt.begin(), test_iter_end = pt.end();
        for (; test_iter != test_iter_end; ++test_iter) {
            test_atom_lst input;
            test_atom_lst expect;
            test_condition cond = test_iter->first;

            ptree test_case = test_iter->second;
            iterator case_it = test_case.begin(), case_it_end = test_case.end();
            for (; case_it != case_it_end; ++case_it) {
                std::string key = case_it->first;
                ptree value = case_it->second;

                if (key == "expect") {
                    // single value
                    if (value.empty() && !value.data().empty()) {
                        expect.push_back(value.get_value<std::string>());
                    } else { // list
                        iterator it = value.begin(), it_end = value.end();
                        for (; it != it_end; ++it) {
                            t_float v = it->second.get_value<t_float>();
                            expect.push_back(v);
                        }
                    }
                }

                if (key == "input") {
                    // single value
                    if (value.empty() && !value.data().empty()) {
                        input.push_back(value.get_value<std::string>());
                    } else { // list
                        iterator it = value.begin(), it_end = value.end();
                        for (; it != it_end; ++it) {
                            t_float v = it->second.get_value<t_float>();
                            input.push_back(v);
                        }
                    }
                }
            }

            add_single_check(x->data, input, cond, expect);
        }
    } catch (std::exception const& e) {
        pd_error(0, "test.data: %s", e.what());
    }
}

static void test_data_addfloat(t_test_data* x, t_float v1, t_symbol* cond, t_float v2)
{
    test_atom_lst in;
    in.push_back(test_atom(v1));
    test_atom_lst exp;
    exp.push_back(test_atom(v2));
    add_single_check(x->data, in, cond->s_name, exp);
}

static void test_data_clear(t_test_data* x)
{
    x->data->clear();
}

static void* test_data_new()
{
    t_test_data* x = (t_test_data*)pd_new(test_data_class);
    outlet_new(&x->x_obj, &s_anything);
    x->out_source = outlet_new(&x->x_obj, &s_anything);
    x->data = create_all_tests();
    return (void*)x;
}

static void test_data_free(t_test_data* x)
{
    outlet_free(x->out_source);
    free_all_tests(x->data);
}

extern "C" void setup_test0x2edata()
{
    test_data_class = class_new(gensym("test.data"),
        (t_newmethod)test_data_new, (t_method)test_data_free,
        sizeof(t_test_data), 0, A_NULL);
    class_addbang(test_data_class, (t_method)test_data_bang);

    class_addmethod(test_data_class,
        (t_method)test_data_clear, gensym("clear"), A_NULL, 0);
    class_addmethod(test_data_class,
        (t_method)test_data_load, gensym("load"), A_DEFSYMBOL, 0);
    class_addmethod(test_data_class,
        (t_method)test_data_addfloat, gensym("add_float"), A_DEFFLOAT, A_DEFSYMBOL, A_DEFFLOAT, 0);
}
