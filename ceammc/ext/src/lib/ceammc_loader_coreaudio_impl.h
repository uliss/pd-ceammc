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
#ifndef CEAMMC_LOADER_COREAUDIO_IMPL_H
#define CEAMMC_LOADER_COREAUDIO_IMPL_H

#include <m_pd.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct audio_fileinfo_ {
    size_t sampleRate;
    size_t channels;
    size_t sampleCount;
} audiofile_info_t;

enum ceammc_coreaudio_error {
    INVALID_ARGS = -1,
    FILEOPEN_ERR = -2,
    FILEINFO_ERR = -3,
    PROPERTY_ERR = -4,
    INVALID_CHAN = -5,
    OFFSET_ERR = -6,
    READ_ERR = -7,
    CONVERTER_ERR = -8,
    INVALID_RS_RATIO = -9,
    CLOSE_ERR = -10,
};

struct audio_player;
typedef struct audio_player t_audio_player;

int ceammc_coreaudio_getinfo(const char* path, audiofile_info_t* info);

t_audio_player* ceammc_coreaudio_player_create();
int ceammc_coreaudio_player_open(t_audio_player* p, const char* path, double sample_rate);
int ceammc_coreaudio_player_close(t_audio_player* p);
void ceammc_coreaudio_player_free(t_audio_player* p);
double ceammc_coreaudio_player_samplerate(t_audio_player* p);
size_t ceammc_coreaudio_player_frames(t_audio_player* p);
int ceammc_coreaudio_player_is_opened(t_audio_player* p);
int ceammc_coreaudio_player_channels(t_audio_player* p);
int ceammc_coreaudio_player_seek(t_audio_player* p, int64_t offset);
int64_t ceammc_coreaudio_player_tell(t_audio_player* p);
int64_t ceammc_coreaudio_player_read(t_audio_player* p, float* dest, size_t count);
int64_t ceammc_coreaudio_player_read_array(t_audio_player* p, t_word* dest, size_t count, size_t channel, float gain);
int ceammc_coreaudio_player_set_resample_ratio(t_audio_player* p, double ratio);

#if defined(__cplusplus)
}
#endif

#endif // CEAMMC_LOADER_COREAUDIO_IMPL_H
