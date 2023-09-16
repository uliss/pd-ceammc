/*****************************************************************************
 * Copyright 2023 Serge Poltavski. All rights reserved.
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
#include "conv_bar2ms.h"
#include "ceammc_factory.h"

ConvBarToMs::ConvBarToMs(const PdArgs& args)
    : BaseObject(args)
{
    createInlet();
    createOutlet();
    createOutlet();

    tempo_ = new BpmProperty("@bpm", { 60, 4 });
    tempo_->setArgIndex(0);
    addProperty(tempo_);

    tsig_ = new TimeSignatureProperty("@tsig");
    tsig_->setArgIndex(1);
    addProperty(tsig_);
}

void ConvBarToMs::onBang()
{
    floatTo(1, music::Duration(1, tsig_->signature().subDivision()).timeMs(tempo_->tempo()));
    floatTo(0, tsig_->signature().timeMs(tempo_->tempo()));
}

void ConvBarToMs::onFloat(t_float bpm)
{
    if (tempo_->setFloat(bpm))
        onBang();
}

void ConvBarToMs::onSymbol(t_symbol* bpm)
{
    if (tempo_->setSymbol(bpm))
        onBang();
}

void ConvBarToMs::onList(const AtomListView& lv)
{
    if (lv.size() > 1) {
        if (!tsig_->set(lv.subView(1, 1)))
            return;
    }

    if (lv.size() > 0) {
        if (!tempo_->set(lv.subView(0, 1)))
            return;
    }

    onBang();
}

void ConvBarToMs::onInlet(size_t n, const AtomListView& lv)
{
    tsig_->set(lv);
}

void setup_conv_bar2ms()
{
    ObjectFactory<ConvBarToMs> obj("conv.bar2ms");
    obj.addAlias("bar->ms");

    obj.setXletsInfo(
        { "bang: output\n"
          "float: set BPM and output\n"
          "symbol: set BPM and output\n"
          "list: set BPM and time signature and output",
            "symbol: set time signature" },
        { "float: whole bar duration",
            "float: beat duration" });

    obj.setDescription("calculate bar and beat duration");
    obj.addAuthor("Serge Poltavsky");
    obj.setKeywords({ "conv", "time", "duration", "tempo", "bpm" });
    obj.setCategory("conv");
    obj.setSinceVersion(0, 9);
}
