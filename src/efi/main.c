#include <efi.h>
#include <efilib.h>
#include <efiprot.h>
#include <stdint.h>



#define PAGE_SIZE 0x1000
#define PAGE_TABLE_MAX_ENTRIES 0x200



extern void __attribute__((ms_abi)) asm_enable_paging(uint64_t pml4);
extern void __attribute__((ms_abi)) asm_halt(void);



static EFI_GUID efi_gop_guid=EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
static EFI_GUID efi_acpi2_guid=ACPI_20_TABLE_GUID;
static EFI_GUID efi_lip_guid=EFI_LOADED_IMAGE_PROTOCOL_GUID;
static EFI_GUID efi_sfs_guid=EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
static uint16_t* kernel_fp=L"EFI\\BOOT\\BOOTX64.EFI";



void efi_main(EFI_HANDLE ih,EFI_SYSTEM_TABLE* st){
	InitializeLib(ih,st);
	EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
	EFI_STATUS s=st->BootServices->LocateProtocol(&efi_gop_guid,NULL,(void**)&gop);
	if (EFI_ERROR(s)){
		Print(L"Unable to load GOP!\r\n");
		goto _end;
	}
	EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* mi;
	UINT64 mi_sz;
	s=gop->QueryMode(gop,(gop->Mode==NULL?0:gop->Mode->Mode),&mi_sz,&mi);
	if (s==EFI_NOT_STARTED){
		s=gop->SetMode(gop,0);
	}
	if (EFI_ERROR(s)){
		Print(L"Unable to get native mode!\r\n");
		goto _end;
	}
	UINT32 mx=0;
	UINT32 my=0;
	UINT64 mp=0;
	UINT64 m=-1;
	for (UINT64 i=0;i<gop->Mode->MaxMode;i++){
		s=gop->QueryMode(gop,i,&mi_sz,&mi);
		if (EFI_ERROR(s)){
			goto _end;
		}
		if (mi->PixelFormat!=PixelRedGreenBlueReserved8BitPerColor&&mi->PixelFormat!=PixelBlueGreenRedReserved8BitPerColor){
			continue;
		}
		UINT64 p=(UINT64)mi->HorizontalResolution*mi->VerticalResolution;
		if (p>mp||(p==mp&&mi->HorizontalResolution>mx)){
			mx=mi->HorizontalResolution;
			my=mi->VerticalResolution;
			mp=p;
			m=i;
		}
	}
	(void)my;
	s=gop->SetMode(gop,m);
	if (EFI_ERROR(s)){
		Print(L"Unable to set mode %03d\r\n",m);
		goto _end;
	}
	else{
		Print(L"%x -> +%d, %d x %d @ %s\r\n",gop->Mode->FrameBufferBase,gop->Mode->FrameBufferSize,gop->Mode->Info->HorizontalResolution,gop->Mode->Info->VerticalResolution,(gop->Mode->Info->PixelFormat==PixelRedGreenBlueReserved8BitPerColor?L"RGB":L"BGR"));
	}
	Print(L"Starting Loader...\r\nReading Memory Map...\r\n");
	UINT64 mm_sz=sizeof(EFI_MEMORY_DESCRIPTOR)*32;
	UINT64 mm_k=0;
	UINT64 mm_ds=0;
	uint32_t mm_v=0;
	uint8_t* bf=NULL;
	do{
		bf=AllocatePool(mm_sz);
		if (bf==NULL){
			break;
		}
		s=st->BootServices->GetMemoryMap(&mm_sz,(EFI_MEMORY_DESCRIPTOR*)bf,&mm_k,&mm_ds,&mm_v);
		if (s!=EFI_SUCCESS){
			FreePool(bf);
			mm_sz+=sizeof(EFI_MEMORY_DESCRIPTOR)*32;
		}
	} while (s!=EFI_SUCCESS);
	if (bf==NULL){
		Print(L"Error!\r\n");
		goto _end;
	}
	uint64_t sz=0;
	uint8_t lt=0;
	uint64_t le=-1;
	uint8_t* nbf=bf;
	for (uint64_t i=0;i<mm_sz/mm_ds;i++){
		if (((EFI_MEMORY_DESCRIPTOR*)nbf)->Type==EfiConventionalMemory||((EFI_MEMORY_DESCRIPTOR*)nbf)->Type==EfiBootServicesCode||((EFI_MEMORY_DESCRIPTOR*)nbf)->Type==EfiBootServicesData||((EFI_MEMORY_DESCRIPTOR*)nbf)->Type==EfiACPIReclaimMemory){
			uint8_t t=(((EFI_MEMORY_DESCRIPTOR*)nbf)->Type==EfiACPIReclaimMemory);
			if (lt!=t||le!=((EFI_MEMORY_DESCRIPTOR*)nbf)->PhysicalStart){
				sz++;
				lt=t;
			}
			le=((EFI_MEMORY_DESCRIPTOR*)nbf)->PhysicalStart+((EFI_MEMORY_DESCRIPTOR*)nbf)->NumberOfPages*PAGE_SIZE;
		}
		nbf+=mm_ds;
	}
	uint64_t* mm=AllocatePool(sz*sizeof(uint64_t));
	uint64_t tm=0;
	uint64_t j=0;
	lt=0;
	le=0;
	*mm=0;
	*(mm+1)=0;
	for (uint64_t i=0;i<mm_sz/mm_ds;i++){
		if (((EFI_MEMORY_DESCRIPTOR*)bf)->Type==EfiConventionalMemory||((EFI_MEMORY_DESCRIPTOR*)bf)->Type==EfiBootServicesCode||((EFI_MEMORY_DESCRIPTOR*)bf)->Type==EfiBootServicesData||((EFI_MEMORY_DESCRIPTOR*)bf)->Type==EfiACPIReclaimMemory){
			uint8_t t=(((EFI_MEMORY_DESCRIPTOR*)bf)->Type==EfiACPIReclaimMemory);
			if (lt!=t||le!=((EFI_MEMORY_DESCRIPTOR*)bf)->PhysicalStart){
				Print(L"  %llx - +%llx (%d)\r\n",(*(mm+j))&0x7fffffffffffffff,*(mm+j+1),*(mm+j)>>63);
				j+=2;
				*(mm+j)=((EFI_MEMORY_DESCRIPTOR*)bf)->PhysicalStart|((uint64_t)t<<63);
				*(mm+j+1)=0;
				lt=t;
			}
			*(mm+j+1)+=((EFI_MEMORY_DESCRIPTOR*)bf)->NumberOfPages*PAGE_SIZE;
			le=((EFI_MEMORY_DESCRIPTOR*)bf)->PhysicalStart+((EFI_MEMORY_DESCRIPTOR*)bf)->NumberOfPages*PAGE_SIZE;
			tm+=((EFI_MEMORY_DESCRIPTOR*)bf)->NumberOfPages*PAGE_SIZE;
		}
		bf+=mm_ds;
	}
	FreePool(bf);
	Print(L"  %llx - +%llx (%d)\r\nTotal: %llu (%llu sectors)\r\nAllocating Pages...\r\n",(*(mm+j))&0x7fffffffffffffff,*(mm+j+1),*(mm+j)>>63,tm,sz);
	uint64_t* pml4;
	s=st->BootServices->AllocatePages(AllocateAnyPages,0x80000000,2,(EFI_PHYSICAL_ADDRESS*)&pml4);
	st->BootServices->SetMem(pml4,2*PAGE_TABLE_MAX_ENTRIES*sizeof(uint64_t),0);
	Print(L"PML4 Pointer: %llx\r\nSetting Up Tables...\r\n",pml4);
	*(pml4+PAGE_TABLE_MAX_ENTRIES-1)=((uint64_t)pml4+PAGE_TABLE_MAX_ENTRIES*sizeof(uint64_t))|0x003;
	*(pml4+PAGE_TABLE_MAX_ENTRIES*2-2)=0x83;
	*(pml4+PAGE_TABLE_MAX_ENTRIES*2-1)=0x40000083;
	uint64_t* cr3;
	__asm__ volatile("mov %%cr3,%0":"=r"(cr3));
	for (uint16_t i=0;i<PAGE_TABLE_MAX_ENTRIES/2;i++){
		*(pml4+i)=*(cr3+i);
	}
	asm_enable_paging((uint64_t)pml4);
	void* acpi=NULL;
	for (uint64_t i=0;i<st->NumberOfTableEntries;i++){
		if (CompareGuid(&(st->ConfigurationTable+i)->VendorGuid,&efi_acpi2_guid)){
			acpi=(st->ConfigurationTable+i)->VendorTable;
			break;
		}
	}
	if (acpi==NULL){
		Print(L"No ACPI Table Found!\r\n");
		goto _end;
	}
	Print(L"Found ACPI Table: %llx\r\n",acpi);
	EFI_LOADED_IMAGE_PROTOCOL* lip;
	s=st->BootServices->HandleProtocol(ih,&efi_lip_guid,(void**)&lip);
	if (EFI_ERROR(s)){
		Print(L"Unable to load LIP!\r\n");
		goto _end;
	}
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* sfs;
	s=st->BootServices->HandleProtocol(lip->DeviceHandle,&efi_sfs_guid,(void**)&sfs);
	if (EFI_ERROR(s)){
		Print(L"Unable to load SFS!\r\n");
		goto _end;
	}
	EFI_FILE_PROTOCOL* r_fs;
	s=sfs->OpenVolume(sfs,&r_fs);
	if (EFI_ERROR(s)){
		Print(L"Unable to open Root Volume!\r\n");
		goto _end;
	}
	EFI_FILE_PROTOCOL* kf;
	s=r_fs->Open(r_fs,&kf,kernel_fp,EFI_FILE_MODE_READ,0);
	if (EFI_ERROR(s)){
		Print(L"Unable to open Kernel File!\r\n");
		goto _end;
	}
_end:
	asm_halt();
}
