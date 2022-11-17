#ifndef CEAMMC_STUB_H
#define CEAMMC_STUB_H

#include "ceammc_factory.h"
#include "ceammc_object.h"
#include "ceammc_sound_external.h"

namespace ceammc {

class StubObjectImpl : public SoundExternal {
public:
    StubObjectImpl(const PdArgs& args, int SigIn, int SigOut, int CtlIn, int CtlOut, const char* errMsg = "stub object")
        : SoundExternal(args)
    {
        for (int i = 0; i < SigIn; i++)
            createSignalInlet();

        for (int i = 0; i < SigOut; i++)
            createSignalOutlet();

        for (int i = 0; i < CtlIn; i++)
            createInlet();

        for (int i = 0; i < CtlOut; i++)
            createOutlet();

        OBJ_ERR << errMsg;
    }

    void processBlock(const t_sample** in, t_sample** out) final { }
};
}

#define AUDIO_OBJECT_STUB(name, ain, aout, cin, cout, msg, fn)                       \
    namespace {                                                                      \
        class ControlStub : public ceammc::StubObjectImpl {                          \
        public:                                                                      \
            ControlStub(const ceammc::PdArgs& args)                                  \
                : ceammc::StubObjectImpl(args, ain, aout, cin, cout, msg)            \
            {                                                                        \
            }                                                                        \
            static void init() { ceammc::SoundExternalFactory<ControlStub> obj(name, \
                ceammc::OBJECT_FACTORY_NO_DEFAULT_INLET); }                          \
        };                                                                           \
    }                                                                                \
    extern "C" void setup_##fn()                                                     \
    {                                                                                \
        ControlStub::init();                                                         \
    }

#define CONTROL_OBJECT_STUB(name, in, out, msg, fn) \
    AUDIO_OBJECT_STUB(name, 0, 0, in, out, msg, fn)

#endif // CEAMMC_STUB_H
