#include "hw_rpi_display_ssd1306.h"
#include "args/argcheck.h"
#include "ceammc_crc32.h"
#include "ceammc_factory.h"

CEAMMC_DEFINE_SYM_HASH(i2c)
CEAMMC_DEFINE_SYM_HASH(spi)

HwRpiDisplaySsd1306::HwRpiDisplaySsd1306(const PdArgs& args)
    : DispatchedObject<BaseObject>(args)
{
    createOutlet();

    mode_ = new SymbolEnumProperty("@mode", { sym_i2c(), sym_spi() });
    mode_->setInitOnly();
    mode_->setArgIndex(0);
    addProperty(mode_);

    spi_ = new ListProperty("@spi");
    spi_->setInitOnly();
    addProperty(spi_);

    i2c_ = new ListProperty("@i2c");
    i2c_->setInitOnly();
    addProperty(i2c_);
}

HwRpiDisplaySsd1306::~HwRpiDisplaySsd1306()
{
    ceammc_hw_display_ssd1306_free(display_);
}

void HwRpiDisplaySsd1306::initDone()
{
    switch (crc32_hash(mode_->value())) {
    case hash_i2c:
        OBJ_ERR << "not implemented";
        break;
    case hash_spi: {
        static const args::ArgChecker chk("BUS:b DC:b CS:b FREQ:i?");
        if (!chk.check(spi_->value(), this))
            return chk.usage(this);

        auto& args = spi_->value();
        auto bus = args.intAt(0, 0);
        auto dc = args.intAt(1, 0);
        auto cs = args.intAt(2, 0);
        auto freq = args.intAt(3, 1000000);

        display_ = ceammc_hw_display_ssd1306_new_spi(bus, dc, cs, freq,
            { subscriberId(), [](size_t id) { Dispatcher::instance().send({ id, 0 }); } },
            { this, [](void* user, const char* msg) {
                 Error err(static_cast<HwRpiDisplaySsd1306*>(user));
                 err << msg;
             } });
    } break;
    default:
        OBJ_ERR << "not implemented";
        break;
    }
}

bool HwRpiDisplaySsd1306::notify(int code)
{
    return ceammc_hw_display_ssd1306_proc_reply(display_);
}

void HwRpiDisplaySsd1306::m_clear(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("FLUSH:B?");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_display_ssd1306_clear(display_, lv.boolAt(0, false));
}

void HwRpiDisplaySsd1306::m_flush(t_symbol* s, const AtomListView& lv)
{
    ceammc_hw_display_ssd1306_flush(display_);
}

void HwRpiDisplaySsd1306::m_invert(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("INVERT:B");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_display_ssd1306_invert(display_, lv.boolAt(0, false));
}

void HwRpiDisplaySsd1306::m_mirror(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("MIRROR:B");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_display_ssd1306_mirror(display_, lv.boolAt(0, false));
}

void HwRpiDisplaySsd1306::m_switch_on(t_symbol* s, const AtomListView& lv)
{
    static const args::ArgChecker chk("ON:B");
    if (!chk.check(lv, this))
        return chk.usage(this, s);

    ceammc_hw_display_ssd1306_switch_on(display_, lv.boolAt(0, false));
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

    obj.addMethod("clear", &HwRpiDisplaySsd1306::m_clear);
    obj.addMethod("flush", &HwRpiDisplaySsd1306::m_flush);
    obj.addMethod("invert", &HwRpiDisplaySsd1306::m_invert);
    obj.addMethod("mirror", &HwRpiDisplaySsd1306::m_mirror);
    obj.addMethod("switch_on", &HwRpiDisplaySsd1306::m_switch_on);
    obj.addMethod("text", &HwRpiDisplaySsd1306::m_text);
}
