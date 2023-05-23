#include "flt_bpf36e.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

class FltBpf36e : public faust_flt_bpf36e_tilde {
public:
    FltBpf36e(const PdArgs& args)
        : faust_flt_bpf36e_tilde(args)
    {

        auto freq = findUIProperty("@freq");
        if (freq) {
            freq->setInitOnly();
            freq->setArgIndex(0);
        }

        auto q = findUIProperty("@q");
        if (q) {
            q->setInitOnly();
            q->setArgIndex(1);
        }
    }
};

void setup_flt_bpf36e_tilde()
{
    FaustFactory<FltBpf36e> obj("flt.bpf36e~");

    obj.setDescription("6th order elliptical filter");
    obj.setCategory("flt");
    obj.setKeywords({ "filter", "elliptic", "bpf" });
}
