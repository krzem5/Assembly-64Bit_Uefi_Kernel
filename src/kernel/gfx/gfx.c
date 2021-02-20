#include <shared.h>
#include <font.h>
#include <gfx/gfx.h>
#include <kmain.h>
#include <stdint.h>
#include <stdlib.h>



#define SPACE_CHAR_ID 32



uint32_t* _gfx_vmem;
uint32_t* _gfx_vmem_bf;
uint64_t _gfx_vmem_l;
uint64_t _gfx_vmem_w;
uint64_t _gfx_vmem_h;



void KERNEL_CALL gfx_init(KernelArgs* ka){
	_gfx_vmem=ka->vmem;
	// _gfx_vmem_bf=malloc(ka->vmem_l*sizeof(uint32_t));
	_gfx_vmem_l=ka->vmem_l;
	_gfx_vmem_w=ka->vmem_w;
	_gfx_vmem_h=ka->vmem_h;
	for (uint64_t i=0;i<_gfx_vmem_l;i++){
		*(_gfx_vmem+i)=0;
	}
}



void KERNEL_CALL gfx_print_char(uint8_t c,uint64_t x,uint64_t y,color_t cl,Font f,uint8_t sc){
	uint64_t g=(c<=f.mx?f.dt[(uint16_t)c*2]:f.dt[(uint16_t)SPACE_CHAR_ID*2]);
	uint64_t i=x+y*_gfx_vmem_w;
	uint8_t j=0;
	while (j<64){
		if (sc==1){
			*(_gfx_vmem+i)=(g&1?cl:0);
		}
		else{
			color_t v=(g&1?cl:0);
			uint64_t k=i;
			for (uint8_t l=0;l<sc;l++){
				for (uint8_t m=0;m<sc;m++){
					*(_gfx_vmem+k+m)=v;
				}
				k+=_gfx_vmem_w;
			}
		}
		g>>=1;
		i+=sc;
		j++;
		if (j%8==0){
			i+=_gfx_vmem_w*sc-8*sc;
		}
	}
	g=(c<=f.mx?f.dt[(uint16_t)c*2+1]:f.dt[(uint16_t)SPACE_CHAR_ID*2+1]);
	i=x+(y+8*sc)*_gfx_vmem_w;
	j=0;
	while (j<64){
		if (sc==1){
			*(_gfx_vmem+i)=(g&1?cl:0);
		}
		else{
			color_t v=(g&1?cl:0);
			uint64_t k=i;
			for (uint8_t l=0;l<sc;l++){
				for (uint8_t m=0;m<sc;m++){
					*(_gfx_vmem+k+m)=v;
				}
				k+=_gfx_vmem_w;
			}
		}
		g>>=1;
		i+=sc;
		j++;
		if (j%8==0){
			i+=_gfx_vmem_w*sc-8*sc;
		}
	}
}



void KERNEL_CALL gfx_update_screen(void){
	//
}
