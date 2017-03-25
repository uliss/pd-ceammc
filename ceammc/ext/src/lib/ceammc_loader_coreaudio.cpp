/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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
#include "ceammc_loader_coreaudio.h"
#include "ceammc_loader_coreaudio_impl.h"

using namespace ceammc;
using namespace ceammc::sound;

CoreAudioFile::CoreAudioFile(const std::string& fname)
    : SoundFile(fname)
    , sample_rate_(0)
    , channels_(0)
    , sample_count_(0)
{
    audiofile_info_t fi = { 0 };
    ceammc_coreaudio_getinfo(fname.c_str(), &fi);
    sample_rate_ = fi.sampleRate;
    sample_count_ = fi.sampleCount;
    channels_ = fi.channels;
}

size_t CoreAudioFile::sampleCount() const
{
    return sample_count_;
}

size_t CoreAudioFile::sampleRate() const
{
    return sample_rate_;
}

size_t CoreAudioFile::channels() const
{
    return channels_;
}

bool CoreAudioFile::isOpened() const
{
}

bool CoreAudioFile::close()
{
}

long CoreAudioFile::read(t_word* dest, size_t sz, size_t channel, long offset)
{
}

StringList CoreAudioFile::supportedFormats()
{
    return StringList();
}
