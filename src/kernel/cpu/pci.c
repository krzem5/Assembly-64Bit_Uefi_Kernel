#include <shared.h>
#include <cpu/pci.h>
#include <cpu/ports.h>
#include <gfx/console.h>
#include <stddef.h>
#include <stdint.h>



pci_device_list_t pci_list={
	0,
	NULL
};



void KERNEL_CALL KERNEL_UNMAP_AFTER_LOAD pci_init(void){
	for (uint8_t b=0;b<255;b++){
		for (uint8_t d=0;d<32;d++){
			for (uint8_t f=0;f<8;f++){
				uint32_t p_dt=0x80000000|(b<<16)|(d<<11)|(f<<8);
				asm_port_out32(0xcf8,p_dt);
				uint32_t v=asm_port_in32(0xcfc);
				if (v!=0xffffffff){
					pci_list.l++;
					pci_device_t pci={
						v&0xffff,
						v>>16
					};
					asm_port_out32(0xcf8,p_dt|0x04);
					v=asm_port_in32(0xcfc);
					pci.command=v&0xffff;
					pci.status=v>>16;
					asm_port_out32(0xcf8,p_dt|0x08);
					v=asm_port_in32(0xcfc);
					pci.revision=v&0xff;
					pci.interface=(v>>8)&0xff;
					pci.subclass=(v>>16)&0xff;
					pci.class_=v>>24;
					asm_port_out32(0xcf8,p_dt|0x0c);
					v=asm_port_in32(0xcfc);
					pci.cache_line_size=v&0xff;
					pci.latency_timer=(v>>8)&0xff;
					pci.type=(v>>16)&0xff;
					pci.bist=v>>24;
					pci.class_str="-";
					pci.subclass_str="-";
					pci.interface_str="-";
					if (pci.class_==0){
						pci.class_str="Unclassified";
						if (pci.subclass==0){
							pci.subclass_str="Non-VGA-Compatible devices";
						}
						else if (pci.subclass==1){
							pci.subclass_str="VGA-Compatible devices";
						}
					}
					else if (pci.class_==1){
						pci.class_str="Mass Storage Controller";
						if (pci.subclass==0){
							pci.subclass_str="SCSI Bus Controller";
						}
						else if (pci.subclass==1){
							pci.subclass_str="IDE Controller";
							if (pci.interface==0){
								pci.interface_str="ISA Compatibility mode-only controller";
							}
							else if (pci.interface==5){
								pci.interface_str="PCI native mode-only controller";
							}
							else if (pci.interface==10){
								pci.interface_str="ISA Compatibility mode controller, supports both channels switched to PCI native mode";
							}
							else if (pci.interface==15){
								pci.interface_str="PCI native mode controller, supports both channels switched to ISA compatibility mode";
							}
							else if (pci.interface==128){
								pci.interface_str="ISA Compatibility mode-only controller, supports bus mastering";
							}
							else if (pci.interface==133){
								pci.interface_str="PCI native mode-only controller, supports bus mastering";
							}
							else if (pci.interface==138){
								pci.interface_str="ISA Compatibility mode controller, supports both channels switched to PCI native mode, supports bus mastering";
							}
							else if (pci.interface==143){
								pci.interface_str="PCI native mode controller, supports both channels switched to ISA compatibility mode, supports bus mastering";
							}
						}
						else if (pci.subclass==2){
							pci.subclass_str="Floppy Disk Controller";
						}
						else if (pci.subclass==3){
							pci.subclass_str="IPI Bus Controller";
						}
						else if (pci.subclass==4){
							pci.subclass_str="RAID Controller";
						}
						else if (pci.subclass==5){
							pci.subclass_str="ATA Controller";
							if (pci.interface==32){
								pci.interface_str="Single DMA";
							}
							else if (pci.interface==48){
								pci.interface_str="Chained DMA";
							}
						}
						else if (pci.subclass==6){
							pci.subclass_str="Serial ATA";
							if (pci.interface==0){
								pci.interface_str="Vendor Specific Interface";
							}
							else if (pci.interface==1){
								pci.interface_str="AHCI 1.0";
							}
							else if (pci.interface==2){
								pci.interface_str="Serial Storage Bus";
							}
						}
						else if (pci.subclass==7){
							pci.subclass_str="Serial Attached SCSI";
							if (pci.interface==0){
								pci.interface_str="SAS";
							}
							else if (pci.interface==1){
								pci.interface_str="Serial Storage Bus";
							}
						}
						else if (pci.subclass==8){
							pci.subclass_str="Non-Volatile Memory Controller";
							if (pci.interface==1){
								pci.interface_str="NVMHCI";
							}
							else if (pci.interface==2){
								pci.interface_str="NVM Express";
							}
						}
					}
					else if (pci.class_==2){
						pci.class_str="Network Controller";
						if (pci.subclass==0){
							pci.subclass_str="Ethernet Controller";
						}
						else if (pci.subclass==1){
							pci.subclass_str="Token Ring Controller";
						}
						else if (pci.subclass==2){
							pci.subclass_str="FDDI Controller";
						}
						else if (pci.subclass==3){
							pci.subclass_str="ATM Controller";
						}
						else if (pci.subclass==4){
							pci.subclass_str="ISDN Controller";
						}
						else if (pci.subclass==5){
							pci.subclass_str="WorldFip Controller";
						}
						else if (pci.subclass==6){
							pci.subclass_str="PICMG 2.14 Multi Computing";
						}
						else if (pci.subclass==7){
							pci.subclass_str="Infiniband Controller";
						}
						else if (pci.subclass==8){
							pci.subclass_str="Fabric Controller";
						}
					}
					else if (pci.class_==3){
						pci.class_str="Display Controller";
						if (pci.subclass==0){
							pci.subclass_str="VGA Compatible Controller";
							if (pci.interface==0){
								pci.interface_str="VGA Controller";
							}
							else if (pci.interface==1){
								pci.interface_str="8514-Compatible Controller";
							}
						}
						else if (pci.subclass==1){
							pci.subclass_str="XGA Controller";
						}
						else if (pci.subclass==2){
							pci.subclass_str="3D Controller";
						}
					}
					else if (pci.class_==4){
						pci.class_str="Multimedia Controller";
						if (pci.subclass==0){
							pci.subclass_str="Multimedia Video Controller";
						}
						else if (pci.subclass==1){
							pci.subclass_str="Multimedia Audio Controller";
						}
						else if (pci.subclass==2){
							pci.subclass_str="Computer Telephony Device";
						}
						else if (pci.subclass==3){
							pci.subclass_str="Audio Device";
						}
					}
					else if (pci.class_==5){
						pci.class_str="Memory Controller";
						if (pci.subclass==0){
							pci.subclass_str="RAM Controller";
						}
						else if (pci.subclass==1){
							pci.subclass_str="Flash Controller";
						}
					}
					else if (pci.class_==6){
						pci.class_str="Bridge Controller";
						if (pci.subclass==0){
							pci.subclass_str="Host Bridge";
						}
						else if (pci.subclass==1){
							pci.subclass_str="ISA Bridge";
						}
						else if (pci.subclass==2){
							pci.subclass_str="EISA Bridge";
						}
						else if (pci.subclass==3){
							pci.subclass_str="MCA Bridge";
						}
						else if (pci.subclass==4){
							pci.subclass_str="PCI-to-PCI Bridge";
							if (pci.interface==0){
								pci.interface_str="Normal Decode";
							}
							else if (pci.interface==1){
								pci.interface_str="Subtractive Decode";
							}
						}
						else if (pci.subclass==5){
							pci.subclass_str="PCMCIA Bridge";
						}
						else if (pci.subclass==6){
							pci.subclass_str="NuBus Bridge";
						}
						else if (pci.subclass==7){
							pci.subclass_str="CardBus Bridge";
						}
						else if (pci.subclass==8){
							pci.subclass_str="RACEway Bridge";
							if (pci.interface==0){
								pci.interface_str="Transparent Mode";
							}
							else if (pci.interface==1){
								pci.interface_str="Endpoint Mode";
							}
						}
						else if (pci.subclass==9){
							pci.subclass_str="PCI-to-PCI Bridge";
							if (pci.interface==64){
								pci.interface_str="Semi-Transparent, Primary bus towards host CPU";
							}
							else if (pci.interface==128){
								pci.interface_str="Semi-Transparent, Secondary bus towards host CPU";
							}
						}
						else if (pci.subclass==10){
							pci.subclass_str="InfiniBand-to-PCI Host Bridge";
						}
					}
					else if (pci.class_==7){
						pci.class_str="Simple Communication Controller";
						if (pci.subclass==0){
							pci.subclass_str="Serial Controller";
							if (pci.interface==0){
								pci.interface_str="8250-Compatible (Generic XT)";
							}
							else if (pci.interface==1){
								pci.interface_str="16450-Compatible";
							}
							else if (pci.interface==2){
								pci.interface_str="16550-Compatible";
							}
							else if (pci.interface==3){
								pci.interface_str="16650-Compatible";
							}
							else if (pci.interface==4){
								pci.interface_str="16750-Compatible";
							}
							else if (pci.interface==5){
								pci.interface_str="16850-Compatible";
							}
							else if (pci.interface==6){
								pci.interface_str="16950-Compatible";
							}
						}
						else if (pci.subclass==1){
							pci.subclass_str="Parallel Controller";
							if (pci.interface==0){
								pci.interface_str="Standard Parallel Port";
							}
							else if (pci.interface==1){
								pci.interface_str="Bi-Directional Parallel Port";
							}
							else if (pci.interface==2){
								pci.interface_str="ECP 1.X Compliant Parallel Port";
							}
							else if (pci.interface==3){
								pci.interface_str="IEEE 1284 Controller";
							}
							else if (pci.interface==254){
								pci.interface_str="IEEE 1284 Target Device";
							}
						}
						else if (pci.subclass==2){
							pci.subclass_str="Multiport Serial Controller";
						}
						else if (pci.subclass==3){
							pci.subclass_str="Modem";
							if (pci.interface==0){
								pci.interface_str="Generic Modem";
							}
							else if (pci.interface==1){
								pci.interface_str="Hayes 16450-Compatible Interface";
							}
							else if (pci.interface==2){
								pci.interface_str="Hayes 16550-Compatible Interface";
							}
							else if (pci.interface==3){
								pci.interface_str="Hayes 16650-Compatible Interface";
							}
							else if (pci.interface==4){
								pci.interface_str="Hayes 16750-Compatible Interface";
							}
						}
						else if (pci.subclass==4){
							pci.subclass_str="IEEE 488.1/2 (GPIB) Controller";
						}
						else if (pci.subclass==5){
							pci.subclass_str="Smart Card";
						}
					}
					else if (pci.class_==8){
						pci.class_str="Base System Peripheral";
						if (pci.subclass==0){
							pci.subclass_str="PIC";
							if (pci.interface==0){
								pci.interface_str="Generic 8259-Compatible";
							}
							else if (pci.interface==1){
								pci.interface_str="ISA-Compatible";
							}
							else if (pci.interface==2){
								pci.interface_str="EISA-Compatible";
							}
							else if (pci.interface==16){
								pci.interface_str="I/O APIC Interrupt Controller";
							}
							else if (pci.interface==32){
								pci.interface_str="I/O(x) APIC Interrupt Controller";
							}
						}
						else if (pci.subclass==1){
							pci.subclass_str="DMA Controller";
							if (pci.interface==0){
								pci.interface_str="Generic 8237-Compatible";
							}
							else if (pci.interface==1){
								pci.interface_str="ISA-Compatible";
							}
							else if (pci.interface==2){
								pci.interface_str="EISA-Compatible";
							}
						}
						else if (pci.subclass==2){
							pci.subclass_str="Timer";
							if (pci.interface==0){
								pci.interface_str="Generic 8254-Compatible";
							}
							else if (pci.interface==1){
								pci.interface_str="ISA-Compatible";
							}
							else if (pci.interface==2){
								pci.interface_str="EISA-Compatible";
							}
							else if (pci.interface==3){
								pci.interface_str="HPET";
							}
						}
						else if (pci.subclass==3){
							pci.subclass_str="RTC Controller";
							if (pci.interface==0){
								pci.interface_str="Generic RTC";
							}
							else if (pci.interface==1){
								pci.interface_str="ISA-Compatible";
							}
						}
						else if (pci.subclass==4){
							pci.subclass_str="PCI Hot-Plug Controller";
						}
						else if (pci.subclass==5){
							pci.subclass_str="SD Host controller";
						}
						else if (pci.subclass==6){
							pci.subclass_str="IOMMU";
						}
					}
					else if (pci.class_==9){
						pci.class_str="Input Device Controller";
						if (pci.subclass==0){
							pci.subclass_str="Keyboard Controller";
						}
						else if (pci.subclass==1){
							pci.subclass_str="Digitizer Pen";
						}
						else if (pci.subclass==2){
							pci.subclass_str="Mouse Controller";
						}
						else if (pci.subclass==3){
							pci.subclass_str="Scanner Controller";
						}
						else if (pci.subclass==4){
							pci.subclass_str="Gameport Controller";
							if (pci.interface==0){
								pci.interface_str="Generic";
							}
							else if (pci.interface==16){
								pci.interface_str="Extended";
							}
						}
					}
					else if (pci.class_==10){
						pci.class_str="Docking Station";
						if (pci.subclass==0){
							pci.subclass_str="Generic";
						}
					}
					else if (pci.class_==11){
						pci.class_str="Processor";
						if (pci.subclass==0){
							pci.subclass_str="386";
						}
						else if (pci.subclass==1){
							pci.subclass_str="486";
						}
						else if (pci.subclass==2){
							pci.subclass_str="Pentium";
						}
						else if (pci.subclass==3){
							pci.subclass_str="Pentium Pro";
						}
						else if (pci.subclass==4){
							pci.subclass_str="Alpha";
						}
						else if (pci.subclass==5){
							pci.subclass_str="PowerPC";
						}
						else if (pci.subclass==6){
							pci.subclass_str="MIPS";
						}
						else if (pci.subclass==7){
							pci.subclass_str="Co-Processor";
						}
					}
					else if (pci.class_==12){
						pci.class_str="Serial Bus Controller";
						if (pci.subclass==0){
							pci.subclass_str="FireWire (IEEE 1394) Controller";
							if (pci.interface==0){
								pci.interface_str="Generic";
							}
							else if (pci.interface==16){
								pci.interface_str="OHCI";
							}
						}
						else if (pci.subclass==1){
							pci.subclass_str="ACCESS Bus";
						}
						else if (pci.subclass==2){
							pci.subclass_str="SSA";
						}
						else if (pci.subclass==3){
							pci.subclass_str="USB Controller";
							if (pci.interface==0){
								pci.interface_str="UHCI Controller";
							}
							else if (pci.interface==16){
								pci.interface_str="OHCI Controller";
							}
							else if (pci.interface==32){
								pci.interface_str="EHCI (USB2) Controller";
							}
							else if (pci.interface==48){
								pci.interface_str="XHCI (USB3) Controller";
							}
							else if (pci.interface==254){
								pci.interface_str="USB Device (Not a host controller)";
							}
						}
						else if (pci.subclass==4){
							pci.subclass_str="Fibre Channel";
						}
						else if (pci.subclass==5){
							pci.subclass_str="SMBus";
						}
						else if (pci.subclass==6){
							pci.subclass_str="InfiniBand";
						}
						else if (pci.subclass==7){
							pci.subclass_str="IPMI Interface";
							if (pci.interface==0){
								pci.interface_str="SMIC";
							}
							else if (pci.interface==1){
								pci.interface_str="Keyboard Controller Style";
							}
							else if (pci.interface==2){
								pci.interface_str="Block Transfer";
							}
						}
						else if (pci.subclass==8){
							pci.subclass_str="SERCOS Interface (IEC 61491)";
						}
						else if (pci.subclass==9){
							pci.subclass_str="CANbus";
						}
					}
					else if (pci.class_==13){
						pci.class_str="Wireless Controller";
						if (pci.subclass==0){
							pci.subclass_str="iRDA Compatible Controller";
						}
						else if (pci.subclass==1){
							pci.subclass_str="Consumer IR Controller";
						}
						else if (pci.subclass==2){
							pci.subclass_str="RF Controller";
						}
						else if (pci.subclass==3){
							pci.subclass_str="Bluetooth Controller";
						}
						else if (pci.subclass==4){
							pci.subclass_str="Broadband Controller";
						}
						else if (pci.subclass==5){
							pci.subclass_str="Ethernet Controller (802.1a)";
						}
						else if (pci.subclass==6){
							pci.subclass_str="Ethernet Controller (802.1b)";
						}
					}
					else if (pci.class_==14){
						pci.class_str="Intelligent Controller";
						if (pci.subclass==0){
							pci.subclass_str="I20";
						}
					}
					else if (pci.class_==15){
						pci.class_str="Satellite Communication Controller";
						if (pci.subclass==0){
							pci.subclass_str="Satellite TV Controller";
						}
						else if (pci.subclass==1){
							pci.subclass_str="Satellite Audio Controller";
						}
						else if (pci.subclass==2){
							pci.subclass_str="Satellite Voice Controller";
						}
						else if (pci.subclass==3){
							pci.subclass_str="Satellite Data Controller";
						}
					}
					else if (pci.class_==16){
						pci.class_str="Encryption Controller";
						if (pci.subclass==0){
							pci.subclass_str="Network and Computing Encrpytion/Decryption";
						}
						else if (pci.subclass==1){
							pci.subclass_str="Entertainment Encryption/Decryption";
						}
					}
					else if (pci.class_==17){
						pci.class_str="Signal Processing Controller";
						if (pci.subclass==0){
							pci.subclass_str="DPIO Modules";
						}
						else if (pci.subclass==1){
							pci.subclass_str="Performance Counters";
						}
						else if (pci.subclass==2){
							pci.subclass_str="Communication Synchronizer";
						}
						else if (pci.subclass==3){
							pci.subclass_str="Signal Processing Management";
						}
					}
					else if (pci.class_==18){
						pci.class_str="Processing Accelerator";
					}
					else if (pci.class_==19){
						pci.class_str="Non-Essential Instrumentation";
					}
					else if (pci.class_==64){
						pci.class_str="Co-Processor";
					}
					console_log("PCI Device:\n  bus:       %hhu\n  device:    %hhu\n  function:  %hhu\n  class:     %hhu (%s)\n  subclass:  %hhu (%s)\n  interface: %hhu (%s)\n",b,d,f,pci.class_,pci.class_str,pci.subclass,pci.subclass_str,pci.interface,pci.interface_str);
				}
			}
		}
	}
}
