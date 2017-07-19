#ifndef ARRAY_STRETCH_H
#define ARRAY_STRETCH_H

#include <boost/shared_ptr.hpp>

#include "array_base.h"

class PdSoundTouch;

class ArrayStretch : public BaseObject {
    Array src_array_;
    Array dest_array_;

    t_symbol* src_array_name_;
    t_symbol* dest_array_name_;

    FloatProperty* pitch_;
    FloatProperty* tempo_;
    FloatProperty* rate_;
    FlagProperty* speech_;
    BoolProperty* anti_alias_;

    boost::shared_ptr<PdSoundTouch> soundtouch_;

public:
    ArrayStretch(const PdArgs& a);
    void onBang();

    AtomList propSrcArray() const;
    AtomList propDestArray() const;
    void propSetSrcArray(const AtomList& l);
    void propSetDestArray(const AtomList& l);

    bool setSrcArray(t_symbol* s);
    bool setDestArray(t_symbol* s);

private:
    void setupSoundTouch();
};

extern "C" void setup_array0x2estretch();

#endif // ARRAY_STRETCH_H
