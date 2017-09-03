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

#include <AudioToolBox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>

#include <ctype.h>
#include <stdio.h>

typedef struct convert_settings_t {
    AudioFileID inputFile;
    AudioStreamBasicDescription inputFormat;
} convert_settings;

static const size_t PLAYER_BUF_SIZE = 1024;

struct audio_player {
    ExtAudioFileRef file_ref;
    AudioStreamBasicDescription in_asbd;
    AudioStreamBasicDescription out_asbd;
    UInt8 buf[PLAYER_BUF_SIZE];
    int is_opened;
};

static int checkError(OSStatus error, const char* op)
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

static Boolean getASBD(ExtAudioFileRef file, AudioStreamBasicDescription* asbd)
{
    UInt32 size = sizeof(AudioStreamBasicDescription);
    OSStatus err = ExtAudioFileGetProperty(file, kExtAudioFileProperty_FileDataFormat, &size, asbd);

    if (err == noErr)
        return true;

    checkError(err, "error: can't get AudioStreamBasicDescription from file");
    return false;
}

static Boolean getPacketCount(AudioFileID file, UInt64* packetCount)
{
    UInt32 propSize = sizeof(UInt64);
    OSStatus err = AudioFileGetProperty(file, kAudioFilePropertyAudioDataPacketCount, &propSize, packetCount);
    if (err == noErr)
        return true;

    checkError(err, "error: can't get packet count");
    return false;
}

static Boolean getMaxPacketSize(AudioFileID file, UInt32* maxPacketSize)
{
    UInt32 propSize = sizeof(UInt32);
    OSStatus err = AudioFileGetProperty(file, kAudioFilePropertyMaximumPacketSize, &propSize, maxPacketSize);
    if (err == noErr)
        return true;

    checkError(err, "error: can't get max packet size");
    return false;
}

static Boolean getPacketTableInfo(AudioFileID file, AudioFilePacketTableInfo* info)
{
    UInt32 propSize = sizeof(AudioFilePacketTableInfo);
    OSStatus err = AudioFileGetProperty(file, kAudioFilePropertyPacketTableInfo, &propSize, &info);

    if (err == noErr)
        return true;

    checkError(err, "error: can't get packet table info");
    return false;
}

static Boolean getVBRPacketBufferSize(AudioStreamBasicDescription* in_asbd, UInt32* out_bufsize)
{
    AudioStreamBasicDescription out_asbd;
    fillOutputASBD(&out_asbd, in_asbd);

    AudioConverterRef converter;
    OSStatus err = AudioConverterNew(in_asbd, &out_asbd, &converter);
    if (err != noErr) {
        checkError(err, "AudioConverterNew");
        return false;
    }

    UInt32 sizePerPacket = in_asbd->mBytesPerPacket;
    UInt32 outputBufferSize = 1024;

    Boolean isCompressed = (sizePerPacket == 0);
    if (isCompressed) {
        UInt32 sizePerPacket;
        UInt32 size = sizeof(UInt32);
        OSStatus err = AudioConverterGetProperty(converter,
            kAudioConverterPropertyMaximumOutputPacketSize,
            &size, &sizePerPacket);

        if (err != noErr) {
            checkError(err, "error: kAudioConverterPropertyMaximumOutputPacketSize");
            return false;
        }

        if (sizePerPacket > outputBufferSize)
            outputBufferSize = sizePerPacket;
    }

    *out_bufsize = outputBufferSize;

    return true;
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
    ExtAudioFileRef converter;
    if (!openConverter(path, &converter))
        return FILEOPEN_ERR;

    AudioStreamBasicDescription asbd = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    if (!getASBD(converter, &asbd))
        return FILEINFO_ERR;

    info->sampleRate = asbd.mSampleRate;
    info->channels = asbd.mChannelsPerFrame;

    SInt64 totalFrameCount;
    UInt32 size = sizeof(totalFrameCount);
    OSStatus err = ExtAudioFileGetProperty(converter, kExtAudioFileProperty_FileLengthFrames, &size, &totalFrameCount);
    if (err != noErr) {
        checkError(err, "error: kExtAudioFileProperty_FileLengthFrames");
        ExtAudioFileDispose(converter);
        return FILEINFO_ERR;
    }

    info->sampleCount = totalFrameCount;
    ExtAudioFileDispose(converter);
    return 0;
}

int64_t ceammc_coreaudio_load(const char* path, size_t channel, size_t offset, size_t count, t_word* buf)
{
    if (count == 0 || buf == 0)
        return INVALID_ARGS;

    ExtAudioFileRef converter;
    if (!openConverter(path, &converter)) {
        return FILEOPEN_ERR;
    }

    AudioStreamBasicDescription asbd;
    if (!getASBD(converter, &asbd)) {
        ExtAudioFileDispose(converter);
        return FILEINFO_ERR;
    }

    if (channel >= asbd.mChannelsPerFrame) {
        ExtAudioFileDispose(converter);
        return INVALID_CHAN;
    }

    AudioStreamBasicDescription audioFormat;
    fillOutputASBD(&audioFormat, &asbd);

    if (!setOutputFormat(converter, &audioFormat)) {
        ExtAudioFileDispose(converter);
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
    size_t frameIdx = 0, k = 0;

    OSStatus err = ExtAudioFileSeek(converter, offset);
    if (err != noErr) {
        checkError(err, "error: ExtAudioFileSeek");
        ExtAudioFileDispose(converter);
        return OFFSET_ERR;
    }

    while (frameCount > 0) {
        err = ExtAudioFileRead(converter, &frameCount, &convertedData);
        if (err != noErr) {
            checkError(err, "error: ExtAudioFileRead");
            ExtAudioFileDispose(converter);
            return OFFSET_ERR;
        }

        if (frameCount > 0) {
            AudioBuffer audioBuffer = convertedData.mBuffers[0];
            float* data = (float*)audioBuffer.mData;

            for (UInt32 i = 0; i < frameCount && (frameIdx < count); i++) {
                buf[frameIdx].w_float = data[audioFormat.mChannelsPerFrame * i + channel];
                frameIdx++;
            }

            k += frameCount;
        }
    }

    ExtAudioFileDispose(converter);

    return k;
}

t_audio_player* ceammc_coreaudio_player_create()
{
    t_audio_player* p = (t_audio_player*)calloc(1, sizeof(t_audio_player));
    return p;
}

int ceammc_coreaudio_player_open(t_audio_player* p, const char* path)
{
    if (!p)
        return INVALID_ARGS;

    if (p->is_opened)
        ceammc_coreaudio_player_close(p);

    if (!openConverter(path, &p->file_ref)) {
        p->is_opened = 0;
        return FILEOPEN_ERR;
    }

    if (!getASBD(p->file_ref, &p->in_asbd)) {
        p->is_opened = 0;
        return FILEINFO_ERR;
    }

    fillOutputASBD(&p->out_asbd, &p->in_asbd);

    if (!setOutputFormat(p->file_ref, &p->out_asbd)) {
        p->is_opened = 0;
        return PROPERTY_ERR;
    }

    p->is_opened = 1;
    return 0;
}

void ceammc_coreaudio_player_free(t_audio_player* p)
{
    ceammc_coreaudio_player_close(p);
    free(p);
}

int ceammc_coreaudio_player_seek(t_audio_player* p, int64_t offset)
{
    if (!p)
        return 0;

    OSStatus err = ExtAudioFileSeek(p->file_ref, offset);
    if (err != noErr)
        return 0;
    else
        return 1;
}

int64_t ceammc_coreaudio_player_read(t_audio_player* p, t_sample** dest, size_t count)
{
    if (!p)
        return INVALID_ARGS;

    UInt32 sizePerPacket = p->out_asbd.mBytesPerPacket;
    UInt32 packetsPerBuffer = PLAYER_BUF_SIZE;
    UInt32 outputBufferSize = packetsPerBuffer * sizePerPacket;

    AudioBufferList convertedData;

    convertedData.mNumberBuffers = 1;
    convertedData.mBuffers[0].mNumberChannels = p->out_asbd.mChannelsPerFrame;
    convertedData.mBuffers[0].mDataByteSize = outputBufferSize;
    convertedData.mBuffers[0].mData = p->buf;

    UInt32 frameCount = PLAYER_BUF_SIZE;
    size_t frameIdx = 0, k = 0;

    while (frameCount > 0) {
        OSStatus err = ExtAudioFileRead(p->file_ref, &frameCount, &convertedData);
        if (err != noErr)
            return READ_ERR;

        if (frameCount > 0) {
            AudioBuffer audioBuffer = convertedData.mBuffers[0];
            float* data = (float*)audioBuffer.mData;

            const UInt32 CHAN_NUM = p->out_asbd.mChannelsPerFrame;

            for (UInt32 i = 0; i < frameCount && (frameIdx < count); i++) {

                for (size_t ch = 0; i < CHAN_NUM; ch++) {
                    dest[ch][frameIdx] = data[CHAN_NUM * i + ch];
                }

                frameIdx++;
            }

            k += frameCount;
        }
    }

    return k;
}

double ceammc_coreaudio_player_samplerate(t_audio_player* p)
{
    return p ? p->in_asbd.mSampleRate : 0;
}

int ceammc_coreaudio_player_channel_count(t_audio_player* p)
{
    return p ? p->in_asbd.mChannelsPerFrame : 0;
}

void ceammc_coreaudio_player_close(t_audio_player* p)
{
    if (!p || !p->is_opened)
        return;

    ExtAudioFileDispose(p->file_ref);
    p->is_opened = 0;
}

int ceammc_coreaudio_player_is_opened(t_audio_player* p)
{
    return p ? p->is_opened : 0;
}

int64_t ceammc_coreaudio_player_tell(t_audio_player* p)
{
    if (!p)
        return 0;

    SInt64 off;
    OSStatus err = ExtAudioFileTell(p->file_ref, &off);

    return err == noErr ? off : 0;
}
