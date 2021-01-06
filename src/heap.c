#include <heap.h>
#include <libc/stdint.h>
#include <libc/stddef.h>
#include <kmain.h>
#include <libc/_libc_internal.h>



struct __PAGE_TABLE_ENTRY{
	void* pg;
} _pg_h={
	NULL
};



void init_heap(KernelArgs* ka){
	__set_pg_func(NULL,NULL);
	(void)ka;
}
