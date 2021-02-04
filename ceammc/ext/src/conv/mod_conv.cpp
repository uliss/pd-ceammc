#include "mod_conv.h"
#include "conv_amp2dbfs.h"
#include "conv_dbfs2amp.h"
#include "conv_degree2key.h"
#include "conv_int2bits.h"
#include "conv_list2many.h"
#include "conv_list2props.h"
#include "conv_midi2freq.h"
#include "conv_pitch2midi.h"
#include "conv_sig2float.h"

void setup_conv_sec2str();
void setup_conv_str2sec();

void setup_conv_cc2amp();

void setup_conv_bpm2hz();
void setup_conv_bpm2ms();
void setup_conv_bpm2sec();
void setup_conv_ms2bpm();
void setup_conv_ms2samp();
void setup_conv_samp2ms();
void setup_conv_samp2sec();
void setup_conv_sec2bpm();
void setup_conv_sec2samp();

void setup_conv_angles();

void setup_conv_lin2curve();
void setup_conv_lin2exp();
void setup_conv_lin2lin();
void setup_conv_lin2lin();
void setup_conv_lin2lin_tilde();

void setup_conv_hex2int();
void setup_conv_bits2bang();
void setup_conv_bits2int();
void setup_conv_bits2note();
void setup_conv_bits2pos();

void ceammc_conv_setup()
{
    setup_conv_sec2str();
    setup_conv_str2sec();

    setup_conv_lin2curve();
    setup_conv_lin2exp();
    setup_conv_lin2lin();
    setup_conv_lin2lin_tilde();

    setup_conv_bpm2hz();
    setup_conv_bpm2ms();
    setup_conv_bpm2sec();
    setup_conv_ms2bpm();
    setup_conv_ms2samp();
    setup_conv_samp2ms();
    setup_conv_samp2sec();
    setup_conv_sec2bpm();
    setup_conv_sec2samp();

    setup_conv_amp2dbfs();
    setup_conv_dbfs2amp();

    setup_conv_cc2amp();
    setup_conv_pitch2midi();
    setup_conv_midi2freq();
    setup_conv_list2many();
    setup_conv_list2props();

    setup_conv_angles();
    setup_conv_bits2bang();
    setup_conv_bits2int();
    setup_conv_bits2note();
    setup_conv_bits2pos();
    setup_conv_hex2int();
    setup_conv_int2bits();

    setup_conv_degree2key();

    setup_conv_sig2float();
}
