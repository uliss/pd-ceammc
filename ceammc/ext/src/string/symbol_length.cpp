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

void setup_symbol_length()
{
    ObjectFactory<SymbolLength> obj("symbol.length");

    obj.setDescription("number of characters in symbol (unicode supported)");
    obj.setCategory("symbol");
    obj.setKeywords({"symbol", "length"});
}
