/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
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
#ifndef CEAMMC_SOUND_EXTERNAL_H
#define CEAMMC_SOUND_EXTERNAL_H

#include "ceammc_control.h"
#include "ceammc_object.h"

namespace ceammc {
class SoundExternal : public BaseObject {
    const static size_t MAX_SIG_NUM = 64;

private:
    size_t block_size_;
    size_t n_in_;
    size_t n_out_;
    size_t sample_rate_;
    t_sample* in_[MAX_SIG_NUM];
    t_sample* out_[MAX_SIG_NUM];

public:
    SoundExternal(const PdArgs& a);

    /**
     * @brief setup signal processing
     * @param sp - pointer to signals vector
     */
    virtual void setupDSP(t_signal** sp);

    /**
     * @brief creates new signal inlet
     * @return pointer to new inlet
     */
    t_inlet* createSignalInlet();

    /**
     * @brief creates new signal outlet
     * @return pointer to new outlet
     */
    t_outlet* createSignalOutlet();

    /**
     * remove all create control/sound inlets/outlets
     */
    void clearAllXlets();

    /**
     * @brief returns current DSP block size
     */
    size_t blockSize() const { return block_size_; }

    /**
     * @brief return current sample rate
     */
    size_t samplerate() const { return sample_rate_; }

    /**
     * @brief returns number of sound inlets
     */
    size_t numInputChannels() const { return n_in_; }

    /**
     * @brief returns number of sound outlets
     */
    size_t numOutputChannels() const { return n_out_; }

    /**
     * @brief main DSP function. Overload to sound processing.
     */
    virtual void processBlock(const t_sample** in, t_sample** out) = 0;

    /**
     * @brief dump - dumps internal into to Pd window
     */
    void dump() const override;

    /**
     * @brief return pointer to input blocks
     */
    t_sample** inputBlocks() { return in_; }

    /**
     * @brief return pointer to output blocks
     */
    t_sample** outputBlocks() { return out_; }

    /**
     * @brief override this to get block size change notification while setupDSP step
     * @param bs - new block size
     */
    virtual void blockSizeChanged(size_t bs);

    /**
     * @brief override this to get samplerate change notification while setupDSP step
     * @param sr - new sample rate
     */
    virtual void samplerateChanged(size_t sr);

protected:
    /**
     * @brief copy signals pointer
     * @param sp
     */
    void signalInit(t_signal** sp);

private:
    inline void _processBlock()
    {
        processBlock(const_cast<const t_sample**>(static_cast<t_sample**>(in_)), out_);
    }

    inline static t_int* dspPerform(t_int* w)
    {
        SoundExternal* ext = reinterpret_cast<SoundExternal*>(w[1]);
        ext->_processBlock();
        return (w + 2);
    }

protected:
    /* only for testing! */
    void setBlockSize(size_t s) { block_size_ = s; }
    void setSamplerate(size_t sr) { sample_rate_ = sr; }
};
}

#endif // CEAMMC_SOUND_EXTERNAL_H
