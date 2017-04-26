#include "ceammc_factory.h"
#include "ceammc_object.h"

#include <fstream>

using namespace ceammc;

class IsFile : public BaseObject {
    t_canvas* cnv_;

public:
    IsFile(const PdArgs& a)
        : BaseObject(a)
        , cnv_(canvas_getcurrent())
    {
        createOutlet();
    }

    void onSymbol(t_symbol* s)
    {
        std::string path = s->s_name;

        if (!sys_isabsolutepath(path.c_str())) {
            if (cnv_) {
                t_symbol* canvas_dir = canvas_getdir(cnv_);
                if (canvas_dir)
                    path = std::string(canvas_dir->s_name) + "/" + path;
            }
        }

        std::ifstream f(path.c_str());
        floatTo(0, f.is_open() ? 1 : 0);
    }
};

extern "C" void is_file_setup()
{
    ObjectFactory<IsFile> obj("is_file");
}
