#include "snd_play.h"
#include "ceammc_factory.h"

static const size_t BUF_SIZE = 1024;

SoundPlay::SoundPlay(const PdArgs& a)
    : SoundExternal(a)
    , channels_(0)
    , player_(sound::SoundFileLoader::player())
    , real_channels_(0)
    , state_(STATE_STOPPED)
{
    channels_ = new SizeTPropertyClosedRange("@ch", positionalFloatArgument(0, 2), 1, 16);
    createProperty(channels_);

    createCbProperty("@filename", &SoundPlay::p_filename);
    createCbProperty("@channels", &SoundPlay::p_channels);
    createCbProperty("@samplerate", &SoundPlay::p_sampleRate);
    createCbProperty("@samples", &SoundPlay::p_samples);
    createCbProperty("@current", &SoundPlay::p_currentSample);

    for (size_t i = 0; i < channels_->value(); i++) {
        buffer_.push_back(Channel(BUF_SIZE, 0.f));
        createSignalOutlet();
    }

    extra_buffer_.assign(BUF_SIZE, 0);
}

void SoundPlay::m_open(t_symbol*, const AtomList& lst)
{
    if (!checkArgs(lst, ARG_SYMBOL))
        return;

    const char* name = lst[0].asSymbol()->s_name;

    if (!player_->open(name)) {
        OBJ_ERR << "can't open soundfile: " << name;
        return;
    }

    player_->setSamplerate(sys_getsr());
    real_channels_ = player_->channels();
    ptr_buffer_.clear();

    for (size_t i = 0; i < real_channels_; i++) {
        if (i < channels_->value())
            ptr_buffer_.push_back(buffer_[i].data());
        else
            ptr_buffer_.push_back(extra_buffer_.data());
    }

    state_ = STATE_STOPPED;
}

void SoundPlay::m_play(t_symbol*, const AtomList& lst)
{
    if (state_ == STATE_PLAYING) {
        OBJ_ERR << "already playing";
        return;
    }

    state_ = STATE_PLAYING;
}

void SoundPlay::m_stop(t_symbol*, const AtomList&)
{
    if (state_ == STATE_STOPPED) {
        OBJ_ERR << "already stopped";
        return;
    }

    state_ = STATE_STOPPED;
    player_->seek(0);
}

void SoundPlay::m_pause(t_symbol*, const AtomList&)
{
    state_ = STATE_PAUSED;
}

void SoundPlay::m_seek(t_symbol*, const AtomList& lst)
{
    if (lst.empty())
        return;

    if (lst[0].isFloat()) {
        player_->seek(lst[0].asFloat());
    }
}

void SoundPlay::m_close(t_symbol*, const AtomList&)
{
    if (!player_->isOpened()) {
        OBJ_DBG << "file is not opened";
        return;
    }

    player_->close();
    ptr_buffer_.clear();
    state_ = STATE_STOPPED;
}

AtomList SoundPlay::p_filename() const
{
    return Atom(gensym(player_->filename().c_str()));
}

AtomList SoundPlay::p_channels() const
{
    return Atom(player_->channels());
}

AtomList SoundPlay::p_sampleRate() const
{
    return Atom(player_->sampleRate());
}

AtomList SoundPlay::p_samples() const
{
    return Atom(player_->sampleCount());
}

AtomList SoundPlay::p_currentSample() const
{
    return Atom(player_->tell());
}

void SoundPlay::processBlock(const t_sample**, t_sample** out)
{
    switch (state_) {
    case STATE_PLAYING: {
        t_sample** buf = ptr_buffer_.data();
        long rc = player_->read(buf, blockSize());

        if (rc < 0) {
            OBJ_ERR << "can't read data from file";
            return;
        }

        if (rc == 0) {
            state_ = STATE_STOPPED;
            player_->seek(0);
            return;
        }

        for (size_t samp = 0; samp < blockSize(); samp++) {
            for (size_t ch = 0; ch < numOutputChannels(); ch++) {
                out[ch][samp] = buffer_[ch][samp];
            }
        }

    } break;
    default: {
        for (size_t samp = 0; samp < blockSize(); samp++) {
            for (size_t ch = 0; ch < numOutputChannels(); ch++) {
                out[ch][samp] = 0;
            }
        }
    } break;
    }
}

void setup_snd_play()
{
    SoundExternalFactory<SoundPlay> obj("snd.play~");
    obj.addMethod("open", &SoundPlay::m_open);
    obj.addMethod("play", &SoundPlay::m_play);
    obj.addMethod("stop", &SoundPlay::m_stop);
    obj.addMethod("seek", &SoundPlay::m_seek);
    obj.addMethod("pause", &SoundPlay::m_pause);
}
