#include "fx_delay.h"
#include "ceammc_faust_factory.h"

using namespace ceammc;

constexpr const char* ALIAS_DEPRECATED = "fx.sdelay~";

class FxDelay : public faust_fx_delay_tilde {
    UIProperty* delay_;

public:
    FxDelay(const PdArgs& args)
        : faust_fx_delay_tilde(args)
        , delay_(findUIProperty("@delay"))
    {
        warnDeprectedName(ALIAS_DEPRECATED);

        bindPositionalArgsToProps({ gensym("@delay"), gensym("@feedback") });

        createInlet();
    }

    void onInlet(size_t n, const AtomListView& lv) final
    {
        if (delay_)
            delay_->set(lv);
    }
};

void setup_fx_delay_tilde()
{
    FaustFactory<FxDelay> obj("fx.delay~");
    obj.addAlias(ALIAS_DEPRECATED);

    obj.setXletsInfo({ "signal: input", "float: delay time(ms)" }, { "signal: output" });

    obj.setDescription("smooth delay with a feedback control");
    obj.setCategory("fx");
    obj.setKeywords({ "fx", "echo", "delay" });
}
