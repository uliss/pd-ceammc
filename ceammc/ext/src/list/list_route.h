#ifndef LIST_ROUTE_H
#define LIST_ROUTE_H

#include "list_base.h"

using namespace ceammc;

class ListRoute : public ListBase {
    FlagProperty* trim_;
    FlagProperty* as_any_;
    BoolProperty* simplify_types_;
    ListProperty* args_;
    std::vector<std::string> out_annotations_;

public:
    ListRoute(const PdArgs& args);

    void initDone() override;

    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomList& lst) override;

    int outletIndex(const Atom& a) const;
    const char* annotateOutlet(size_t n) const final;

private:
    void outputList(size_t idx, const AtomList& l);
};

void setup_list_route();

#endif // LIST_ROUTE_H
