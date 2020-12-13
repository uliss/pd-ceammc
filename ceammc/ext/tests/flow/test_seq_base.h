#ifndef TEST_SEQ_BASE_H_
#define TEST_SEQ_BASE_H_

// windows PureData delay time fix
static int operator""_wd(unsigned long long x) {
#ifdef __WIN32
     return x + 1;
#else
     return x;
#endif
 }

#endif // TEST_SEQ_BASE_H_
