#include <shared.h>
#include <driver/console.h>
#include <fatal_error.h>



void KERNEL_CALL fatal_error(char* s){
	console_error(s);
	asm_halt_cpu();
}



void KERNEL_CALL fatal_error_line(char* s,const char* f,int l,const char* fn,char* p){
	console_error("%s: %u (%s): %s: %s\n",f,l,fn,p,s);
	asm_halt_cpu();
}
