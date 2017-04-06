#include "mod_data.h"

extern "C" void setup_data0x2efifo();
extern "C" void setup_data0x2elist();
extern "C" void setup_global0x2efloat();
extern "C" void setup_global0x2eint();
extern "C" void setup_global0x2ejson();
extern "C" void setup_global0x2elist();
extern "C" void setup_local0x2efloat();
extern "C" void setup_local0x2eint();
extern "C" void setup_local0x2ejson();
extern "C" void setup_local0x2elist();

void ceammc_data_setup() {
     setup_data0x2efifo();
     setup_data0x2elist();
     setup_global0x2efloat();
     setup_global0x2eint();
     setup_global0x2ejson();
     setup_global0x2elist();
     setup_local0x2efloat();
     setup_local0x2eint();
     setup_local0x2ejson();
     setup_local0x2elist();
}
