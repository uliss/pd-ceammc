#include "debug_gensym.h"
#include "ceammc.hpp"
#include "ceammc_factory.h"
#include "ceammc_object.h"
#include "x_ceammc.h"

using namespace ceammc;

namespace pd {

void gensym_info(t_ceammc_gensym_info* info)
{
    t_symbol** table = pd_ceammc_gensym_hash_table();
    const size_t sz = pd_ceammc_gensym_hash_table_size();

    info->table_size = sz;
    info->max_chain = 0;
    info->memory_size = 0;
    info->symbol_count = 0;

    size_t max_chain_size = 0;

    for (size_t i = 0; i < sz; i++) {
        t_symbol* s = table[i];
        if (!s)
            continue;

        info->symbol_count++;
        info->memory_size += strlen(s->s_name) + 1;

        size_t chain_size = 1;

        t_symbol* next = s->s_next;
        while (next) {
            info->symbol_count++;
            info->memory_size += strlen(next->s_name) + 1;
            chain_size++;

            next = next->s_next;
        }

        if (chain_size > max_chain_size)
            max_chain_size = chain_size;
    }

    info->max_chain = max_chain_size;
}
}

class DebugGensym : public BaseObject {
    mutable ::pd::t_ceammc_gensym_info info_;

public:
    DebugGensym(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
        createCbProperty("@memsize", &DebugGensym::getMemSize);
        createCbProperty("@tabsize", &DebugGensym::getTableSize);
        createCbProperty("@count", &DebugGensym::getCount);
        createCbProperty("@maxchain", &DebugGensym::getMaxChain);

        update();
    }

    void onBang()
    {
        update();
        AtomList a(info_.memory_size, info_.symbol_count);
        listTo(0, a);
    }

    void dump() const
    {
        BaseObject::dump();

        update();

        OBJ_DBG << "hash table size:            " << info_.table_size;
        OBJ_DBG << "number of t_symbols:        " << info_.symbol_count;
        OBJ_DBG << "max t_symbol chain length:  " << info_.max_chain;
        OBJ_DBG << "memory used:                " << info_.memory_size;
    }

private:
    void update() const
    {
        ::pd::gensym_info(&info_);
    }

    AtomList getMemSize() const
    {
        update();
        return AtomList(info_.memory_size);
    }

    AtomList getTableSize() const
    {
        // no update required
        return AtomList(info_.table_size);
    }

    AtomList getCount() const
    {
        update();
        return AtomList(info_.symbol_count);
    }

    AtomList getMaxChain() const
    {
        update();
        return AtomList(info_.max_chain);
    }
};

extern "C" void setup_debug0x2egensym()
{
    ObjectFactory<DebugGensym> obj("debug.gensym");
}
