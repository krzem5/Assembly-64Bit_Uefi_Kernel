#ifndef __KERNEL_FONT_H__
#define __KERNEL_FONT_H__
#include <libc/stdint.h>



typedef struct __FONT{
	uint8_t mx;
	const uint64_t* dt;
} Font;



#endif
