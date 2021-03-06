#include <shared.h>
#include <cpu/cpu_info.h>
#include <gfx/console.h>



cpu_info_t cpu_info_data;



void KERNEL_CALL cpu_info_init(void){
	cpuid_t dt;
	asm_cpuid(0,&dt);
	cpu_info_data.mx=dt.eax;
	cpu_info_data.id[0]=dt.ebx&0xff;
	cpu_info_data.id[1]=(dt.ebx>>8)&0xff;
	cpu_info_data.id[2]=(dt.ebx>>16)&0xff;
	cpu_info_data.id[3]=dt.ebx>>24;
	cpu_info_data.id[4]=dt.edx&0xff;
	cpu_info_data.id[5]=(dt.edx>>8)&0xff;
	cpu_info_data.id[6]=(dt.edx>>16)&0xff;
	cpu_info_data.id[7]=dt.edx>>24;
	cpu_info_data.id[8]=dt.ecx&0xff;
	cpu_info_data.id[9]=(dt.ecx>>8)&0xff;
	cpu_info_data.id[10]=(dt.ecx>>16)&0xff;
	cpu_info_data.id[11]=dt.ecx>>24;
	cpu_info_data.id[12]=0;
	asm_cpuid(1,&dt);
	cpu_info_data.f=((uint64_t)dt.ecx<<32)|dt.edx;
	asm_cpuid(0x80000000,&dt);
	if (dt.eax>=0x80000004){
		asm_cpuid(0x80000002,&dt);
		cpu_info_data.nm[0]=dt.eax&0xff;
		cpu_info_data.nm[1]=(dt.eax>>8)&0xff;
		cpu_info_data.nm[2]=(dt.eax>>16)&0xff;
		cpu_info_data.nm[3]=dt.eax>>24;
		cpu_info_data.nm[4]=dt.ebx&0xff;
		cpu_info_data.nm[5]=(dt.ebx>>8)&0xff;
		cpu_info_data.nm[6]=(dt.ebx>>16)&0xff;
		cpu_info_data.nm[7]=dt.ebx>>24;
		cpu_info_data.nm[8]=dt.ecx&0xff;
		cpu_info_data.nm[9]=(dt.ecx>>8)&0xff;
		cpu_info_data.nm[10]=(dt.ecx>>16)&0xff;
		cpu_info_data.nm[11]=dt.edx>>24;
		cpu_info_data.nm[12]=dt.edx&0xff;
		cpu_info_data.nm[13]=(dt.edx>>8)&0xff;
		cpu_info_data.nm[14]=(dt.edx>>16)&0xff;
		cpu_info_data.nm[15]=dt.edx>>24;
		asm_cpuid(0x80000003,&dt);
		cpu_info_data.nm[16]=dt.eax&0xff;
		cpu_info_data.nm[17]=(dt.eax>>8)&0xff;
		cpu_info_data.nm[18]=(dt.eax>>16)&0xff;
		cpu_info_data.nm[19]=dt.eax>>24;
		cpu_info_data.nm[20]=dt.ebx&0xff;
		cpu_info_data.nm[21]=(dt.ebx>>8)&0xff;
		cpu_info_data.nm[22]=(dt.ebx>>16)&0xff;
		cpu_info_data.nm[23]=dt.ebx>>24;
		cpu_info_data.nm[24]=dt.ecx&0xff;
		cpu_info_data.nm[25]=(dt.ecx>>8)&0xff;
		cpu_info_data.nm[26]=(dt.ecx>>16)&0xff;
		cpu_info_data.nm[27]=dt.edx>>24;
		cpu_info_data.nm[28]=dt.edx&0xff;
		cpu_info_data.nm[29]=(dt.edx>>8)&0xff;
		cpu_info_data.nm[30]=(dt.edx>>16)&0xff;
		cpu_info_data.nm[31]=dt.edx>>24;
		asm_cpuid(0x80000004,&dt);
		cpu_info_data.nm[32]=dt.eax&0xff;
		cpu_info_data.nm[33]=(dt.eax>>8)&0xff;
		cpu_info_data.nm[34]=(dt.eax>>16)&0xff;
		cpu_info_data.nm[35]=dt.eax>>24;
		cpu_info_data.nm[36]=dt.ebx&0xff;
		cpu_info_data.nm[37]=(dt.ebx>>8)&0xff;
		cpu_info_data.nm[38]=(dt.ebx>>16)&0xff;
		cpu_info_data.nm[39]=dt.ebx>>24;
		cpu_info_data.nm[40]=dt.ecx&0xff;
		cpu_info_data.nm[41]=(dt.ecx>>8)&0xff;
		cpu_info_data.nm[42]=(dt.ecx>>16)&0xff;
		cpu_info_data.nm[43]=dt.edx>>24;
		cpu_info_data.nm[44]=dt.edx&0xff;
		cpu_info_data.nm[45]=(dt.edx>>8)&0xff;
		cpu_info_data.nm[46]=(dt.edx>>16)&0xff;
		cpu_info_data.nm[47]=dt.edx>>24;
	}
	else{
		cpu_info_data.nm[0]='U';
		cpu_info_data.nm[1]='n';
		cpu_info_data.nm[2]='k';
		cpu_info_data.nm[3]='n';
		cpu_info_data.nm[4]='o';
		cpu_info_data.nm[5]='w';
		cpu_info_data.nm[6]='n';
		cpu_info_data.nm[7]=' ';
		cpu_info_data.nm[8]='C';
		cpu_info_data.nm[9]='P';
		cpu_info_data.nm[10]='U';
		cpu_info_data.nm[11]=0;
	}
	console_log("CPU Data:\n  Max CPUID: %u\n  ID: %s\n  Features:",cpu_info_data.mx,cpu_info_data.id);
	if (cpu_info_data.f&CPU_INFO_FEATURE_FPU){
		console_log(" FPU");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_VME){
		console_log(" VME");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_DE){
		console_log(" DE");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_PSE){
		console_log(" PSE");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_TSC){
		console_log(" TSC");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_MSR){
		console_log(" MSR");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_PAE){
		console_log(" PAE");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_MCE){
		console_log(" MCE");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_CX8){
		console_log(" CX8");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_APIC){
		console_log(" APIC");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_SEP){
		console_log(" SEP");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_MTRR){
		console_log(" MTRR");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_PGE){
		console_log(" PGE");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_MCA){
		console_log(" MCA");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_CMOV){
		console_log(" CMOV");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_PAT){
		console_log(" PAT");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_PSE36){
		console_log(" PSE36");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_PSN){
		console_log(" PSN");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_CLF){
		console_log(" CLF");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_DTES){
		console_log(" DTES");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_ACPI){
		console_log(" ACPI");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_MMX){
		console_log(" MMX");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_FXSR){
		console_log(" FXSR");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_SSE){
		console_log(" SSE");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_SSE2){
		console_log(" SSE2");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_SS){
		console_log(" SS");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_HTT){
		console_log(" HTT");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_TM1){
		console_log(" TM1");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_IA64){
		console_log(" IA64");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_PBE){
		console_log(" PBE");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_SSE3){
		console_log(" SSE3");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_PCLMUL){
		console_log(" PCLMUL");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_DTES64){
		console_log(" DTES64");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_MONITOR){
		console_log(" MONITOR");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_DS_CPL){
		console_log(" DS_CPL");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_VMX){
		console_log(" VMX");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_SMX){
		console_log(" SMX");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_EST){
		console_log(" EST");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_TM2){
		console_log(" TM2");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_SSSE3){
		console_log(" SSSE3");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_CID){
		console_log(" CID");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_FMA){
		console_log(" FMA");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_CX16){
		console_log(" CX16");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_ETPRD){
		console_log(" ETPRD");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_PDCM){
		console_log(" PDCM");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_PCIDE){
		console_log(" PCIDE");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_DCA){
		console_log(" DCA");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_SSE4_1){
		console_log(" SSE4_1");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_SSE4_2){
		console_log(" SSE4_2");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_x2APIC){
		console_log(" x2APIC");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_MOVBE){
		console_log(" MOVBE");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_POPCNT){
		console_log(" POPCNT");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_AES){
		console_log(" AES");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_XSAVE){
		console_log(" XSAVE");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_OSXSAVE){
		console_log(" OSXSAVE");
	}
	if (cpu_info_data.f&CPU_INFO_FEATURE_AVX){
		console_log(" AVX");
	}
	console_log("\n  Name: %s\n",cpu_info_data.nm);
}
