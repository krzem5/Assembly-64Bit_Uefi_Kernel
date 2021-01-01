#include <efi.h>
#include <efilib.h>
#include <efi_interface.h>



void efi_main(EFI_HANDLE ih,EFI_SYSTEM_TABLE* st){
	init_efi(ih,st);
	Print(L"Start!");
	for(;;) __asm__("hlt");
}
