#include "mod_flow.h"

#include "flow_change.h"
#include "flow_count.h"
#include "flow_demultiplex.h"
#include "flow_demultiplex2_tilde.h"
#include "flow_demultiplex_tilde.h"
#include "flow_dollar.h"
#include "flow_dup.h"
#include "flow_gate.h"
#include "flow_greater.h"
#include "flow_greater_eq.h"
#include "flow_group.h"
#include "flow_interval.h"
#include "flow_less.h"
#include "flow_less_eq.h"
#include "flow_list2many.h"
#include "flow_loop.h"
#include "flow_mem.h"
#include "flow_multiplex.h"
#include "flow_multiplex2_tilde.h"
#include "flow_multiplex_tilde.h"
#include "flow_once.h"
#include "flow_pack.h"
#include "flow_pass.h"
#include "flow_pass_float.h"
#include "flow_pass_if.h"
#include "flow_queue.h"
#include "flow_reject.h"
#include "flow_reject_if.h"
#include "flow_ring.h"
#include "flow_route.h"
#include "flow_select.h"
#include "flow_speedlim.h"
#include "flow_split.h"
#include "flow_stack.h"
#include "flow_sync.h"
#include "flow_sync_pack.h"
#include "flow_tee_tilde.h"

void setup_flow_append();
void setup_flow_delay();
void setup_flow_match();
void setup_flow_pipe();
void setup_flow_space();

void setup_seq_arp();
void setup_seq_bangs();
void setup_seq_counter();
void setup_seq_matrix();
void setup_seq_nbangs();
void setup_seq_phasor();
void setup_seq_sequencer();
void setup_seq_toggles();
void setup_seq_life();

void setup_route_float();

void ceammc_flow_setup()
{
    setup_flow_append();
    setup_flow_change();
    setup_flow_count();
    setup_flow_delay();
    setup_flow_demultiplex();
    setup_flow_demultiplex2_tilde();
    setup_flow_demultiplex_tilde();
    setup_flow_dollar();
    setup_flow_dup();
    setup_flow_gate();
    setup_flow_greater();
    setup_flow_greater_eq();
    setup_flow_group();
    setup_flow_interval();
    setup_flow_less();
    setup_flow_less_eq();
    setup_flow_list2many();
    setup_flow_ring();
    setup_flow_loop();
    setup_flow_match();
    setup_flow_mem();
    setup_flow_multiplex();
    setup_flow_multiplex2_tilde();
    setup_flow_multiplex_tilde();
    setup_flow_once();
    setup_flow_pack();
    setup_flow_pass();
    setup_flow_pass_float();
    setup_flow_pass_if();
    setup_flow_pipe();
    setup_flow_queue();
    setup_flow_reject();
    setup_flow_reject_if();
    setup_flow_route();
    setup_flow_select();
    setup_flow_space();
    setup_flow_speedlim();
    setup_flow_split();
    setup_flow_stack();
    setup_flow_sync();
    setup_flow_sync_pack();
    setup_flow_tee_tilde();

    setup_seq_arp();
    setup_seq_bangs();
    setup_seq_counter();
    setup_seq_matrix();
    setup_seq_nbangs();
    setup_seq_phasor();
    setup_seq_sequencer();
    setup_seq_toggles();
    setup_seq_life();

    setup_route_float();
}
