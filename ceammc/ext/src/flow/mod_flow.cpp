#include "mod_flow.h"

#include "flow_append.h"
#include "flow_change.h"
#include "flow_count.h"
#include "flow_demultiplex.h"
#include "flow_demultiplex2_tilde.h"
#include "flow_demultiplex_tilde.h"
#include "flow_dup.h"
#include "flow_gate.h"
#include "flow_greater.h"
#include "flow_greater_eq.h"
#include "flow_group.h"
#include "flow_interval.h"
#include "flow_less.h"
#include "flow_less_eq.h"
#include "flow_loop.h"
#include "flow_multiplex.h"
#include "flow_multiplex2_tilde.h"
#include "flow_multiplex_tilde.h"
#include "flow_once.h"
#include "flow_pack.h"
#include "flow_pass.h"
#include "flow_pass_if.h"
#include "flow_prepend.h"
#include "flow_reject.h"
#include "flow_reject_if.h"
#include "flow_route.h"
#include "flow_speedlim.h"
#include "flow_split.h"
#include "flow_stack.h"
#include "flow_sync.h"
#include "flow_sync_pack.h"
#include "flow_tee_tilde.h"

void setup_flow_match();
void setup_flow_delay();

void ceammc_flow_setup()
{
    setup_flow_append();
    setup_flow_change();
    setup_flow_count();
    setup_flow_delay();
    setup_flow_demultiplex();
    setup_flow_demultiplex2_tilde();
    setup_flow_demultiplex_tilde();
    setup_flow_dup();
    setup_flow_gate();
    setup_flow_greater();
    setup_flow_greater_eq();
    setup_flow_group();
    setup_flow_interval();
    setup_flow_less();
    setup_flow_less_eq();
    setup_flow_loop();
    setup_flow_match();
    setup_flow_multiplex();
    setup_flow_multiplex2_tilde();
    setup_flow_multiplex_tilde();
    setup_flow_once();
    setup_flow_pack();
    setup_flow_pass();
    setup_flow_pass_if();
    setup_flow_prepend();
    setup_flow_reject();
    setup_flow_reject_if();
    setup_flow_route();
    setup_flow_speedlim();
    setup_flow_split();
    setup_flow_stack();
    setup_flow_sync();
    setup_flow_sync_pack();
    setup_flow_tee_tilde();
}
