#include "ceammc.h"
#include "ceammc_factory.h"
#include "ceammc_object.h"

using namespace ceammc;

class DebugGensym : public BaseObject {
    mutable t_ceammc_gensym_info info_;

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

        t_ceammc_gensym_info info;
        ceammc_gensym_info(&info);

        OBJ_DBG << "hash table size:            " << info.table_size;
        OBJ_DBG << "number of t_symbols:        " << info.symbol_count;
        OBJ_DBG << "max t_symbol chain length:  " << info.max_chain;
        OBJ_DBG << "memory used:                " << info.memory_size;
    }

private:
    void update() const
    {
        ceammc_gensym_info(&info_);
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
