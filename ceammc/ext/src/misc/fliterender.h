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
#ifndef FLITERENDER_H
#define FLITERENDER_H

#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include "ceammc_array.h"

typedef struct cst_wave_struct cst_wave;
typedef std::lock_guard<std::mutex> Lock;

enum ExitCode {
    EXIT_CODE_OK = 0,
    EXIT_CODE_UNKNOWN_VOICE,
    EXIT_CODE_ERROR_SYNTH
};

class ThreadError {
public:
    ExitCode code;
    std::string msg;

    ThreadError();
    void reset();
};

class FliteFeatures {
public:
    float speed;
    float pitch;
};

class FliteThread {
    std::string str_;
    std::string voice_;
    std::thread thread_;
    std::mutex mutex_;
    bool is_running_;
    ThreadError thread_error_;
    std::vector<float> wave_;

public:
    FliteThread();
    ~FliteThread();

    bool start(const std::string& str, const std::string& voice = "kal16", const FliteFeatures& features = FliteFeatures());
    bool isRunning() const;

    bool copyToArray(ceammc::Array& a);

    // thread context
    void storeWave(cst_wave* w);
    void setError(ExitCode rc, const std::string& msg);

public:
    friend class ThreadTracker;

private:
    void threadDone();
};

#endif // FLITERENDER_H
