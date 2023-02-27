#include "flt_notch.h"
#include "ceammc_factory.h"

using namespace ceammc;

constexpr const char* STR_PROP_FREQ = "@freq";
constexpr const char* STR_PROP_WIDTH = "@width";

class FltNotch : public faust_flt_notch_tilde {
    UIProperty* freq_;

public:
    FltNotch(const PdArgs& args)
        : faust_flt_notch_tilde(args)
        , freq_(static_cast<UIProperty*>(property(STR_PROP_FREQ)))
    {
        bindPositionalArgsToProps({ gensym(STR_PROP_FREQ), gensym(STR_PROP_WIDTH) });
        createInlet();
    }

    void onInlet(size_t n, const AtomListView& lv)
    {
        if (freq_)
            freq_->set(lv);
    }
};

void setup_flt_notch_tilde()
{
    SoundExternalFactory<FltNotch> obj("flt.notch~");

    obj.setXletsInfo({ "signal: input", "float: freq" }, { "signal: output" });

    obj.setDescription("band-rejection filter");
    obj.setCategory("flt");
    obj.setKeywords({ "filter", "lowshelf" });
}
