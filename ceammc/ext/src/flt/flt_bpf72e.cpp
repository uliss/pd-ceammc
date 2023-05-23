#include "ceammc_faust_factory.h"
#include "flt_bpf72e.h"

using namespace ceammc;

class FltBpf72e : public faust_flt_bpf72e_tilde {
public:
    FltBpf72e(const PdArgs& args)
        : faust_flt_bpf72e_tilde(args)
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

void setup_flt_bpf72e_tilde()
{
    FaustFactory<FltBpf72e> obj("flt.bpf72e~");

    obj.setDescription("12-th order elliptical band-pass filter");
    obj.setCategory("flt");
    obj.setKeywords({ "filter", "elliptic", "bpf" });
}
