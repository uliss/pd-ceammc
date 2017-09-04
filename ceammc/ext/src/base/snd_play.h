#ifndef SND_PLAY_H
#define SND_PLAY_H

#include "ceammc_object.h"
#include "ceammc_sound.h"

using namespace ceammc;

enum PlayState {
    STATE_STOPPED = 0,
    STATE_PLAYING = 1,
    STATE_PAUSED = 2
};

class SoundPlay : public SoundExternal {
    SizeTPropertyClosedRange* channels_;
    sound::SoundFilePlayerPtr player_;

    typedef std::vector<t_sample> Channel;
    typedef std::vector<Channel> Channels;

    Channels buffer_;
    Channel extra_buffer_;
    size_t real_channels_;

    std::vector<t_sample*> ptr_buffer_;

    PlayState state_;

public:
    SoundPlay(const PdArgs& a);

    void m_open(t_symbol*, const AtomList& lst);
    void m_play(t_symbol*, const AtomList& lst);
    void m_stop(t_symbol*, const AtomList&);
    void m_pause(t_symbol*, const AtomList&);
    void m_seek(t_symbol*, const AtomList& lst);
    void m_close(t_symbol*, const AtomList&);

    AtomList p_filename() const;
    AtomList p_channels() const;
    AtomList p_sampleRate() const;
    AtomList p_samples() const;
    AtomList p_currentSample() const;

    void processBlock(const t_sample**, t_sample** out);
};

void setup_snd_play();

#endif // SND_PLAY_H
