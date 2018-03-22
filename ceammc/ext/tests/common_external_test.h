#ifndef COMMON_EXTERNAL_TEST_H
#define COMMON_EXTERNAL_TEST_H

#include "ceammc_atomlist.h"
#include "ceammc_dataatom.h"
#include "ceammc_dataatomlist.h"

#include <iostream>

extern "C" void obj_init();
extern "C" void pd_init();

using namespace ceammc;

#define S(v) Atom(gensym(v))
#define F(v) Atom(float(v))
#define P(v) Atom(gensym(v))

// clang-format off
static Atom test_atom_wrap(const char* v) { return Atom(gensym(v)); }
static Atom test_atom_wrap(t_symbol* v) { return Atom(v); }
static Atom test_atom_wrap(float v) { return Atom(v); }
static Atom test_atom_wrap(const Atom& v) { return v; }
static Atom test_atom_wrap(const DataPtr& d) { return d.asAtom(); }

static AtomList test_list_wrap(const Atom& a1) { return AtomList(a1); }
static AtomList test_list_wrap(const Atom& a1, const Atom& a2) { return AtomList(a1, a2); }
static AtomList test_list_wrap(const Atom& a1, const Atom& a2, const Atom& a3) {
    AtomList res(a1, a2); res.append(a3); return res;
}
static AtomList test_list_wrap(const Atom& a1, const Atom& a2, const Atom& a3, const Atom& a4) {
    AtomList res(a1, a2); res.append(a3); res.append(a4); return res;
}
static AtomList test_list_wrap(const Atom& a1, const Atom& a2, const Atom& a3, const Atom& a4,
                        const Atom& a5) {
    AtomList res(a1, a2); res.append(a3); res.append(a4); res.append(a5); return res;
}
static AtomList test_list_wrap(const Atom& a1, const Atom& a2, const Atom& a3, const Atom& a4,
                        const Atom& a5, const Atom& a6) {
    AtomList res(a1, a2); res.append(a3); res.append(a4); res.append(a5); res.append(a6); return res;
}
static AtomList test_list_wrap(const Atom& a1, const Atom& a2, const Atom& a3, const Atom& a4,
                        const Atom& a5, const Atom& a6, const Atom& a7) {
    AtomList res(a1, a2); res.append(a3); res.append(a4);
    res.append(a5); res.append(a6); res.append(a7); return res;
}
static AtomList test_list_wrap(const Atom& a1, const Atom& a2, const Atom& a3, const Atom& a4,
                        const Atom& a5, const Atom& a6, const Atom& a7, const Atom& a8) {
    AtomList res(a1, a2); res.append(a3); res.append(a4);
    res.append(a5); res.append(a6); res.append(a7); res.append(a8); return res;
}

static DataAtomList test_datalist_wrap(const Atom& a1) { return DataAtomList(a1); }

static DataAtomList test_datalist_wrap(const Atom& a1, const Atom& a2) {
    DataAtomList res(a1);
    res.append(a2);
    return res;
}

static DataAtomList test_datalist_wrap(const Atom& a1, const Atom& a2, const Atom& a3) {
    DataAtomList res(a1);
    res.append(a2);
    res.append(a3);
    return res;
}

static DataAtomList test_datalist_wrap(const Atom& a1, const Atom& a2, const Atom& a3, const Atom& a4) {
    DataAtomList res(a1);
    res.append(a2);
    res.append(a3);
    res.append(a4);
    return res;
}
// clang-format on

static Atom D(DataType t, DataId id)
{
    return Atom(DataDesc(t, id));
}

#define D1(v) test_datalist_wrap(test_atom_wrap(v))
#define D2(v1, v2) test_datalist_wrap(test_atom_wrap(v1), test_atom_wrap(v2))
#define D3(v1, v2, v3) test_datalist_wrap(test_atom_wrap(v1), test_atom_wrap(v2), test_atom_wrap(v3))
#define D4(v1, v2, v3, v4) test_datalist_wrap(test_atom_wrap(v1), test_atom_wrap(v2), \
    test_atom_wrap(v3), test_atom_wrap(v4))

#define A(v) test_atom_wrap(v)
#define L1(v) test_list_wrap(test_atom_wrap(v))
#define L2(v1, v2) test_list_wrap(test_atom_wrap(v1), test_atom_wrap(v2))
#define L3(v1, v2, v3) test_list_wrap(test_atom_wrap(v1), test_atom_wrap(v2), test_atom_wrap(v3))
#define L4(v1, v2, v3, v4) test_list_wrap(test_atom_wrap(v1), test_atom_wrap(v2), \
    test_atom_wrap(v3), test_atom_wrap(v4))
#define L5(v1, v2, v3, v4, v5) test_list_wrap(test_atom_wrap(v1), test_atom_wrap(v2), \
    test_atom_wrap(v3), test_atom_wrap(v4), test_atom_wrap(v5))
#define L6(v1, v2, v3, v4, v5, v6) test_list_wrap(test_atom_wrap(v1), test_atom_wrap(v2), \
    test_atom_wrap(v3), test_atom_wrap(v4), test_atom_wrap(v5), test_atom_wrap(v6))
#define L7(v1, v2, v3, v4, v5, v6, v7) test_list_wrap(test_atom_wrap(v1), test_atom_wrap(v2), \
    test_atom_wrap(v3), test_atom_wrap(v4), test_atom_wrap(v5), test_atom_wrap(v6), test_atom_wrap(v7))
#define L8(v1, v2, v3, v4, v5, v6, v7, v8) test_list_wrap(test_atom_wrap(v1), test_atom_wrap(v2),       \
    test_atom_wrap(v3), test_atom_wrap(v4), test_atom_wrap(v5), test_atom_wrap(v6), test_atom_wrap(v7), \
    test_atom_wrap(v8))

class ListApprox {
    AtomList lst_;

public:
    ListApprox();
    ListApprox(float f);
    ListApprox(float f0, float f1);
    ListApprox(float f0, float f1, float f2);
    ListApprox(float f0, float f1, float f2, float f3);
    ListApprox(float f0, float f1, float f2, float f3, float f4);
    ListApprox(float f0, float f1, float f2, float f3, float f4, float f5);
    ListApprox(float f0, float f1, float f2, float f3, float f4, float f5, float f6);
    ListApprox(float f0, float f1, float f2, float f3, float f4, float f5, float f6, float f7);

    std::string toString() const;

    friend ListApprox operator+(const ListApprox& l0, const ListApprox& l1);
    friend bool operator==(const ListApprox& la, const AtomList& al);
};

std::ostream& operator<<(std::ostream& os, const ListApprox& value);
ListApprox operator+(const ListApprox& l0, const ListApprox& l1);
bool operator==(const ListApprox& la, const AtomList& al);
bool operator==(const AtomList& al, const ListApprox& la);

// pd error/debug print
void printPdToStdError(bool value = true);

typedef void(pdPrintFunction)(const char*);
void setPdPrintFunction(pdPrintFunction fn);

#endif // COMMON_EXTERNAL_TEST_H
