/*
Class Helmholtz implements a period-length detector using Philip McLeod's
Specially Normalized AutoCorrelation function (SNAC).

Licensed under three-clause BSD license.

Katja Vetter, Feb 2012.

*/

#include "Helmholtz.h"
#include <math.h>
#include <stdlib.h>

Helmholtz::Helmholtz(int framearg, int overlaparg, t_float biasarg)
{
    inputbuf = nullptr;
    inputbuf2 = nullptr;
    processbuf = nullptr;

    setframesize(framearg);
    setoverlap(overlaparg);
    if (biasarg)
        setbias(biasarg);
    else
        biasfactor = DEFBIAS;

    inputbuf = (t_float*)calloc(framesize, sizeof(t_float));
    inputbuf2 = (t_float*)calloc(framesize, sizeof(t_float));
    processbuf = (t_float*)calloc(framesize * 2, sizeof(t_float));

    timeindex = 0;
    periodindex = 0;
    periodlength = 0.;
    fidelity = 0.;
}

Helmholtz::~Helmholtz()
{
    delete inputbuf;
    inputbuf = nullptr;
    delete inputbuf2;
    inputbuf2 = nullptr;
    delete processbuf;
    processbuf = nullptr;
}

/*********************************************************************************/
/******************************** public *****************************************/
/*********************************************************************************/

void Helmholtz::iosamples(const t_float* in, t_float* out, int size)
{
    int mask = framesize - 1;
    int outindex = 0;

    // call analysis function when it is time
    if (!(timeindex & (framesize / overlap - 1)))
        analyzeframe();

    while (size--) {
        inputbuf[timeindex] = *in++;
        out[outindex++] = processbuf[timeindex++];
        timeindex &= mask;
    }
}

void Helmholtz::setframesize(int frame)
{
    if (!((frame == 128) | (frame == 256) | (frame == 512) | (frame == 1024) | (frame == 2048)))
        frame = DEFFRAMESIZE;
    framesize = frame;

    if (inputbuf)
        inputbuf = (t_float*)realloc(inputbuf, framesize * sizeof(t_float));
    if (inputbuf2)
        inputbuf2 = (t_float*)realloc(inputbuf2, framesize * sizeof(t_float));
    if (processbuf)
        processbuf = (t_float*)realloc(processbuf, framesize * 2 * sizeof(t_float));
}

void Helmholtz::setoverlap(int lap)
{
    if (!((lap == 1) | (lap == 2) | (lap == 4) | (lap == 8)))
        lap = DEFOVERLAP;
    overlap = lap;
}

void Helmholtz::setbias(t_float bias)
{
    if (bias > 1.)
        bias = 1.;
    if (bias < 0.)
        bias = 0.;
    biasfactor = bias;
}

t_float Helmholtz::getperiod() const
{
    return (periodlength);
}

t_float Helmholtz::getfidelity() const
{
    return (fidelity);
}

/*********************************************************************************/
/***************************** private procedures ********************************/
/*********************************************************************************/

// main analysis function
void Helmholtz::analyzeframe()
{
    int n, tindex = timeindex;
    int mask = framesize - 1;
    t_float norm = 1. / sqrt(t_float(framesize * 2));

    // copy input to processing buffer
    for (n = 0; n < framesize; n++)
        processbuf[n] = inputbuf[tindex++ & mask] * norm;

    // copy for normalization function
    for (n = 0; n < framesize; n++)
        inputbuf2[n] = inputbuf[tindex++ & mask];

    // zeropadding
    for (n = framesize; n < (framesize << 1); n++)
        processbuf[n] = 0.;

    // call analysis procedures
    autocorrelation();
    normalize();
    pickpeak();
    periodandfidelity();
}

void Helmholtz::autocorrelation()
{
    int n;
    int fftsize = framesize * 2;

    REALFFT(fftsize, processbuf);

    // compute power spectrum
    processbuf[0] *= processbuf[0]; // DC
    processbuf[framesize] *= processbuf[framesize]; // Nyquist

    for (n = 1; n < framesize; n++) {
        processbuf[n] = processbuf[n] * processbuf[n]
            + processbuf[fftsize - n] * processbuf[fftsize - n]; // imag coefficients appear reversed
        processbuf[fftsize - n] = 0.;
    }

    REALIFFT(fftsize, processbuf);
}

void Helmholtz::normalize()
{
    int n, mask = framesize - 1;
    int seek = framesize * SEEK;
    t_float signal1, signal2;
    double normintegral = processbuf[0] * 2.;

    // normalize biased autocorrelation function
    processbuf[0] = 1.;
    for (n = 1; n < seek; n++) {
        signal1 = inputbuf2[n - 1];
        signal2 = inputbuf2[framesize - n];
        normintegral -= (double)(signal1 * signal1 + signal2 * signal2);
        processbuf[n] /= (t_float)normintegral * 0.5;
    }

    // flush instable function tail
    for (n = seek; n < framesize; n++)
        processbuf[n] = 0.;
}

// select the peak which most probably represents period length
void Helmholtz::pickpeak()
{
    int n, peakindex = 0;
    int seek = framesize * SEEK;
    t_float maxvalue = 0.;
    t_float previous[2];
    t_float bias = biasfactor / (t_float)framesize; // user-controlled bias
    t_float realpeak;

    // skip main lobe
    for (n = 1; n < seek; n++) {
        if (processbuf[n] < 0.)
            break;
    }

    // find interpolated / biased maximum in specially normalized autocorrelation function
    // interpolation finds the 'real maximum'
    // biasing favours the first candidate
    for (; n < seek - 1; n++) {
        if (processbuf[n] > processbuf[n - 1]) {
            if (processbuf[n] > processbuf[n + 1]) // we have a local peak
            {
                realpeak = interpolate3max(processbuf, n);

                if ((realpeak * (1. - n * bias)) > maxvalue) {
                    maxvalue = realpeak;
                    peakindex = n;
                }
            }
        }
    }
    periodindex = peakindex;
}

void Helmholtz::periodandfidelity()
{
    if (periodindex) {
        periodlength = periodindex + interpolate3phase(processbuf, periodindex);
        fidelity = interpolate3max(processbuf, periodindex);
    }
}

/*********************************************************************************/
/***************************** private functions *********************************/
/*********************************************************************************/

inline t_float Helmholtz::interpolate3max(t_float* buf, int peakindex)
{
    t_float realpeak;

    t_float a = buf[peakindex - 1];
    t_float b = buf[peakindex];
    t_float c = buf[peakindex + 1];

    realpeak = b + 0.5 * (0.5 * ((c - a) * (c - a))) / (2 * b - a - c);

    return (realpeak);
}

inline t_float Helmholtz::interpolate3phase(t_float* buf, int peakindex)
{
    t_float fraction;

    t_float a = buf[peakindex - 1];
    t_float b = buf[peakindex];
    t_float c = buf[peakindex + 1];

    fraction = (0.5 * (c - a)) / (2. * b - a - c);

    return (fraction);
}
