#include "list_shuffle.h"
#include "ceammc_containers.h"
#include "ceammc_factory.h"
#include "datatype_mlist.h"

#include <algorithm>
#include <ctime>
#include <random>

static std::mt19937 gen(std::time(nullptr));

ListShuffle::ListShuffle(const PdArgs& args)
    : BaseObject(args)
{
    createOutlet();
}

void ListShuffle::onList(const AtomListView& lv)
{
    AtomList32 res;
    res.insert_back(lv);
    std::shuffle(res.begin(), res.end(), gen);
    listTo(0, res.view());
}

void ListShuffle::onDataT(const MListAtom& ml)
{
    MListAtom res(*ml);
    res->shuffle();
    atomTo(0, res);
}

void setup_list_shuffle()
{
    ObjectFactory<ListShuffle> obj("list.shuffle");
    obj.processData<DataTypeMList>();

    obj.setDescription("randomly rearranges elements in list");
    obj.setCategory("list");
    obj.setKeywords({"list", "shuffle", "random"});
}
