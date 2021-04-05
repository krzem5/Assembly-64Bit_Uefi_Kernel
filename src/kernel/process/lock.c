#include <shared.h>
#include <cpu/fatal_error.h>
#include <gfx/console.h>
#include <memory/paging.h>
#include <memory/vm.h>
#include <process/lock.h>
#include <stdint.h>



#define LOCK_4KB_PAGES_COUNT 64
#define LOCK_COUNT ((LOCK_4KB_PAGES_COUNT<<PAGE_4KB_POWER_OF_2)/sizeof(lock_t))
#define LOCK_GET_PRESENT(p) ((p)->f&1)
#define LOCK_GET_PRIORITY(p) ((p)->f>>1)
#define LOCK_SET_PRESENT 1
#define LOCK_SET_PRIORITY(p) ((p)<<1)



lock_t _sl;
uint32_t _nsi=0;



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD lock_init(void){
	_sl=(lock_t)(void*)vm_commit(LOCK_4KB_PAGES_COUNT);
	for (uint32_t i=0;i<LOCK_COUNT;i++){
		*(_sl+i)=UINT8_MAX;
	}
	console_log("Lock List Len: %llu\n",LOCK_COUNT);
}



lock_t KERNEL_CALL lock_create(void){
	lock_t o=_sl+_nsi;
	*o=0;
	_nsi++;
	while (*(_sl+_nsi)!=UINT8_MAX){
		_nsi++;
		if (_nsi>=LOCK_COUNT){
			fatal_error("Final Lock Allocated!\n");
		}
	}
	return o;
}
