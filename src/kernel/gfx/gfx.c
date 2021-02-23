#include <shared.h>
#include <font.h>
#include <gfx/gfx.h>
#include <kmain.h>
#include <stdint.h>



color_t* _gfx_vmem;
uint64_t _gfx_vmem_l;
uint64_t _gfx_vmem_w;
uint64_t _gfx_vmem_h;



void KERNEL_CALL gfx_init(KernelArgs* ka){
	_gfx_vmem=ka->vmem;
	_gfx_vmem_l=ka->vmem_l;
	_gfx_vmem_w=ka->vmem_w;
	_gfx_vmem_h=ka->vmem_h;
	for (uint64_t i=0;i<_gfx_vmem_l;i++){
		*(_gfx_vmem+i)=0;
	}
}



void KERNEL_CALL gfx_print_char(uint8_t c,uint64_t x,uint64_t y,color_t cl,Font f,uint8_t sc){
	if (c>f.mx){
		gfx_fill_rect(x,y,sc*16,sc*8,COLOR(0,0,0));
		return;
	}
	uint64_t g=f.dt[(uint16_t)c*2];
	uint64_t i=x+y*_gfx_vmem_w;
	uint8_t j=0;
	while (j<64){
		if (sc==1){
			*(_gfx_vmem+i)=(g&1?cl:COLOR(0,0,0));
		}
		else{
			color_t v=(g&1?cl:COLOR(0,0,0));
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
	g=f.dt[(uint16_t)c*2+1];
	i=x+(y+8*sc)*_gfx_vmem_w;
	j=0;
	while (j<64){
		if (sc==1){
			*(_gfx_vmem+i)=(g&1?cl:COLOR(0,0,0));
		}
		else{
			color_t v=(g&1?cl:COLOR(0,0,0));
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



void KERNEL_CALL gfx_fill_rect(uint64_t x,uint64_t y,uint64_t w,uint64_t h,color_t cl){
	w+=x;
	h+=y;
	for (;y<h;y++){
		uint64_t i=x+y*_gfx_vmem_w;
		for (uint64_t j=x;j<w;j++){
			*(_gfx_vmem+i)=cl;
			i++;
		}
	}
}
