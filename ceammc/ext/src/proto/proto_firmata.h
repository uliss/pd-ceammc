#ifndef HW_FIRMATA_H
#define HW_FIRMATA_H

#include "ceammc_object.h"
#include "firmata/firmata_parser.h"

using namespace ceammc;

class Firmata : public BaseObject {
    firmata::FirmataParser parser_;

public:
    Firmata(const PdArgs& a);
    void onFloat(t_float f) override;
    void onAny(t_symbol* s, const AtomListView& lv) override;

    void processFirmataInput(int cmd);
    void toArduino(const std::string& cmd);

public:
    static t_symbol* pinModeToSym(int mode);
    static int symToPinMode(t_symbol* s);
};

void setup_proto_firmata();

#endif // HW_FIRMATA_H
