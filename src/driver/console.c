#include <driver/console.h>
#include <types.h>
#include <kmain.h>
#include <gfx.h>
#include <font_8x8.h>



#define DEFAULT_FONT FONT_8X8_FONT
#define SCALE 2



uint64_t _console_x;
uint64_t _console_y;
uint64_t _console_w;
uint64_t _console_h;



void _print_char(char c,color_t cl,uint64_t f[]){
	if (c=='\n'){
		_console_x=0;
		_console_y++;
		return;
	}
	if (c!=' '){
		gfx_print_char(c,_console_x*SCALE*8,_console_y*SCALE*8,cl,f,SCALE);
	}
	_console_x++;
	if (_console_x==_console_w){
		_console_x=0;
		_console_y++;
	}
}



void console_init(KernelArgs ka){
	_console_x=0;
	_console_y=0;
	_console_w=ka.vmem_w/(8*SCALE);
	_console_h=ka.vmem_h/(8*SCALE);
}



void console_print(char* s,enum CONSOLE_PRINT_LEVEL l){
	if (l==CONSOLE_PRINT_LEVEL_CONTINUE){
		_console_x+=8;
	}
	else{
		_print_char('[',CONSOLE_PRINT_LEVEL_COLOR,DEFAULT_FONT);
		if (l==CONSOLE_PRINT_LEVEL_OK){
			_print_char('o',CONSOLE_PRINT_LEVEL_OK_COLOR,DEFAULT_FONT);
			_print_char('k',CONSOLE_PRINT_LEVEL_OK_COLOR,DEFAULT_FONT);
			_console_x+=3;
		}
		else if (l==CONSOLE_PRINT_LEVEL_WARN){
			_print_char('w',CONSOLE_PRINT_LEVEL_WARN_COLOR,DEFAULT_FONT);
			_print_char('a',CONSOLE_PRINT_LEVEL_WARN_COLOR,DEFAULT_FONT);
			_print_char('r',CONSOLE_PRINT_LEVEL_WARN_COLOR,DEFAULT_FONT);
			_print_char('n',CONSOLE_PRINT_LEVEL_WARN_COLOR,DEFAULT_FONT);
			_console_x++;
		}
		else if (l==CONSOLE_PRINT_LEVEL_ERROR){
			_print_char('e',CONSOLE_PRINT_LEVEL_ERROR_COLOR,DEFAULT_FONT);
			_print_char('r',CONSOLE_PRINT_LEVEL_ERROR_COLOR,DEFAULT_FONT);
			_print_char('r',CONSOLE_PRINT_LEVEL_ERROR_COLOR,DEFAULT_FONT);
			_print_char('o',CONSOLE_PRINT_LEVEL_ERROR_COLOR,DEFAULT_FONT);
			_print_char('r',CONSOLE_PRINT_LEVEL_ERROR_COLOR,DEFAULT_FONT);
		}
		else{
			_print_char('l',CONSOLE_PRINT_LEVEL_LOG_COLOR,DEFAULT_FONT);
			_print_char('o',CONSOLE_PRINT_LEVEL_LOG_COLOR,DEFAULT_FONT);
			_print_char('g',CONSOLE_PRINT_LEVEL_LOG_COLOR,DEFAULT_FONT);
			_console_x+=2;
		}
		_print_char(']',CONSOLE_PRINT_LEVEL_COLOR,DEFAULT_FONT);
		_console_x++;
	}
	while (*s){
		if (*s=='\n'){
			_console_x=8;
			_console_y++;
			s++;
			continue;
		}
		_print_char(*s,COLOR(255,255,255),DEFAULT_FONT);
		s++;
	}
	_console_x=0;
	_console_y++;
}
