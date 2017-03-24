/*****************************************************************************
 *
 * Created: 17.10.2017 by Serge Poltavski
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
#ifndef CEAMMC_ARRAY_LOAD_PATTERN_H
#define CEAMMC_ARRAY_LOAD_PATTERN_H

#include <string>
#include <vector>

/**
 * @brief array_load_parse - parses given string and returns array of indexes
 * @param str - string to parse
 * @param count - pointer to index count
 * @param vec - pointer to index array
 * @return true on success, false on error
 * @example "1,2,30" - returns pointer to array of three integers: 1, 2 and 30
 * @example "1-20,7"
 */
bool array_load_parse(const std::string& str, size_t* count, std::string** vec);

std::vector<std::string> array_pattern_names(const std::string& pattern);

#endif // CEAMMC_ARRAY_LOAD_PATTERN_H
