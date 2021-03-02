/*****************************************************************************
 * Copyright 2021 Serge Poltavsky. All rights reserved.
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
#ifndef EXT_LOAD_LIB_H
#define EXT_LOAD_LIB_H

#include <cstdio>
#include <cstdlib>

using setup_fn = void (*)();

using list_objects_fn = void (*)(int);
extern list_objects_fn list_objects;

using list_methods_fn = bool (*)(int, char*[]);
extern list_methods_fn list_methods;

using dump_json_fn = bool (*)(int, char*[]);
extern dump_json_fn dump_json;

using list_props_fn = bool (*)(int, char*[]);
extern list_props_fn list_props;

bool load_ceammc();

#endif // EXT_LOAD_LIB_H
