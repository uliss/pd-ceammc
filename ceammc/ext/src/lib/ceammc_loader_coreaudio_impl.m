/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/

#include "ceammc_loader_coreaudio_impl.h"

#import <AudioToolBox/AudioToolbox.h>
#import <Foundation/Foundation.h>

#include <ctype.h>
#include <stdio.h>

typedef struct convert_settings_t {
    AudioFileID inputFile;
    AudioStreamBasicDescription inputFormat;
} convert_settings;

int checkError(OSStatus error, const char* op)
{
    if (error == noErr)
        return 0;

    char errorString[20];
    *(UInt32*)(errorString + 1) = CFSwapInt32HostToBig(error);

    if (isprint(errorString[1])
        && isprint(errorString[2])
        && isprint(errorString[3])
        && isprint(errorString[4])) {
        errorString[0] = errorString[5] = '\'';
        errorString[6] = '\0';
    } else {
        sprintf(errorString, "%d", (int)error);
    }

    fprintf(stderr, "Error: %s (%s)\n", op, errorString);
    return -1;
}

static Boolean getASBD(AudioFileID file, AudioStreamBasicDescription* asbd)
{
    UInt32 propSize = sizeof(AudioStreamBasicDescription);
    OSStatus err = AudioFileGetProperty(file, kAudioFilePropertyDataFormat, &propSize, asbd);

    if (err == noErr)
        return true;

    checkError(err, "error: can't get AudioStreamBasicDescription from file");
    return false;
}

static Boolean openAudiofile(const char* path, AudioFileID* file)
{
    CFStringRef name = CFStringCreateWithCString(kCFAllocatorDefault, path, kCFStringEncodingUTF8);
    CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, name, kCFURLPOSIXPathStyle, false);
    OSStatus err = AudioFileOpenURL(url, kAudioFileReadPermission, 0, file);

    CFRelease(url);
    CFRelease(name);

    if (err == noErr)
        return true;

    checkError(err, "error: AudioFileOpenURL");
    return false;
}

static Boolean openConverter(const char* path, ExtAudioFileRef* file)
{
    CFStringRef name = CFStringCreateWithCString(kCFAllocatorDefault, path, kCFStringEncodingUTF8);
    CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, name, kCFURLPOSIXPathStyle, false);
    OSStatus err = ExtAudioFileOpenURL(url, file);

    CFRelease(url);
    CFRelease(name);

    if (err == noErr)
        return true;

    checkError(err, "error: AudioFileOpenURL");
    return false;
}

static void fillOutputASBD(AudioStreamBasicDescription* out, const AudioStreamBasicDescription* in)
{
    out->mSampleRate = in->mSampleRate;
    out->mFormatID = kAudioFormatLinearPCM;
    out->mFormatFlags = kLinearPCMFormatFlagIsFloat | kAudioFormatFlagsNativeFloatPacked;
    out->mBitsPerChannel = sizeof(Float32) * 8;
    out->mChannelsPerFrame = in->mChannelsPerFrame;
    out->mBytesPerFrame = out->mChannelsPerFrame * sizeof(Float32);
    out->mFramesPerPacket = 1;
    out->mBytesPerPacket = out->mFramesPerPacket * out->mBytesPerFrame;
}

static Boolean setOutputFormat(ExtAudioFileRef file, AudioStreamBasicDescription* format)
{
    OSStatus err = ExtAudioFileSetProperty(file,
        kExtAudioFileProperty_ClientDataFormat,
        sizeof(AudioStreamBasicDescription),
        format);

    if (err == noErr)
        return true;

    checkError(err, "error: ExtAudioFileSetProperty");
    return false;
}

int ceammc_coreaudio_getinfo(const char* path, audiofile_info_t* info)
{
    AudioFileID in_file;
    if (!openAudiofile(path, &in_file))
        return -1;

    AudioStreamBasicDescription asbd;
    if (!getASBD(in_file, &asbd)) {
        AudioFileClose(in_file);
        return -1;
    }

    UInt64 packetCount = 0;
    UInt32 propSize = sizeof(packetCount);
    OSStatus err = AudioFileGetProperty(in_file, kAudioFilePropertyAudioDataPacketCount, &propSize, &packetCount);
    if (err != noErr) {
        AudioFileClose(in_file);
        return checkError(err, "error: can't get data format in file");
    }

    info->sampleRate = asbd.mSampleRate;
    info->channels = asbd.mChannelsPerFrame;

    if (asbd.mBytesPerPacket)
        info->sampleCount = packetCount * asbd.mFramesPerPacket;
    else
        info->sampleCount = 0;

    AudioFileClose(in_file);

    return 0;
}

int64_t ceammc_coreaudio_load(const char* path, size_t channel, size_t offset, size_t count, float* buf)
{
    if (count == 0 || buf == 0)
        return INVALID_ARGS;

    AudioFileID in_file;
    if (!openAudiofile(path, &in_file))
        return FILEOPEN_ERR;

    AudioStreamBasicDescription asbd;
    if (!getASBD(in_file, &asbd)) {
        AudioFileClose(in_file);
        return FILEINFO_ERR;
    }

    if (channel >= asbd.mChannelsPerFrame) {
        AudioFileClose(in_file);
        return INVALID_CHAN;
    }

    ExtAudioFileRef converter;
    if (!openConverter(path, &converter)) {
        AudioFileClose(in_file);
        return FILEINFO_ERR;
    }

    AudioStreamBasicDescription audioFormat;
    fillOutputASBD(&audioFormat, &asbd);

    if (!setOutputFormat(converter, &audioFormat)) {
        ExtAudioFileDispose(converter);
        AudioFileClose(in_file);
        return PROPERTY_ERR;
    }

    UInt32 numSamples = 1024; //How many samples to read in at a time
    UInt32 sizePerPacket = audioFormat.mBytesPerPacket;
    UInt32 packetsPerBuffer = numSamples;
    UInt32 outputBufferSize = packetsPerBuffer * sizePerPacket;
    UInt8* outputBuffer = (UInt8*)malloc(sizeof(UInt8) * outputBufferSize);

    AudioBufferList convertedData;

    convertedData.mNumberBuffers = 1;
    convertedData.mBuffers[0].mNumberChannels = audioFormat.mChannelsPerFrame;
    convertedData.mBuffers[0].mDataByteSize = outputBufferSize;
    convertedData.mBuffers[0].mData = outputBuffer;

    UInt32 frameCount = numSamples;
    size_t j = 0;

    OSStatus err = ExtAudioFileSeek(converter, offset);
    if (err != noErr) {
        ExtAudioFileDispose(converter);
        AudioFileClose(in_file);
        return OFFSET_ERR;
    }

    while (frameCount > 0) {
        ExtAudioFileRead(converter, &frameCount, &convertedData);

        if (frameCount > 0) {
            AudioBuffer audioBuffer = convertedData.mBuffers[0];
            float* data = (float*)audioBuffer.mData;

            for (UInt32 i = 0; i < frameCount && (j < count); i++) {
                buf[j] = data[audioFormat.mChannelsPerFrame * i + channel];
                j++;
            }
        }
    }

    ExtAudioFileDispose(converter);
    AudioFileClose(in_file);

    return j;
}
