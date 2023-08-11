
#line 1 "lex/ragel_music.rl"

#line 214 "lex/ragel_music.rl"


# include "ragel_music.h"

namespace ceammc {
namespace ragel {
    const ChordSuffixList& chord_suffix_list()
    {
        static ChordSuffixList chords {
            "maj", "min", "aug", "+6", "sus4", "sus2",
            "7", "maj7", "min7", "aug7", "augmaj7", "dim7", "halfdim7", "7-5", "7sus2", "7sus4",
            "9", "maj9", "min9", "7-9", "minmaj9", "augmaj9", "aug9", "halfdim9", "halfdim-9", "dim9", "dim-9", "9sus4",
            "11", "maj11", "min11", "minmaj11", "augmaj11", "aug11", "halfdim11", "dim11",
            "13", "maj13", "min13", "minmaj13", "augmaj13", "aug13", "halfdim13",
        };
        return chords;
    }
}
}
