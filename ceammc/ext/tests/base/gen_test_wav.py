#!/usr/bin/env python3

import wave
import random
import struct
import argparse
import re
import matplotlib.pyplot as plt
import numpy as np
import math as m


def main():
    parser = argparse.ArgumentParser(
        description='Generator for test wav files')
    parser.add_argument('name', metavar='NAME', action='store',
                        help='file name without generated suffixes and .wav extension')
    parser.add_argument('-sr', '--samplerate', metavar='SR',
                        action='store', default='44100',
                        help='WAV file samplerate')

    parser.add_argument('-l', '--length', action='store', default='0.01',
                        help='WAV file length in seconds')
    parser.add_argument('-ch', '--channels', action='store', default=1, type=int,
                        help='WAV file channels')
    parser.add_argument('-v', '--value', action='store', default=None, type=int,
                        help='WAV file fill value in [-0x7fff ... +0x7fff] range')
    parser.add_argument('--range', action='store', default=0x7fff, type=int,
                        help='16bit range value')
    parser.add_argument('-s', '--show', dest='show', action='store_const', const=True,
                        help='Show plot')
    parser.add_argument('-p', '--png', dest='save_png', action='store_const', const=True,
                        help='Save plot in png')

    args = parser.parse_args()
    SR = float(re.sub('[^0-9.]', '', args.samplerate))
    if args.samplerate[-1] == 'k':
        SR = int(SR * 1000)

    SAMPLES = int(SR * float(args.length))
    CH = int(args.channels)

    fname_tmpl = "{name}_ch{channels:0>2}_{sr}_{len}samp.wav"

    if SR % 1000:
        SR_TXT = "{}k".format(SR/1000)
    else:
        SR_TXT = "{}k".format(int(SR/1000))

    fname = fname_tmpl.format(
        name=args.name, channels=args.channels, len=SAMPLES, sr=SR_TXT)

    print(f"Generating '{fname}'...")
    file = wave.open(fname, 'w')
    file.setparams((CH, 2, SR, 0, 'NONE', 'not compressed'))
    RANGE = args.range

    for i in range(0, SAMPLES):
        for j in range(0, CH):
            if args.value is not None:
                v0 = args.value
            elif CH == 1:
                v0 = 0
            else:
                v0 = (((2 * RANGE) // (CH - 1)) * j) - RANGE

            v = struct.pack('<h', v0)
            file.writeframes(v)

    file.close()

    with wave.open(fname, 'r') as wav_file:
        # Extract Raw Audio from Wav File
        signal = wav_file.readframes(-1)
        signal = np.fromstring(signal, 'Int16')

        # Split the data into channels
        channels = [[] for channel in range(wav_file.getnchannels())]
        for index, datum in enumerate(signal):
            channels[index % len(channels)].append(datum)

        # Get time from indices
        fs = wav_file.getframerate()
        Time = np.linspace(0, len(signal) / len(channels) /
                           fs, num=len(signal) / len(channels))

        # Plot
        plt.figure(1)
        plt.title('Signal Wave...')
        for channel in channels:
            plt.plot(Time, channel)

        if args.show:
            plt.show()

        if args.save_png:
            png_fname = re.sub('wav$', 'png', fname)
            plt.savefig(png_fname)


if __name__ == '__main__':
    main()
