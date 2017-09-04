#ifndef CEAMMC_PLAYER_COREAUDIO_H
#define CEAMMC_PLAYER_COREAUDIO_H

#include "ceammc_sound.h"

struct audio_player;
typedef struct audio_player t_audio_player;

namespace ceammc {
namespace sound {
    class CoreAudioPlayer : public SoundFilePlayer {
        t_audio_player* handle_;

        size_t out_sample_rate_;

        size_t sample_rate_;
        size_t channels_;
        size_t sample_count_;
        bool is_opened_;

    public:
        CoreAudioPlayer();
        CoreAudioPlayer(const std::string& filename);
        ~CoreAudioPlayer();

        size_t sampleCount() const;
        size_t sampleRate() const;
        size_t channels() const;
        bool isOpened() const;

        long read(t_sample** dest, size_t n);

        bool seek(size_t offset);
        size_t tell() const;

        bool open(const std::string& filename);
        bool close();

        void setSamplerate(size_t sr);

    public:
        static FormatList supportedFormats();
    };
}
}

#endif // CEAMMC_PLAYER_COREAUDIO_H
