#include "ceammc_factory.h"
#include "ceammc_object.h"
#include "ceammc_string.h"

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
        floatTo(0, string::utf8_strlen(s->s_name));
    }
};

extern "C" void setup_symbol0x2elength()
{
    ObjectFactory<SymbolLength> obj("symbol.length");
}
