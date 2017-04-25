#include "ceammc_factory.h"
#include "ceammc_object.h"
#include "ceammc_platform.h"

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
};

extern "C" void setup_path0x2edirname()
{
    ObjectFactory<PathDirname> obj("path.dirname");
}
