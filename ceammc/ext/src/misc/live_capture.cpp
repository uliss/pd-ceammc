#include "live_capture.h"
#include "ceammc_factory.h"

using namespace ceammc;

class LiveCapture : public faust_live_capture_tilde {
public:
    LiveCapture(const PdArgs& args)
        : faust_live_capture_tilde(args)
    {
        createInlet();
    }

    void m_reset(t_symbol*, const AtomListView&)
    {
        dsp_->instanceClear();
    }

    void m_record(t_symbol*, const AtomListView&)
    {
        setProperty(gensym("@gate"), Atom(1));
    }

    void m_stop(t_symbol*, const AtomListView&)
    {
        setProperty(gensym("@gate"), Atom(0.f));
    }

    void onInlet(size_t, const AtomListView& lv) override
    {
        if (!checkArgs(lv, ARG_BOOL)) {
            OBJ_ERR << "1 or 0 expected: " << lv;
            return;
        }

        setProperty(gensym("@gate"), Atom(lv.asT<bool>()));
    }
};

void setup_live_capture_tilde()
{
    SoundExternalFactory<LiveCapture> obj("live.capture~");
    obj.addMethod("reset", &LiveCapture::m_reset);
    obj.addMethod("record", &LiveCapture::m_record);
    obj.addMethod("stop", &LiveCapture::m_stop);

    obj.setXletsInfo({ "signal: input\nmethods: @gate, start, stop", "float: 1|0 to start or stop recording" }, { "signal: recorded loop" });
}
