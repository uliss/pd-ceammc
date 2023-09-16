#include "live_capture.h"
#include "ceammc_factory.h"

using namespace ceammc;

class LiveCapture : public faust_live_capture_tilde {
    UIProperty *gate_ { 0 }, *div_ { 0 };

public:
    LiveCapture(const PdArgs& args)
        : faust_live_capture_tilde(args)
        , gate_(findUIProperty("@gate"))
        , div_(findUIProperty("@.div"))
    {
        createInlet();
    }

    void m_reset(t_symbol*, const AtomListView&)
    {
        dsp_->instanceClear();
    }

    void m_record(t_symbol*, const AtomListView&)
    {
        switchOn(true);
    }

    void m_stop(t_symbol*, const AtomListView&)
    {
        switchOn(false);
    }

    void onInlet(size_t, const AtomListView& lv) override
    {
        if (!checkArgs(lv, ARG_BOOL)) {
            OBJ_ERR << "1 or 0 expected: " << lv;
            return;
        }

        switchOn(lv.asT<bool>());
    }

    void m_div(t_symbol* s, const AtomListView& lv)
    {
        if (div_)
            div_->setValue(lv.asFloat(1), true);
    }

    void switchOn(bool v)
    {
        if (div_)
            div_->setValue(1, true);

        if (gate_)
            gate_->setValue(v);
    }
};

void setup_live_capture_tilde()
{
    SoundExternalFactory<LiveCapture> obj("live.capture~");
    obj.addMethod("reset", &LiveCapture::m_reset);
    obj.addMethod("record", &LiveCapture::m_record);
    obj.addMethod("stop", &LiveCapture::m_stop);
    obj.addMethod("div", &LiveCapture::m_div);

    obj.setXletsInfo({ "signal: input\n"
                       "methods: @gate, start, stop",
                         "float: 1|0 to start or stop recording" },
        { "signal: recorded loop" });
}
