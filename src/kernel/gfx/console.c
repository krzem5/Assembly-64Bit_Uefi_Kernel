#include <shared.h>
#include <_libc_internal.h>
#include <font_spleen.h>
#include <gfx/console.h>
#include <gfx/gfx.h>
#include <kmain.h>
#include <memory/vm.h>
#include <process/lock.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>



#define CHAR_AND_COLOR(c,cl) (((c)<<24)|(cl))
#define GET_CHAR(v) ((v)>>24)
#define GET_COLOR(v) ((v)&0xffffff)
#define DEFAULT_FONT FONT_SPLEEN
#define SCALE 1



uint64_t _console_x=0;
uint64_t _console_y=0;
uint64_t _console_w;
uint64_t _console_h;
uint32_t* _console_bf=NULL;
lock_t _console_l;



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD _console_print_char(char c,color_t cl,Font f){
	if (c=='\n'){
		_console_x=0;
		_console_y++;
		goto _check_y;
	}
	if (c!=' '){
		*(_console_bf+_console_y*_console_w+_console_x)=CHAR_AND_COLOR(c,cl);
		gfx_print_char(c,_console_x*SCALE*8,_console_y*SCALE*16,cl,f,SCALE);
	}
	_console_x++;
	if (_console_x==_console_w){
		_console_x=0;
		_console_y++;
		goto _check_y;
	}
	return;
_check_y:
	if (_console_y==_console_h){
		uint64_t i=0;
		while (i<_console_w*(_console_h-1)){
			*(_console_bf+i)=*(_console_bf+i+_console_w);
			if (GET_CHAR(*(_console_bf+i))!=' '){
				gfx_print_char(GET_CHAR(*(_console_bf+i)),(i%_console_w)*SCALE*8,(i/_console_w)*SCALE*16,GET_COLOR(*(_console_bf+i)),f,SCALE);
			}
			else{
				gfx_fill_rect((i%_console_w)*SCALE*8,(i/_console_w)*SCALE*16,SCALE*8,SCALE*16,COLOR(0,0,0));
			}
			i++;
		}
		while (i<_console_w*_console_h){
			*(_console_bf+i)=CHAR_AND_COLOR(' ',COLOR(0,0,0));
			i++;
		}
		_console_y--;
		gfx_fill_rect(0,_console_y*SCALE*16,_console_w*SCALE*8,SCALE*16,COLOR(0,0,0));
	}
}



void LIBC_CALL _console_vprintf_write_func(char c,void* ctx){
	if (c){
		_console_print_char(c,*((color_t*)ctx),DEFAULT_FONT);
	}
}



void KERNEL_CALL console_init(KernelArgs* ka){
	_console_w=ka->vmem_w/(8*SCALE);
	_console_h=ka->vmem_h/(16*SCALE);
	_console_bf=(uint32_t*)(void*)vm_commit((_console_w*_console_h*sizeof(uint32_t)+4095)>>12);
	_console_l=lock_create();
	for (uint64_t i=0;i<_console_w*_console_h;i++){
		*(_console_bf+i)=CHAR_AND_COLOR(' ',COLOR(0,0,0));
	}
}



void KERNEL_CALL _console_print(const char* s,color_t cl){
	if (!_console_bf){
		return;
	}
	asm_lock_acquire(_console_l);
	while (*s){
		_console_print_char(*s,cl,DEFAULT_FONT);
		s++;
	}
	asm_lock_release(_console_l);
}



void KERNEL_CALL _console_vprint(const char* s,color_t cl,...){
	if (!_console_bf){
		return;
	}
	asm_lock_acquire(_console_l);
	va_list v;
	va_start(v,cl);
	__vprintf_raw(&cl,NULL,_console_vprintf_write_func,s,v);
	va_end(v);
	asm_lock_release(_console_l);
}
