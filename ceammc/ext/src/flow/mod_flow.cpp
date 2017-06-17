#include "mod_flow.h"

#include "flow_change.h"
#include "flow_group.h"
#include "flow_pass.h"
#include "flow_pass_if.h"
#include "flow_reject.h"
#include "flow_reject_if.h"
#include "flow_split.h"
#include "flow_sync.h"

void ceammc_flow_setup()
{
    setup_flow0x2echange();
    setup_flow0x2egroup();
    setup_flow0x2epass();
    setup_flow0x2epass_if();
    setup_flow0x2ereject();
    setup_flow0x2ereject_if();
    setup_flow0x2esplit();
    setup_flow0x2esync();
}
