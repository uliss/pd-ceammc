/*****************************************************************************
 * Copyright 2018 Serge Poltavsky. All rights reserved.
 *
 * This file may be distributed under the terms of GNU Public License version
 * 3 (GPL v3) as defined by the Free Software Foundation (FSF). A copy of the
 * license should have been included with this file, or the project in which
 * this file belongs to. You may also find the details of GPL v3 at:
 * http://www.gnu.org/licenses/gpl-3.0.txt
 *
 * If you have any questions regarding the use of this file, feel free to
 * contact the author of this file, or the owner of the project in which
 * this file belongs to.
 *****************************************************************************/
#ifndef DICT_PARSER_IMPL_H
#define DICT_PARSER_IMPL_H

#include "m_pd.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _dict;
typedef struct _dict t_dict;

t_dict* dict_new();
void dict_free(t_dict* dict);

void dict_dump(t_dict* dict);
void dict_clear(t_dict* dict);

void dict_push_to_list(t_dict* d, t_symbol* s);
void dict_store(t_dict* d, int n);
void dict_insert_pair_list(t_dict* d, t_symbol* key);
void dict_insert_pair_dict(t_dict* d, t_symbol* key);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace ceammc {
class DataTypeDict;
DataTypeDict& dict_get(t_dict* dict);
}
#endif

#endif // DICT_PARSER_IMPL_H
