#include <kmain.h>
#include <types.h>



void __attribute__((ms_abi)) kmain(KernelArgs a){
	for (uint64_t i=0;i<16777216;i+=4){
		*((char*)3221225472+i)=255;
		*((char*)3221225472+i+1)=170;
		*((char*)3221225472+i+2)=85;
	}
	for (uint64_t i=0;i<a->vmem_l;i++){
		*((char*)a->vmem+i)=255;
	}
}
