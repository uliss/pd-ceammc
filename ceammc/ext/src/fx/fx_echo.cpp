#include "fx_echo.h"
#include "ceammc_factory.h"
#include "ceammc_faust_factory.h"

#include <cstring>

constexpr const char* PROP_DELAY = "@delay";
constexpr const char* PROP_FEEDBACK = "@feedback";
constexpr const char* ALIAS_DEPRECATED = "fx.secho~";

using namespace ceammc;

class FxEcho : public faust_fx_echo_tilde {
    UIProperty* delay_;

public:
    FxEcho(const PdArgs& args)
        : faust_fx_echo_tilde(args)
        , delay_(findUIProperty(PROP_DELAY))
    {
        warnDeprectedName(ALIAS_DEPRECATED);

        bindPositionalArgsToProps({ PROP_DELAY, PROP_FEEDBACK });

        createInlet();
    }

    void onInlet(size_t n, const AtomListView& lv) final
    {
        if (delay_)
            delay_->set(lv);
    }
};

void setup_fx_echo_tilde()
{
    FaustFactory<FxEcho> obj("fx.echo~");
    obj.setXletsInfo({ "signal: input", "float: delay time in msec" }, { "signal: output" });
    obj.addAlias(ALIAS_DEPRECATED);

    obj.setDescription("simple echo effect");
    obj.setCategory("fx");
    obj.setKeywords({ "fx", "echo", "delay" });
}
