#include "mod_data.h"
#include "data_dict.h"
#include "data_fifo.h"
#include "data_float.h"
#include "data_int.h"
#include "data_list.h"
#include "data_mlist.h"
#include "data_set.h"
#include "dict_contains.h"
#include "dict_each.h"
#include "dict_from_list.h"
#include "dict_get.h"
#include "dict_is.h"
#include "dict_keys.h"
#include "dict_pass.h"
#include "dict_reject.h"
#include "dict_size.h"
#include "dict_to_list.h"
#include "dict_values.h"
#include "global_dict.h"
#include "global_float.h"
#include "global_int.h"
#include "global_list.h"
#include "global_mlist.h"
#include "global_set.h"
#include "local_dict.h"
#include "local_float.h"
#include "local_int.h"
#include "local_list.h"
#include "local_mlist.h"
#include "local_set.h"
#include "mlist_flatten.h"
#include "set_contains.h"
#include "set_difference.h"
#include "set_equal.h"
#include "set_intersection.h"
#include "set_size.h"
#include "set_symdiff.h"
#include "set_to_list.h"
#include "set_union.h"

void setup_color_mix();
void setup_color_scale();
void setup_data_bimap();
void setup_data_color();
void setup_data_copy();
void setup_global_bimap();
void setup_local_bimap();
void setup_rtree_to_list();

void ceammc_data_setup()
{
    setup_set_contains();
    setup_set_difference();
    setup_set_equal();
    setup_set_intersection();
    setup_set_list();
    setup_set_size();
    setup_set_union();
    setup_set_symdiff();

    setup_color_mix();
    setup_color_scale();

    setup_data_bimap();
    setup_data_color();
    setup_data_copy();
    setup_data_dict();
    setup_data_fifo();
    setup_data_float();
    setup_data_int();
    setup_data_list();
    setup_data_mlist();
    setup_data_set();

    setup_dict_contains();
    setup_dict_each();
    setup_dict_from_list();
    setup_dict_get();
    setup_dict_keys();
    setup_dict_pass();
    setup_dict_reject();
    setup_dict_size();
    setup_dict_to_list();
    setup_dict_values();
    setup_is_dict();

    setup_local_bimap();
    setup_local_dict();
    setup_local_float();
    setup_local_int();
    setup_local_list();
    setup_local_mlist();
    setup_local_set();

    setup_global_bimap();
    setup_global_dict();
    setup_global_float();
    setup_global_int();
    setup_global_list();
    setup_global_mlist();
    setup_global_set();

    setup_mlist_flatten();
    setup_rtree_to_list();
}
