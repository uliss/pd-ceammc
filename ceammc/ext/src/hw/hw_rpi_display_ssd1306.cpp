#include "hw_rpi_display_ssd1306.h"
#include "ceammc_factory.h"

HwRpiDisplaySsd1306::HwRpiDisplaySsd1306(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
{
    createOutlet();
}

HwRpiDisplaySsd1306::~HwRpiDisplaySsd1306()
{
    ceammc_hw_display_ssd1306_free(display_);
}

void HwRpiDisplaySsd1306::initDone()
{
    display_ = ceammc_hw_display_ssd1306_new(0,
        { subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } },
        { this, [](void* user, const char* msg) {
             Error err(static_cast<HwRpiDisplaySsd1306*>(user));
             err << msg;
         } });
}

bool HwRpiDisplaySsd1306::notify(int code)
{
    return ceammc_hw_display_ssd1306_proc_reply(display_);
}

void HwRpiDisplaySsd1306::m_text(t_symbol* s, const AtomListView& lv)
{
    auto txt = lv.symbolAt(0, &s_);
    auto x = lv.intAt(1, 0);
    auto y = lv.intAt(2, 0);
    ceammc_hw_display_ssd1306_text(display_, txt->s_name, x, y);
}

void setup_hw_rpi_display_ssd1306()
{
    ObjectFactory<HwRpiDisplaySsd1306> obj("hw.rpi.display.ssd1306");
    obj.addMethod("text", &HwRpiDisplaySsd1306::m_text);
}
