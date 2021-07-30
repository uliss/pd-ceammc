#include "ceammc_data.h"
#include "ceammc_factory.h"
#include "ceammc_object.h"
#include "ceammc_platform.h"
#include "datatype_string.h"

using namespace ceammc;

class PathDirname : public BaseObject {
public:
    PathDirname(const PdArgs& a)
        : BaseObject(a)
    {
        createOutlet();
    }

    void onSymbol(t_symbol* s)
    {
        symbolTo(0, gensym(platform::dirname(s->s_name).c_str()));
    }

    void onDataT(const StringAtom& str)
    {
        atomTo(0, StringAtom(platform::dirname(str->str().c_str())));
    }
};

void setup_path_dirname()
{
    ObjectFactory<PathDirname> obj("path.dirname");
    obj.processData<DataTypeString>();
}
