#include "flow_tee_tilde.h"
#include "ceammc_factory.h"

FlowTeeTilde::FlowTeeTilde(const PdArgs& a)
    : SoundExternal(a)
{
    createSignalOutlet();
    createOutlet();
}

void FlowTeeTilde::onBang()
{
    bangTo(1);
}

void FlowTeeTilde::onSymbol(t_symbol* s)
{
    symbolTo(1, s);
}

void FlowTeeTilde::onFloat(t_float f)
{
    floatTo(1, f);
}

void FlowTeeTilde::onList(const AtomList& l)
{
    listTo(1, l);
}

void FlowTeeTilde::onAny(t_symbol* s, const AtomListView& lv)
{
    anyTo(1, s, lv);
}

void FlowTeeTilde::processBlock(const t_sample** in, t_sample** out)
{
    const size_t BS = blockSize();
    for (size_t i = 0; i < BS; i += 16) {
        out[0][i + 0] = in[0][i + 0];
        out[0][i + 1] = in[0][i + 1];
        out[0][i + 2] = in[0][i + 2];
        out[0][i + 3] = in[0][i + 3];
        out[0][i + 4] = in[0][i + 4];
        out[0][i + 5] = in[0][i + 5];
        out[0][i + 6] = in[0][i + 6];
        out[0][i + 7] = in[0][i + 7];
        out[0][i + 8] = in[0][i + 8];
        out[0][i + 9] = in[0][i + 9];
        out[0][i + 10] = in[0][i + 10];
        out[0][i + 11] = in[0][i + 11];
        out[0][i + 12] = in[0][i + 12];
        out[0][i + 13] = in[0][i + 13];
        out[0][i + 14] = in[0][i + 14];
        out[0][i + 15] = in[0][i + 15];
    }
}

bool FlowTeeTilde::processAnyProps(t_symbol*, const AtomListView&)
{
    return false;
}

void setup_flow_tee_tilde()
{
    SoundExternalFactory<FlowTeeTilde> obj("flow.tee~");
    obj.addAlias("tee~");
    obj.addAlias("~->");

    obj.setXletsInfo({ "intermixed control/signal" }, { "output signal", "output control messages" });
}
