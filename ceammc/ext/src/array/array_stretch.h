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
    bool speech_;

    FloatProperty* pitch_;
    FloatProperty* tempo_;
    FloatProperty* rate_;

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

    /**
     * Enable/disable anti-alias filter in pitch transposer (0 = disable)
     */
    AtomList propAnitAlias() const;
    void propSetAntiAlias(const AtomList& l);

    /**
     * Pitch transposer anti-alias filter length (8 .. 128 taps, default = 32)
     */
    AtomList propAnitAliasLength() const;
    void propSetAntiAliasLength(const AtomList& l);

    /**
     * Time-stretch algorithm single processing sequence length in milliseconds. 
     * This determines to how long sequences the original sound is chopped 
     * in the time-stretch algorithm.  
     */
    AtomList propSequence() const;
    void propSetSequence(const AtomList& ms);

    /**
     * Time-stretch algorithm overlap length in milliseconds. When the chopped sound sequences 
     * are mixed back together, to form a continuous sound stream, this parameter defines over 
     * how long period the two consecutive sequences are let to overlap each other.
    */
    AtomList propSeekWindow() const;
    void propSetSeekWindow(const AtomList& ms);

    /**
     * Time-stretch algorithm overlap length in milliseconds. When the chopped sound sequences 
     * are mixed back together, to form a continuous sound stream, this parameter defines over 
     * how long period the two consecutive sequences are let to overlap each other
     */
    AtomList propOverlap() const;
    void propSetOverlap(const AtomList& ms);

    /**
     * Enables props tuned for speech
     */
    AtomList propSpeech() const;
    void propSetSpeech(const AtomList& l);

private:
    void setupSoundTouch();
};

extern "C" void setup_array0x2estretch();

#endif // ARRAY_STRETCH_H
