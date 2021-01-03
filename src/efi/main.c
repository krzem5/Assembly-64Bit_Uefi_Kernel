#define KERNEL_ARGS_STRUCT_ONLY 1
#include <efi.h>
#include <efilib.h>
#include <efiprot.h>
#include <stdint.h>
#include <kmain.h>



#define PAGE_SIZE 0x1000
#define PAGE_TABLE_MAX_ENTRIES 0x200
#define ELF_HEADER_MAGIC 0x464c457f
#define ELF_HEADER_WORD_SIZE 2
#define ELF_HEADER_ENDIANESS 1
#define ELF_HEADER_VERSION 1
#define ELF_HEADER_OS_ABI 0
#define ELF_HEADER_TAREGT_MACHINE 0x3e
#define ELF_PT_LOAD 0x01



extern void __attribute__((ms_abi)) asm_enable_paging(uint64_t pml4);
extern void __attribute__((ms_abi)) asm_halt(void);



typedef void (*kmain)(KernelArgs a);



struct ELF_HEADER{
	uint32_t m;
	uint8_t ws;
	uint8_t ed;
	uint8_t hv;
	uint8_t abi;
	uint64_t _;
	uint16_t t;
	uint16_t tm;
	uint32_t v;
	uint64_t e;
	uint64_t ph_off;
	uint64_t sh_off;
	uint32_t f;
	uint16_t eh_sz;
	uint16_t pn_sz;
	uint16_t ph_n;
	uint16_t sh_sz;
	uint16_t sh_n;
	uint16_t sh_str_i;
} __attribute__((packed));




struct ELF_PROGRAM_HEADER{
	uint32_t t;
	uint32_t f;
	uint64_t off;
	uint64_t va;
	uint64_t pa;
	uint64_t f_sz;
	uint64_t m_sz;
	uint64_t a;
} __attribute__((packed));




struct ELF_SECTION_HEADER{
	uint32_t nm;
	uint32_t t;
	uint64_t f;
	uint64_t va;
	uint64_t off;
	uint64_t sz;
	uint32_t lnk;
	uint32_t inf;
	uint64_t a;
	uint64_t e_sz;
} __attribute__((packed));



static EFI_GUID efi_gop_guid=EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
static EFI_GUID efi_acpi2_guid=ACPI_20_TABLE_GUID;
static EFI_GUID efi_lip_guid=EFI_LOADED_IMAGE_PROTOCOL_GUID;
static EFI_GUID efi_sfs_guid=EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
static EFI_GUID efi_fi_guid=EFI_FILE_INFO_ID;
static uint16_t* kernel_fp=L"KERNEL.ELF";



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
	Print(L"Starting Loader...\r\n");
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
	uint8_t kf_i_bf[sizeof(EFI_FILE_INFO)+128];
	uint64_t sz=sizeof(EFI_FILE_INFO)+128;
	s=kf->GetInfo(kf,&efi_fi_guid,&sz,&kf_i_bf);
	if (EFI_ERROR(s)){
		Print(L"Unable to Retrive Kernel File Info!\r\n");
		goto _end;
	}
	EFI_FILE_INFO* kf_i=(EFI_FILE_INFO*)kf_i_bf;
	Print(L"Kernel File Size: %llu (%llu Pages)\r\n",kf_i->FileSize,(kf_i->FileSize+PAGE_SIZE-1)>>12);
	void* k_dt;
	s=st->BootServices->AllocatePages(AllocateAnyPages,0x80000000,(kf_i->FileSize+PAGE_SIZE-1)>>12,(EFI_PHYSICAL_ADDRESS*)&k_dt);
	if (EFI_ERROR(s)){
		Print(L"Unable to Allocate Memory for Kernel!\r\n");
		goto _end;
	}
	s=kf->Read(kf,&kf_i->FileSize,k_dt);
	FreePool(kf_i);
	if (EFI_ERROR(s)){
		Print(L"Unable to Read Kernel into Memory!\r\n");
		goto _end;
	}
	struct ELF_HEADER* kh=(struct ELF_HEADER*)k_dt;
	uint64_t __TEMP_E=kh->e;
	if (kh->m!=ELF_HEADER_MAGIC){
		Print(L"Invalid ELF Magic: 0x%lx\r\n",kh->m);
		goto _end;
	}
	if (kh->ws!=ELF_HEADER_WORD_SIZE){
		Print(L"Unsupported ELF Word Size: %u\r\n",kh->ws);
		goto _end;
	}
	if (kh->ed!=ELF_HEADER_ENDIANESS){
		Print(L"Unsupported ELF Endianess: %u\r\n",kh->ed);
		goto _end;
	}
	if (kh->hv!=ELF_HEADER_VERSION){
		Print(L"Unsupported ELF Header Version: %u\r\n",kh->hv);
		goto _end;
	}
	if (kh->abi!=ELF_HEADER_OS_ABI){
		Print(L"Unsupported ELF OS ABI: %u\r\n",kh->abi);
		goto _end;
	}
	if (kh->tm!=ELF_HEADER_TAREGT_MACHINE){
		Print(L"Unsupported ELF Target Machine: %u\r\n",kh->tm);
		goto _end;
	}
	// struct ELF_PROGRAM_HEADER* k_ph=(struct ELF_PROGRAM_HEADER*)((uint8_t*)k_dt+kh->ph_off);
	// for (uint16_t i=0;i<kh->ph_n;i++){
	// 	if ((k_ph+i)->t!=ELF_PT_LOAD){
	// 		continue;
	// 	}
	// 	Print(L"Program Header: %lx, %lx, %llx, %llx, %llx, %llx, %llx, %llx\r\n",(k_ph+i)->t,(k_ph+i)->f,(k_ph+i)->off,(k_ph+i)->va,(k_ph+i)->pa,(k_ph+i)->f_sz,(k_ph+i)->m_sz,(k_ph+i)->a);
	// }
	// struct ELF_SECTION_HEADER* k_sh=(struct ELF_SECTION_HEADER*)((uint8_t*)k_dt+kh->sh_off);
	// for (uint16_t i=0;i<kh->sh_n;i++){
	// 	if ((k_sh+i)->t==0){
	// 		continue;
	// 	}
	// 	Print(L"Section Header: %lu, %lx, %llx, %llx, %llx, %llx, %lx, %lx, %llx, %llx\r\n",(k_sh+i)->nm,(k_sh+i)->t,(k_sh+i)->f,(k_sh+i)->va,(k_sh+i)->off,(k_sh+i)->sz,(k_sh+i)->lnk,(k_sh+i)->inf,(k_sh+i)->a,(k_sh+i)->e_sz);
	// }
	// goto _end;
	struct ELF_PROGRAM_HEADER* k_ph=(struct ELF_PROGRAM_HEADER*)((uint8_t*)k_dt+kh->ph_off);
	uint64_t pb=-1;
	uint64_t pe=0;
	for (uint16_t i=0;i<kh->ph_n;i++){
		if ((k_ph+i)->t!=ELF_PT_LOAD){
			continue;
		}
		if ((k_ph+i)->va<pb){
			pb=(k_ph+i)->va;
		}
		if ((k_ph+i)->va+(k_ph+i)->m_sz>pe){
			pe=(k_ph+i)->va+(k_ph+i)->m_sz;
		}
	}
	uint64_t k_pg_c=1;
	uint16_t li[3]={-1,-1,-1};
	for (uint64_t i=pb;i<pe;i+=PAGE_SIZE){
		for (uint8_t k=0;k<3;k++){
			uint16_t l=(i>>(39-9*k))&0x1ff;
			if (li[k]!=l){
				k_pg_c++;
				li[k]=l;
			}
		}
	}
	Print(L"Kernel Data: %llx - +%llx (%llu Pages); Entrypoint: %llx\r\n",pb,pe-pb,k_pg_c,kh->e);
	void* k_pg;
	s=st->BootServices->AllocatePages(AllocateAnyPages,0x80000000,(pe-pb+PAGE_SIZE-1)>>12,(EFI_PHYSICAL_ADDRESS*)&k_pg);
	if (EFI_ERROR(s)){
		Print(L"Unable Allocate Pages for Kernel Code!\r\n");
		goto _end;
	}
	for (uint16_t i=0;i<kh->ph_n;i++){
		if ((k_ph+i)->t!=ELF_PT_LOAD){
			continue;
		}
		st->BootServices->CopyMem(k_pg+(k_ph+i)->va-pb,k_dt+(k_ph+i)->off,(k_ph+i)->f_sz);
		Print(L"Kernel Section#%u: %llx -> %llx\r\n",i,k_pg,(k_ph+i)->va);
	}
	uint64_t mm_sz=sizeof(EFI_MEMORY_DESCRIPTOR)*32;
	uint64_t mm_k=0;
	uint64_t mm_ds=0;
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
	sz=0;
	uint8_t lt=0;
	uint64_t le=-1;
	uint8_t* nbf=bf;
	for (uint64_t i=0;i<mm_sz/mm_ds;i++){
		if (((EFI_MEMORY_DESCRIPTOR*)nbf)->Type==EfiConventionalMemory||((EFI_MEMORY_DESCRIPTOR*)nbf)->Type==EfiBootServicesCode||((EFI_MEMORY_DESCRIPTOR*)nbf)->Type==EfiBootServicesData||((EFI_MEMORY_DESCRIPTOR*)nbf)->Type==EfiLoaderCode||((EFI_MEMORY_DESCRIPTOR*)nbf)->Type==EfiLoaderData||((EFI_MEMORY_DESCRIPTOR*)nbf)->Type==EfiACPIReclaimMemory){
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
		if (((EFI_MEMORY_DESCRIPTOR*)bf)->Type==EfiConventionalMemory||((EFI_MEMORY_DESCRIPTOR*)bf)->Type==EfiBootServicesCode||((EFI_MEMORY_DESCRIPTOR*)bf)->Type==EfiBootServicesData||((EFI_MEMORY_DESCRIPTOR*)bf)->Type==EfiLoaderCode||((EFI_MEMORY_DESCRIPTOR*)bf)->Type==EfiLoaderData||((EFI_MEMORY_DESCRIPTOR*)bf)->Type==EfiACPIReclaimMemory){
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
	s=st->BootServices->AllocatePages(AllocateAnyPages,0x80000000,k_pg_c,(EFI_PHYSICAL_ADDRESS*)&pml4);
	st->BootServices->SetMem(pml4,k_pg_c*PAGE_TABLE_MAX_ENTRIES*sizeof(uint64_t),0);
	Print(L"PML4 Pointer: %llx\r\nSetting Up Tables...\r\n",pml4);
	li[0]=-1;
	li[1]=-1;
	li[2]=-1;
	li[3]=-1;
	uint64_t* pt[4]={pml4,0,0,0};
	uint64_t pg_id=1;
	for (uint64_t i=pb;i<pe;i+=PAGE_SIZE){
		for (uint8_t k=0;k<4;k++){
			uint16_t l=(i>>(39-9*k))&0x1ff;
			if (li[k]!=l){
				li[k]=l;
				if (k<3){
					Print(L"New Page Table: #%lu [%u] -> %llx\r\n",pg_id,k+1,pml4+pg_id*PAGE_TABLE_MAX_ENTRIES);
					pt[k+1]=(uint64_t*)((uint8_t*)pml4+(pg_id<<12));
					*(pt[k]+l)=((uint64_t)pt[k+1])|0x003;
					pg_id++;
				}
				else{
					*(pt[3]+l)=((uint64_t)k_pg+(i-pb))|0x003;
				}
			}
		}
		Print(L"Kernel Page: [%u : %u : %u : %u] ->  %llx\r\n",li[0],li[1],li[2],li[3],i);
	}
	(void)pt;
	if (pg_id!=k_pg_c){
		Print(L"Not All Kernel Pages Used (%llu / %llu)!\r\n",pg_id,k_pg_c);
		goto _end;
	}
	KernelArgs ka;
	s=st->BootServices->AllocatePool(0x80000000,sizeof(struct _KERNEL_ARGS),(void**)&ka);
	if (EFI_ERROR(s)){
		Print(L"Failed to Allocate Memory for Kernel!\r\n");
		goto _end;
	}
	ka->vmem=(void*)gop->Mode->FrameBufferBase;
	ka->vmem_l=gop->Mode->FrameBufferSize;
	uint64_t* cr3;
	__asm__ volatile("mov %%cr3,%0":"=r"(cr3));
	for (uint16_t i=0;i<PAGE_TABLE_MAX_ENTRIES/2;i++){
		*(pml4+i)=*(cr3+i);
	}
	mm_sz=sizeof(EFI_MEMORY_DESCRIPTOR)*32;
	mm_k=0;
	mm_ds=0;
	mm_v=0;
	bf=NULL;
	do{
		s=st->BootServices->AllocatePool(0x80000000,mm_sz,(void**)&bf);
		if (EFI_ERROR(s)){
			Print(L"AAA\r\n");
			goto _end;
		}
		if (bf==NULL){
			break;
		}
		s=st->BootServices->GetMemoryMap(&mm_sz,(EFI_MEMORY_DESCRIPTOR*)bf,&mm_k,&mm_ds,&mm_v);
		if (s!=EFI_SUCCESS){
			st->BootServices->FreePool(bf);
			mm_sz+=sizeof(EFI_MEMORY_DESCRIPTOR)*32;
		}
	} while (s!=EFI_SUCCESS);
	s=st->BootServices->ExitBootServices(ih,mm_k);
	if (EFI_ERROR(s)){
		Print(L"Failed to Exit EFI Loader!\r\n");
		goto _end;
	}
	for (uint64_t i=0;i<mm_sz/mm_ds;i++){
		((EFI_MEMORY_DESCRIPTOR*)bf)->VirtualStart=((EFI_MEMORY_DESCRIPTOR*)bf)->PhysicalStart;
	}
	asm_enable_paging((uint64_t)pml4);
	s=st->RuntimeServices->SetVirtualAddressMap(mm_sz,mm_ds,mm_v,(EFI_MEMORY_DESCRIPTOR*)bf);
	if (EFI_ERROR(s)){
		Print(L"Failed to Virtualize EFI!\r\n");
		goto _end;
	}
	((kmain)__TEMP_E)(ka);
_end:
	asm_halt();
}
