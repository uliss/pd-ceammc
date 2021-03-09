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
#include "ext_load_lib.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s OBJECT_NAME [ARGS...]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (!init_pd())
        return EXIT_FAILURE;

    if (!load_ceammc())
        return EXIT_FAILURE;

    if (list_props && list_props(argc - 1, argv + 1))
        return EXIT_SUCCESS;

    return EXIT_FAILURE;
}
