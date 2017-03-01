#include "x_ceammc.h"

size_t pd_ceammc_gensym_hash_size()
{
    size_t res = 0;
    t_symbol** table = pd_ceammc_gensym_hash_table();
    const size_t table_size = pd_ceammc_gensym_hash_table_size();

    for (size_t i = 0; i < table_size; i++) {
        t_symbol* sym = table[i];
        t_symbol* next = 0;

        if (sym == 0)
            continue;

        res++;

        while ((next = sym->s_next)) {
            sym = next;
            res++;
        }
    }

    return res;
}

size_t pd_ceammc_gensym_hash_max_chain()
{
    size_t res = 0;

    t_symbol** table = pd_ceammc_gensym_hash_table();
    const size_t table_size = pd_ceammc_gensym_hash_table_size();

    for (size_t i = 0; i < table_size; i++) {
        t_symbol* sym = table[i];
        t_symbol* next = 0;

        if (sym == 0)
            continue;

        size_t chain_len = 1;
        while ((next = sym->s_next)) {
            sym = next;
            chain_len++;
        }

        if (chain_len > res)
            res = chain_len;
    }

    return res;
}
