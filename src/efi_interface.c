#include <efi.h>
#include <efilib.h>



void init_efi(EFI_HANDLE ih,EFI_SYSTEM_TABLE* st){
	InitializeLib(ih,st);
}
