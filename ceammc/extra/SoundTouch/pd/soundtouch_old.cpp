/* 
soundtouch~ version 0.9, a pitch shifter class for Pure Data.

soundtouch~ is a Pure Data port of Olli Parviainen's SoundTouch library. 
SoundTouch is copyrighted by Olli Parviainen and published under LGPL version 2.1.

This file created and copyrighted 2010/2011 Katja Vetter.
(katjavetter@gmail.com, www.katjaas.nl)

This software is published under standard BSD licensing terms.
THE AUTHOR MAKES NO WARRANTY, EXPRESS OR IMPLIED,
IN CONNECTION WITH THIS SOFTWARE! 

Based on Pure Data by Miller Puckette and others.
*/

#include "SoundTouch.h" // SoundTouch API header
#include "m_pd.h" // Pure Data API header

using namespace soundtouch;

// Pure Data 'class' declaration
static t_class* soundtouch_class = NULL;

typedef struct
{
    t_float left;
    t_float right;
} t_stereo;

// struct definition for soundtouch class
typedef struct
{
    t_object x_obj; // Pd object struct
    t_float f; // for MAINSIGNALIN
    SoundTouch* pSoundTouch; // pointer to SoundTouch class instance
    t_stereo* stereoInbuf; // pointer to interleaved-buffer for stereo input signal
    t_stereo* stereoOutbuf; // pointer to interleaved-buffer for stereo output signal
    t_int nchannels; // number of signal channels (1 or 2)
    t_int vectorsize; // signal vector size
    t_int bypass; // bypass flag
} t_soundtouch;

// function declarations
static t_int* soundtouch_perform(t_int* w);
static void soundtouch_dsp(t_soundtouch* x, t_signal** sp);
static void* soundtouch_new(t_floatarg sequence, t_floatarg nchannels);
static void soundtouch_pitch(t_soundtouch* x, t_floatarg pitch);
static void soundtouch_sequence(t_soundtouch* x, t_floatarg sequence);
static void soundtouch_settings3(t_soundtouch* x, t_floatarg set);
static void soundtouch_init(t_soundtouch* x, int sequence);
static inline void flushStereobuf(t_stereo* buffer, t_int buffersize);
static void soundtouch_interleave(t_stereo* stereo, t_sample* left, t_sample* right, t_int bufsize);
static inline void soundtouch_deinterleave(t_stereo* stereo, t_sample* left, t_sample* right, t_int bufsize);
extern "C" void soundtouch_tilde_setup(void);

// initialize an instance of the SoundTouch class
static void soundtouch_init(t_soundtouch* x, int sequence)
{
    x->pSoundTouch->setSampleRate(44100);
    x->pSoundTouch->setChannels(x->nchannels);
    x->pSoundTouch->setTempo(1.);
    x->pSoundTouch->setPitch(1.);
    x->pSoundTouch->setRate(1.);
    x->pSoundTouch->setSetting(SETTING_USE_QUICKSEEK, 1);
    x->pSoundTouch->setSetting(SETTING_USE_AA_FILTER, 0);

    // initial settings dependent on sequence size:
    x->pSoundTouch->setSetting(SETTING_SEQUENCE_MS, sequence);
    x->pSoundTouch->setSetting(SETTING_SEEKWINDOW_MS, (sequence / 2));
    x->pSoundTouch->setSetting(SETTING_OVERLAP_MS, (sequence / 4));
}

///////////////////////////////////////////////////////////////////////// perform function ////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// when initialized as mono object, use this perform method (default)
static t_int* soundtouch_perform(t_int* w)
{
    // copy pointers from signal proxies struct
    t_soundtouch* x = (t_soundtouch*)w[1]; // pointer to object (struct instance)
    t_sample* signalvectorIn = (t_sample*)w[2]; // pointer to input signal vector
    t_sample* signalvectorOut = (t_sample*)w[3]; // pointer to output signal vector
    t_int vectorsize = (t_int)w[4]; // signal vector size

    if (x->bypass) {
        while (vectorsize--)
            *signalvectorIn++ = *signalvectorOut++;
        return (w + 5);
    }

    x->pSoundTouch->putSamples(signalvectorIn, vectorsize);
    x->pSoundTouch->receiveSamples(signalvectorOut, vectorsize);
    return (w + 5);
}
// end of soundtouch_perform function definition

// when initialized as stereo object, use this perform method
static t_int* soundtouch_perform2(t_int* w)
{

    // copy pointers from signal proxies struct
    t_soundtouch* x = (t_soundtouch*)w[1]; // pointer to object (struct instance)
    t_sample* signalvectorInLeft = (t_sample*)w[2]; // pointer to input signal vector
    t_sample* signalvectorInRight = (t_sample*)w[3]; // pointer to input signal vector
    t_sample* signalvectorOutLeft = (t_sample*)w[4]; // pointer to output signal vector
    t_sample* signalvectorOutRight = (t_sample*)w[5]; // pointer to output signal vector
    t_int vectorsize = (t_int)w[6]; // signal vector size

    if (x->bypass) {
        while (vectorsize--) {
            *signalvectorInLeft++ = *signalvectorOutLeft++;
            *signalvectorInRight++ = *signalvectorOutRight++;
        }
        return (w + 7);
    }

    t_float* signalvectorIn = (t_float*)x->stereoInbuf; // pointer aliases
    t_float* signalvectorOut = (t_float*)x->stereoOutbuf;

    // process input
    soundtouch_interleave(x->stereoInbuf, signalvectorInLeft, signalvectorInRight, vectorsize);
    x->pSoundTouch->putSamples(signalvectorIn, vectorsize);

    // process output
    x->pSoundTouch->receiveSamples(signalvectorOut, vectorsize);
    soundtouch_deinterleave(x->stereoOutbuf, signalvectorOutLeft, signalvectorOutRight, vectorsize);

    return (w + 7);
}
// end of soundtouch_perform2 function definition

/////////////////////////////////////////////////// other functions that interact with the API ////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void soundtouch_dsp(t_soundtouch* x, t_signal** sp)
// this method will be called everytime when dsp is turned on, and also when signal vector size is changed during dsp
{
    if (x->nchannels == 1) // mono, default case
        dsp_add(soundtouch_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
    if (x->nchannels == 2) // stereo
        dsp_add(soundtouch_perform2, 6, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec, sp[3]->s_vec, sp[0]->s_n);

    if ((x->nchannels == 2) && (x->vectorsize != sp[0]->s_n)) {
        x->stereoInbuf = (t_stereo*)resizebytes(x->stereoInbuf, (x->vectorsize * sizeof(t_stereo)), (sp[0]->s_n * sizeof(t_stereo)));
        x->stereoOutbuf = (t_stereo*)resizebytes(x->stereoOutbuf, (x->vectorsize * sizeof(t_stereo)), (sp[0]->s_n * sizeof(t_stereo)));
        x->vectorsize = sp[0]->s_n;
    }
}

static void* soundtouch_new(t_floatarg sequence, t_floatarg nchannels)
{
    if (nchannels != 2.)
        nchannels = 1.;

    t_soundtouch* x = NULL;
    x = (t_soundtouch*)pd_new(soundtouch_class); // create 'object' (struct instance)
    if (nchannels == 2.)
        inlet_new(&x->x_obj, &x->x_obj.ob_pd, &s_signal, &s_signal); // extra inlet if stereo
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("float"), gensym("pitch"));
    outlet_new(&x->x_obj, &s_signal);
    if (nchannels == 2.)
        outlet_new(&x->x_obj, &s_signal); // extra outlet if stereo
    x->nchannels = (t_int)nchannels;
    x->vectorsize = 64;
    x->bypass = 0;

    // buffers for left/right signal interleave - not initialised for mono instance of soundtouch~
    x->stereoInbuf = x->stereoOutbuf = NULL;
    if (nchannels == 2) {
        x->stereoInbuf = (t_stereo*)getbytes(x->vectorsize * sizeof(t_stereo));
        if (x->stereoInbuf)
            flushStereobuf(x->stereoInbuf, x->vectorsize);
        x->stereoOutbuf = (t_stereo*)getbytes(x->vectorsize * sizeof(t_stereo));
        if (x->stereoOutbuf)
            flushStereobuf(x->stereoOutbuf, x->vectorsize);
    }

    // create instance of class SoundTouch
    if ((x->pSoundTouch = new SoundTouch()) == NULL) {
        post("soundtouch~: no memory allocated");
        return NULL;
    }

    if (!sequence)
        sequence = 40.; // default sequence size
    if (sequence < 20.)
        sequence = 20.; // minimum sequence size
    if (sequence > 100.)
        sequence = 100.; // maximum sequence size
    soundtouch_init(x, (int)sequence); // initialize settings of SoundTouch object

    return (x);
}
// end of soundtouch_new function definition

// memory de-allocation
static void soundtouch_free(t_soundtouch* x)
{
    if (x->stereoInbuf)
        freebytes(x->stereoInbuf, x->vectorsize * sizeof(t_stereo));
    if (x->stereoOutbuf)
        freebytes(x->stereoOutbuf, x->vectorsize * sizeof(t_stereo));
    delete x->pSoundTouch;
}

static void soundtouch_pitch(t_soundtouch* x, t_floatarg pitch)
{
    if (pitch > 2.)
        pitch = 2.;
    if (pitch < 0.25)
        pitch = 0.25;

    x->pSoundTouch->setPitch(pitch);
}

static void soundtouch_settings3(t_soundtouch* x, t_floatarg set) // all settings at once in ratio 4/2/1
{
    if (set > 100.)
        set = 100.;
    if (set < 10.)
        set = 10.;

    x->pSoundTouch->setSetting(SETTING_SEQUENCE_MS, (int)set);
    x->pSoundTouch->setSetting(SETTING_SEEKWINDOW_MS, (int)(set / 2.));
    x->pSoundTouch->setSetting(SETTING_OVERLAP_MS, (int)(set / 4.));
}

static void soundtouch_sequence(t_soundtouch* x, t_floatarg sequence) // sequence in milliseconds
{
    if (sequence > 100.)
        sequence = 100.;
    if (sequence < 1.)
        sequence = 1.;

    x->pSoundTouch->setSetting(SETTING_SEQUENCE_MS, (int)sequence);
}

static void soundtouch_seekwindow(t_soundtouch* x, t_floatarg seekwindow) // seekwindow in milliseconds
{
    if (seekwindow > 100.)
        seekwindow = 100;
    if (seekwindow < 1.)
        seekwindow = 1;
    x->pSoundTouch->setSetting(SETTING_SEEKWINDOW_MS, (int)seekwindow);
}

static void soundtouch_overlap(t_soundtouch* x, t_floatarg overlap) // overlap in milliseconds
{
    if (overlap > 100.)
        overlap = 100;
    if (overlap < 1.)
        overlap = 1.;
    x->pSoundTouch->setSetting(SETTING_OVERLAP_MS, (int)overlap);
}

static inline void flushStereobuf(t_stereo* buffer, t_int buffersize)
{
    t_int index;
    for (index = 0; index < buffersize; index++) {
        buffer[index].left = 0.;
        buffer[index].right = 0.;
    }
}

static void soundtouch_interleave(t_stereo* stereo, t_sample* left, t_sample* right, t_int bufsize)
{
    unsigned int n = 0;

    while (bufsize--) {
        stereo[n].left = *left++;
        stereo[n].right = *right++;
        n++;
    }
}

static inline void soundtouch_deinterleave(t_stereo* stereo, t_sample* left, t_sample* right, t_int bufsize)
{
    unsigned int n = 0;
    unsigned int size = bufsize;

    while (size--) {
        *left++ = stereo[n].left;
        *right++ = stereo[n].right;
        n++;
    }
}

static inline void soundtouch_bypass(t_soundtouch* x, t_floatarg bypass)
{
    x->bypass = (t_int)bypass;
}

// main function to set up soundtouch_class
// this function must always be exported
extern "C" void soundtouch_tilde_setup(void)
{
    soundtouch_class = class_new(gensym("soundtouch~"), (t_newmethod)soundtouch_new,
        (t_method)soundtouch_free, sizeof(t_soundtouch), CLASS_DEFAULT, A_DEFFLOAT, A_DEFFLOAT, A_NULL);

    CLASS_MAINSIGNALIN(soundtouch_class, t_soundtouch, f);
    class_addmethod(soundtouch_class, (t_method)soundtouch_dsp, gensym("dsp"), (t_atomtype)0);
    class_addmethod(soundtouch_class, (t_method)soundtouch_pitch, gensym("pitch"), A_FLOAT, 0);
    class_addmethod(soundtouch_class, (t_method)soundtouch_settings3, gensym("set"), A_FLOAT, 0);
    class_addmethod(soundtouch_class, (t_method)soundtouch_sequence, gensym("sequence"), A_FLOAT, 0);
    class_addmethod(soundtouch_class, (t_method)soundtouch_seekwindow, gensym("seekwindow"), A_FLOAT, 0);
    class_addmethod(soundtouch_class, (t_method)soundtouch_overlap, gensym("overlap"), A_FLOAT, 0);
    class_addmethod(soundtouch_class, (t_method)soundtouch_bypass, gensym("bypass"), A_FLOAT, 0);

    post("soundtouch~ version 0.9, by Katja Vetter");
    post("soundtouch~ is based on Olli Parviainen's SoundTouch library");
}
