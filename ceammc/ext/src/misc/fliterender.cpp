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

#include <chrono>
#include <functional>

extern "C" {
#include "flite.h"
}

#include "m_pd.h"

#include <iostream>

using namespace ceammc;

//#define FLITE_DEBUG

class ThreadTracker {
    FliteThread& a_;
#ifdef FLITE_DEBUG
    std::chrono::high_resolution_clock::time_point t_;
#endif

public:
    ThreadTracker(FliteThread& a)
        : a_(a)
    {
#ifdef FLITE_DEBUG
        std::cerr << "[flite_thread] " << std::this_thread::get_id() << " started\n";
        std::cerr << "     voice:   " << a_.voice_ << "\n";
        std::cerr << "     message: \"" << a_.str_ << "\"\n";

        t_ = std::chrono::high_resolution_clock::now();
#endif
    }

    ~ThreadTracker()
    {

#ifdef FLITE_DEBUG
        std::cerr << "[flite_thread] " << std::this_thread::get_id() << " done\n";
        double dif = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - t_).count();
        std::cerr << "     time: " << dif << " μs\n";
#endif

        a_.threadDone();
    }
};

static void worker(const std::string& str, const std::string& voice, int SR, FliteThread& flite)
{
    ThreadTracker logger(flite);

    cst_voice* vc = flite_voice_select(voice.c_str());
    if (!vc) {
        std::ostringstream ss;
        ss << "can't load voice: " << voice;
        flite.setError(EXIT_CODE_UNKNOWN_VOICE, ss.str());
        return;
    }

    // render
    cst_wave* wave = flite_text_to_wave(str.c_str(), vc);

    if (!wave) {
        std::ostringstream ss;
        ss << "synth error: " << str;
        flite.setError(EXIT_CODE_ERROR_SYNTH, ss.str());
        return;
    }

    // resample
    cst_wave_resample(wave, SR);

    // ok
    flite.setError(EXIT_CODE_OK, "");
    // store
    flite.storeWave(wave);

    delete_wave(wave);
}

FliteThread::FliteThread()
    : is_running_(false)
{
}

FliteThread::~FliteThread()
{
    if (thread_.joinable())
        thread_.join();
}

bool FliteThread::start(const std::string& str, const std::string& voice)
{
    if (is_running_) {
        LIB_ERR << "[flite] thread is already running...";
        return false;
    }

    str_ = str;
    voice_ = voice;
    is_running_ = true;

    thread_error_.reset();

    if (thread_.joinable())
        thread_.join();

    thread_ = std::thread(&worker, str, voice, sys_getsr(), std::ref(*this));

    return true;
}

bool FliteThread::isRunning() const
{
    return is_running_;
}

bool FliteThread::copyToArray(ceammc::Array& a)
{
    Lock g(mutex_);

    if (!a.resize(wave_.size())) {
        LIB_ERR << "can't resize array: " << a.name();
        return false;
    }

    std::copy(wave_.begin(), wave_.end(), a.begin());
    return true;
}

void FliteThread::storeWave(cst_wave* w)
{
    Lock g(mutex_);

    wave_.resize(w->num_samples);
    for (int i = 0; i < w->num_samples; i++)
        wave_[i] = w->samples[i] / 32768.0;
}

void FliteThread::setError(ExitCode rc, const std::string& msg)
{
    Lock g(mutex_);

    thread_error_.code = rc;
    thread_error_.msg = msg;
}

void FliteThread::threadDone()
{
    is_running_ = false;
}

ThreadError::ThreadError()
    : code(EXIT_CODE_OK)
{
}

void ThreadError::reset()
{
    code = EXIT_CODE_OK;
    msg.clear();
}
