import os
import re
import requests



OUT_NAME="rsrc/font_8x16.h"
MAX_CHAR=0x7e



dt=requests.get("https://raw.githubusercontent.com/fcambus/spleen/master/spleen-8x16.bdf",headers={"Accept":"application/vnd.github.v3+json","User-Agent":"Font Generation"}).content.replace(b"\r",b"\n").split(b"\n")
i=0
o=[0 for _ in range(0,(MAX_CHAR+1)*2)]
while (i<len(dt)):
	if (dt[i][:9]==b"STARTCHAR"):
		i+=1
		id_=-1
		j=-1
		while (dt[i]!=b"ENDCHAR"):
			if (dt[i][:8]==b"ENCODING"):
				id_=int(dt[i][8:].strip())
				if (id_>MAX_CHAR):
					while (dt[i]!=b"ENDCHAR"):
						i+=1
					break
			elif (dt[i][:6]==b"BITMAP"):
				j=0
			elif (j!=-1):
				o[id_*2+j//8]|=int(bin(int(dt[i],16))[2:].rjust(8,'0')[::-1],2)<<(8*(j%8))
				j+=1
			i+=1
	i+=1
with open(OUT_NAME,"w") as f:
	f.write(f"#ifndef __{OUT_NAME.replace(chr(92),'/').replace('/','_').split('.')[0].upper()}_FONT_H__\n#define __{OUT_NAME.replace(chr(92),'/').replace('/','_').split('.')[0].upper()}_FONT_H__ 1\n#include <font.h>\n#include <stdint.h>\n\n\n\nconst static uint64_t {OUT_NAME.replace(chr(92),'/').split('/')[-1].split('.')[0].upper()}_FONT_DATA[{(MAX_CHAR+1)*2}]={{\n\t{(','+chr(10)+chr(9)).join(['0x'+hex(e)[2:].rjust(16,'0') for e in o])}\n}};\n\n\n\nFont {OUT_NAME.replace(chr(92),'/').split('/')[-1].split('.')[0].upper()}_FONT={{\n\t{MAX_CHAR},\n\t{OUT_NAME.replace(chr(92),'/').split('/')[-1].split('.')[0].upper()}_FONT_DATA\n}};\n\n\n\n#endif\n")
