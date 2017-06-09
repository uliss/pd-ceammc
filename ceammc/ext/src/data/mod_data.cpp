#include "mod_data.h"
#include "data_set.h"
#include "set_contains.h"
#include "set_difference.h"
#include "set_equal.h"
#include "set_intersection.h"
#include "set_size.h"
#include "set_symdiff.h"
#include "set_to_list.h"
#include "set_union.h"

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

void ceammc_data_setup()
{
    setup_data0x2efifo();
    setup_data0x2elist();
    setup_data0x2eset();
    setup_global0x2efloat();
    setup_global0x2eint();
    setup_global0x2ejson();
    setup_global0x2elist();
    setup_local0x2efloat();
    setup_local0x2eint();
    setup_local0x2ejson();
    setup_local0x2elist();
    setup_set0x2econtains();
    setup_set0x2edifference();
    setup_set0x2eequal();
    setup_set0x2eintersection();
    setup_set0x2elist();
    setup_set0x2esize();
    setup_set0x2eunion();
    setup_set0x2esymdiff();
}
