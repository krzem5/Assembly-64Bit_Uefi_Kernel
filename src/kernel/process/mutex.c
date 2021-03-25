#include <shared.h>
#include <cpu/fatal_error.h>
#include <gfx/console.h>
#include <memory/paging.h>
#include <memory/vm.h>
#include <process/mutex.h>
#include <stdint.h>



#define MUTEX_4KB_PAGES_COUNT 64
#define MUTEX_COUNT ((MUTEX_4KB_PAGES_COUNT<<PAGE_4KB_POWER_OF_2)/sizeof(mutex_t))
#define MUTEX_GET_PRESENT(p) ((p)->f&1)
#define MUTEX_GET_PRIORITY(p) ((p)->f>>1)
#define MUTEX_SET_PRESENT 1
#define MUTEX_SET_PRIORITY(p) ((p)<<1)



mutex_t sl;
uint32_t sll=0;
uint32_t _nsi=0;



void KERNEL_CALL mutex_init(void){
	sl=(mutex_t)(void*)vm_commit(MUTEX_4KB_PAGES_COUNT);
	for (uint32_t i=0;i<MUTEX_COUNT;i++){
		*(sl+i)=UINT8_MAX;
	}
	console_log("Mutex List Len: %llu\n",MUTEX_COUNT);
}



mutex_t KERNEL_CALL mutex_create(void){
	mutex_t o=sl+_nsi;
	*o=0;
	_nsi++;
	while (*(sl+_nsi)!=UINT8_MAX){
		_nsi++;
		if (_nsi>=MUTEX_COUNT){
			fatal_error("Final Mutex Allocated!\n");
		}
	}
	return o;
}
