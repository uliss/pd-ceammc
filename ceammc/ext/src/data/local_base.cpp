#include "local_base.h"
#include "ceammc_string.h"
#include "ceammc_string_types.h"
#include "fmt/core.h"

namespace ceammc {

PdArgs make_local_id(const PdArgs& a)
{
    PdArgs res(a);
    void* cnv = static_cast<void*>(canvas_getcurrent());

    string::MediumString buf;

    try {
        if (res.args.empty()) {
            fmt::format_to(std::back_inserter(buf), "{}-default", cnv);
            buf.push_back('\0');
            res.args.append(gensym(buf.data()));
        } else {
            fmt::format_to(std::back_inserter(buf), "{}-", cnv);
            string::atom_to_string(res.args[0], buf);
            buf.push_back('\0');
            res.args[0] = gensym(buf.data());
        }
    } catch (std::exception& e) {
        LIB_ERR << "local id exception: " << e.what();
    }

    return res;
}

}
