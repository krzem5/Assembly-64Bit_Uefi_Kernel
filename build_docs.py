import os
import re



INCLUDE_FILE_REGEX=re.compile(br"^\s*#\s*include\s*<([^>]*?)>$",re.M)
REQUIRED_STRUCTURE_OFFSETS={}
REQUIRED_STRUCTURE_SIZE=[]
REQUIRED_DEFINITIONS=[]
REQUIRED_TYPE_SIZE=[]
with open("exports.txt","rb") as f:
	c=None
	c_s=None
	c_s_i=None
	indt=[b""]
	for k in f.read().replace(b"\r\n",b"\n").split(b"\n"):
		c_i=b""
		while (k[:1] in b" \t\r\n\v\f"):
			c_i+=k[:1]
			k=k[1:]
			if (len(k)==0):
				break
		if (len(k)==0):
			break
		if (len(c_i)==0 and k in [b"structures:",b"defs:",b"sizes:"]):
			c=k[:-1]
		else:
			if (c==b"structures"):
				if (c_s==None or len(c_i)==len(c_s_i)):
					c_s=k[:-1]
					c_s_i=c_i
				else:
					if (k==b"$$size$$"):
						REQUIRED_STRUCTURE_SIZE.append(c_s)
					else:
						if (c_s not in REQUIRED_STRUCTURE_OFFSETS):
							REQUIRED_STRUCTURE_OFFSETS[c_s]=[k]
						else:
							REQUIRED_STRUCTURE_OFFSETS[c_s].append(k)
			elif (c==b"defs"):
				REQUIRED_DEFINITIONS.append(k)
			else:
				REQUIRED_TYPE_SIZE.append(k)



src_fl=list(os.walk("src"))+list(os.walk("rsrc"))
f_dt={}
asm_inc={}
for k,v in REQUIRED_STRUCTURE_OFFSETS.items():
	for e in v:
		asm_inc[f"__C_{str(k,'utf-8').strip('_').upper()}_STRUCT_{str(e,'utf-8').upper()}_OFFSET__"]=[[],[]]
for k in REQUIRED_STRUCTURE_SIZE:
	asm_inc[f"__C_{str(k,'utf-8').strip('_').upper()}_STRUCT_SIZE__"]=[[],[]]
for k in REQUIRED_DEFINITIONS:
	asm_inc[f"__C_{str(k,'utf-8').strip('_').upper()}__"]=[[],[]]
for r,_,fl in src_fl:
	r=r.replace("\\","/")+"/"
	for f in fl:
		if (f[-2:]==".c" or f[-2:]==".h"):
			with open(r+f,"rb") as rf:
				dt=rf.read()
			f_dt[r+f]={"type":("Header" if f[-1]=="h" else "Source"),"size":len(dt.replace(b"\r\n",b"\n")),"loc":0,"sloc":0,"refs":[],"refs_far":[]}
			for k in dt.split(b"\n"):
				f_dt[r+f]["loc"]+=1
				if (len(k.strip())>0):
					f_dt[r+f]["sloc"]+=1
			for e in INCLUDE_FILE_REGEX.findall(dt):
				e=str(e.replace(b"\\",b"/").split(b"/")[-1],"utf-8")
				for tr,_,tfl in src_fl:
					tr=tr.replace("\\","/")+"/"
					for tf in tfl:
						if ((tr+tf).replace("\\","/").split("/")[-1]==e):
							f_dt[r+f]["refs"].append((tr+tf))
			if (f[-1]=="h"):
				for k in set(REQUIRED_STRUCTURE_OFFSETS.keys())|set(REQUIRED_STRUCTURE_SIZE):
					m=re.search(br"struct\s+"+k+br"\s*\{",dt)
					if (m is not None):
						i=m.end()
						while (True):
							if (dt[i:i+1] in b" \t\r\n\v\f"):
								i+=1
								continue
							e=b""
							while (dt[i:i+1]!=b";"):
								e+=dt[i:i+1]
								i+=1
							i+=1
							if (e[0:1]==b"}"):
								break
							if (k in REQUIRED_STRUCTURE_OFFSETS):
								nm=e.split(b" ")[-1].replace(b"[]",b"")
								if (nm in REQUIRED_STRUCTURE_OFFSETS[k]):
									asm_inc[f"__C_{str(k,'utf-8').strip('_').upper()}_STRUCT_{str(nm,'utf-8').upper()}_OFFSET__"][0].append(r+f)
									REQUIRED_STRUCTURE_OFFSETS[k].remove(nm)
									if (len(REQUIRED_STRUCTURE_OFFSETS[k])==0 and k not in REQUIRED_STRUCTURE_SIZE):
										break
						if (k in REQUIRED_STRUCTURE_SIZE):
							asm_inc[f"__C_{str(k,'utf-8').strip('_').upper()}_STRUCT_SIZE__"][0]+=[r+f]
							REQUIRED_STRUCTURE_SIZE.remove(k)
						if (k in REQUIRED_STRUCTURE_OFFSETS):
							del REQUIRED_STRUCTURE_OFFSETS[k]
			for k in REQUIRED_DEFINITIONS:
				if (len(re.findall(br"\#define\s+"+k+br"\s*(.*)$",dt,re.M))==1):
					asm_inc[f"__C_{str(k,'utf-8').strip('_').upper()}__"][0].append(r+f)
		elif (f[-4:]==".asm"):
			with open(r+f,"r") as rf:
				dt=rf.read()
			f_dt[r+f]={"type":"Assembly Source","size":len(dt.replace("\r\n","\n")),"loc":0,"sloc":0,"refs":[],"refs_far":[]}
			for k in dt.split("\n"):
				f_dt[r+f]["loc"]+=1
				if (len(k.strip())>0):
					f_dt[r+f]["sloc"]+=1
			for k in asm_inc:
				if (k in dt):
					asm_inc[k][1].append(r+f)
for k,v in asm_inc.items():
	for e in v[1]:
		for se in v[0]:
			if (se not in f_dt[e]["refs"]):
				f_dt[e]["refs"].append(se)
while (True):
	u=False
	for k,v in f_dt.items():
		for e in v["refs"]+v["refs_far"]:
			for se in f_dt[e]["refs"]+f_dt[e]["refs_far"]:
				if (se not in v["refs"] and se not in v["refs_far"]):
					u=True
					v["refs_far"].append(se)
	if (not u):
		break
dt={"total":{"size":0,"loc":0,"sloc":0},"files":{}}
for k,v in sorted(f_dt.items(),key=lambda e:e[0]):
	dt["total"]["size"]+=v["size"]
	dt["total"]["loc"]+=v["loc"]
	dt["total"]["sloc"]+=v["sloc"]
	dt["files"][k]={"type":v["type"],"size":v["size"],"loc":v["loc"],"sloc":v["sloc"],"refs":sorted(v["refs"]),"refs_far":sorted(v["refs_far"])}
for k,v in dt["files"].items():
	print(f"File '{k}':\n  type: {v['type']}\n  size: {v['size']:,} ({v['size']/dt['total']['size']*100:.2f}%) (loc: {v['loc']} ({v['loc']/dt['total']['loc']*100:.2f}%), sloc: {v['sloc']} ({v['sloc']/dt['total']['sloc']*100:.2f}%))\n  references:\n    {('(none)' if len(v['refs'])==0 else (chr(10)+'    ').join(v['refs']))}\n  indirect references:\n    {('(none)' if len(v['refs_far'])==0 else (chr(10)+'    ').join(v['refs_far']))}")
