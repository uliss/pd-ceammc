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

#import <AudioToolBox/AudioToolbox.h>
#import <Foundation/Foundation.h>

#include <ctype.h>
#include <stdio.h>

#include "ceammc_loader_coreaudio_impl.h"

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

int ceammc_coreaudio_getinfo(const char* path, audiofile_info_t* info)
{
    AudioFileID in_file;
    AudioStreamBasicDescription asbd;
    OSStatus err = 0;
    CFStringRef name = CFStringCreateWithCString(kCFAllocatorDefault, path, kCFStringEncodingUTF8);
    CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, name, kCFURLPOSIXPathStyle, false);

    err = AudioFileOpenURL(url, kAudioFileReadPermission, 0, &in_file);

    CFRelease(url);
    CFRelease(name);

    if (err != noErr)
        return checkError(err, "error: AudioFileOpenURL");

    UInt32 propSize = sizeof(asbd);
    err = AudioFileGetProperty(in_file, kAudioFilePropertyDataFormat, &propSize, &asbd);
    if (err != noErr) {
        AudioFileClose(in_file);
        return checkError(err, "error: can't get data format in file");
    }

    UInt64 packetCount = 0;
    propSize = sizeof(packetCount);
    err = AudioFileGetProperty(in_file, kAudioFilePropertyAudioDataPacketCount, &propSize, &packetCount);
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

void convert(convert_settings* s)
{
    OSStatus err = 0;
    //    UInt32 outputBufferSize = 32 * 1024;
    //    UInt32 sizePerPacket = s->outputFormat.mBytesPerPacket;
    //    UInt32 packetsPerBuffer = outputBufferSize / sizePerPacket;
    //    float* buf = (float*)malloc(sizeof(float) * outputBufferSize);
    //    UInt32 outputFilePacketPosition = 0;

    //    while (1) {
    //        AudioBufferList convertedData;
    //        convertedData.mNumberBuffers = 1;
    //        convertedData.mBuffers[0].mNumberChannels = s->outputFormat.mChannelsPerFrame;
    //        convertedData.mBuffers[0].mDataByteSize = outputBufferSize;
    //        convertedData.mBuffers[0].mData = buf;

    //        UInt32 frameCount = packetsPerBuffer;
    //        err = ExtAudioFileRead(s->inputFile, &frameCount, &convertedData);
    //        if (err != noErr) {
    //            checkError(err, "error: ExtAudioFileRead");
    //            return;
    //        }

    //        if (frameCount == 0)
    //            return;
    //    }
}

void openFile(const char* path)
{
    //    AudioFileID playbackFile = 0;
    //    OSStatus err;

    //    convert_settings s = { 0 };

    //    CFStringRef name = CFStringCreateWithCString(kCFAllocatorDefault, path, kCFStringEncodingUTF8);
    //    CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, name, kCFURLPOSIXPathStyle, false);

    //    err = ExtAudioFileOpenURL(url, &s.inputFile);
    //    CFRelease(name);
    //    CFRelease(url);

    //    if (checkError(err, "error: ExtAudioFileOpenURL") != 0)
    //        return 0;

    //    s.outputFormat.mSampleRate = 44100.0;
    //    s.outputFormat.mFormatID = kAudioFormatLinearPCM | kAudioFormatFlagIsFloat;
    //    s.outputFormat.mChannelsPerFrame = 1;
    //    //    s.outputFormat.mBitsPerChannel = 16;
    //    //    s.out

    //    err = ExtAudioFileSetProperty(s.inputFile, kExtAudioFileProperty_ClientDataFormat,
    //        sizeof(AudioStreamBasicDescription), &s.outputFormat);

    //    if (checkError(err, "error: ExtAudioFileOpenURL") != 0)
    //        return 0;

    //    convert(&s);

    //    ExtAudioFileDispose(s.inputFile);

    //    AudioStreamBasicDescription dataFormat;
    //    UInt32 propSize = sizeof(dataFormat);

    //    err = AudioFileGetProperty(playbackFile, kAudioFilePropertyDataFormat, &propSize, &dataFormat);
    //    if (checkError(err, "error: ExtAudioFileOpenURL") != 0)
    //        return 0;

    //    return playbackFile;
}
