#ifndef LIST_ROUTE_H
#define LIST_ROUTE_H

#include "list_base.h"

using namespace ceammc;

class ListRoute : public ListBase {
    FlagProperty* trim_;
    FlagProperty* as_any_;
    BoolProperty* simplify_types_;

public:
    ListRoute(const PdArgs& args);
    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& lst) override;

    int outletIndex(const Atom& a) const;

private:
    void outputList(size_t idx, const AtomList& l);
};

void setup_list_route();

#endif // LIST_ROUTE_H
