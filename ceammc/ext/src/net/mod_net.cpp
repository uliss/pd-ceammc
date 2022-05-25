#include "mod_net.h"
#include "net_host.h"

extern void setup_net_osc();

void ceammc_net_setup()
{
    setup_net_host();
    setup_net_osc();
}
