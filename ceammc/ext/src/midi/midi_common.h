#ifndef MIDI_COMMON_H
#define MIDI_COMMON_H

#include "ceammc_music_theory.h"

namespace ceammc {
namespace music {
    class Tonality;
}

namespace midi {

    /**
     * Returns instrument name family
     * @param instrIndex - instrument index in range [1-128]
     * @return pointer to empty symbol on error
     */
    const char* instrument_family(size_t instrIndex);

    /**
     * Returns instrument name
     * @param instrIndex
     * @return pointer to empty symbol on error
     */
    const char *instrument_name(size_t instrIndex);

    /**
     * Returns instrument index by given name
     * @param name
     * @return index in range [1-128] or 0 on error
     */
    size_t instrument_index(const char *name);

    /**
     * Returns controller index by given controller number
     * @param ctrNum - controller number
     * @return empty symbol (not NULL) if controller has no common name
     */
    const char* controller_name(size_t ctrNum);

    std::string key_to_name(size_t key, const music::Tonality& t, bool up = true);
}
}

#endif // MIDI_COMMON_H
