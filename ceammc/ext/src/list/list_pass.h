#ifndef LIST_PASS_H
#define LIST_PASS_H

#include "ceammc_data.h"
#include "ceammc_object.h"
using namespace ceammc;

class ListPass : public BaseObject {
    ListProperty* args_;

public:
    ListPass(const PdArgs& args);

    void onFloat(t_float f) override;
    void onSymbol(t_symbol* s) override;
    void onList(const AtomListView& lv) override;
    void onInlet(size_t n, const AtomListView& lv) override;

    void onDataT(const MListAtom& ml);

private:
    bool contains(const Atom& a) const;
};

void setup_list_pass();

#endif // LIST_PASS_H
