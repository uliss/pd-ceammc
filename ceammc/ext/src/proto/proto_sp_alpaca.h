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
#ifndef PROTO_SP_ALPACA_H
#define PROTO_SP_ALPACA_H

#include "ceammc_object.h"

#include <boost/container/static_vector.hpp>
#include <cstdint>

using namespace ceammc;

static const size_t PROTO_ALPACA_MAX_CMD_SIZE = 16;

class ProtoSpAlpaca : public BaseObject {
    typedef boost::container::static_vector<uint8_t, PROTO_ALPACA_MAX_CMD_SIZE> StaticVector;

    StaticVector in_cmd_;
    int state_;

public:
    ProtoSpAlpaca(const PdArgs& args);
    void onFloat(t_float f) override;

    bool fsm_clear();
    bool fsm_add(uint8_t v);
    bool fsm_output_digital();
    bool fsm_output_analog();
    bool fsm_output_analog_raw();
    bool fsm_output_response();

    void m_brightness(t_symbol* s, const AtomListView& l);
    void m_char(t_symbol* s, const AtomListView& l);
    void m_clear(t_symbol* s, const AtomListView& l);
    void m_col(t_symbol* s, const AtomListView& l);
    void m_fill(t_symbol* s, const AtomListView& l);
    void m_get_version(t_symbol* s, const AtomListView& l);
    void m_invert(t_symbol* s, const AtomListView& l);
    void m_mode(t_symbol* s, const AtomListView& l);
    void m_pixel(t_symbol* s, const AtomListView& l);
    void m_row(t_symbol* s, const AtomListView& l);
    void m_str(t_symbol* s, const AtomListView& l);
    void m_sync(t_symbol* s, const AtomListView& l);

private:
    void parse(uint8_t v);
    void drawChar(int ch, int offset = 0);
};

void setup_proto_sp_alpaca();

#endif // PROTO_SP_ALPACA_H
