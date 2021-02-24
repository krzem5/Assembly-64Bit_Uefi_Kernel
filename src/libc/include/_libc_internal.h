#ifndef __LIBC_LIBC_INTERNAL_H__
#define __LIBC_LIBC_INTERNAL_H__ 1
#include <stdarg.h>
#include <stdint.h>



#define LIBC_CALL __attribute__((ms_abi))



typedef void (LIBC_CALL *__vprintf_reserve_func_t)(uint64_t sz,void* ctx);
typedef void (LIBC_CALL *__vprintf_write_func_t)(char c,void* ctx);



int LIBC_CALL __vprintf_buffer(char** bf,const char* t,va_list v);



int LIBC_CALL __vprintf_raw(void* ctx,__vprintf_reserve_func_t rs,__vprintf_write_func_t cb,const char* t,va_list v);



#endif
