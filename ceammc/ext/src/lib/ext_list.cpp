/*****************************************************************************
 * Copyright 2017 Serge Poltavsky. All rights reserved.
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

#include <iostream>
#include <string>

extern "C" void pd_init();

int main(int argc, char* argv[])
{
    bool vanilla = false;
    bool with_aliases = false;

    if (argc > 1) {
        if (std::string(argv[1]) == "-v") {
            vanilla = true;
        } else {
            std::cerr << "usage: " << argv[0] << " [-v]"
                      << "\n\t '-v': print vanilla objects" << std::endl;
            return EXIT_FAILURE;
        }
    }

    pd_init();

    if (!load_ceammc())
        return EXIT_FAILURE;

    if (list_objects)
        list_objects(vanilla);

    return EXIT_SUCCESS;
}
