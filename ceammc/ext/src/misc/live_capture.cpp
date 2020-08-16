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

    void onInlet(size_t, const AtomList& l)
    {
        bool ok = !l.empty() && l[0].isFloat();
        if (!ok) {
            OBJ_ERR << "1 or 0 expected: " << l;
            return;
        }

        t_float f = l[0].asFloat();
        setProperty(gensym("@gate"), Atom(f == 0.f ? 0.f : 1));
    }
};

void setup_live_capture_tilde()
{
    SoundExternalFactory<LiveCapture> obj("live.capture~");
    obj.addMethod("reset", &LiveCapture::m_reset);
    obj.addMethod("record", &LiveCapture::m_record);
    obj.addMethod("stop", &LiveCapture::m_stop);
}
