# include "parser_ui_bind.h"

# include <cstring>
# include <cstdio>
# include "ceammc_crc32.h"

namespace ceammc {
namespace parser {

static inline void add_digit(std::uint8_t& v, char ch) { (v *= 10) += (ch - '0'); }

%%{
    machine ui_bind;

    action on_midi_chan  { add_digit(opts.midi_chan, *fpc); }
    action on_midi_param { add_digit(opts.midi_param, *fpc); }
    action on_midi_value { add_digit(opts.midi_value, *fpc); }
    action on_key_code   { add_digit(opts.key_code, *fpc); }
    action on_key_name   {
        opts.type = UI_BIND_KEY_NAME;
        opts.cmp = UI_BIND_CMP_EQ;
    }

    name = ([a-zA-Z0-9_]+) >{ opts.key_name_hash = crc32_hash_seed(); }
                           ${ opts.key_name_hash = crc32_append_char(opts.key_name_hash, std::tolower(*fpc)); };
    cmp = '=='          %{ opts.cmp = UI_BIND_CMP_EQ; }
        | '>'           %{ opts.cmp = UI_BIND_CMP_GT; }
        | '>='          %{ opts.cmp = UI_BIND_CMP_GE; }
        | '<'           %{ opts.cmp = UI_BIND_CMP_LT; }
        | '<='          %{ opts.cmp = UI_BIND_CMP_LE; }
        ;
    uint = '0' | ([1-9][0-9]{0,2});
    midi_chan  =  ':'       >{ opts.midi_chan = 0; }
                  uint      $on_midi_chan;
    midi_param =  uint      >{ opts.midi_param = 0; } $on_midi_param;
    midi_value =  uint      >{ opts.midi_value = 0; } $on_midi_value;
    num  = '[' midi_param midi_chan? ']';
    op   = cmp midi_value;

    note = ('note' num op?)   %{ opts.type = UI_BIND_MIDI_NOTE; };
    cc   = ('cc'   num op?)   %{ opts.type = UI_BIND_MIDI_CC; };
    pgm  = ('pgm'  num)       %{ opts.type = UI_BIND_MIDI_PGM; };
    midi = note | cc | pgm;

    mod      = 'ctl'   %{ opts.key_mode |= UI_BIND_MODE_CTL; }
             | 'alt'   %{ opts.key_mode |= UI_BIND_MODE_ALT; }
             | 'shift' %{ opts.key_mode |= UI_BIND_MODE_SHIFT; }
             ;
    mod_list    = mod ('+' mod)*;
    key_mod     = '[' mod_list ']';
    keycode_val = uint >{ opts.key_code = 0; } $on_key_code;
    keycode = ('keycode' key_mod? '=' keycode_val)
                                            %{ opts.type = UI_BIND_KEY_CODE; opts.cmp = UI_BIND_CMP_EQ; };
    keyname = 'key' key_mod? '=' name   %on_key_name;
    key = keycode | keyname;

    main := (midi | key);
    write data;
}%%

bool parse_ui_bind(const char* str, UIBindOptions& opts)
{
    int cs = 0;
    const auto len = strlen(str);
    const char* p = str;
    const char* pe = p + len;
    const char* eof = pe;

    %% write init;
    %% write exec;

    const auto ok = cs >= %%{ write first_final; }%%;
    return ok;
}

}
}
