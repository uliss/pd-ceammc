#include "ceammc_player_coreaudio.h"
#include "ceammc_loader_coreaudio_impl.h"

#include <iostream>

using namespace ceammc::sound;

CoreAudioPlayer::CoreAudioPlayer()
    : SoundFilePlayer()
    , handle_(ceammc_coreaudio_player_create())
    , out_sample_rate_(44100)
{
}

CoreAudioPlayer::CoreAudioPlayer(const std::string& filename)
    : SoundFilePlayer()
    , handle_(ceammc_coreaudio_player_create())
    , out_sample_rate_(44100)
{
    open(filename);
}

CoreAudioPlayer::~CoreAudioPlayer()
{
    ceammc_coreaudio_player_free(handle_);
}

size_t CoreAudioPlayer::sampleCount() const
{
    return isOpened() ? sample_count_ : 0;
}

size_t CoreAudioPlayer::sampleRate() const
{
    return isOpened() ? sample_rate_ : 0;
}

size_t CoreAudioPlayer::channels() const
{
    return isOpened() ? channels_ : 0;
}

bool CoreAudioPlayer::isOpened() const
{
    return ceammc_coreaudio_player_is_opened(handle_);
}

long CoreAudioPlayer::read(t_sample** dest, size_t n)
{
    return ceammc_coreaudio_player_read(handle_, dest, n);
}

bool CoreAudioPlayer::seek(size_t offset)
{
    return ceammc_coreaudio_player_seek(handle_, offset);
}

size_t CoreAudioPlayer::tell() const
{
    return ceammc_coreaudio_player_tell(handle_);
}

bool CoreAudioPlayer::open(const std::string& filename)
{
    if (ceammc_coreaudio_player_open(handle_, filename.c_str(), out_sample_rate_) == 0) {
        path_ = filename;
        channels_ = ceammc_coreaudio_player_channel_count(handle_);
        sample_rate_ = ceammc_coreaudio_player_samplerate(handle_);
        sample_count_ = ceammc_coreaudio_player_samples(handle_);
        return true;
    } else {
        std::cerr << "[PLAYER] error while opening: " << filename << "\n";
        return false;
    }
}

bool CoreAudioPlayer::close()
{
    if (isOpened()) {
        path_.clear();
        ceammc_coreaudio_player_close(handle_);
        return true;
    }

    return false;
}

void CoreAudioPlayer::setSamplerate(size_t sr)
{
    out_sample_rate_;
}
