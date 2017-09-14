#ifndef MIDI_COMMON_H
#define MIDI_COMMON_H

#include "ceammc_atom.h"
#include "ceammc_music_theory.h"

namespace ceammc {
namespace midi {

    /**
     * Returns instrument name family
     * @param instrIndex - instrument index in range [1-128]
     * @return pointer to empty symbol on error
     */
    t_symbol* instrument_family(size_t instrIndex);

    /**
     * Returns instrument name
     * @param instrIndex
     * @return pointer to empty symbol on error
     */
    t_symbol* instrument_name(size_t instrIndex);

    /**
     * Returns instrument index by given name
     * @param name
     * @return index in range [1-128] ir 0 on error
     */
    size_t instrument_index(t_symbol* name);

    /**
     * Returns controller index by given controller number
     * @param ctrNum - controller number
     * @return empty symbol (not NULL) if controller has no common name
     */
    t_symbol* controller_name(size_t ctrNum);

    t_symbol* key_to_name(size_t key, const Tonality& t, bool up = true);
}
}

#endif // MIDI_COMMON_H
