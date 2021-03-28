#ifndef __KERNEL_SHARED_H__
#define __KERNEL_SHARED_H__ 1



#define OS_VERSION 1
#define KERNEL_CALL __attribute__((ms_abi))
#define KERNEL_UNMAP_AFTER_LOAD __attribute__((section(".unmap")))



#endif
