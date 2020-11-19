#include "mod_proto.h"
#include "proto_firmata.h"
#include "proto_hui.h"
#include "proto_sp_alpaca.h"

void ceammc_proto_setup()
{
    proto_firmata_setup();
    setup_proto_hui();
    setup_proto_sp_alpaca();
}
