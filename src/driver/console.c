#include <driver/console.h>
#include <libc/stdint.h>
#include <font.h>
#include <kmain.h>
#include <gfx.h>
#include <libc/stdio.h>
#include <libc/stdarg.h>
#include <libc/_libc_internal.h>
#include <libc/stdlib.h>
#include <font_8x16.h>



#define DEFAULT_FONT FONT_8X16_FONT
#define SCALE 1



uint64_t _console_x;
uint64_t _console_y;
uint64_t _console_w;
uint64_t _console_h;



void _console_print_char(char c,color_t cl,Font f){
	if (c=='\n'){
		_console_x=0;
		_console_y++;
		return;
	}
	if (c!=' '){
		gfx_print_char(c,_console_x*SCALE*8,_console_y*SCALE*16,cl,f,SCALE);
	}
	_console_x++;
	if (_console_x==_console_w){
		_console_x=0;
		_console_y++;
	}
}



void _console_vprintf_write_func(char c,void* ctx){
	if (c){
		_console_print_char(c,*((color_t*)ctx),DEFAULT_FONT);
	}
}



void console_init(KernelArgs* ka){
	_console_x=0;
	_console_y=0;
	_console_w=ka->vmem_w/(8*SCALE);
	_console_h=ka->vmem_h/(16*SCALE);
}



void _console_print(const char* s,color_t cl){
	while (*s){
		_console_print_char(*s,cl,DEFAULT_FONT);
		s++;
	}
}



void _console_vprint(const char* s,color_t cl,...){
	va_list v;
	va_start(v,cl);
	__vprintf_raw(&cl,NULL,_console_vprintf_write_func,s,v);
	va_end(v);
}
