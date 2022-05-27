#include "ceammc_data.h"
#include "ceammc_factory.h"
#include "ceammc_object.h"
#include "ceammc_platform.h"
#include "datatype_string.h"

using namespace ceammc;

class PathBasename : public BaseObject {
public:
    PathBasename(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
    }

    void onSymbol(t_symbol* s)
    {
        symbolTo(0, gensym(platform::basename(s->s_name).c_str()));
    }

    void onDataT(const StringAtom& str)
    {
        atomTo(0, StringAtom(platform::basename(str->str().c_str())));
    }
};

void setup_path_basename()
{
    ObjectFactory<PathBasename> obj("path.basename");
    obj.processData<DataTypeString>();
}
