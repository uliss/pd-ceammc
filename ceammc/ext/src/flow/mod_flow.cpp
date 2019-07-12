#include "mod_flow.h"

#include "flow_append.h"
#include "flow_change.h"
#include "flow_count.h"
#include "flow_demultiplex.h"
#include "flow_demultiplex2_tilde.h"
#include "flow_demultiplex_tilde.h"
#include "flow_gate.h"
#include "flow_group.h"
#include "flow_interval.h"
#include "flow_less.h"
#include "flow_less_eq.h"
#include "flow_multiplex.h"
#include "flow_multiplex2_tilde.h"
#include "flow_multiplex_tilde.h"
#include "flow_once.h"
#include "flow_pack.h"
#include "flow_pass.h"
#include "flow_pass_if.h"
#include "flow_reject.h"
#include "flow_reject_if.h"
#include "flow_route.h"
#include "flow_speedlim.h"
#include "flow_split.h"
#include "flow_sync.h"
#include "flow_tee_tilde.h"

void setup_flow_match();

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
    setup_flow0x2etee_tilde();

    setup_flow_append();
    setup_flow_count();
    setup_flow_demultiplex();
    setup_flow_demultiplex2_tilde();
    setup_flow_demultiplex_tilde();
    setup_flow_gate();
    setup_flow_interval();
    setup_flow_less();
    setup_flow_less_eq();
    setup_flow_match();
    setup_flow_multiplex();
    setup_flow_multiplex2_tilde();
    setup_flow_multiplex_tilde();
    setup_flow_once();
    setup_flow_pack();
    setup_flow_route();
    setup_flow_speedlim();
}
