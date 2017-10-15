#include "../string/datatype_string.h"
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
        floatTo(0, isFile(s->s_name) ? 1 : 0);
    }

    void onDataT(const DataTypeString& s)
    {
        floatTo(0, isFile(s.str()) ? 1 : 0);
    }

    bool isFile(const std::string& p) const
    {
        std::string path(p);

        if (!sys_isabsolutepath(path.c_str())) {
            if (cnv_) {
                t_symbol* canvas_dir = canvas_getdir(cnv_);
                if (canvas_dir)
                    path = std::string(canvas_dir->s_name) + "/" + path;
            }
        }

        std::ifstream f(path.c_str());
        return f.is_open();
    }
};

extern "C" void is_file_setup()
{
    ObjectFactory<IsFile> obj("is_file");
    obj.processData<DataTypeString>();
}
