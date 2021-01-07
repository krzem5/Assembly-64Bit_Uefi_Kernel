#ifndef __LIBC_LIBC_INTERNAL_H__
#define __LIBC_LIBC_INTERNAL_H__ 1
#include <libc/stdarg.h>
#include <libc/stdint.h>



typedef void* (*__set_pg_acquire_func_t)(uint64_t c);
typedef void (*__set_pg_release_func_t)(void* p,uint64_t c);
typedef void (*__vprintf_reserve_func_t)(uint64_t sz,void* ctx);
typedef void (*__vprintf_write_func_t)(char c,void* ctx);



void __set_pg_func(__set_pg_acquire_func_t af,__set_pg_release_func_t rf);



void __printf_set_console_func(__vprintf_reserve_func_t rs,__vprintf_write_func_t cb);



int __vprintf_buffer(char** bf,const char* t,va_list v);



int __vprintf_raw(void* ctx,__vprintf_reserve_func_t rs,__vprintf_write_func_t cb,const char* t,va_list v);



#endif
