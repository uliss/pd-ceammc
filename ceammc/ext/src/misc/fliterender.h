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

#include <pthread.h>
#include <string>

typedef struct cst_wave_struct cst_wave;

class FliteRender {
    std::string str_;
    std::string voice_;
    std::string array_;
    pthread_t thread_;
    mutable pthread_mutex_t mutex_;
    int sr_;
    mutable bool is_running_;
    mutable cst_wave* result_;

public:
    FliteRender();
    ~FliteRender();

    bool start(const std::string& array, const std::string& str, const std::string& voice = "kal16");
    bool stop();

    const std::string& str() const;
    const std::string& voice() const;
    int samplerate() const;
    void setResultWave(cst_wave* w);

public:
    void threadFinished();
};

#endif // FLITERENDER_H
