import minify
import os
import re



GITHUB_BASE_LINK=b"https://github.com/Krzem5/Assembly-64Bit_Uefi_Kernel/"
INCLUDE_FILE_REGEX=re.compile(br"^\s*#\s*include\s*<([^>]*?)>$",re.M)
UNQUOTED_JS_KEY=re.compile(r"^[a-zA-Z0-9_]+$")
MARKDOWN_EMPTY_LINE_REGEX=re.compile(br"^[ \t]+$",re.M)
MARKDOWN_TOKEN_REGEX={"newline":re.compile(br"\n+"),"break":re.compile(br" {0,3}((?:-[ \t]*){3,}|(?:_[ \t]*){3,}|(?:\*[ \t]*){3,})\n+"),"code":re.compile(br"( {0,3})(`{3,}|~{3,})([^`\n]*)\n(?:|([\s\S]*?)\n)(?: {0,3}\2[~`]* *\n+|$)"),"indent_code":re.compile(br"\n*(?:(?: {4}| *\t)[^\n]+\n*)+"),"block_quote":re.compile(br"(?: {0,3}>[^\n]*\n)+"),"list":re.compile(br"( {0,3})([\*\+-]|\d{1,9}[.)])(?:[ \t]*|[ \t][^\n]+)\n+"),"head":re.compile(br" {0,3}(#{1,6})(?!#+)(?: *\n+|\s+([^\n]*?)(?:\n+|\s+?#+\s*\n+))")}
REQUIRED_STRUCTURE_OFFSETS={}
REQUIRED_STRUCTURE_SIZE=[]
REQUIRED_DEFINITIONS=[]
REQUIRED_TYPE_SIZE=[]
with open("src/_build/exports.txt","rb") as f:
	c=None
	c_s=None
	c_s_i=None
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
				if (c_s is None or len(c_i)==len(c_s_i)):
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



def _render_md(fp):
	md_fp="src/docs/raw/"+fp.replace("/","$").split(".")[0]+".md"
	if (not os.path.exists(md_fp)):
		with open(md_fp,"wb") as f:
			f.write(b"# `"+bytes(fp.split("/")[-1],"utf-8")+b"`\n\n*No Description*\n")
	with open(md_fp,"rb") as f:
		return MARKDOWN_EMPTY_LINE_REGEX.sub(br"",f.read().replace(b"\r\n",b"\n").rstrip()+b"\n")



def _write_f_tree(dt):
	def _rec(d,i,p):
		o=b""
		for k,v in d[0].items():
			for j in range(0,i):
				o+=b"<span class=\"vl\">  </span>"
			tp=p+(b"$" if len(p)>0 else b"")+bytes(k,"utf-8")
			o+=b"<a class=\""+(b"root" if i==0 else b"dir")+b"\" href=\""+tp+b".html\">"+bytes(k.split(".")[0],"utf-8")+b"</a><span class=\"txt\">:</span>\n"+_rec(v,i+1,tp)
		for k in d[1]:
			for j in range(0,i):
				o+=b"<span class=\"vl\">  </span>"
			o+=b"<a class=\""+(b"c" if k[-1]=="c" else (b"h" if k[-1]=="h" else b"asm"))+b"\" href=\""+p+b"$"+bytes(k.split(".")[0],"utf-8")+b".html\">"+bytes(k,"utf-8")+b"</a>\n"
		return o
	r=({},[])
	for s in dt.keys():
		s=s.split("/")
		p=""
		for k in s:
			t=r
			for e in p[1:].split("/"):
				if (len(e)==0):
					continue
				t=t[0][e]
			if (k==s[-1]):
				t[1].append(k)
				continue
			if (k not in t[0]):
				t[0][k]=({},[])
				_t=sorted([[e,t[0][e]] for e in t[0].keys()],key=lambda e:e[0])
				t[0].clear()
				for e in _t:
					t[0][e[0]]=e[1]
			p+=f"/{k}"
	return _rec(r,0,b"")



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
			f_dt[r+f]={"type":("Header" if f[-1]=="h" else "Source"),"size":len(dt.replace(b"\r\n",b"\n")),"loc":0,"sloc":0,"refs":[],"refs_far":[],"desc":_render_md(r+f)}
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
			f_dt[r+f]={"type":"Assembly Source","size":len(dt.replace("\r\n","\n")),"loc":0,"sloc":0,"refs":[],"refs_far":[],"desc":_render_md(r+f)}
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
dt={"total":{"size":0,"loc":0,"sloc":0},"dirs":{},"files":{}}
for k,v in sorted(f_dt.items(),key=lambda e:e[0]):
	p=""
	for e in k.split("/")[:-1]:
		p+=("/" if len(p) else "")+e
		if (p not in dt["dirs"]):
			dt["dirs"][p]={"files":0,"size":0,"loc":0,"sloc":0,"desc":_render_md(p)}
		dt["dirs"][p]["files"]+=1
		dt["dirs"][p]["size"]+=v["size"]
		dt["dirs"][p]["loc"]+=v["loc"]
		dt["dirs"][p]["sloc"]+=v["sloc"]
	dt["total"]["size"]+=v["size"]
	dt["total"]["loc"]+=v["loc"]
	dt["total"]["sloc"]+=v["sloc"]
	dt["files"][k]={"type":v["type"],"size":v["size"],"loc":v["loc"],"sloc":v["sloc"],"refs":sorted(v["refs"]),"refs_far":sorted(v["refs_far"]),"desc":v["desc"]}
if (not os.path.exists("docs")):
	os.mkdir("docs")
for k in os.listdir("docs"):
	os.remove(f"docs/{k}")
ft=_write_f_tree(dt["files"])
with open("docs/index.html","wb") as wf,open("src/docs/web/index.html","rb") as rf:
	wf.write(minify.minify_html(rf.read().replace(b"$$__FILE_TREE__$$",ft).replace(b"$$__BASE_URL__$$",GITHUB_BASE_LINK),"src/docs/web/index.html","src/docs/web"))
with open("src/docs/web/file_template.html","rb") as f:
	ft_dt=f.read().replace(b"$$__FILE_TREE__$$",ft).replace(b"$$__BASE_URL__$$",GITHUB_BASE_LINK)
for k,v in dt["dirs"].items():
	with open(f"docs/{k.replace('/','$')}.html","wb") as wf:
		o=b""
		p=b""
		for i,e in enumerate(k.split("/")):
			if (i):
				o+=b"/"
			p+=(b"$" if len(p)>0 else b"")+bytes(e,"utf-8")
			o+=b"<a class=\""+(b"root" if i==0 else b"dir")+(b" e" if i==k.count("/") else b"")+(b"\" href=\""+p+b".html\">" if i<k.count("/") else b"\">")+bytes(e,"utf-8")+b"</a>"
		wf.write(minify.minify_html(ft_dt.replace(b"$$__GH_LINK__$$",GITHUB_BASE_LINK+b"tree/main/"+bytes(k,"utf-8")).replace(b"$$__PATH__$$",o).replace(b"$$__DESC__$$",v["desc"]),f"src/docs/web/{k.replace('/','$')}.html","src/docs/web"))
for k,v in dt["files"].items():
	with open(f"docs/{k.replace('/','$').split('.')[0]}.html","wb") as wf:
		o=b""
		p=b""
		for i,e in enumerate(k.split("/")):
			if (i):
				o+=b"/"
			p+=(b"$" if len(p)>0 else b"")+bytes(e,"utf-8")
			o+=b"<a class=\""+((b"root" if i==0 else b"dir") if i<k.count("/") else bytes(k.split(".")[-1],"utf-8")+b" e")+(b"\" href=\""+p+b".html\">" if i<k.count("/") else b"\">")+bytes(e,"utf-8")+b"</a>"
		wf.write(minify.minify_html(ft_dt.replace(b"$$__GH_LINK__$$",GITHUB_BASE_LINK+b"blob/main/"+bytes(k,"utf-8")).replace(b"$$__PATH__$$",o).replace(b"$$__DESC__$$",v["desc"]),f"src/docs/web/{k.replace('/','$').split('.')[0]}.html","src/docs/web"))
