#include <fatal_error.h>
#include <driver/console.h>



void fatal_error(char* s){
	console_error(s);
	asm_halt_cpu();
}



void fatal_error_line(char* s,const char* f,int l,const char* fn){
	console_error("%s: %u (%s): %s\n",f,l,fn,s);
	asm_halt_cpu();
}
