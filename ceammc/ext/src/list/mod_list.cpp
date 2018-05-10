#include "mod_list.h"
#include "list_route.h"

extern "C" void setup_list0x2eall_of();
extern "C" void setup_list0x2eany_of();
extern "C" void setup_list0x2eapply_to();
extern "C" void setup_list0x2eat();
extern "C" void setup_list0x2echoice();
extern "C" void setup_list0x2ecount();
extern "C" void setup_list0x2ecount_if();
extern "C" void setup_list0x2edeinterleave();
extern "C" void setup_list0x2edelta();
extern "C" void setup_list0x2edistribution();
extern "C" void setup_list0x2edo();
extern "C" void setup_list0x2eeach();
extern "C" void setup_list0x2eenumerate();
extern "C" void setup_list0x2eequal();
extern "C" void setup_list0x2efirst();
extern "C" void setup_list0x2egen();
extern "C" void setup_list0x2ehistogram();
extern "C" void setup_list0x2eintegrator();
extern "C" void setup_list0x2einterleave();
extern "C" void setup_list0x2elast();
extern "C" void setup_list0x2elength();
extern "C" void setup_list0x2emax();
extern "C" void setup_list0x2emean();
extern "C" void setup_list0x2emin();
extern "C" void setup_list0x2enone_of();
extern "C" void setup_list0x2enormalize();
extern "C" void setup_list0x2epass_if();
extern "C" void setup_list0x2eproduct();
extern "C" void setup_list0x2erange();
extern "C" void setup_list0x2eremove_if();
extern "C" void setup_list0x2erepack();
extern "C" void setup_list0x2erepeat();
extern "C" void setup_list0x2eresize();
extern "C" void setup_list0x2ereverse();
extern "C" void setup_list0x2erotate();
extern "C" void setup_list0x2eseparate();
extern "C" void setup_list0x2eseq();
extern "C" void setup_list0x2eshift();
extern "C" void setup_list0x2eshuffle();
extern "C" void setup_list0x2eslice();
extern "C" void setup_list0x2esort();
extern "C" void setup_list0x2esort_with();
extern "C" void setup_list0x2esplit();
extern "C" void setup_list0x2estretch();
extern "C" void setup_list0x2esum();
extern "C" void setup_list0x2eunique();
extern "C" void setup_list0x2eunpack();
extern "C" void setup_list0x2eunwrap();
extern "C" void setup_list0x2ewalk();
extern "C" void setup_list0x2ewrap();
extern "C" void setup_list0x2ezip();

void setup_list_append();
void setup_list_contains();
void setup_list_index();
void setup_list_prepend();
void setup_list_reduce();
void setup_list_search();

void ceammc_list_setup()
{
    setup_list0x2eall_of();
    setup_list0x2eany_of();
    setup_list0x2eapply_to();
    setup_list0x2eat();
    setup_list0x2echoice();
    setup_list0x2ecount();
    setup_list0x2ecount_if();
    setup_list0x2edeinterleave();
    setup_list0x2edelta();
    setup_list0x2edistribution();
    setup_list0x2edo();
    setup_list0x2eeach();
    setup_list0x2eenumerate();
    setup_list0x2eequal();
    setup_list0x2efirst();
    setup_list0x2egen();
    setup_list0x2ehistogram();
    setup_list0x2eintegrator();
    setup_list0x2einterleave();
    setup_list0x2elast();
    setup_list0x2elength();
    setup_list0x2emax();
    setup_list0x2emean();
    setup_list0x2emin();
    setup_list0x2enone_of();
    setup_list0x2enormalize();
    setup_list0x2epass_if();
    setup_list0x2eproduct();
    setup_list0x2erange();
    setup_list0x2eremove_if();
    setup_list0x2erepack();
    setup_list0x2erepeat();
    setup_list0x2eresize();
    setup_list0x2ereverse();
    setup_list0x2erotate();
    setup_list0x2eseq();
    setup_list0x2eseparate();
    setup_list0x2eshift();
    setup_list0x2eshuffle();
    setup_list0x2eslice();
    setup_list0x2esort();
    setup_list0x2esort_with();
    setup_list0x2esplit();
    setup_list0x2estretch();
    setup_list0x2esum();
    setup_list0x2eunique();
    setup_list0x2eunpack();
    setup_list0x2eunwrap();
    setup_list0x2ewalk();
    setup_list0x2ewrap();
    setup_list0x2ezip();

    setup_list_append();
    setup_list_contains();
    setup_list_index();
    setup_list_prepend();
    setup_list_reduce();
    setup_list_route();
    setup_list_search();
}
