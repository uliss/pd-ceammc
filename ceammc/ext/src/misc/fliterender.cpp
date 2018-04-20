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
#include "fliterender.h"
#include "ceammc_array.h"
#include "ceammc_log.h"
#include "ceammc_thread.h"

#include "flite.h"
#include "m_pd.h"

#include <iostream>

using namespace ceammc;

#define THREAD_CHECK(status, msg)                                   \
    {                                                               \
        if (status != 0) {                                          \
            LIB_ERR << "[flite] " << msg << ", status = " << status \
                    << ", str: " << strerror(status) << "\n";       \
        }                                                           \
    }

class ThreadTracker {
    FliteRender* a_;

public:
    ThreadTracker(FliteRender* a)
        : a_(a)
    {
        std::cerr << "[flite_thread] " << pthread_self() << " started\n";
    }

    ~ThreadTracker()
    {
        std::cerr << "[flite_thread] " << pthread_self() << " finished\n";
        a_->threadFinished();
    }
};

static void* worker(void* v)
{
    FliteRender* flite = static_cast<FliteRender*>(v);
    ThreadTracker logger(flite);

    // copy to thread
    std::string str(flite->str());
    std::string voice(flite->voice());
    const int SR = flite->samplerate();

    cst_voice* vc = flite_voice_select(voice.c_str());
    if (!vc) {
        std::cerr << "can't load voice: " << voice << "\n";
        return 0;
    }

    cst_wave* wave = flite_text_to_wave(str.c_str(), vc);

    if (!wave) {
        std::cerr << "synthesis failed for text '" << str << "'";
        return 0;
    }

    cst_wave_resample(wave, SR);
    flite->setResultWave(wave);

    return 0;
}

FliteRender::FliteRender()
    : voice_(0)
    , sr_(0)
    , is_running_(false)
    , result_(0)
{
    int status = 0;
    status = pthread_mutex_init(&mutex_, 0);
    THREAD_CHECK(status, "can't init mutex");
}

FliteRender::~FliteRender()
{
    stop();

    int err = pthread_mutex_destroy(&mutex_);
    THREAD_CHECK(err, "can't destroy mutex");
}

bool FliteRender::start(const std::string& array, const std::string& str, const std::string& voice)
{
    Array arr(array.c_str());
    if (!arr.isValid()) {
        LIB_ERR << "invalid array: " << array;
        return false;
    }

    if (is_running_) {
        LIB_ERR << "[flite] thread is running...\n";
        return false;
    }

    array_ = array;
    str_ = str;
    voice_ = voice;
    sr_ = sys_getsr();
    result_ = 0;

    int err = pthread_create(&thread_, 0, worker, this);

    THREAD_CHECK(err, "thread create failed");
    if (err)
        return false;

    is_running_ = true;
    return true;
}

bool FliteRender::stop()
{
    if (!is_running_) {
        LIB_ERR << "[flite] thread is not running...\n";
        return false;
    }

    int err = pthread_join(thread_, 0);
    THREAD_CHECK(err, "thread join failed");
    if (err)
        return false;

    is_running_ = false;
    return true;
}

const std::string& FliteRender::str() const
{
    return str_;
}

const std::string& FliteRender::voice() const
{
    return voice_;
}

void FliteRender::setResultWave(cst_wave* w)
{
    thread::Lock lock(mutex_);
    result_ = w;
}

void FliteRender::threadFinished()
{
    thread::Lock lock(mutex_);
    is_running_ = false;

    Array array(array_.c_str());
    if (!array.isValid()) {
        LIB_ERR << "can't open array: " << array_;
    } else {
        if (!array.resize(result_->num_samples)) {
            LIB_ERR << "can't resize array to " << result_->num_samples;
        } else {
            // copy samples
            for (int i = 0; i < result_->num_samples; i++)
                array[i] = result_->samples[i] / 32767.0;
        }
    }

    delete_wave(result_);
}
