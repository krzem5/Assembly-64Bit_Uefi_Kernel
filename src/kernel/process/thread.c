#include <shared.h>
#include <process/process.h>
#include <process/thread.h>



tid_t KERNEL_CALL create_thread(thread_start_t a,uint8_t p,void* arg){
	process_t* o=process_create(p);
	o->dt.rip=(uint64_t)a;
	o->dt.rcx=(uint64_t)arg;
	return (tid_t)o->id;
}
