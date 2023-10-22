#!/usr/bin/env python
import wave
import random
import struct

SAMPLE_LEN = 441

print "Generating 'test_data0.wav'..."
test0 = wave.open('test_data0.wav', 'w')
test0.setparams((1, 2, 44100, 0, 'NONE', 'not compressed'))

for i in range(0, SAMPLE_LEN):
    value = 10 * i
    packed_value = struct.pack('h', value)
    test0.writeframes(packed_value)

test0.close()

print "Generating 'test_data1.wav'..."
test1 = wave.open('test_data1.wav', 'w')
test1.setparams((2, 2, 44100, 0, 'NONE', 'not compressed'))

for i in range(0, SAMPLE_LEN):
    v0 = struct.pack('h', 10 * i)
    v1 = struct.pack('h', -10 * i)
    test1.writeframes(v0)
    test1.writeframes(v1)

test1.close()


print "Generating 'snd_mono_48k.wav'..."
test3 = wave.open('snd_mono_48k.wav', 'w')
test3.setparams((1, 2, 48000, 0, 'NONE', 'not compressed'))

for i in range(0, 4800):
    v0 = struct.pack('h', 2**10)
    test3.writeframes(v0)

test3.close()


print "Generating 'snd_stereo_48k.wav'..."
test4 = wave.open('snd_stereo_48k.wav', 'w')
test4.setparams((2, 2, 48000, 0, 'NONE', 'not compressed'))

for i in range(0, 4800):
    v0 = struct.pack('h', 2**10)
    v1 = struct.pack('h', -2**10)
    test4.writeframes(v0)
    test4.writeframes(v1)

test4.close()

