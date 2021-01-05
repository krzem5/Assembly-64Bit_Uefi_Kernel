#include <libc/stdio.h>
#include <libc/_libc_internal.h>
#include <libc/stdint.h>
#include <libc/stddef.h>
#include <libc/stdarg.h>
#include <libc/stdlib.h>



struct __VPRINTF_BUFFER_CTX{
	char** bf;
	uint64_t sz;
	uint64_t i;
};



void __vprintf_buffer_reserve_func(uint64_t sz,void* ctx){
	((struct __VPRINTF_BUFFER_CTX*)ctx)->sz+=sz;
	*((struct __VPRINTF_BUFFER_CTX*)ctx)->bf=realloc(*((struct __VPRINTF_BUFFER_CTX*)ctx)->bf,((struct __VPRINTF_BUFFER_CTX*)ctx)->sz);
}



void __vprintf_buffer_write_func(char c,void* ctx){
	*(*((struct __VPRINTF_BUFFER_CTX*)ctx)->bf+((struct __VPRINTF_BUFFER_CTX*)ctx)->i)=c;
	((struct __VPRINTF_BUFFER_CTX*)ctx)->i++;
}



int __vprintf_buffer(char** bf,const char* t,va_list v){
	*bf=NULL;
	struct __VPRINTF_BUFFER_CTX ctx={
		bf,
		0,
		0
	};
	return __vprintf_raw((void*)&ctx,__vprintf_buffer_reserve_func,__vprintf_buffer_write_func,t,v);
}



int __vprintf_raw(void* ctx,__vprintf_reserve_func_t rs,__vprintf_write_func_t cb,const char* t,va_list v){
	size_t i=0;
	while (*t){
		rs(1,ctx);
		cb(*t,ctx);
		i++;
	}
	rs(1,ctx);
	cb(0,ctx);
	return i+1;
}
