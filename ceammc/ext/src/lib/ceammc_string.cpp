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
#include "ceammc_string.h"
#include "utf8rewind/utf8rewind.h"

#include <boost/scoped_array.hpp>
#include <cstdlib>
#include <iostream>

size_t ceammc::string::utf8_strlen(const char* str)
{
    return utf8len(str);
}

std::string ceammc::string::utf8_to_upper(const char* str)
{
    size_t input_size = strlen(str);
    size_t converted_size = 0;
    int32_t errors = 0;

    if ((converted_size = utf8toupper(str, input_size, NULL, 0, UTF8_LOCALE_DEFAULT, &errors)) == 0
        || errors != UTF8_ERR_NONE) {
        return std::string();
    }

    boost::scoped_array<char> converted(new char[converted_size + 1]);

    if (utf8toupper(str, input_size, converted.get(), converted_size, UTF8_LOCALE_DEFAULT, &errors) == 0
        || errors != UTF8_ERR_NONE) {
        return std::string();
    }

    return std::string(converted.get(), converted_size);
}

std::string ceammc::string::utf8_to_lower(const char* str)
{
    size_t input_size = strlen(str);
    size_t converted_size = 0;
    int32_t errors = 0;

    if ((converted_size = utf8tolower(str, input_size, NULL, 0, UTF8_LOCALE_DEFAULT, &errors)) == 0
        || errors != UTF8_ERR_NONE) {
        return std::string();
    }

    boost::scoped_array<char> converted(new char[converted_size + 1]);

    if (utf8tolower(str, input_size, converted.get(), converted_size, UTF8_LOCALE_DEFAULT, &errors) == 0
        || errors != UTF8_ERR_NONE) {
        return std::string();
    }

    return std::string(converted.get(), converted_size);
}
