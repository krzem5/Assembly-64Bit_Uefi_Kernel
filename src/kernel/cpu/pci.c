#include <shared.h>
#include <cpu/pci.h>
#include <driver/driver_list.h>
#include <gfx/console.h>
#include <memory/paging.h>
#include <memory/vm.h>
#include <stdint.h>



pci_device_list_t pci_list={
	0
};



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD pci_init(void){
	uint64_t d_pci[128];
	for (uint8_t b=0;b<255;b++){
		if (b<128){
			d_pci[b]=0;
		}
		for (uint8_t d=0;d<32;d++){
			uint8_t i=b>>1;
			uint64_t bi=1ull<<(((b&1)<<5)|d);
			for (uint8_t f=0;f<8;f++){
				if ((asm_pci_read(0x80000000|(b<<16)|(d<<11)|(f<<8)|PCI_VENDOR_REGISTER)&0xffff)!=0xffff){
					d_pci[i]|=bi;
					pci_list.l++;
				}
			}
		}
	}
	pci_list.dt=(void*)vm_commit((pci_list.l*sizeof(pci_device_t)+PAGE_4KB_SIZE-1)>>PAGE_4KB_POWER_OF_2);
	pci_device_t* pci=pci_list.dt;
	for (uint8_t i=0;i<128;i++){
		uint64_t e=d_pci[i];
		while (e){
			uint8_t bi=__builtin_ctzll(e);
			e&=~(1ull<<bi);
			uint8_t b=(i<<1)|(bi>>5);
			uint8_t d=bi&0x1f;
			for (uint8_t f=0;f<8;f++){
				uint32_t p=0x80000000|(b<<16)|(d<<11)|(f<<8);
				uint32_t v=asm_pci_read(p|PCI_VENDOR_REGISTER);
				if ((v&0xffff)!=0xffff){
					pci->port=p;
					pci->bus=b;
					pci->dev_func=(d<<3)|f;
					pci->vendor_id=v&0xffff;
					pci->device_id=v>>16;
					v=asm_pci_read(pci->port|PCI_ID_REGISTER);
					pci->revision=v&0xff;
					pci->interface=(v>>8)&0xff;
					pci->subclass=(v>>16)&0xff;
					pci->class_code=v>>24;
					v=asm_pci_read(pci->port|PCI_MISC_REGISTER);
					pci->cache_line_size=v&0xff;
					pci->latency_timer=(v>>8)&0xff;
					pci->type=(v>>16)&0x7f;
					pci->bist=v>>24;
					pci->class_code_str="-";
					pci->subclass_str="-";
					if (!pci->class_code){
						pci->class_code_str="Unclassified";
						if (!pci->subclass){
							pci->subclass_str="Non-VGA-Compatible devices";
						}
						else if (pci->subclass==1){
							pci->subclass_str="VGA-Compatible devices";
						}
					}
					else if (pci->class_code==1){
						pci->class_code_str="Mass Storage Controller";
						if (!pci->subclass){
							pci->subclass_str="SCSI Bus Controller";
						}
						else if (pci->subclass==1){
							pci->subclass_str="ATA Controller";
						}
						else if (pci->subclass==2){
							pci->subclass_str="Floppy Disk Controller";
						}
						else if (pci->subclass==3){
							pci->subclass_str="IPI Bus Controller";
						}
						else if (pci->subclass==4){
							pci->subclass_str="RAID Controller";
						}
						else if (pci->subclass==5){
							pci->subclass_str="ATA Controller";
						}
						else if (pci->subclass==6){
							pci->subclass_str="Serial ATA";
						}
						else if (pci->subclass==7){
							pci->subclass_str="Serial Attached SCSI";
						}
						else if (pci->subclass==8){
							pci->subclass_str="Non-Volatile Memory Controller";
						}
						else if (pci->subclass==128){
							pci->subclass_str="Other";
						}
					}
					else if (pci->class_code==2){
						pci->class_code_str="Network Controller";
						if (!pci->subclass){
							pci->subclass_str="Ethernet Controller";
						}
						else if (pci->subclass==1){
							pci->subclass_str="Token Ring Controller";
						}
						else if (pci->subclass==2){
							pci->subclass_str="FDDI Controller";
						}
						else if (pci->subclass==3){
							pci->subclass_str="ATM Controller";
						}
						else if (pci->subclass==4){
							pci->subclass_str="ISDN Controller";
						}
						else if (pci->subclass==5){
							pci->subclass_str="WorldFip Controller";
						}
						else if (pci->subclass==6){
							pci->subclass_str="PICMG 2.14 Multi Computing";
						}
						else if (pci->subclass==7){
							pci->subclass_str="Infiniband Controller";
						}
						else if (pci->subclass==8){
							pci->subclass_str="Fabric Controller";
						}
						else if (pci->subclass==128){
							pci->subclass_str="Other";
						}
					}
					else if (pci->class_code==3){
						pci->class_code_str="Display Controller";
						if (!pci->subclass){
							pci->subclass_str="VGA Compatible Controller";
						}
						else if (pci->subclass==1){
							pci->subclass_str="XGA Controller";
						}
						else if (pci->subclass==2){
							pci->subclass_str="3D Controller";
						}
						else if (pci->subclass==128){
							pci->subclass_str="Other";
						}
					}
					else if (pci->class_code==4){
						pci->class_code_str="Multimedia Controller";
						if (!pci->subclass){
							pci->subclass_str="Multimedia Video Controller";
						}
						else if (pci->subclass==1){
							pci->subclass_str="Multimedia Audio Controller";
						}
						else if (pci->subclass==2){
							pci->subclass_str="Computer Telephony Device";
						}
						else if (pci->subclass==3){
							pci->subclass_str="Audio Device";
						}
						else if (pci->subclass==128){
							pci->subclass_str="Other";
						}
					}
					else if (pci->class_code==5){
						pci->class_code_str="Memory Controller";
						if (!pci->subclass){
							pci->subclass_str="RAM Controller";
						}
						else if (pci->subclass==1){
							pci->subclass_str="Flash Controller";
						}
						else if (pci->subclass==128){
							pci->subclass_str="Other";
						}
					}
					else if (pci->class_code==6){
						pci->class_code_str="Bridge Device";
						if (!pci->subclass){
							pci->subclass_str="Host Bridge";
						}
						else if (pci->subclass==1){
							pci->subclass_str="ISA Bridge";
						}
						else if (pci->subclass==2){
							pci->subclass_str="EISA Bridge";
						}
						else if (pci->subclass==3){
							pci->subclass_str="MCA Bridge";
						}
						else if (pci->subclass==4){
							pci->subclass_str="PCI-to-PCI Bridge";
						}
						else if (pci->subclass==5){
							pci->subclass_str="PCMCIA Bridge";
						}
						else if (pci->subclass==6){
							pci->subclass_str="NuBus Bridge";
						}
						else if (pci->subclass==7){
							pci->subclass_str="CardBus Bridge";
						}
						else if (pci->subclass==8){
							pci->subclass_str="RACEway Bridge";
						}
						else if (pci->subclass==9){
							pci->subclass_str="PCI-to-PCI Bridge";
						}
						else if (pci->subclass==10){
							pci->subclass_str="InfiniBand-to-PCI Host Bridge";
						}
						else if (pci->subclass==128){
							pci->subclass_str="Other";
						}
					}
					else if (pci->class_code==7){
						pci->class_code_str="Simple Communication Controller";
						if (!pci->subclass){
							pci->subclass_str="Serial Controller";
						}
						else if (pci->subclass==1){
							pci->subclass_str="Parallel Controller";
						}
						else if (pci->subclass==2){
							pci->subclass_str="Multiport Serial Controller";
						}
						else if (pci->subclass==3){
							pci->subclass_str="Modem";
						}
						else if (pci->subclass==4){
							pci->subclass_str="IEEE 488.1/2 (GPIB) Controller";
						}
						else if (pci->subclass==5){
							pci->subclass_str="Smart Card";
						}
						else if (pci->subclass==128){
							pci->subclass_str="Other";
						}
					}
					else if (pci->class_code==8){
						pci->class_code_str="Base System Peripheral";
						if (!pci->subclass){
							pci->subclass_str="PIC";
						}
						else if (pci->subclass==1){
							pci->subclass_str="DMA Controller";
						}
						else if (pci->subclass==2){
							pci->subclass_str="Timer";
						}
						else if (pci->subclass==3){
							pci->subclass_str="RTC Controller";
						}
						else if (pci->subclass==4){
							pci->subclass_str="PCI Hot-Plug Controller";
						}
						else if (pci->subclass==5){
							pci->subclass_str="SD Host controller";
						}
						else if (pci->subclass==6){
							pci->subclass_str="IOMMU";
						}
						else if (pci->subclass==128){
							pci->subclass_str="Other";
						}
					}
					else if (pci->class_code==9){
						pci->class_code_str="Input Device Controller";
						if (!pci->subclass){
							pci->subclass_str="Keyboard Controller";
						}
						else if (pci->subclass==1){
							pci->subclass_str="Digitizer Pen";
						}
						else if (pci->subclass==2){
							pci->subclass_str="Mouse Controller";
						}
						else if (pci->subclass==3){
							pci->subclass_str="Scanner Controller";
						}
						else if (pci->subclass==4){
							pci->subclass_str="Gameport Controller";
						}
						else if (pci->subclass==128){
							pci->subclass_str="Other";
						}
					}
					else if (pci->class_code==10){
						pci->class_code_str="Docking Station";
						if (!pci->subclass){
							pci->subclass_str="Generic";
						}
						else if (pci->subclass==128){
							pci->subclass_str="Other";
						}
					}
					else if (pci->class_code==11){
						pci->class_code_str="Processor";
						if (!pci->subclass){
							pci->subclass_str="386";
						}
						else if (pci->subclass==1){
							pci->subclass_str="486";
						}
						else if (pci->subclass==2){
							pci->subclass_str="Pentium";
						}
						else if (pci->subclass==3){
							pci->subclass_str="Pentium Pro";
						}
						else if (pci->subclass==4){
							pci->subclass_str="Alpha";
						}
						else if (pci->subclass==5){
							pci->subclass_str="PowerPC";
						}
						else if (pci->subclass==6){
							pci->subclass_str="MIPS";
						}
						else if (pci->subclass==7){
							pci->subclass_str="Co-Processor";
						}
						else if (pci->subclass==128){
							pci->subclass_str="Other";
						}
					}
					else if (pci->class_code==12){
						pci->class_code_str="Serial Bus Controller";
						if (!pci->subclass){
							pci->subclass_str="FireWire (IEEE 1394) Controller";
						}
						else if (pci->subclass==1){
							pci->subclass_str="ACCESS Bus";
						}
						else if (pci->subclass==2){
							pci->subclass_str="SSA";
						}
						else if (pci->subclass==3){
							pci->subclass_str="USB Controller";
						}
						else if (pci->subclass==4){
							pci->subclass_str="Fibre Channel";
						}
						else if (pci->subclass==5){
							pci->subclass_str="SMBus";
						}
						else if (pci->subclass==6){
							pci->subclass_str="InfiniBand";
						}
						else if (pci->subclass==7){
							pci->subclass_str="IPMI Interface";
						}
						else if (pci->subclass==8){
							pci->subclass_str="SERCOS Interface (IEC 61491)";
						}
						else if (pci->subclass==9){
							pci->subclass_str="CANbus";
						}
						else if (pci->subclass==128){
							pci->subclass_str="Other";
						}
					}
					else if (pci->class_code==13){
						pci->class_code_str="Wireless Controller";
						if (!pci->subclass){
							pci->subclass_str="iRDA Compatible Controller";
						}
						else if (pci->subclass==1){
							pci->subclass_str="Consumer IR Controller";
						}
						else if (pci->subclass==2){
							pci->subclass_str="RF Controller";
						}
						else if (pci->subclass==3){
							pci->subclass_str="Bluetooth Controller";
						}
						else if (pci->subclass==4){
							pci->subclass_str="Broadband Controller";
						}
						else if (pci->subclass==5){
							pci->subclass_str="Ethernet Controller (802.1a)";
						}
						else if (pci->subclass==6){
							pci->subclass_str="Ethernet Controller (802.1b)";
						}
						else if (pci->subclass==128){
							pci->subclass_str="Other";
						}
					}
					else if (pci->class_code==14){
						pci->class_code_str="Intelligent Controller";
						if (!pci->subclass){
							pci->subclass_str="I20";
						}
					}
					else if (pci->class_code==15){
						pci->class_code_str="Satellite Communication Controller";
						if (!pci->subclass){
							pci->subclass_str="Satellite TV Controller";
						}
						else if (pci->subclass==1){
							pci->subclass_str="Satellite Audio Controller";
						}
						else if (pci->subclass==2){
							pci->subclass_str="Satellite Voice Controller";
						}
						else if (pci->subclass==3){
							pci->subclass_str="Satellite Data Controller";
						}
					}
					else if (pci->class_code==16){
						pci->class_code_str="Encryption Controller";
						if (!pci->subclass){
							pci->subclass_str="Network and Computing Encrpytion/Decryption";
						}
						else if (pci->subclass==1){
							pci->subclass_str="Entertainment Encryption/Decryption";
						}
						else if (pci->subclass==128){
							pci->subclass_str="Other Encryption/Decryption";
						}
					}
					else if (pci->class_code==17){
						pci->class_code_str="Signal Processing Controller";
						if (!pci->subclass){
							pci->subclass_str="DPIO Modules";
						}
						else if (pci->subclass==1){
							pci->subclass_str="Performance Counters";
						}
						else if (pci->subclass==2){
							pci->subclass_str="Communication Synchronizer";
						}
						else if (pci->subclass==3){
							pci->subclass_str="Signal Processing Management";
						}
						else if (pci->subclass==128){
							pci->subclass_str="Other";
						}
					}
					else if (pci->class_code==18){
						pci->class_code_str="Processing Accelerator";
					}
					else if (pci->class_code==19){
						pci->class_code_str="Non-Essential Instrumentation";
					}
					else if (pci->class_code==64){
						pci->class_code_str="Co-Processor";
					}
					console_log("PCI Device:\n  Bus:       %hhu\n  Device:    %hhu\n  Function:  %hhu\n  Class:     %hhu (%s)\n  Subclass:  %hhu (%s)\n  Interface: %hhu\n",b,d,f,pci->class_code,pci->class_code_str,pci->subclass,pci->subclass_str,pci->interface);
					driver_list_load(pci);
					pci++;
				}
			}
		}
	}
}
