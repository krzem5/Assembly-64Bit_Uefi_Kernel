#ifndef __LIBC_STDLIB_H__
#define __LIBC_STDLIB_H__
#include <_libc_internal.h>
#include <stddef.h>
#include <stdint.h>



void* LIBC_CALL malloc(size_t sz);



void* LIBC_CALL calloc(size_t c,size_t sz);



void* LIBC_CALL realloc(void* p,size_t sz);



void LIBC_CALL free(void* p);



#endif
