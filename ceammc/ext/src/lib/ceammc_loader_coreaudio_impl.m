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
#include "m_pd.h"

#include <AudioToolbox/AudioToolbox.h>
#include <CoreFoundation/CoreFoundation.h>

#include <ctype.h>
#include <stdio.h>

typedef struct convert_settings_t {
    AudioFileID inputFile;
    AudioStreamBasicDescription inputFormat;
} convert_settings;

struct audio_player {
    ExtAudioFileRef file_ref;
    AudioStreamBasicDescription in_asbd;
    AudioStreamBasicDescription out_asbd;
    struct ceammc_coreaudio_logger logger;
    int is_opened;
};

static int checkError(OSStatus error, const char* op, struct ceammc_coreaudio_logger* log)
{
    if (error == noErr)
        return 0;

    char errorString[20] = { 0 };
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

    if (!log || !log->obj || !log->log_err)
        fprintf(stderr, "[coreaudio] %s (%s)\n", op, errorString);
    else {
        char buf[128];
        snprintf(buf, sizeof(buf) - 1, "[coreaudio] %s (%s)", op, errorString);
        log->log_err(log->obj, buf);
    }

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

static Boolean getASBD(ExtAudioFileRef file, AudioStreamBasicDescription* asbd, struct ceammc_coreaudio_logger* log)
{
    UInt32 size = sizeof(AudioStreamBasicDescription);
    OSStatus err = ExtAudioFileGetProperty(file, kExtAudioFileProperty_FileDataFormat, &size, asbd);

    if (err == noErr)
        return true;

    checkError(err, "error: can't get AudioStreamBasicDescription from file", log);
    return false;
}

static Boolean getPacketCount(AudioFileID file, UInt64* packetCount, struct ceammc_coreaudio_logger* log)
{
    UInt32 propSize = sizeof(UInt64);
    OSStatus err = AudioFileGetProperty(file, kAudioFilePropertyAudioDataPacketCount, &propSize, packetCount);
    if (err == noErr)
        return true;

    checkError(err, "error: can't get packet count", log);
    return false;
}

static Boolean getMaxPacketSize(AudioFileID file, UInt32* maxPacketSize, struct ceammc_coreaudio_logger* log)
{
    UInt32 propSize = sizeof(UInt32);
    OSStatus err = AudioFileGetProperty(file, kAudioFilePropertyMaximumPacketSize, &propSize, maxPacketSize);
    if (err == noErr)
        return true;

    checkError(err, "error: can't get max packet size", log);
    return false;
}

static Boolean getPacketTableInfo(AudioFileID file, AudioFilePacketTableInfo* info, struct ceammc_coreaudio_logger* log)
{
    UInt32 propSize = sizeof(AudioFilePacketTableInfo);
    OSStatus err = AudioFileGetProperty(file, kAudioFilePropertyPacketTableInfo, &propSize, &info);

    if (err == noErr)
        return true;

    checkError(err, "error: can't get packet table info", log);
    return false;
}

static Boolean getVBRPacketBufferSize(AudioStreamBasicDescription* in_asbd, UInt32* out_bufsize, struct ceammc_coreaudio_logger* log)
{
    AudioStreamBasicDescription out_asbd;
    fillOutputASBD(&out_asbd, in_asbd);

    AudioConverterRef converter;
    OSStatus err = AudioConverterNew(in_asbd, &out_asbd, &converter);
    if (err != noErr) {
        checkError(err, "AudioConverterNew", log);
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
            checkError(err, "error: kAudioConverterPropertyMaximumOutputPacketSize", log);
            return false;
        }

        if (sizePerPacket > outputBufferSize)
            outputBufferSize = sizePerPacket;
    }

    *out_bufsize = outputBufferSize;

    return true;
}

static Boolean openAudiofile(const char* path, AudioFileID* file, struct ceammc_coreaudio_logger* log)
{
    CFStringRef name = CFStringCreateWithCString(kCFAllocatorDefault, path, kCFStringEncodingUTF8);
    CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, name, kCFURLPOSIXPathStyle, false);
    OSStatus err = AudioFileOpenURL(url, kAudioFileReadPermission, 0, file);

    CFRelease(url);
    CFRelease(name);

    if (err == noErr)
        return true;

    checkError(err, "error: AudioFileOpenURL", log);
    return false;
}

static Boolean openConverter(const char* path, ExtAudioFileRef* file, struct ceammc_coreaudio_logger* log)
{
    CFStringRef name = CFStringCreateWithCString(kCFAllocatorDefault, path, kCFStringEncodingUTF8);
    CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, name, kCFURLPOSIXPathStyle, false);
    OSStatus err = ExtAudioFileOpenURL(url, file);

    CFRelease(url);
    CFRelease(name);

    if (err == noErr)
        return true;

    checkError(err, "error: AudioFileOpenURL", log);
    return false;
}

static Boolean setOutputFormat(ExtAudioFileRef file, AudioStreamBasicDescription* format, struct ceammc_coreaudio_logger* log)
{
    OSStatus err = ExtAudioFileSetProperty(file,
        kExtAudioFileProperty_ClientDataFormat,
        sizeof(AudioStreamBasicDescription),
        format);

    if (err == noErr)
        return true;

    checkError(err, "error: ExtAudioFileSetProperty", log);
    return false;
}

int ceammc_coreaudio_getinfo(const char* path, audiofile_info_t* info, struct ceammc_coreaudio_logger* log)
{
    ExtAudioFileRef converter = 0;
    if (!openConverter(path, &converter, log)) {
        ExtAudioFileDispose(converter);
        return FILEOPEN_ERR;
    }

    AudioStreamBasicDescription asbd = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    if (!getASBD(converter, &asbd, log)) {
        ExtAudioFileDispose(converter);
        return FILEINFO_ERR;
    }

    info->sampleRate = asbd.mSampleRate;
    info->channels = asbd.mChannelsPerFrame;

    SInt64 totalFrameCount;
    UInt32 size = sizeof(totalFrameCount);
    OSStatus err = ExtAudioFileGetProperty(converter, kExtAudioFileProperty_FileLengthFrames, &size, &totalFrameCount);
    if (err != noErr) {
        checkError(err, "error: kExtAudioFileProperty_FileLengthFrames", log);
        ExtAudioFileDispose(converter);
        return FILEINFO_ERR;
    }

    info->sampleCount = totalFrameCount;
    ExtAudioFileDispose(converter);
    return 0;
}

t_audio_player* ceammc_coreaudio_player_create()
{
    t_audio_player* p = (t_audio_player*)calloc(1, sizeof(t_audio_player));
    return p;
}

int ceammc_coreaudio_player_open(t_audio_player* p, const char* path, double sample_rate, const struct ceammc_coreaudio_logger* log)
{
    if (!p)
        return INVALID_ARGS;

    if (p->is_opened)
        ceammc_coreaudio_player_close(p);

    if (!openConverter(path, &p->file_ref, &p->logger)) {
        p->is_opened = 0;
        return FILEOPEN_ERR;
    }

    if (!getASBD(p->file_ref, &p->in_asbd, &p->logger)) {
        p->is_opened = 0;
        return FILEINFO_ERR;
    }

    fillOutputASBD(&p->out_asbd, &p->in_asbd);

    if (!setOutputFormat(p->file_ref, &p->out_asbd, &p->logger)) {
        p->is_opened = 0;
        return PROPERTY_ERR;
    }

    p->is_opened = 1;

    if (log) {
        p->logger = *log;
    } else {
        p->logger.obj = 0;
    }
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
    if (err != noErr) {
        checkError(err, "error: ExtAudioFileSeek", &p->logger);
        return 1;
    } else
        return 0;
}

int64_t ceammc_coreaudio_player_read(t_audio_player* p, float* dest, size_t count)
{
    if (!p)
        return INVALID_ARGS;

    AudioBufferList convertedData;

    float buf[9000];

    convertedData.mNumberBuffers = 1;
    convertedData.mBuffers[0].mNumberChannels = p->out_asbd.mChannelsPerFrame;
    convertedData.mBuffers[0].mDataByteSize = sizeof(buf);
    convertedData.mBuffers[0].mData = (void*)&buf[0];

    const UInt32 CHAN_NUM = p->out_asbd.mChannelsPerFrame;
    UInt32 numFramesLefToRead = count;
    UInt32 totalFrameCount = 0;
    size_t outFrameIdx = 0;

    do {
        OSStatus err = ExtAudioFileRead(p->file_ref, &numFramesLefToRead, &convertedData);
        if (err != noErr)
            return READ_ERR;

        UInt32 doneFrames = numFramesLefToRead;
        if (doneFrames == 0) // EOF
            break;

        totalFrameCount += doneFrames;

        assert(count >= totalFrameCount);
        numFramesLefToRead = count - totalFrameCount;

        for (UInt32 i = 0; i < doneFrames; i++, outFrameIdx++) {
            if (outFrameIdx >= count) // done
                goto end;

            for (UInt32 ch = 0; ch < CHAN_NUM; ch++) {
                t_sample s = buf[CHAN_NUM * i + ch];
                dest[outFrameIdx * CHAN_NUM + ch] = s;
            }
        }

    } while (true);

end:
    return outFrameIdx;
}

double ceammc_coreaudio_player_samplerate(t_audio_player* p)
{
    return p ? p->in_asbd.mSampleRate : 0;
}

int ceammc_coreaudio_player_channels(t_audio_player* p)
{
    return p ? p->in_asbd.mChannelsPerFrame : 0;
}

int ceammc_coreaudio_player_close(t_audio_player* p)
{
    if (!p || !p->is_opened)
        return INVALID_ARGS;

    OSStatus err = ExtAudioFileDispose(p->file_ref);
    p->is_opened = 0;

    if (err != noErr) {
        checkError(err, "error: ExtAudioFileDispose", &p->logger);
        return CLOSE_ERR;
    } else
        return 0;
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

size_t ceammc_coreaudio_player_frames(t_audio_player* p)
{
    if (!p)
        return 0;

    SInt64 totalFrameCount;
    UInt32 size = sizeof(totalFrameCount);
    OSStatus err = ExtAudioFileGetProperty(p->file_ref,
        kExtAudioFileProperty_FileLengthFrames,
        &size, &totalFrameCount);

    if (err != noErr) {
        checkError(err, "error: kExtAudioFileProperty_FileLengthFrames", &p->logger);
        return 0;
    }

    return totalFrameCount;
}

int64_t ceammc_coreaudio_player_read_array(t_audio_player* p, t_word* dest, size_t count, size_t channel, float gain)
{
    if (!p)
        return INVALID_ARGS;

    if (channel >= p->in_asbd.mChannelsPerFrame)
        return INVALID_CHAN;

    AudioBufferList convertedData;

    float buf[1024];

    convertedData.mNumberBuffers = 1;
    convertedData.mBuffers[0].mNumberChannels = p->out_asbd.mChannelsPerFrame;
    convertedData.mBuffers[0].mDataByteSize = sizeof(buf);
    convertedData.mBuffers[0].mData = (void*)&buf[0];

    const UInt32 CHAN_NUM = p->out_asbd.mChannelsPerFrame;
    UInt32 numFramesLefToRead = count;
    UInt32 totalFrameCount = 0;
    size_t outFrameIdx = 0;

    do {
        OSStatus err = ExtAudioFileRead(p->file_ref, &numFramesLefToRead, &convertedData);
        if (err != noErr)
            return READ_ERR;

        UInt32 doneFrames = numFramesLefToRead;

        if (doneFrames == 0) // EOF
            break;

        totalFrameCount += doneFrames;

        assert(count >= totalFrameCount);
        numFramesLefToRead = count - totalFrameCount;

        for (UInt32 i = 0; i < doneFrames; i++, outFrameIdx++) {
            if (outFrameIdx >= count) // done
                goto end;

            dest[outFrameIdx].w_float = buf[i * CHAN_NUM + channel] * gain;
        }

    } while (true);

end:
    return outFrameIdx;
}

int ceammc_coreaudio_player_set_resample_ratio(t_audio_player* p, double ratio)
{
    if (!p)
        return INVALID_ARGS;

    if (ratio < 0.001)
        return INVALID_RS_RATIO;

    p->out_asbd.mSampleRate = p->in_asbd.mSampleRate * ratio;

    if (!setOutputFormat(p->file_ref, &p->out_asbd, &p->logger))
        return PROPERTY_ERR;

    return 0;
}
