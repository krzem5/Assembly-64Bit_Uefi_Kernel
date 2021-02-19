#ifndef __LIBC_STDLIB_H__
#define __LIBC_STDLIB_H__
#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/_libc_internal.h>



void* LIBC_CALL malloc(size_t sz);



void* LIBC_CALL calloc(size_t c,size_t sz);



void* LIBC_CALL realloc(void* p,size_t sz);



void LIBC_CALL free(void* p);



#endif
