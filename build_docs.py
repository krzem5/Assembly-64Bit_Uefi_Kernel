import os
import re



INCLUDE_FILE_REGEX=re.compile(br"^\s*#\s*include\s*<([^>]*?)>$",re.M)
REQUIRED_STRUCTURE_OFFSETS={b"__KERNEL_ARGS":[b"k_sp"],b"__CPU":[b"s",b"rsp0",b"apic",b"apic_tpus",b"id"],b"__CPUID_INFO":[b"eax",b"ebx",b"ecx",b"edx"],b"__IDT_TABLE":[b"b"]}
REQUIRED_STRUCTURE_SIZE=[b"__THREAD_DATA",b"__IDT_ENTRY"]
REQUIRED_DEFINITIONS=[b"LOW_MEM_AP_INIT_ADDR",b"LOW_MEM_AP_PML4_ADDR",b"MSR_GS_BASE",b"PAGE_PRESENT",b"PAGE_READ_WRITE",b"PAGE_DIR_PRESENT",b"PAGE_DIR_READ_WRITE",b"PAGE_4KB_SIZE",b"TOTAL_INTERRUPT_NUMBER",b"APIC_EOI_REGISTER",b"MSR_APIC_BASE",b"APIC_LVT_ERROR_REGISER",b"APIC_ERROR_INTERRUPT",b"APIC_SPURIOUS_REGISTER",b"APIC_SPURIOUS_INTERRUPT",b"APIC_SVR_ENABLE",b"APIC_LVT_TIMER_REGISER",b"APIC_TIMER_CALIB_US",b"APIC_TIMER_DIVISOR_REGISER",b"APIC_TIMER_INIT_REGISER",b"APIC_TIMER_VALUE_REGISER",b"APIC_TIMER_REPEAT",b"APIC_TIMER_INTERRUPT",b"APIC_ID_REGISTER"]



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
							if (REQUIRED_STRUCTURE_OFFSETS[k]):
								for e in REQUIRED_STRUCTURE_OFFSETS[k]:
									print(f"Unable to Find Element '{str(e,'utf-8')}' in Structure '{str(k,'utf-8')}'!")
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
f_dt={k:{"type":v["type"],"size":v["size"],"loc":v["loc"],"sloc":v["sloc"],"refs":sorted(v["refs"]),"refs_far":sorted(v["refs_far"])} for k,v in sorted(f_dt.items(),key=lambda e:e[0])}
for k,v in f_dt.items():
	print(f"File '{k}':\n  type: {v['type']}\n  size: {v['size']:,} (loc: {v['loc']}, sloc: {v['sloc']})\n  references:\n    {('(none)' if len(v['refs'])==0 else (chr(10)+'    ').join(v['refs']))}\n  indirect references:\n    {('(none)' if len(v['refs_far'])==0 else (chr(10)+'    ').join(v['refs_far']))}")
