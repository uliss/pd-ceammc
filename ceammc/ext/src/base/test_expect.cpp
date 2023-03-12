#include "m_pd.h"

struct atom_array {
    atom_array(size_t n)
        : argc(n)
        , argv(0)
    {
        argv = reinterpret_cast<t_atom*>(getbytes(sizeof(t_atom) * argc));
    }
    atom_array(size_t n, t_atom* data)
        : argc(n)
    {
        argv = reinterpret_cast<t_atom*>(copybytes(data, sizeof(t_atom) * argc));
    }
    ~atom_array() { freebytes(argv, argc); }
    inline t_atom* at(size_t n) { return &argv[n]; }
    int argc;
    t_atom* argv;

private:
    atom_array(atom_array&);
};

t_class* test_expect_class;
typedef struct test_expect {
    t_object x_obj;
    t_inlet* input;
    atom_array* data;
} t_test_expect;

static void test_expect_equal(t_test_expect* x, t_symbol* s, int argc, t_atom* argv)
{
    if (argc != x->data->argc) {
        pd_error(0, "test.expect: expected data length not equal to given: %d != %d", argc, x->data->argc);

        outlet_float(x->x_obj.te_outlet, 0.0f);
        return;
    }

    for (int i = 0; i < argc; i++) {
        if (argv->a_type != x->data->at(i)->a_type) {
            pd_error(0, "test.expect: data type not equal for index %i", i);
            outlet_float(x->x_obj.te_outlet, 0.0f);
            return;
        }

        if (argv->a_type == A_FLOAT) {
            t_float expected = atom_getfloat(&argv[i]);
            t_float real = atom_getfloat(x->data->at(i));
            if (expected != real) {
                pd_error(0, "test.expect: expected value is %f, but get %f", expected, real);
                outlet_float(x->x_obj.te_outlet, 0.0f);
                return;
            }
        }
    }

    post("test.expect: ok");
    outlet_float(x->x_obj.te_outlet, 1.0f);
}

static void test_expect_store(t_test_expect* x, t_symbol* s, int argc, t_atom* argv)
{
    delete x->data;
    x->data = new atom_array(argc, argv);
}

static void* test_expect_new()
{
    t_test_expect* x = (t_test_expect*)pd_new(test_expect_class);
    x->input = inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_list, gensym("store"));
    outlet_new(&x->x_obj, &s_float);
    x->data = 0;
    return (void*)x;
}

static void test_expect_free(t_test_expect* x)
{
    inlet_free(x->input);
    delete x->data;
}

extern "C" void setup_test0x2eexpect()
{
    test_expect_class = class_new(gensym("test.expect"),
        (t_newmethod)test_expect_new, (t_method)test_expect_free,
        sizeof(t_test_expect), 0, A_NULL);
    //    class_addlist(test_expect_class, test_expect_list);
    class_addmethod(test_expect_class,
        (t_method)test_expect_equal, gensym("=="), A_GIMME, 0);
    class_addmethod(test_expect_class,
        (t_method)test_expect_store, gensym("store"), A_GIMME, 0);
}
