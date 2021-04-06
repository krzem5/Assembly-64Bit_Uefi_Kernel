#ifndef __LIBC_STDIO_H__
#define __LIBC_STDIO_H__ 1
#include <_libc_internal.h>



#ifdef __KERNEL__
#define printf(t,...) console_log(t,__VA_ARGS__)
#else
int LIBC_CALL printf(const char* t,...);
#endif



#endif
