#include "ceammc_factory.h"
#include "ceammc_object.h"

#include <glib.h>

using namespace ceammc;

class SymbolLength : public BaseObject {
public:
    SymbolLength(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
    }

    void onSymbol(t_symbol* s)
    {
        glong len = g_utf8_strlen(s->s_name, -1);
        floatTo(0, len);
    }
};

extern "C" void setup_symbol0x2elength()
{
    ObjectFactory<SymbolLength> obj("symbol.length");
}
