/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef AUDIOEFFECTX_H
#define AUDIOEFFECTX_H

#include <cstdint>
#include <cstring>

using VstInt32 = int32_t;
enum VstPlugCategory {
    kPlugCategEffect
};

constexpr int kVstMaxParamStrLen = 0;
constexpr int kVstMaxProductStrLen = 0;
constexpr int kVstMaxProgNameLen = 0;
constexpr int kVstMaxVendorStrLen = 0;

using audioMasterCallback = void (*)();

class AudioEffect {
};

class AudioEffectX : public AudioEffect {
    float samplerate_ { 0 };

public:
    AudioEffectX(audioMasterCallback, int, int)
        : samplerate_(44100)
    {
    }

    float getSampleRate() const { return samplerate_; }
    void setSampleRate(size_t sr) { samplerate_ = sr; }

    void setNumInputs(int) { }
    void setNumOutputs(int) { }
    void setUniqueID(int) { }
    void canProcessReplacing() { }
    void canDoubleReplacing() { }
    void programsAreChunks(bool) { }

    void float2string(int, char*, size_t) { }
    void vst_strncpy(char* dst, const char* src, size_t n) { }
};

#endif // AUDIOEFFECTX_H
