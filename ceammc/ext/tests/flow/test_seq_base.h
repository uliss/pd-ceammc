#ifndef TEST_SEQ_BASE_H_
#define TEST_SEQ_BASE_H_

#include "m_pd.h"

#include <type_traits>

// windows PureData delay time fix
static int operator""_wd(unsigned long long x)
{
#ifdef __WIN32
    return x + 1;
#else
    return x + std::is_same<double, t_float>::value;
#endif
}

#endif // TEST_SEQ_BASE_H_
