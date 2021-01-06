#ifndef __KERNEL_FATAL_ERROR_H__
#define __KERNEL_FATAL_ERROR_H__ 1



#define CHECK_NOT_NULL(p) ((void*)(uint64_t)((p)||(fatal_error_line("Null Pointer",__FILE__,__LINE__,__func__),0)))



void fatal_error(char* s);



void fatal_error_line(char* s,const char* f,int l,const char* fn);



extern void asm_halt_cpu(void);



#endif
