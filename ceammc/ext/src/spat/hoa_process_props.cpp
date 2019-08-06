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
#include "hoa_process_props.h"
#include "ceammc_canvas.h"
#include "ceammc_factory.h"
#include "ceammc_format.h"
#include "hoa_process.h"

#include <sstream>
#include <stdexcept>

static t_symbol* SYM_2D;
static t_symbol* SYM_3D;
static t_symbol* SYM_HARMONICS;
static t_symbol* SYM_PLANEWAVES;

HoaProcessProps::HoaProcessProps(const PdArgs& args)
    : BaseObject(args)
    , args_ { &s_, &s_, 0, 0, 0, 0, 0 }
{
    createOutlet();

    createCbProperty("@pmode", &HoaProcessProps::propPMode);
    createCbProperty("@ptype", &HoaProcessProps::propPType);
    createCbProperty("@order", &HoaProcessProps::propOrder);
    createCbProperty("@order", &HoaProcessProps::propOrder);
    createCbProperty("@total", &HoaProcessProps::propTotal);
    createCbProperty("@index", &HoaProcessProps::propIndex);
    createCbProperty("@hdegree", &HoaProcessProps::propHarmDegree);
    createCbProperty("@horder", &HoaProcessProps::propHarmOrder);

    auto cnv_args = canvas_info_args(canvas());
    if (!cnv_args.empty())
        args_ = processHoaProps(cnv_args);
}

AtomList HoaProcessProps::propPMode() const
{
    return Atom(args_.mode);
}

AtomList HoaProcessProps::propPType() const
{
    return Atom(args_.type);
}

AtomList HoaProcessProps::propOrder() const
{
    return Atom(args_.order);
}

AtomList HoaProcessProps::propTotal() const
{
    return Atom(args_.total);
}

AtomList HoaProcessProps::propIndex() const
{
    return Atom(args_.index);
}

AtomList HoaProcessProps::propHarmDegree() const
{
    return Atom(args_.harm_degree);
}

AtomList HoaProcessProps::propHarmOrder() const
{
    return Atom(args_.harm_order);
}

HoaProcessPropsData processHoaProps(const AtomList& lst)
{
    if (lst.size() < 5) {
        std::ostringstream os;
        os << "at least 5 arguments required: " << lst;
        throw std::runtime_error(os.str());
    }

    auto mode = lst[0].asSymbol();
    auto type = lst[1].asSymbol();
    auto order = lst[2];
    auto idx0 = lst[3];
    auto idx1 = lst[4];

    if (mode != SYM_2D && mode != SYM_3D) {
        std::ostringstream os;
        os << "invalid mode: " << mode << ", expecting " << SYM_2D << " or " << SYM_3D;
        throw std::runtime_error(os.str());
    }

    if (type != SYM_HARMONICS && type != SYM_PLANEWAVES) {
        std::ostringstream os;
        os << "invalid type: " << type << ", expecting " << SYM_HARMONICS << " or " << SYM_PLANEWAVES;
        throw std::runtime_error(os.str());
    }

    if (!order.isFloat()) {
        std::ostringstream os;
        os << "order value expected: " << order;
        throw std::runtime_error(os.str());
    }

    if (order.asFloat() < 1) {
        std::ostringstream os;
        os << "order value should be >= 1: " << order;
        throw std::runtime_error(os.str());
    }

    if (!idx0.isFloat() || !idx1.isFloat()) {
        std::ostringstream os;
        os << "integer values expected: " << idx0 << " " << idx1;
        throw std::runtime_error(os.str());
    }

    HoaProcessPropsData res;
    res.mode = mode;
    res.type = type;
    res.order = order.asInt();

    // total
    if (mode == SYM_2D && type == SYM_HARMONICS) {
        res.total = HoaProcess::calcNumHarm2d(res.order);
    } else if (mode == SYM_3D && type == SYM_HARMONICS) {
        res.total = HoaProcess::calcNumHarm3d(res.order);
    } else {
        res.total = res.order;
    }

    // index
    if (mode == SYM_2D && type == SYM_HARMONICS) {
        res.index = HoaProcess::harmToIndex2d(idx0.asInt(), idx1.asInt());
    } else if (mode == SYM_3D && type == SYM_HARMONICS) {
        res.index = HoaProcess::harmToIndex3d(idx0.asInt(), idx1.asInt());
    } else {
        res.index = idx0.asInt();
    }

    // harm degree
    if (type == SYM_HARMONICS)
        res.harm_degree = idx0.asInt();
    else
        res.harm_degree = -1;

    // harm order
    if (type == SYM_HARMONICS)
        res.harm_order = idx1.asInt();
    else
        res.harm_order = -1;

    return res;
}

void setup_spat_hoa_process_props()
{
    SYM_2D = gensym("2d");
    SYM_3D = gensym("3d");
    SYM_HARMONICS = gensym("harmonics");
    SYM_PLANEWAVES = gensym("planewaves");

    ObjectFactory<HoaProcessProps> obj("hoa.@process");
}
