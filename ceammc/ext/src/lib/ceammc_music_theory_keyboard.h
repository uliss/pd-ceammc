#ifndef CEAMMC_MUSIC_THEORY_KEYBOARD_H
#define CEAMMC_MUSIC_THEORY_KEYBOARD_H

namespace ceammc {
namespace music {
    namespace keyboard {

        template <class Integer>
        bool is_black_key(Integer key_num)
        {
            key_num = key_num % 12;
            return ((key_num - (key_num > 4 ? 1 : 0)) % 2) == 1;
        }

        template <class Integer>
        bool is_white_key(Integer key_num)
        {
            return !is_black_key(key_num);
        }

        template <class Integer>
        Integer wkey_to_key(Integer wkey)
        {
            Integer oct_n = wkey / 7;
            Integer oct_key = wkey % 7;

            Integer k = oct_key < 3 ? (oct_key * 2) : (oct_key * 2) - 1;
            return oct_n * 12 + k;
        }
    }
}
}

#endif // CEAMMC_MUSIC_THEORY_KEYBOARD_H
