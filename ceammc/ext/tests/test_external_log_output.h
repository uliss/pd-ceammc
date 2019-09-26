/*****************************************************************************
 * Copyright 2019 Serge Poltavsky. All rights reserved.
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
#ifndef TEST_EXTERNAL_LOG_OUTPUT_H
#define TEST_EXTERNAL_LOG_OUTPUT_H

#include "ceammc_message.h"
#include "ceammc_pd.h"

template <typename... Args>
std::vector<ceammc::Message> messageList(Args... args)
{
    return std::vector<ceammc::Message>({ Message(args)... });
}

class LogExternalOutput : public ceammc::pd::External {
public:
    typedef std::vector<ceammc::Message> MessageList;

public:
    LogExternalOutput();
    ceammc::Message msg();
    MessageList msgList();
    void reset();

public:
    static void setup();
};

struct t_log_output_single {
    t_object x_obj;
    ceammc::Message* msg;
};

struct t_log_output_multi {
    t_object x_obj;
    std::vector<ceammc::Message>* msg_list;
};

bool setup_log_output_single();
bool setup_log_output_multi();

#endif // TEST_EXTERNAL_LOG_OUTPUT_H
