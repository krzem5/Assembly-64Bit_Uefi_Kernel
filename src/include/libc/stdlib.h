#ifndef __LIBC_STDLIB_H__
#define __LIBC_STDLIB_H__
#include <libc/stdint.h>
#include <libc/stddef.h>



void* malloc(size_t sz);



void* calloc(size_t c,size_t sz);



void* realloc(void* p,size_t sz);



void free(void* p);



#endif
