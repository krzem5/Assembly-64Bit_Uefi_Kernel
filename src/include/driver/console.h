#ifndef __KERNEL_DRIVER_CONSOLE_H__
#define __KERNEL_DRIVER_CONSOLE_H__ 1
#include <libc/stdint.h>
#include <kmain.h>
#include <gfx.h>



#define CONSOLE_PRINT_LEVEL_COLOR COLOR(50,50,50)
#define CONSOLE_PRINT_LEVEL_OK_COLOR COLOR(109,170,45)
#define CONSOLE_PRINT_LEVEL_LOG_COLOR COLOR(230,230,230)
#define CONSOLE_PRINT_LEVEL_WARN_COLOR COLOR(227,191,27)
#define CONSOLE_PRINT_LEVEL_ERROR_COLOR COLOR(220,35,21)



#define console_ok(s,...) _CONSOLE_CHECK_ARGS(s,CONSOLE_PRINT_LEVEL_OK, ## __VA_ARGS__)
#define console_log(s,...) _CONSOLE_CHECK_ARGS(s,CONSOLE_PRINT_LEVEL_LOG, ## __VA_ARGS__)
#define console_warn(s,...) _CONSOLE_CHECK_ARGS(s,CONSOLE_PRINT_LEVEL_WARN, ## __VA_ARGS__)
#define console_error(s,...) _CONSOLE_CHECK_ARGS(s,CONSOLE_PRINT_LEVEL_ERROR, ## __VA_ARGS__)
#define console_continue(s,...) _CONSOLE_CHECK_ARGS(s,CONSOLE_PRINT_LEVEL_CONTINUE, ## __VA_ARGS__)
#define _CONSOLE_CHECK_ARGS_CONCAT2(a,b) a ## b
#define _CONSOLE_CHECK_ARGS_CONCAT(a,b) _CONSOLE_CHECK_ARGS_CONCAT2(a,b)
#define _CONSOLE_CHECK_ARGS_COUNT(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,_63,_64,N,...) N
#define _CONSOLE_CHECK_ARGS(s,l,...) _CONSOLE_CHECK_ARGS_CONCAT(_CONSOLE_PRINT,_CONSOLE_CHECK_ARGS_COUNT(,##__VA_ARGS__,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0))(s,l, ## __VA_ARGS__)
#define _CONSOLE_PRINT0(s,l) console_print(s,l)
#define _CONSOLE_PRINT1(s,l,...) console_vprint(s,l,__VA_ARGS__)



enum CONSOLE_PRINT_LEVEL{
	CONSOLE_PRINT_LEVEL_OK=0x00,
	CONSOLE_PRINT_LEVEL_LOG=0x01,
	CONSOLE_PRINT_LEVEL_WARN=0x02,
	CONSOLE_PRINT_LEVEL_ERROR=0x03,
	CONSOLE_PRINT_LEVEL_CONTINUE=0xff
};



void console_init(KernelArgs* ka);



void console_print(const char* s,enum CONSOLE_PRINT_LEVEL l);



void console_vprint(const char* s,enum CONSOLE_PRINT_LEVEL l,...);



#endif
