#include "ceammc_player_coreaudio.h"
#include "ceammc_loader_coreaudio_impl.h"

#include <iostream>

using namespace ceammc::sound;

CoreAudioPlayer::CoreAudioPlayer(const std::string& fname)
    : SoundFilePlayer(fname)
    , handle_(ceammc_coreaudio_player_create())
{
    if (ceammc_coreaudio_player_open(handle_, filename().c_str()) != 0) {
        std::cerr << "[PLAYER] error while opening: " << filename() << "\n";
    }
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

size_t CoreAudioPlayer::read(t_sample** dest, size_t n)
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
