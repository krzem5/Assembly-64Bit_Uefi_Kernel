#include <_libc_internal.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



#define FLAGS_PAD_ZERO 1
#define FLAGS_SIGN 2
#define FLAGS_SPACE_SIGN 4
#define FLAGS_JUSTIFY_LEFT 8
#define FLAGS_ALTERNATIVE_FORM 16
#define FLAGS_PERCISION 32
#define FLAGS_HH_BITS 64
#define FLAGS_H_BITS 128
#define FLAGS_L_BITS 256
#define FLAGS_LL_BITS 512
#define FLAGS_UPPERCASE 1024



__vprintf_reserve_func_t __printf_rs;
__vprintf_write_func_t __printf_cb;



struct __VPRINTF_BUFFER_CTX{
	char** bf;
	uint64_t sz;
	uint64_t i;
};



int LIBC_CALL __vprintf_handle_sign(void* ctx,__vprintf_reserve_func_t rs,__vprintf_write_func_t cb,uint16_t f,bool n){
	if ((f&FLAGS_SIGN)||(f&FLAGS_SPACE_SIGN)||n){
		if (rs){
			rs(1,ctx);
		}
		if (cb){
			cb((n?'-':(f&FLAGS_SPACE_SIGN?' ':'+')),ctx);
		}
		return 1;
	}
	return 0;
}



int LIBC_CALL __vprintf_write_uint(void* ctx,__vprintf_reserve_func_t rs,__vprintf_write_func_t cb,uint16_t f,uint8_t b,uint64_t v,unsigned int w){
	if (!(f&FLAGS_PERCISION)){
		w=1;
	}
	if (!w&&(f&FLAGS_PERCISION)&&v==0){
		if ((f&FLAGS_ALTERNATIVE_FORM)&&b==8){
			if (w==0){
				w=1;
			}
			if (rs){
				rs(w,ctx);
			}
			int i=w;
			if (cb){
				w--;
				if (!(f&FLAGS_JUSTIFY_LEFT)){
					while (w){
						w--;
						cb('0',ctx);
					}
				}
				cb('0',ctx);
				if (f&FLAGS_JUSTIFY_LEFT){
					while (w){
						w--;
						cb('0',ctx);
					}
				}
			}
			return i;
		}
		if (w&&rs){
			rs(w,ctx);
		}
		int i=w;
		if (cb){
			while (w){
				w--;
				cb('0',ctx);
			}
		}
		return i;
	}
	int i=0;
	if ((f&FLAGS_ALTERNATIVE_FORM)&&b==8){
		if (rs){
			rs(1,ctx);
		}
		if (cb){
			cb('0',ctx);
		}
		i++;
		if (w>0){
			w--;
		}
	}
	else if ((f&FLAGS_ALTERNATIVE_FORM)&&b==16){
		if (rs){
			rs(2,ctx);
		}
		if (cb){
			cb('0',ctx);
			cb((f&FLAGS_UPPERCASE?'X':'x'),ctx);
		}
		i+=2;
		if (w>1){
			w-=2;
		}
		else{
			w=0;
		}
	}
	if (v==0){
		if (w==0){
			w=1;
		}
		if (rs){
			rs(w,ctx);
		}
		i+=w;
		if (cb){
			w--;
			if (!(f&FLAGS_JUSTIFY_LEFT)){
				while (w){
					w--;
					cb('0',ctx);
				}
			}
			cb('0',ctx);
			if (f&FLAGS_JUSTIFY_LEFT){
				while (w){
					w--;
					cb('0',ctx);
				}
			}
		}
		return i;
	}
	if (b==8){
		uint8_t sz=0;
		uint64_t nv=v;
		while (nv>0){
			nv>>=3;
			sz++;
		}
		if (w<sz){
			w=sz;
		}
		if (rs){
			rs(w,ctx);
		}
		i+=w;
		if (cb){
			w-=sz;
			if (!(f&FLAGS_JUSTIFY_LEFT)){
				while (w){
					w--;
					cb('0',ctx);
				}
			}
			for (uint8_t j=sz*3;j>0;){
				j-=3;
				cb(48+((v>>j)&7),ctx);
			}
			if (f&FLAGS_JUSTIFY_LEFT){
				while (w){
					w--;
					cb('0',ctx);
				}
			}
		}
		return i;
	}
	if (b==10){
		uint8_t sz=1;
		uint64_t pw=10;
		while (pw<v+1){
			sz++;
			if (sz==20){
				break;
			}
			pw*=10;
		}
		if (w<sz){
			w=sz;
		}
		if (rs){
			rs(w,ctx);
		}
		i+=w;
		if (cb){
			w-=sz;
			if (!(f&FLAGS_JUSTIFY_LEFT)){
				while (w){
					w--;
					cb('0',ctx);
				}
			}
			if (sz!=20){
				pw/=10;
			}
			while (sz>0){
				cb(48+(v/pw)%10,ctx);
				sz--;
				pw/=10;
			}
			if (f&FLAGS_JUSTIFY_LEFT){
				while (w){
					w--;
					cb('0',ctx);
				}
			}
		}
		return i;
	}
	uint8_t sz=0;
	uint64_t nv=v;
	while (nv>0){
		nv>>=4;
		sz++;
	}
	if (w<sz){
		w=sz;
	}
	if (rs){
		rs(w,ctx);
	}
	i+=w;
	if (cb){
		w-=sz;
		if (!(f&FLAGS_JUSTIFY_LEFT)){
			while (w){
				w--;
				cb('0',ctx);
			}
		}
		for (uint8_t j=sz*4;j>0;){
			j-=4;
			uint8_t c=(v>>j)&0xf;
			cb(48+(c>9?(f&FLAGS_UPPERCASE?7:39):0)+c,ctx);
		}
		if (f&FLAGS_JUSTIFY_LEFT){
			while (w){
				w--;
				cb('0',ctx);
			}
		}
	}
	return i;
}



void LIBC_CALL __vprintf_buffer_reserve_func(uint64_t sz,void* ctx){
	((struct __VPRINTF_BUFFER_CTX*)ctx)->sz+=sz;
	*((struct __VPRINTF_BUFFER_CTX*)ctx)->bf=realloc(*((struct __VPRINTF_BUFFER_CTX*)ctx)->bf,((struct __VPRINTF_BUFFER_CTX*)ctx)->sz);
}



void LIBC_CALL __vprintf_buffer_write_func(char c,void* ctx){
	*(*((struct __VPRINTF_BUFFER_CTX*)ctx)->bf+((struct __VPRINTF_BUFFER_CTX*)ctx)->i)=c;
	((struct __VPRINTF_BUFFER_CTX*)ctx)->i++;
}



int LIBC_CALL __vprintf_buffer(char** bf,const char* t,va_list v){
	*bf=NULL;
	struct __VPRINTF_BUFFER_CTX ctx={
		bf,
		0,
		0
	};
	return __vprintf_raw((void*)&ctx,__vprintf_buffer_reserve_func,__vprintf_buffer_write_func,t,v);
}



int LIBC_CALL __vprintf_raw(void* ctx,__vprintf_reserve_func_t rs,__vprintf_write_func_t cb,const char* t,va_list v){
	if (!rs&&!cb){
		return 0;
	}
	size_t i=0;
	while (*t){
		if (*t!='%'){
			if (rs){
				rs(1,ctx);
			}
			if (cb){
				cb(*t,ctx);
			}
			i++;
			t++;
			continue;
		}
		const char* _st=t;
		t++;
		uint16_t f=0;
		while (true){
			if (*t=='0'){
				f|=FLAGS_PAD_ZERO;
			}
			else if (*t=='+'){
				f|=FLAGS_SIGN;
			}
			else if (*t==' '){
				f|=FLAGS_SPACE_SIGN;
			}
			else if (*t=='-'){
				f|=FLAGS_JUSTIFY_LEFT;
			}
			else if (*t=='#'){
				f|=FLAGS_ALTERNATIVE_FORM;
			}
			else{
				break;
			}
			t++;
		}
		unsigned int w=0;
		unsigned int p=0;
		if (*t=='*'){
			int a=va_arg(v,int);
			if (a<0){
				f|=FLAGS_JUSTIFY_LEFT;
				a=-a;
			}
			w=(unsigned int)a;
			t++;
		}
		else{
			while (48<=*t&&*t<58){
				w=w*10+(*t-48);
				t++;
			}
		}
		if (*t=='.'){
			f|=FLAGS_PERCISION;
			t++;
			if (*t=='*'){
				int a=va_arg(v,int);
				if (a<0){
					a=0;
				}
				p=(unsigned int)a;
				t++;
			}
			else{
				while (48<=*t&&*t<58){
					p=p*10+(*t-48);
					t++;
				}
			}
		}
		if (*t=='h'){
			t++;
			if (*t=='h'){
				f|=FLAGS_HH_BITS;
				t++;
			}
			else{
				f|=FLAGS_H_BITS;
			}
		}
		else if (*t=='l'){
			t++;
			if (*t=='l'){
				f|=FLAGS_LL_BITS;
				t++;
			}
			else{
				f|=FLAGS_L_BITS;
			}
		}
		else if (*t=='j'){
			f|=(sizeof(uintmax_t)==sizeof(unsigned long long int)?FLAGS_LL_BITS:FLAGS_L_BITS);
			t++;
		}
		else if (*t=='z'){
			f|=(sizeof(size_t)==sizeof(unsigned long long int)?FLAGS_LL_BITS:FLAGS_L_BITS);
			t++;
		}
		else if (*t=='t'){
			f|=(sizeof(ptrdiff_t)==sizeof(signed long long int)?FLAGS_LL_BITS:FLAGS_L_BITS);
			t++;
		}
		if (*t=='%'){
			if (rs){
				rs(1,ctx);
			}
			if (cb){
				cb('%',ctx);
			}
			i++;
		}
		else if (*t=='c'){
			if (w==0){
				w=1;
			}
			if (rs){
				rs(w,ctx);
			}
			if (cb){
				w--;
				if (!(f&FLAGS_JUSTIFY_LEFT)){
					while (w>0){
						w--;
						cb(' ',ctx);
					}
				}
				cb((char)va_arg(v,int),ctx);
				if (f&FLAGS_JUSTIFY_LEFT){
					while (w>0){
						w--;
						cb(' ',ctx);
					}
				}
			}
			i++;
		}
		else if (*t=='s'){
			char* s=va_arg(v,char*);
			if (s==NULL){
				if (rs){
					rs(6,ctx);
				}
				if (cb){
					cb('(',ctx);
					cb('n',ctx);
					cb('u',ctx);
					cb('l',ctx);
					cb('l',ctx);
					cb(')',ctx);
				}
				i+=6;
			}
			else{
				size_t l=p;
				if (!(f&FLAGS_PERCISION)){
					l=0;
					while (*(s+l)){
						l++;
					}
				}
				if (rs){
					rs((l>w?l:w),ctx);
				}
				i+=(l>w?l:w);
				if (w<l){
					w=0;
				}
				else{
					w-=l;
				}
				if (cb){
					if (!(f&FLAGS_JUSTIFY_LEFT)){
						while (w>0){
							w--;
							cb(' ',ctx);
						}
					}
					size_t j=l;
					while (j){
						cb(*s,ctx);
						j--;
						s++;
					}
					if (f&FLAGS_JUSTIFY_LEFT){
						while (w>0){
							w--;
							cb(' ',ctx);
						}
					}
				}
			}
		}
		else if (*t=='d'||*t=='i'||*t=='o'||*t=='x'||*t=='X'||*t=='u'){
			if (*t=='X'){
				f|=FLAGS_UPPERCASE;
			}
			uint64_t n;
			if (*t=='d'||*t=='i'){
				int64_t sn;
				if (f&FLAGS_HH_BITS){
					sn=(signed char)va_arg(v,signed int);
				}
				else if (f&FLAGS_H_BITS){
					sn=(signed short int)va_arg(v,signed int);
				}
				else if (f&FLAGS_L_BITS){
					sn=(signed long int)va_arg(v,signed long int);
				}
				else if (f&FLAGS_LL_BITS){
					sn=va_arg(v,signed long long int);
				}
				else{
					sn=va_arg(v,signed int);
				}
				i+=__vprintf_handle_sign(ctx,rs,cb,f,(sn<0));
				n=(uint64_t)(sn<0?-sn:sn);
			}
			else{
				if (f&FLAGS_HH_BITS){
					n=(unsigned char)va_arg(v,unsigned int);
				}
				else if (f&FLAGS_H_BITS){
					n=(unsigned short int)va_arg(v,unsigned int);
				}
				else if (f&FLAGS_L_BITS){
					n=va_arg(v,unsigned long int);
				}
				else if (f&FLAGS_LL_BITS){
					n=va_arg(v,unsigned long long int);
				}
				else{
					n=va_arg(v,unsigned int);
				}
				i+=__vprintf_handle_sign(ctx,rs,cb,f,false);
			}
			i+=__vprintf_write_uint(ctx,rs,cb,f,(*t=='o'?8:(*t=='x'||*t=='X'?16:10)),n,p);
		}
		else if (*t=='p'){
			i+=sizeof(uintptr_t)*2;
			if (rs){
#ifdef __KERNEL__
				rs(sizeof(uintptr_t)*2+1,ctx);
#else
				rs(sizeof(uintptr_t)*2,ctx);
#endif
			}
			uintptr_t p=(uintptr_t)va_arg(v,void*);
			if (cb){
				for (int8_t j=sizeof(uintptr_t)*8-4;j>=0;j-=4){
#ifdef __KERNEL__
					if (j==28){
						cb('_',ctx);
					}
#endif
					uint8_t v=(p>>j)%16;
					cb(48+(v>9?39:0)+v,ctx);
				}
			}
		}
		else{
			t=_st;
			if (rs){
				rs(1,ctx);
			}
			if (cb){
				cb('%',ctx);
			}
			i++;
		}
		t++;
	}
	if (rs){
		rs(1,ctx);
	}
	if (cb){
		cb(0,ctx);
	}
	return i+1;
}



int LIBC_CALL printf(const char* t,...){
	va_list v;
	va_start(v,t);
	int o=__vprintf_raw(NULL,__printf_rs,__printf_cb,t,v);
	va_end(v);
	return o;
}
