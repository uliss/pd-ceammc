#ifndef HW_MOTU_AVB_H
#define HW_MOTU_AVB_H

#include "ceammc_clock.h"
#include "ceammc_pollthread_object.h"
#include "ceammc_thread.h"
#include "datatype_dict.h"
using namespace ceammc;

struct MotuAvbRequest {
    DataTypeDict data;
    std::string host;
    std::string device;
    int port;
    int type;
};

using MotuAvbBase = PollThreadQueueObject<MotuAvbRequest, DataTypeDict>;

class HwMotuAvb : public MotuAvbBase {
    SymbolProperty* device_;
    SymbolProperty* host_;
    IntProperty* port_;
    ThreadPdLogger logger_;
    MotuAvbRequest req_set_;
    ClockLambdaFunction req_timer_;

public:
    HwMotuAvb(const PdArgs& args);

    Future createTask() final;
    void processMessage(const DataTypeDict& msg) final;

    void m_sync(t_symbol* s, const AtomListView& lv);
    void m_phantom(t_symbol* s, const AtomListView& lv);
    void m_mic_gain(t_symbol* s, const AtomListView& lv);
    void m_guitar_gain(t_symbol* s, const AtomListView& lv);
    void m_input_gain(t_symbol* s, const AtomListView& lv);
    void m_mic_pad(t_symbol* s, const AtomListView& lv);
    void m_main_gain(t_symbol* s, const AtomListView& lv);
    void m_phones_gain(t_symbol* s, const AtomListView& lv);
    void m_output_gain(t_symbol* s, const AtomListView& lv);

private:
    bool updateRequest(t_symbol* s, MotuAvbRequest& req, int type) const;
    void scheduleTask(const MotuAvbRequest& req);
    void m_set_single(t_symbol* s, const char* key, int ch, int val, const AtomListView& lv);
};

void setup_hw_motu_avb();

#endif // HW_MOTU_AVB_H
