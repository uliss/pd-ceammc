#ifndef HW_MOTU_AVB_H
#define HW_MOTU_AVB_H

#include "ceammc_pollthread_object.h"
#include "ceammc_thread.h"
#include "datatype_dict.h"
using namespace ceammc;

using MotuAvbBase = PollThreadQueueObject<DataTypeDict, DataTypeDict>;

class HwMotuAvb : public MotuAvbBase {
    SymbolProperty* device_;
    SymbolProperty* host_;
    IntProperty* port_;
    ThreadPdLogger logger_;

public:
    HwMotuAvb(const PdArgs& args);

    Future createTask() final;
    void processMessage(const DataTypeDict& msg) final;

    void m_sync(t_symbol* s, const AtomListView& lv);
    void m_phantom(t_symbol* s, const AtomListView& lv);
    void m_mic_gain(t_symbol* s, const AtomListView& lv);
    void m_guitar_gain(t_symbol* s, const AtomListView& lv);
    void m_input_gain(t_symbol* s, const AtomListView& lv);

private:
    bool fillRequestDict(t_symbol* s, DataTypeDict& dict, int type) const;
    bool scheduleTask(t_symbol* s, DataTypeDict&& dict);
};

void setup_hw_motu_avb();

#endif // HW_MOTU_AVB_H
