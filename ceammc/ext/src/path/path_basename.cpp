#include "ceammc_factory.h"
#include "ceammc_object.h"
#include "ceammc_platform.h"

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
};

void setup_path_basename()
{
    ObjectFactory<PathBasename> obj("path.basename");
}
