#include "hw_spi_max7219.h"
#include "ceammc_factory.h"

HwSpiMax7219::HwSpiMax7219(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
{

    mx_ = ceammc_hw_max7219_new(0,
        { subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } }, //
        { this, [](void* user, const char* msg) {
             LIB_ERR << msg;
         } });
}

HwSpiMax7219::~HwSpiMax7219()
{
    ceammc_hw_max7219_free(mx_);
}

bool HwSpiMax7219::notify(int code)
{
    return true;
}

void HwSpiMax7219::m_intensity(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_max7219_intensity(mx_, lv.intAt(0, 0));
}

void setup_hw_spi_max7219()
{
    ObjectFactory<HwSpiMax7219> obj("hw.spi.max7219");
    obj.addMethod("intensity", &HwSpiMax7219::m_intensity);
}
