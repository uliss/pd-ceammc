#ifndef LIST_ROUTE_H
#define LIST_ROUTE_H

#include "ceammc_object.h"

using namespace ceammc;

class DataTypeMList;

class ListRoute : public BaseObject {
    FlagProperty* trim_;
    FlagProperty* as_any_;
    BoolProperty* simplify_types_;

public:
    ListRoute(const PdArgs& args);
    void onFloat(t_float f);
    void onSymbol(t_symbol* s);
    void onList(const AtomList& lst);
    void onDataT(const DataTypeMList& lst);

    int outletIndex(const Atom& a) const;

private:
    void outputList(size_t idx, const AtomList& l);
};

void setup_list_route();

#endif // LIST_ROUTE_H
