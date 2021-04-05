import requests



FONT_OUT_NAME="font_spleen"
FONT_URL="https://raw.githubusercontent.com/fcambus/spleen/master/spleen-8x16.bdf"
FONT_MAX_CHAR=0x7e



print(f"Font characters: {FONT_MAX_CHAR+1}\nFont URL: {FONT_URL}\nFont Files: rsrc/include/{FONT_OUT_NAME}.h, rsrc/{FONT_OUT_NAME}.c")
dt=requests.get(FONT_URL,headers={"Accept":"application/vnd.github.v3+json","User-Agent":"Font Generation"}).content.replace(b"\r",b"\n").split(b"\n")
i=0
o=[0 for _ in range(0,(FONT_MAX_CHAR+1)*2)]
while (i<len(dt)):
	if (dt[i][:9]==b"STARTCHAR"):
		i+=1
		id_=-1
		j=-1
		while (dt[i]!=b"ENDCHAR"):
			if (dt[i][:8]==b"ENCODING"):
				id_=int(dt[i][8:].strip())
				if (id_>FONT_MAX_CHAR):
					while (dt[i]!=b"ENDCHAR"):
						i+=1
					break
			elif (dt[i][:6]==b"BITMAP"):
				j=0
			elif (j!=-1):
				o[id_*2+j//8]|=int(bin(int(dt[i],16))[2:].rjust(8,"0")[::-1],2)<<(8*(j%8))
				j+=1
			i+=1
	i+=1
with open(f"rsrc/include/{FONT_OUT_NAME}.h","w") as f:
	f.write(f"#ifndef __{FONT_OUT_NAME.upper()}_H__\n#define __{FONT_OUT_NAME.upper()}_H__ 1\n#include <gfx/font.h>\n#include <stdint.h>\n\n\n\nextern const uint64_t {FONT_OUT_NAME.upper()}_DATA[];\n\n\n\nextern Font {FONT_OUT_NAME.upper()};\n\n\n\n#endif\n")
with open(f"rsrc/{FONT_OUT_NAME}.c","w") as f:
	f.write(f"#include <gfx/font.h>\n#include <stdint.h>\n#include <{FONT_OUT_NAME}.h>\n\n\n\nconst uint64_t {FONT_OUT_NAME.upper()}_DATA[{(FONT_MAX_CHAR+1)*2}]={{\n\t{(','+chr(10)+chr(9)).join(['0x'+hex(e)[2:].rjust(16,'0') for e in o])}\n}};\n\n\n\nFont {FONT_OUT_NAME.upper()}={{\n\t{FONT_MAX_CHAR},\n\t{FONT_OUT_NAME.upper()}_DATA\n}};\n")
