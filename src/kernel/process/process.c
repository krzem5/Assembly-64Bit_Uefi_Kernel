#include <shared.h>
#include <cpu/fatal_error.h>
#include <gfx/console.h>
#include <memory/vm.h>
#include <process/process.h>
#include <process/thread.h>
#include <stdint.h>
#include <stdlib.h>



#define PAGE_4KB_POWER_OF_2 12
#define PROCESS_4KB_PAGES_COUNT 64
#define MAX_PROCESS_ID ((PROCESS_4KB_PAGES_COUNT<<PAGE_4KB_POWER_OF_2)/sizeof(process_t)-1)
#define PROCESS_GET_PRESENT(p) ((p)->f&1)
#define PROCESS_GET_PRIORITY(p) ((p)->f>>1)
#define PROCESS_SET_PRESENT 1
#define PROCESS_SET_PRIORITY(p) ((p)<<1)



process_t* pl;
uint32_t pll=0;
uint32_t _npi=0;



void KERNEL_CALL process_init(void){
	pl=(process_t*)(void*)vm_commit(PROCESS_4KB_PAGES_COUNT);
	for (uint32_t i=0;i<MAX_PROCESS_ID+1;i++){
		(pl+i)->f=0;
		(pl+i)->id=i;
	}
	console_log("Max Thread ID: %llu\n",MAX_PROCESS_ID);
}



process_t* KERNEL_CALL process_create(uint8_t p){
	process_t* o=pl+_npi;
	o->f|=PROCESS_SET_PRESENT|PROCESS_SET_PRIORITY(p);
	_npi++;
	while (PROCESS_GET_PRESENT(pl+_npi)){
		_npi++;
		if (_npi>MAX_PROCESS_ID){
			fatal_error("Final Process Allocated!\n");
		}
	}
	return o;
}
