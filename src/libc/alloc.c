#include <libc/stdlib.h>
#include <libc/_libc_internal.h>
#include <libc/stdint.h>



__set_pg_acquire_func_t _libc_pg_af;
__set_pg_release_func_t _libc_pg_rf;



void* malloc(size_t sz);



void* calloc(size_t c,size_t sz);



void* realloc(void* p,size_t sz);



void free(void* p);



void __set_pg_func(__set_pg_acquire_func_t af,__set_pg_release_func_t rf){
	af=af;
	rf=rf;
}
