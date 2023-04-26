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

    void processBlock(const t_sample** /*in*/, t_sample** /*out*/) final { }
};
}

#define AUDIO_OBJECT_STUB(class_name, ain, aout, cin, cout, msg)          \
    namespace {                                                           \
        class class_name : public ceammc::StubObjectImpl {                \
        public:                                                           \
            class_name(const ceammc::PdArgs& args)                        \
                : ceammc::StubObjectImpl(args, ain, aout, cin, cout, msg) \
            {                                                             \
            }                                                             \
        };                                                                \
    }

#define CONTROL_OBJECT_STUB(name, in, out, msg) \
    AUDIO_OBJECT_STUB(name, 0, 0, in, out, msg)

#define OBJECT_STUB_SETUP(class_name, fn, obj_name, ...)       \
    extern "C" void setup_##fn()                               \
    {                                                          \
        ceammc::SoundExternalFactory<class_name> obj(obj_name, \
            ceammc::OBJECT_FACTORY_NO_DEFAULT_INLET);          \
        std::vector<const char*> aliases = { __VA_ARGS__ };    \
        for (auto a : aliases)                                 \
            obj.addAlias(a);                                   \
    }

#endif // CEAMMC_STUB_H
