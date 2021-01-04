#ifndef __KERNEL_DRIVER_CONSOLE_H__
#define __KERNEL_DRIVER_CONSOLE_H__
#include <types.h>
#include <kmain.h>
#include <gfx.h>



#define CONSOLE_PRINT_LEVEL_COLOR COLOR(50,50,50)
#define CONSOLE_PRINT_LEVEL_OK_COLOR COLOR(109,170,45)
#define CONSOLE_PRINT_LEVEL_LOG_COLOR COLOR(230,230,230)
#define CONSOLE_PRINT_LEVEL_WARN_COLOR COLOR(227,191,27)
#define CONSOLE_PRINT_LEVEL_ERROR_COLOR COLOR(220,35,21)



#define console_ok(s,...) console_print(s,CONSOLE_PRINT_LEVEL_OK, ## __VA_ARGS__)
#define console_log(s,...) console_print(s,CONSOLE_PRINT_LEVEL_LOG, ## __VA_ARGS__)
#define console_warn(s,...) console_print(s,CONSOLE_PRINT_LEVEL_WARN, ## __VA_ARGS__)
#define console_error(s,...) console_print(s,CONSOLE_PRINT_LEVEL_ERROR, ## __VA_ARGS__)
#define console_continue(s,...) console_print(s,CONSOLE_PRINT_LEVEL_CONTINUE, ## __VA_ARGS__)



enum CONSOLE_PRINT_LEVEL{
	CONSOLE_PRINT_LEVEL_OK=0x00,
	CONSOLE_PRINT_LEVEL_LOG=0x01,
	CONSOLE_PRINT_LEVEL_WARN=0x02,
	CONSOLE_PRINT_LEVEL_ERROR=0x03,
	CONSOLE_PRINT_LEVEL_CONTINUE=0xff
};



void console_init(KernelArgs ka);



void console_print(char* s,enum CONSOLE_PRINT_LEVEL l);



#endif
