/*****************************************************************************
 *
 * Created: 17.10.2016 2016 by Serge Poltavski
 *
 * Copyright 2016 Serge Poltavski. All rights reserved.
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
#ifndef CEAMMC_H
#define CEAMMC_H

// MATH prefixes
// to get "math.abs" use CEAMMC_MATH_EXT("abs")
#define CEAMMC_MATH_EXT(name) "math." name

// to get extension setup function use CEAMMC_MATH_MODULE(round)
// it expands to: setup_math0x2eround()
#define CEAMMC_MATH_MODULE(name) setup_math0x2e ## name ()

#endif // CEAMMC_H
