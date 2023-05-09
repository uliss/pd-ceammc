%%{
    # required local result variable: std::uint8_t midi_byte

    machine midi_common;

    action num_hex_digit { (midi_byte <<= 4) |= xchar2digit(fc); }

    # no actions
    midi_x = ([0-9A-Fa-f]{1,2}) $num_hex_digit;
    midi_xpre = '0x' midi_x;
    midi_xpost = midi_x [hH];
    midi_hex =  midi_xpre | midi_xpost;
}%%
