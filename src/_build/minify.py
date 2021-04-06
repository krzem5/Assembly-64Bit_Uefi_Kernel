import hashlib
import os
import re
import requests



CSS_BRACKET_REGEX=re.compile(br"\([^\)]*?\)")
CSS_CLASS_LETTERS="abcdefghijklmnopqrstuvwxyz"
CSS_HEX_COLOR_REGEX=re.compile(br"#([0-9a-fA-F])\1([0-9a-fA-F])\2([0-9a-fA-F])\3(\s|;)")
CSS_KEYFRAMES_LETTERS="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
CSS_KEYFRAMES_NAME_REGEX=re.compile(br"^[a-zA-Z_][a-zA-Z0-9_]*$")
CSS_KEYFRAMES_VALUE_REGEX=re.compile(br"\s*([^{]+?)\s*{\s*([^}]*?)\s*}")
CSS_PROPERTY_KEY_VALUE_REGEX=re.compile(br"\s*(.*?)\s*:\s*(.*?)\s*(?:;|$)")
CSS_SELECTOR_COMMA_REGEX=re.compile(br",\s+")
CSS_SELECTOR_REGEX=re.compile(br"([\>\+\~]|\s*)?(#[a-zA-Z0-9_\-]+|\*|(?:[a-zA-Z0-9\-]+)?)((?:\.[a-zA-Z0-9_\-]+)*)((?:\[[a-zA-Z0-9_]+[~|^$*]?=\])*)((?:\:{1,2}[a-zA-Z0-9_\-]+(?:\([^\)]+\))?)*)")
CSS_SELECTOR_VALUE_REGEX=re.compile(br"\s*([^{]+?)\s*{")
CSS_SELECTOR_WHITESPACE_REGEX=re.compile(br"(?<=[\[\(>+=])\s+|\s+(?=[=~^$|>+\]\)])")
CSS_UNIT_REGEX=re.compile(br":\s*0(\.\d+(?:[cm]m|e[mx]|in|p[ctx]|fr))\s*;")
CSS_URL_REGEX=re.compile(br"""url\(([\"'])([^)]*)\1\)""")
CSS_WHITESPACE_REGEX=re.compile(br"/\*[\s\S]*?\*/")
CSS_WHITESPACE_SPLIT_REGEX=re.compile(br"\s+?")
HTML_ATTRIBUTE_REGEX=re.compile(br'''([a-zA-Z0-9\-_]+)\s*(?:=\s*"((?:[^\"\\]|\\.)*))?"''')
HTML_AUTO_CLOSE_TAGS=["area","base","br","col","embed","hr","img","input","link","meta","param","source","track","wbr"]
HTML_QUOTED_ATTRIBUTE_REGEX=re.compile(br"[^a-z0-9\-_]")
HTML_REMOVE_WHITEPSACE_REGEX=re.compile(br"(?=(?P<tmp>[^\S ]\s*|\s{2,}))(?P=tmp)(?=(?P<txt>(?=(?P<tmp3>(?:(?=(?P<tmp2>[^<]+))(?P=tmp2)|<(?!\/?(?:textarea|pre)\b))*))(?P=tmp3))(?:<(?=(?P<tmp4>textarea|pre))(?P=tmp4)\b|$))",re.I|re.M|re.X)
HTML_TAG_ATTRIBUTE_MAP={"accept":["input"],"accept-charset":["form"],"accesskey":None,"action":["form"],"alt":["area","img","input"],"async":["script"],"autocomplete":["form","input"],"autofocus":["button","input","select","textarea"],"autoplay":["audio","video"],"charset":["meta","script"],"checked":["input"],"cite":["blockquote","del","ins","q"],"class":None,"cols":["textarea"],"colspan":["td","th"],"content":["meta"],"contenteditable":None,"controls":["audio","video"],"coords":["area"],"data":["object"],"datetime":["del","ins","time"],"default":["track"],"defer":["script"],"dir":None,"dirname":["input","textarea"],"disabled":["button","fieldu-ch-t-set","input","optgroup","option","select","textarea"],"download":["a","area"],"draggable":None,"enctype":["form"],"for":["label","output"],"form":["button","fieldset","input","label","meter","object","output","select","textarea"],"formaction":["button","input"],"headers":["td","th"],"height":["canvas","embed","iframe","img","input","object","video"],"hidden":None,"high":["meter"],"href":["a","area","base","link"],"hreflang":["a","area","link"],"http-equiv":["meta"],"id":None,"ismap":["img"],"kind":["track"],"label":["track","option","optgroup"],"lang":None,"list":["input"],"loop":["audio","video"],"low":["meter"],"max":["input","meter","progress"],"maxlength":["input","textarea"],"media":["a","area","link","source","style"],"method":["form"],"min":["input","meter"],"multiple":["input","select"],"muted":["video","audio"],"name":["button","fieldset","form","iframe","input","map","meta","object","output","param","select","textarea"],"novalidate":["form"],"onabort":["audio","embed","img","object","video"],"onafterprint":["body"],"onbeforeprint":["body"],"onbeforeunload":["body"],"onblur":None,"oncanplay":["audio","embed","object","video"],"oncanplaythrough":["audio","video"],"onchange":None,"onclick":None,"oncontextmenu":None,"oncopy":None,"oncuechange":["track"],"oncut":None,"ondblclick":None,"ondrag":None,"ondragend":None,"ondragenter":None,"ondragleave":None,"ondragover":None,"ondragstart":None,"ondrop":None,"ondurationchange":["audio","video"],"onemptied":["audio","video"],"onended":["audio","video"],"onerror":["audio","body","embed","img","object","script","style","video"],"onfocus":None,"onhashchange":["body"],"oninput":None,"oninvalid":None,"onkeydown":None,"onkeypress":None,"onkeyup":None,"onload":["body","iframe","img","input","link","script","style"],"onloadeddata":["audio","video"],"onloadedmetadata":["audio","video"],"onloadstart":["audio","video"],"onmousedown":None,"onmousemove":None,"onmouseout":None,"onmouseover":None,"onmouseup":None,"onmousewheel":None,"onoffline":["body"],"ononline":["body"],"onpagehide":["body"],"onpageshow":["body"],"onpaste":None,"onpause":["audio","video"],"onplay":["audio","video"],"onplaying":["audio","video"],"onpopstate":["body"],"onprogress":["audio","video"],"onratechange":["audio","video"],"onreset":["form"],"onresize":["body"],"onscroll":None,"onsearch":["input"],"onseeked":["audio","video"],"onseeking":["audio","video"],"onselect":None,"onstalled":["audio","video"],"onstorage":["body"],"onsubmit":["form"],"onsuspend":["audio","video"],"ontimeupdate":["audio","video"],"ontoggle":["details"],"onunload":["body"],"onvolumechange":["audio","video"],"onwaiting":["audio","video"],"onwheel":None,"open":["details"],"optimum":["meter"],"pattern":["input"],"placeholder":["input","textarea"],"poster":["video"],"preload":["audio","video"],"readonly":["input","textarea"],"rel":["a","area","form","link"],"required":["input","select","textarea"],"reversed":["ol"],"rows":["textarea"],"rowspan":["td","th"],"sandbox":["iframe"],"scope":["th"],"selected":["option"],"shape":["area"],"size":["input","select"],"sizes":["img","link","source"],"span":["col","colgroup"],"spellcheck":None,"src":["audio","embed","iframe","img","input","script","source","track","video"],"srcdoc":["iframe"],"srclang":["track"],"srcset":["img","source"],"start":["ol"],"step":["input"],"style":None,"tabindex":None,"target":["a","area","base","form"],"title":None,"translate":None,"type":["a","button","embed","input","link","menu","object","script","source","style"],"usemap":["img","object"],"value":["button","input","li","option","meter","progress","param"],"width":["canvas","embed","iframe","img","input","object","video"],"wrap":["textarea"]}
HTML_TAG_JS_ATTRIBUTES=["onabort","onafterprint","onbeforeprint","onbeforeunload","onblur","oncanplay","oncanplaythrough","onchange","onclick","oncontextmenu","oncopy","oncuechange","oncut","ondblclick","ondrag","ondragend","ondragenter","ondragleave","ondragover","ondragstart","ondrop","ondurationchange","onemptied","onended","onerror","onfocus","onhashchange","oninput","oninvalid","onkeydown","onkeypress","onkeyup","onload","onloadeddata","onloadedmetadata","onloadstart","onmousedown","onmousemove","onmouseout","onmouseover","onmouseup","onmousewheel","onoffline","ononline","onpagehide","onpageshow","onpaste","onpause","onplay","onplaying","onpopstate","onprogress","onratechange","onreset","onresize","onscroll","onsearch","onseeked","onseeking","onselect","onstalled","onstorage","onsubmit","onsuspend","ontimeupdate","ontoggle","onunload","onvolumechange","onwaiting","onwheel"]
HTML_TAG_REGEX=re.compile(br"<([!/]?[a-zA-Z0-9\-_]+)\s*(.*?)\s*(/?)>",re.I|re.M|re.X)
HTML_TAGS=["!doctype","a","abbr","address","area","article","aside","audio","b","base","bdi","bdo","blockquote","body","br","button","canvas","caption","cite","code","col","colgroup","data","datalist","dd","del","details","dfn","dialog","div","dl","dt","em","embed","fieldset","figcaption","figure","footer","form","h1","head","header","hr","html","i","iframe","img","input","ins","kbd","label","legend","li","link","main","map","mark","meta","meter","nav","noscript","object","ol","optgroup","option","output","p","param","picture","pre","progress","q","rp","rt","ruby","s","samp","script","section","select","small","source","span","strong","style","sub","summary","sup","svg","table","tbody","td","template","textarea","tfoot","th","thead","time","title","tr","track","u","ul","var","video","wbr"]
HTML_URL_REGEX=re.compile(br"^(?:(?:https?|ftp)://)(?:\S+(?::\S*)?@)?(?:(?!10(?:\.\d{1,3}){3})(?!127(?:\.\d{1,3}){3})(?!169\.254(?:\.\d{1,3}){2})(?!192\.168(?:\.\d{1,3}){2})(?!172\.(?:1[6-9]|2\d|3[0-1])(?:\.\d{1,3}){2})(?:[1-9]\d?|1\d\d|2[01]\d|22[0-3])(?:\.(?:1?\d{1,2}|2[0-4]\d|25[0-5])){2}(?:\.(?:[1-9]\d?|1\d\d|2[0-4]\d|25[0-4]))|(?:(?:[a-z\xa1\xff0-9]+-?)*[a-z\xa1-\xff0-9]+)(?:\.(?:[a-z\xa1-\xff0-9]+-?)*[a-z\xa1-\xff0-9]+)*(?:\.(?:[a-z\xa1-\xff]{2,})))(?::\d{2,5})?(?:/[^\s]*)?$",re.I|re.S)
JS_CONST_LETTERS="ABCDEFGHIJKLMNOPQRSTUVWXYZ"
JS_DOCUMENT_CACHE_PROPERTIES=["adoptNode","captureEvents","caretRangeFromPoint","createAttribute","createAttributeNS","createCDATASection","createComment","createDocumentFragment","createElement","createElementNS","createEvent","createNodeIterator","createProcessingInstruction","createRange","createTextNode","createTouch","createTouchList","createTreeWalker","exitPictureInPicture","exitPointerLock","getElementsByClassName","getElementsByTagName","getElementsByTagNameNS","hasStorageAccess","importNode","releaseCapture","releaseEvents","requestStorageAccess","mozSetImageElement","getElementById","querySelector","querySelectorAll","createExpression","createNSResolver","evaluate","clear","close","getElementsByName","hasFocus","open","write","writeln","caretPositionFromPoint","elementFromPoint","elementsFromPoint","getAnimations","getSelection"]
JS_HTML_STRING_UNESCAPE=re.compile(br"""([^\\])\\(['"])""")
JS_KEYWORDS=["break","case","catch","const","const","continue","debugger","default","delete","do","else","enum","false","finally","for","function","if","in","instanceof","let","new","null","of","return","switch","this","throw","true","try","typeof","var","var","void","while","with"]
JS_OPERATORS=["()=>","_=>","=>","...",">>>=",">>=","<<=","|=","^=","&=","+=","-=","*=","/=","%=",";",",","?",":","||","&&","|","^","&","===","==","=","!==","!=","<<","<=","<",">>>",">>",">=",">","++","--","+","-","*","/","%","!","~",".","[","]","{","}","(",")"]
JS_REGEX_LIST={"dict":re.compile(br"""{\s*(?:[$a-zA-Z0-9_]+|'(?:[^'\\]|\\.)*'|"(?:[^"\\]|\\.)*"|`(?:[^`\\]|\\.)*`)\s*:\s*"""),"dict_elem":re.compile(br""",\s*(?:[$a-zA-Z0-9_]+|'(?:[^'\\]|\\.)*'|"(?:[^"\\]|\\.)*"|`(?:[^`\\]|\\.)*`)\s*:\s*"""),"float":re.compile(br"\d+\.\d*(?:[eE][-+]?\d+)?|^\d+(?:\.\d*)?[eE][-+]?\d+|^\.\d+(?:[eE][-+]?\d+)?"),"int":re.compile(br"0[xX][\da-fA-F]+|0[0-7]*|\d+"),"identifier":re.compile(br"\.?[$_a-zA-Z0-9]+(?:\.[$_a-zA-Z0-9]+)*"),"string":re.compile(br"""'(?:[^'\\]|\\.)*'|^"(?:[^"\\]|\\.)*"|^`(?:[^`\\]|\\.)*`"""),"regex":re.compile(br"\/(?![*+?])(?:[^\r\n\[/\\]|\\.|\[(?:[^\r\n\]\\]|\\.)*\])+\/(?!\/)[igm]{0,3}"),"line_break":re.compile(br"[\n\r]+|/\*(?:.|[\r\n])*?\*/"),"whitespace":re.compile(br"[\ \t]+|//.*?(?:[\r\n]|$)"),"operator":re.compile(bytes("|".join([re.sub(r"([\?\|\^\&\(\)\{\}\[\]\+\-\*\/\.])",r"\\\1",e) for e in JS_OPERATORS]),"utf-8"))}
JS_RESERVED_IDENTIFIERS=JS_KEYWORDS+["AggregateError","alert","arguments","Array","ArrayBuffer","AsyncFunction","AsyncGenerator","AsyncGeneratorFunction","atob","Atomics","BigInt","BigInt64Array","BigUint64Array","blur","Boolean","btoa","caches","cancelAnimationFrame","cancelIdleCallback","captureEvents","chrome","clearInterval","clearTimeout","clientInformation","close","closed","confirm","console","cookieStore","createImageBitmap","crossOriginIsolated","crypto","customElements","DataView","Date","decodeURI","decodeURIComponent","defaultStatus","defaultstatus","devicePixelRatio","document","encodeURI","encodeURIComponent","Error","escape","eval","EvalError","external","fetch","find","Float32Array","Float64Array","focus","frameElement","frames","Function","Generator","GeneratorFunction","getComputedStyle","getSelection","globalThis","history","Image","indexedDB","Infinity","innerHeight","innerWidth","Int16Array","Int32Array","Int8Array","InternalError","Intl","isFinite","isNaN","isSecureContext","JSON","length","localStorage","location","locationbar","Map","matchMedia","Math","menubar","moveBy","moveTo","NaN","navigator","Number","Object","open","openDatabase","opener","origin","originIsolated","outerHeight","outerWidth","pageXOffset","pageYOffset","parent","parseFloat","parseInt","performance","personalbar","postMessage","print","Promise","prompt","Proxy","queueMicrotask","RangeError","ReferenceError","Reflect","RegExp","releaseEvents","requestAnimationFrame","requestIdleCallback","resizeBy","resizeTo","screen","screenLeft","screenTop","screenX","screenY","scroll","scrollbars","scrollBy","scrollTo","scrollX","scrollY","self","sessionStorage","Set","setInterval","setTimeout","SharedArrayBuffer","showDirectoryPicker","showOpenFilePicker","showSaveFilePicker","speechSynthesis","status","statusbar","stop","String","styleMedia","Symbol","SyntaxError","toolbar","top","trustedTypes","TypeError","Uint16Array","Uint32Array","Uint8Array","Uint8ClampedArray","undefined","unescape","uneval","URIError","visualViewport","WeakMap","WeakSet","WebAssembly","webkitCancelAnimationFrame","webkitRequestAnimationFrame","webkitRequestFileSystem","webkitResolveLocalFileSystemURL","WebSocket","window"]
JS_STRING_HTML_TAG_REGEX=re.compile(br"<(/?(?:"+bytes(r"|".join(sorted(HTML_TAGS,key=lambda e:-len(e))),"utf-8")+b"))")
JS_VAR_LETTERS="abcdefghijklmnopqrstuvwxyz"
USER_AGENT="Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/89.0.4356.6 Safari/537.36"



def minify_html(html,fp,fp_b):
	def _get_url_cache(url):
		h=hashlib.md5(url).hexdigest()
		if (not os.path.exists("__url_cache")):
			os.mkdir("__url_cache")
		if (not hasattr(_get_url_cache,"_d")):
			_get_url_cache._d={}
		if (h not in _get_url_cache._d):
			if (not os.path.exists(f"__url_cache/{h}")):
				print(f"    Downloading '{str(url,'utf-8')}'...")
				with open(f"__url_cache/{h}","wb") as f:
					_get_url_cache._d[h]=requests.get(url,headers={"User-Agent":USER_AGENT}).content
					f.write(_get_url_cache._d[h])
			else:
				print(f"    Using Cached '{str(url,'utf-8')}'...")
				with open(f"__url_cache/{h}","rb") as f:
					_get_url_cache._d[h]=f.read()
		else:
			print(f"    Using Cached '{str(url,'utf-8')}'...")
		return _get_url_cache._d[h]
	def _gen_i(il,b,r=None):
		def _gen_next(v,b):
			n=1
			m=len(b)
			while (v>m):
				v-=m
				m*=len(b)
				n+=1
			v-=1
			o=""
			if (v==0):
				o=b[0]*n
			else:
				while (v):
					o=b[v%len(b)]+o
					v=v//len(b)
				o=b[0]*(n-len(o))+o
			return bytes(o,"utf-8")
		if (r is None):
			r=JS_RESERVED_IDENTIFIERS[:]
		for k in il:
			r+=k.values()
		i=1
		o=b""
		while (True):
			o=_gen_next(i,b)
			if (o not in r):
				break
			i+=1
		return o
	def _parse_css_selector(k,tcm,gc):
		l=[]
		ps=-1
		j=0
		while (j<len(k)):
			km=CSS_SELECTOR_REGEX.search(k[j:])
			kms=km.start(0)
			if (kms!=0):
				raise RuntimeError(f"Unable to Parse Part of the CSS Selector: '{str(k[j:],'utf-8')}' ('{str(k,'utf-8')}')")
			if (km.end(0)-kms==0):
				raise RuntimeError(f"Unable to Parse CSS Selector: '{str(k[j:],'utf-8')}' ('{str(k,'utf-8')}')")
			ss,st,sc,sp,se=km.group(1),km.group(2),km.group(3),km.group(4),km.group(5)
			if (len(ss)>0 and ss in b"+~"):
				raise RuntimeError("CSS Selector Symbols '+' and '~' aren't Implemented Yet!")
			if ((ss==b">" or st==b"*" or (len(st)>0 and st[:1]!=b"#") or (1 if len(st) else 0)+sc.count(b".")>1) and ps==-1):
				ps=len(l)-(0 if (1 if len(st) else 0)+sc.count(b".")>1 else 1)
			if (b":not" in se):
				print("CSS Selector :not(...) has been Ignored!")
			l+=[(ss,st,sc,sp,se)]
			j+=km.end(0)
		if (l[0][2].count(b".")==1 and l[0][2][1:] not in gc):
			gc.append(l[0][2][1:])
		o=[]
		for j,(ss,st,sc,sp,se) in enumerate(l):
			if ((ps==-1 and j==len(l)-1) or (ps!=-1 and j>=ps)):
				o+=[((ss if len(o)>0 else b""),st,sc,sp,se)]
				for c in sc.split(b".")[1:]:
					if (c not in tcm):
						tcm[c]=1
					else:
						tcm[c]+=1
		return o
	def _parse_js(js,tcm,vfm,vfma):
		def _map_value(v,vml):
			for i in range(len(vml)-1,-1,-1):
				if (v in vml[i]):
					return vml[i][v]
			return None
		def _args(al):
			o=[]
			for k in al:
				if (len(o)>0):
					o+=[("operator",b",")]
				if (k[1]==True):
					o+=[("operator",b"...")]
				o+=[("identifier",k[0])]
			return o
		def _tokenize(s,c_rgx):
			i=0
			o=[]
			b=0
			while (i<len(s)):
				e=False
				for k,v in JS_REGEX_LIST.items():
					if (k=="regex" and c_rgx==False):
						continue
					mo=re.match(v,s[i:])
					if (mo!=None):
						m=mo.group(0)
						if (k=="line_break"):
							o+=[("operator",b";")]
						elif (k=="string" and m[:1]==b"`"):
							j=0
							ts=b""
							f=False
							while (j<len(m)):
								if (m[j:j+2]==b"${"):
									l,tj=_tokenize(m[j+2:],False)
									j+=tj+2
									o+=[("string"+("M" if f==True else "S"),(b"`"+ts[1:] if f==False else b"}"+ts)+b"${")]+l
									ts=b""
									f=True
								else:
									ts+=m[j:j+1]
								j+=1
							o+=[("string"+("" if f==False else "E"),(b"}"+ts[:-1]+b"`" if f==True else ts))]
						elif (k!="whitespace"):
							if (k=="identifier" and str(m,"utf-8") in JS_KEYWORDS):
								k="keyword"
							elif (k=="identifier" and m.count(b".")>0 and m.split(b".")[0]==b"window" and str(m.split(b".")[1],"utf-8") in JS_RESERVED_IDENTIFIERS and str(m.split(b".")[1],"utf-8") not in JS_KEYWORDS):
								m=m[7:]
							if (k in ["operator","dict"]):
								if (m[:1]==b"{"):
									b+=1
								elif (m==b"}"):
									b-=1
									if (b==-1):
										return (o,i)
							o+=[(k,m)]
						i+=mo.end(0)
						e=True
						break
				if (e==True):
					continue
				raise RuntimeError(f"Unable to Match JS Regex: {str(s[i:],'utf-8')}")
			return (o,i)
		print("    Tokenizing...")
		tl,_=_tokenize(js,True)
		i=0
		vm=[{}]
		ef=[]
		dl=[]
		efbl={}
		ee={}
		bl=0
		v_nm=False
		vdl={}
		print("    Parsing Structure...")
		while (i<len(tl)):
			if (tl[i][0]=="identifier"):
				idl=tl[i][1].split(b".")
				dpc=False
				if (len(idl[0])>0):
					if (idl[0]==b"document" and str(idl[1],"utf-8") in JS_DOCUMENT_CACHE_PROPERTIES):
						if (len(idl)>1 and b"document."+idl[1] not in vfm):
							vfm[b"document."+idl[1]]=1
							if (b"document" not in vfm):
								vfm[b"document"]=2
							else:
								vfm[b"document"]+=2
							if (b"bind" not in vfma):
								vfma[b"bind"]=1
							else:
								vfma[b"bind"]+=1
						else:
							vfm[b"document."+idl[1]]+=1
						dpc=True
					elif (str(idl[0],"utf-8") in JS_RESERVED_IDENTIFIERS):
						if (idl[0] not in vfm):
							vfm[idl[0]]=1
						else:
							vfm[idl[0]]+=1
					else:
						if (i>0 and tl[i-1][0]=="keyword" and tl[i-1][1] in [b"let",b"const",b"var"]):
							vm[-1][idl[0]]=_gen_i(vm,JS_VAR_LETTERS)
							idl[0]=vm[-1][idl[0]]
							if (bl not in vdl):
								vdl[bl]=None
							else:
								if (vdl[bl] is None):
									raise RuntimeError
								tl=tl[:i-1]+tl[i:]
								i-=1
								tl[vdl[bl]]=("operator",b",")
								vdl[bl]=(vdl[bl],i)
						elif (str(idl[0],"utf-8") not in JS_RESERVED_IDENTIFIERS and (i==0 or (tl[i-1][0]!="operator" or tl[i-1][1]!=b"."))):
							mv=_map_value(idl[0],vm)
							if (mv is None):
								print(f"      Variable '{str(idl[0],'utf-8')}' is not mapped!")
								v_nm=True
							else:
								idl[0]=mv
				for k in idl[(2 if dpc==True else 1):]:
					if (k not in vfma):
						vfma[k]=1
					else:
						vfma[k]+=1
				tl[i]=("identifier",b".".join(idl))
			elif (tl[i][0]=="dict"):
				dl+=[True]
			elif (tl[i][0]=="keyword"):
				if (tl[i][1]==b"function"):
					si=i
					assert(i+5<len(tl))
					assert(tl[i+1][0]=="identifier")
					assert(str(tl[i+1][1],"utf-8") not in JS_RESERVED_IDENTIFIERS)
					nm=vm[-1][tl[i+1][1]]=_gen_i(vm,JS_VAR_LETTERS)
					vm+=[{}]
					assert(tl[i+2][0]=="operator")
					assert(tl[i+2][1]==b"(")
					i+=3
					al=[]
					while (True):
						if (len(al)>0 and tl[i][0]=="operator" and tl[i][1]==b","):
							i+=1
						if (tl[i][0]=="operator" and tl[i][1]==b")"):
							i+=1
							break
						va=False
						if (tl[i][0]=="operator" and tl[i][1]==b"..."):
							va=True
							i+=1
						assert(tl[i][0]=="identifier")
						vm[-1][tl[i][1]]=_gen_i(vm,JS_VAR_LETTERS)
						al+=[(vm[-1][tl[i][1]],va)]
						i+=1
					assert(tl[i][0]=="operator")
					assert(tl[i][1]==b"{")
					bl+=1
					dl+=[False]
					ef+=[(bl,bl,si,i-si,nm,al)]
					if (bl not in efbl):
						efbl[bl]=[]
					efbl[bl]+=[len(ef)-1]
			elif (tl[i][0]=="operator"):
				s_ee=True
				if (tl[i][1]==b"{"):
					vm+=[{}]
					dl+=[False]
					ef+=[None]
					bl+=1
				elif (tl[i][1]==b"}"):
					if (dl[-1]==False):
						if (bl in vdl):
							del vdl[bl]
						bl-=1
						if (ef[-1]!=None):
							cbl,ocbl,si,fl,nm,al=ef[-1]
							efbl[ocbl].remove(len(ef)-1)
							j=1
							if (tl[si+fl+1][0]=="keyword" and tl[si+fl+1][1]==b"return"):
								j+=1
								cbl=ocbl
							ftl=([("keyword",b"let"),("identifier",nm),("operator",b"=")] if nm!=None else [])+([("operator",b"_=>")] if len(al)==0 else ([("identifier",al[0][0]),("operator",b"=>")] if len(al)==1 and al[0][1]==False else [("operator",b"(")]+_args(al)+[("operator",b")"),("operator",b"=>")]))
							if (bl in vdl and nm!=None):
								ftl=ftl[1:]
								tl[vdl[bl]]=("operator",b",")
								tl=tl[:vdl[bl]+1]+ftl+([("operator",b"{")] if cbl is None else [])+tl[si+fl+j:i]+([("operator",b"}")] if cbl is None else [])+[("operator",b";")]+tl[vdl[bl]+1:si]+tl[i+1:]
								i+=-fl+len(ftl)-2+(2 if cbl is None else 0)-j+1
								vdl[bl]=i+1
							else:
								tl=tl[:si]+ftl+([("operator",b"{")] if cbl is None else [])+tl[si+fl+j:i]+([("operator",b"}")] if cbl is None else [])+[("operator",b";")]+tl[i+1:]
								i+=-fl+len(ftl)-2+(2 if cbl is None else 0)-j+1
								if (nm!=None):
									vdl[bl]=i+1
						vm=vm[:-1]
						ef=ef[:-1]
					dl=dl[:-1]
					s_ee=False
				elif (tl[i][1]==b")"):
					if (bl in vdl):
						del vdl[bl]
					bl-=1
					s_ee=False
				elif (tl[i][1]==b";"):
					if (bl in vdl):
						if (vdl[bl] is None):
							vdl[bl]=i
						elif (type(vdl[bl])!=int):
							ti,si=vdl[bl]
							ok=True
							if (si-ti!=1):
								for k in tl[si+2:i+1]:
									if (k[0]=="identifier" and k[1].split(b".")[0] not in JS_RESERVED_IDENTIFIERS):
										ok=False
										break
							if (ok==True):
								tl=tl[:ti+1]+tl[si:i+1]+tl[ti+1:si]+tl[i+2:]
								vdl[bl]=ti+i-si+1
							else:
								tl[ti]=("operator",b";")
								tl=tl[:si]+[("keyword",b"let")]+tl[si:]
								i+=1
								vdl[bl]=i
					if ((i>0 and tl[i-1][0]=="operator" and tl[i-1][1]==b"{") or (i+1<len(tl) and ((tl[i+1][0]=="operator" and tl[i+1][1] in [b";",b"}",b")"]) or (tl[i+1][0]=="keyword" and tl[i+1][1]==b"else"))) or i==len(tl)-1):
						tl=tl[:i]+tl[i+1:]
						i-=1
					else:
						s_ee=False
				elif (tl[i][1]==b"()=>"):
					tl[i]=("operator",b"_=>")
				elif (tl[i][1]==b"("):
					af=False
					if (i>0 and tl[i-1][0]=="identifier" and (tl[i-1][1]==b"document.querySelector" or tl[i-1][1]==b"document.querySelectorAll")):
						if (tl[i+1][0]!="string"):
							raise RuntimeError("Unable to Decode JS Selector (querySelector)")
						tl[i+1]=("css_selector",_parse_css_selector(tl[i+1][1][1:-1],tcm,[]))
					elif (i>0 and tl[i-1][0]=="identifier" and (tl[i-1][1].endswith(b"classList.add") or tl[i-1][1].endswith(b"classList.toggle") or tl[i-1][1].endswith(b"classList.remove"))):
						if (tl[i+1][0]!="string"):
							raise RuntimeError("Unable to Decode JS Selector (classList)")
						tl[i+1]=("css_class_str",tl[i+1][1][1:-1])
					elif (i==0 or tl[i-1][0]!="identifier"):
						si=i+0
						al=[]
						i+=1
						vm+=[{}]
						while (True):
							if (len(al)>0 and tl[i][0]=="operator" and tl[i][1]==b","):
								i+=1
							if (tl[i][0]=="operator" and tl[i][1]==b")"):
								i+=1
								break
							va=False
							if (tl[i][0]=="operator" and tl[i][1]==b"..."):
								va=True
								i+=1
							if (tl[i][0]!="identifier"):
								al=None
								break
							vm[-1][tl[i][1]]=_gen_i(vm,JS_VAR_LETTERS)
							al+=[(vm[-1][tl[i][1]],va)]
							i+=1
						if (al is None or tl[i][0]!="operator" or tl[i][1]!=b"=>"):
							i=si
							vm=vm[:-1]
						else:
							i+=1
							af=True
							if (tl[i][0]=="operator" and tl[i][1]==b"{"):
								ef+=[(bl+1,bl+1,si,i-si,None,al)]
								if (bl+1 not in efbl):
									efbl[bl+1]=[]
								efbl[bl+1]+=[len(ef)-1]
								dl+=[False]
								bl+=1
							else:
								i-=1
								if (bl not in ee):
									ee[bl]=[]
								ee[bl]+=[(si,i-si,al)]
					if (af==False):
						bl+=1
						s_ee=False
				if (bl in efbl):
					for j in efbl[bl]:
						if (ef[j][0]==-1):
							ef[j]=(None,*ef[j][1:])
				if (s_ee==False):
					if (bl in ee):
						for k in ee[bl]:
							si,fl,al=k
							ftl=([("operator",b"_=>")] if len(al)==0 else ([("identifier",al[0][0]),("operator",b"=>")] if len(al)==1 and al[0][1]==False else [("operator",b"(")]+_args(al)+[("operator",b")"),("operator",b"=>")]))
							tl=tl[:si]+ftl+tl[si+fl+1:]
							i+=-fl+len(ftl)-1
							vm=vm[:-1]
						del ee[bl]
					if (bl in efbl):
						for j in efbl[bl]:
							if (ef[j][0]==bl):
								ef[j]=(-1,*ef[j][1:])
			i+=1
		if (v_nm):
			quit()
		if (bl!=0):
			raise RuntimeError("JS Contains Unclosed Brackets")
		si=-1
		sw=0
		bf=b""
		il=[]
		cl=[]
		icc=0
		sw=[-1]
		sw_b=[-1]
		sw_sb=[-1]
		b=0
		sb=0
		print("    Parsing HTML Strings...")
		for i,k in enumerate(tl):
			if (k[0]=="stringS" and JS_STRING_HTML_TAG_REGEX.match(k[1][1:])):
				si=i+0
			if (si!=-1):
				if (k[0] in ["stringS","stringM","stringE"]):
					il+=[(len(bf),len(bf)+len(k[1])-(2 if k[0]=="stringE" else 3),i)]
					bf+=k[1][1:-(1 if k[0]=="stringE" else 2)]
					sw=[-1]
					sw_b=[-1]
					sw_sb=[-1]
					b=0
					sb=0
				else:
					if (k[0]=="operator"):
						if (k[1]==b"?"):
							if (sb!=sw_sb[-1]):
								sw+=[0]
								sw_b+=[b]
								sw_sb+=[sb]
							sw[-1]=1
							sb+=1
						elif (k[1]==b":"):
							sb-=1
							while (sw_sb[-1]>sb):
								sw=sw[:-1]
								sw_b=sw_b[:-1]
								sw_sb=sw_sb[:-1]
							if (sw[-1]!=1):
								raise RuntimeError("Quadruple Operators not Implemented!")
							sw[-1]=2
						elif (k[1] in b"[{"):
							b+=1
						elif (k[1] in b"]}"):
							b-=1
					elif (k[0]=="string" and b==0 and b==sw_b[-1] and (i==len(tl)-1 or tl[i+1][0]!="operator" or tl[i+1][1]!=b"?")):
						il+=[(len(bf)+(1 if sw[-1]==2 else 0),len(bf)+len(k[1])+(1 if sw[-1]==2 else 0)-2,i)]
						bf+=(b" " if sw[-1]==2 else b"")+k[1][1:-1]
			if (k[0]=="string" and JS_STRING_HTML_TAG_REGEX.match(k[1][1:])):
				il+=[(len(bf),len(bf)+len(k[1])-2,i)]
				bf+=k[1][1:-1]
			if ((k[0]=="stringE" and si!=-1) or (k[0]=="string" and si==-1)):
				si=-1
				i=0
				while (i<len(bf)):
					m=JS_STRING_HTML_TAG_REGEX.search(bf[i:])
					if (not m):
						break
					i+=m.end(0)
					e=bf[i:i+1]
					ek=b""
					ev=None
					e_nc=False
					while (e!=b">" and i<len(bf)):
						if (e in b" \t\r\n\f\v" and (ev is None or ev[0]==b"\"")):
							if (ev!=None):
								raise RuntimeError("Not-Quoted JS String HTML not Supported!")
						elif (e_nc):
							e_nc=False
							if (ev is None):
								ek+=e
							else:
								ev+=e
						elif (e==b"\\"):
							e_nc=True
							if (ev is None):
								ek+=b"\\"
							else:
								ev+=b"\\"
						elif (e==b"="):
							if (ev!=None and ev[:1]!=b"\""):
								raise RuntimeError("Multiple '=' Signs in String HTML in JS")
							ev=b""
						elif (e==b"\""):
							if (len(ev)>0):
								ev=ev[1:]
								if (ek==b"class"):
									if (len(ev)>0):
										evi=-len(ev)
										for e in ev.split(b" "):
											for sk in il:
												if (sk[0]<=i+evi and i+evi<sk[1]):
													if ((tl[sk[2]][0]!="string" or k[0]=="string") and b" " not in ev):
														cl.insert(0,(0,b"\""+e+b"\"",sk[2],i+evi-sk[0]-1))
													else:
														cl.insert(0,(0,e,sk[2],i+evi-sk[0]))
													break
											evi+=len(e)+1
								elif (str(ek,"utf-8") in HTML_TAG_JS_ATTRIBUTES):
									m=JS_REGEX_LIST["identifier"].match(ev)
									if (m!=None):
										sm=ev[:m.end(0)].split(b".")
										if (len(sm)>1 and sm[0]==b"window" and str(sm[1],"utf-8") in JS_RESERVED_IDENTIFIERS and str(sm[1],"utf-8") not in JS_KEYWORDS):
											sm=sm[1:]
										if (sm[0] not in vfm):
											vfm[sm[0]]=1
										else:
											vfm[sm[0]]+=1
										for sme in sm[1:]:
											if (sme not in vfma):
												vfma[sme]=1
											else:
												vfma[sme]+=1
										for sk in il:
											if (sk[0]<=i-len(ev) and i-len(ev)<sk[1]):
												cl.insert(0,(1,ev[:m.end(0)],sk[2],i-len(ev)-sk[0]))
												icc+=1
												break
								elif (b" " not in ev and len(ev)>0):
									for sk in il:
										if (sk[0]<=i-len(ev) and i-len(ev)<sk[1]):
												cl.insert(0,(2,ev,sk[2],i-sk[0]-len(ev)-1))
												icc+=1
												break
								ek=b""
								ev=None
							else:
								ev+=b"\""
						else:
							if (ev is None):
								ek+=e
							else:
								ev+=e
						i+=1
						e=bf[i:i+1]
					i+=1
				bf=b""
				il=[]
		print(f"    Finding Global Object Substitutions ({len(vfm.keys())} object{('s' if len(vfm.keys())!=1 else '')})...")
		cvml=[]
		for k,v in vfm.items():
			if (v>1):
				cvml+=[(len(k)*v,k,v,False)]
		print(f"    Finding Attribute Substitutions ({len(vfma.keys())} attribute{('s' if len(vfma.keys())!=1 else '')})...")
		for k,v in vfma.items():
			if (v>1 and len(k)>2):
				cvml+=[(len(k)*v,k,v,True)]
		print("    Sorting Substitution List...")
		cvml=sorted(cvml,key=lambda e:-e[0])
		print(f"    Generating Substitutions ({len(cvml)} item{('s' if len(cvml)!=1 else '')})...")
		cvm={}
		cvma={}
		sl=[]
		esl=[]
		for k in cvml:
			if (k[3]==False):
				mv=_gen_i([cvm,cvma],JS_CONST_LETTERS)
				if (len(mv)*(k[2]+1)+len(k[1])+2<=len(k[1])*k[2]):
					cvm[k[1]]=mv
					if (b"." in k[1]):
						if (len(esl)>0):
							esl+=[("operator",b",")]
						esl+=[("identifier",mv),("operator",b"="),("identifier",k[1]+b".bind"),("operator",b"("),("identifier",k[1].split(b".")[0]),("operator",b")")]
					else:
						if (len(sl)==0):
							sl=[("keyword",b"let")]
						else:
							sl+=[("operator",b",")]
						sl+=[("identifier",mv),("operator",b"="),("identifier",k[1])]
			else:
				mv=_gen_i([cvm,cvma],JS_CONST_LETTERS)
				if (len(mv)+len(k[1])+(len(mv)+1)*k[2]+4<=(len(k[1])+1)*k[2]):
					cvma[k[1]]=mv
					if (len(sl)==0):
						sl=[("keyword",b"let")]
					else:
						sl+=[("operator",b",")]
					sl+=[("identifier",mv),("operator",b"="),("string",b"\""+k[1]+b"\"")]
		if (len(esl)>0):
			st=0
			for i,e in enumerate(esl):
				if (e[0]=="identifier"):
					if (st==1):
						idl=e[1].split(b".")
						if (idl[0] in cvm):
							idl[0]=cvm[idl[0]]
						for j,k in enumerate(idl[1:]):
							if (k in cvma):
								idl[j+1]=b"["+cvma[k]+b"]"
							else:
								idl[j+1]=b"."+k
						esl[i]=("identifier",b"".join(idl))
					elif (st==2 and e[1] in cvm):
						esl[i]=("identifier",cvm[e[1]])
					st=(st+1)%3
			if (len(sl)==0):
				sl=[("keyword",b"let")]+esl
			else:
				sl+=[("operator",b",")]+esl
		print(f"    Formatting HTML Strings ({len(cl)-icc} class{('es' if len(cl)-icc!=1 else '')})...")
		for t,e,j,sj in cl:
			v=tl[j][1]
			if (t==0):
				el=len(e)
				if (e[:1]==b"\""):
					e=e[1:-1]
				if (e not in tcm):
					tcm[e]=1
				else:
					tcm[e]+=1
				if (tl[j][0]=="stringS"):
					tl=tl[:j]+[("_rawF",v[:sj+1]),("css_class",e),("stringS",v[sj+el+1:])]+tl[j+1:]
				elif (tl[j][0]=="string" or tl[j][0]=="_raw"):
					tl=tl[:j]+[("_raw",v[:sj+1]),("css_class",e),("_raw",v[sj+el+1:])]+tl[j+1:]
				elif (tl[j][0]=="stringM" or tl[j][0]=="_rawF"):
					tl=tl[:j]+[("_rawF",v[:sj+1]),("css_class",e),("_rawF",v[sj+el+1:])]+tl[j+1:]
				elif (tl[j][0]=="stringE"):
					tl=tl[:j]+[("_rawF",v[:sj+1]),("css_class",e),("stringE",v[sj+el+1:])]+tl[j+1:]
				else:
					raise RuntimeError
			elif (t==1):
				el,ne,e=len(e),e.split(b".")[1:],(cvm[e.split(b".")[0]] if e.split(b".")[0] in cvm else e.split(b".")[0])
				for nee in ne:
					e+=(b"["+cvma[nee]+b"]" if nee in cvma else b"."+nee)
				tl[j]=(tl[j][0],v[:sj+1]+e+v[sj+el+1:])
			else:
				tl[j]=(tl[j][0],v[:sj+1]+e+v[sj+len(e)+3:])
		print("    Collecting String Constants...")
		vfms={}
		vfms_f={}
		l=[]
		si=-1
		for i,(k,v) in enumerate(tl):
			if (k=="string"):
				if (v[1:-1] not in vfms):
					vfms[v[1:-1]]=(1,[(i,i+1)])
				else:
					vfms[v[1:-1]]=(vfms[v[1:-1]][0]+1,vfms[v[1:-1]][1]+[(i,i+1)])
				if (len(l)>0):
					h=tuple(l)
					if (h not in vfms_f):
						vfms_f[h]=(l,1,[(si,i)])
					else:
						vfms_f[h]=(l,vfms_f[h][1]+1,vfms_f[h][2]+[(si,i)])
				l=[]
			elif (k=="_raw" or (k=="css_class" and len(l)>0)):
				if (len(l)==0):
					si=i
				l+=[(v,(True if k=="css_class" else False))]
			else:
				if (len(l)>0):
					h=tuple(l)
					if (h not in vfms_f):
						vfms_f[h]=(l,1,[(si,i)])
					else:
						vfms_f[h]=(l,vfms_f[h][1]+1,vfms_f[h][2]+[(si,i)])
				l=[]
		return (tl,cvm,cvma,vfms,vfms_f,sl)
	def _parse_css(css,tcm):
		sl=len(css)
		print("    Removing Whitespace & Shortening Colors...")
		css=re.sub(CSS_UNIT_REGEX,br":\1;",re.sub(CSS_HEX_COLOR_REGEX,br"#\1\2\3\4",re.sub(CSS_URL_REGEX,br"url(\2)",re.sub(CSS_WHITESPACE_REGEX,b"",css))))
		i=0
		eo=b""
		kfdl=[]
		kfl={}
		gc=[]
		sc=0
		pc=0
		ec=0
		tl=[]
		ml=[]
		print("    Parsing Styles...")
		while (i<len(css)):
			m=re.match(CSS_SELECTOR_VALUE_REGEX,css[i:])
			if (m!=None):
				i+=m.end(0)+1
				b=1
				si=i-1
				while (b!=0):
					i+=1
					if (css[i:i+1]==b"{"):
						b+=1
					if (css[i:i+1]==b"}"):
						b-=1
				s=[re.sub(CSS_SELECTOR_WHITESPACE_REGEX,b"",e.strip()) for e in m.group(1).split(b",")]
				if (CSS_WHITESPACE_SPLIT_REGEX.split(s[0])[0]==b"@keyframes"):
					s=CSS_WHITESPACE_SPLIT_REGEX.split(s[0])
					if (s[1] not in kfl):
						kfl[s[1]]=0
					v=[]
					l=[]
					t=[]
					for k in re.findall(CSS_KEYFRAMES_VALUE_REGEX,css[si:i]):
						v+=[{}]
						l+=[[]]
						t+=[k[0]]
						for e in re.findall(CSS_PROPERTY_KEY_VALUE_REGEX,k[1]):
							if (e[0].lower() not in l[-1]):
								l[-1]+=[e[0].lower()]
							v[-1][e[0].lower()]=e[1]
						pc+=len(l[-1])
						if (len(l[-1])==0):
							v=v[:-1]
							l=l[:-1]
							t=t[:-1]
					if (len(t)>0):
						kfdl+=[(s[1],b"{"+b"".join([k+b"{"+b";".join([e+b":"+re.sub(CSS_SELECTOR_COMMA_REGEX,b",",v[i][e]) for e in sorted(l[i])])+b"}" for i,k in enumerate(t)])+b"}")]
						ec+=1
				elif (CSS_WHITESPACE_SPLIT_REGEX.split(s[0])[0]==b"@media"):
					mr=_parse_css(css[si:i],tcm)
					sc+=mr[5]
					pc+=mr[6]
					ec+=mr[7]
					gc+=mr[8]
					ml+=[(s,mr[1:5])]
					ec+=1
				elif (len(s)==1 and s[0]==b"@font-face"):
					v={}
					l=[]
					for e in re.findall(CSS_PROPERTY_KEY_VALUE_REGEX,css[si:i].strip()):
						if (e[0].lower() not in l):
							l+=[e[0].lower()]
						v[e[0].lower()]=e[1]
					pc+=len(l)
					if (len(l)>0):
						eo+=b"@font-face{"+b";".join([e+b":"+re.sub(CSS_SELECTOR_COMMA_REGEX,b",",v[e]) for e in sorted(l)])+b"}"
						ec+=1
				elif (s[0][:1]==b"@"):
					raise RuntimeError(f"Unknown At-Rule: '{str(s[0],'utf-8')}'")
				else:
					ns=[_parse_css_selector(k,tcm,gc) for k in s]
					v={}
					l=[]
					for k in re.findall(CSS_PROPERTY_KEY_VALUE_REGEX,css[si:i].strip()):
						if (k[0].lower() in l):
							raise RuntimeError(f"Duplicate Property '{str(k[0].lower(),'utf-8')}' in CSS Selector '{str(b''.join(s),'utf-8')}'")
						l+=[k[0].lower()]
						v[k[0].lower()]=k[1]
					pc+=len(l)
					if (len(l)>0):
						tl+=[(l,v,ns)]
						for k in l:
							if (k==b"animation"):
								if (b"," in re.sub(CSS_BRACKET_REGEX,br"",v[k])):
									raise RuntimeError("Commas in animation not yet implemented!")
								nm=None
								for e in CSS_WHITESPACE_SPLIT_REGEX.split(v[k]):
									if (CSS_KEYFRAMES_NAME_REGEX.fullmatch(e) and e not in [b"running",b"paused",b"none",b"forwards",b"backwards",b"both",b"normal",b"reverse",b"alternate",b"alternate-reverse",b"infinite",b"linear",b"ease",b"ease-in",b"ease-out",b"ease-in-out",b"cubic-bezier",b"step-start",b"step-end",b"jump-start",b"jump-end",b"jump-none",b"jump-both",b"start",b"end"]):
										nm=e
										break
								if (nm==None):
									raise RuntimeError("No Animation Specified!")
								if (nm not in kfl):
									kfl[nm]=1
			i+=1
		nkfl={}
		kfl=[(k,v) for k,v in sorted(kfl.items(),key=lambda e:e[1]*len(e[0]))]
		for k,v in kfl:
			if (v==0):
				raise RuntimeError(f"@keyframes '{str(k,'utf-8')}' not Used!")
			nm=_gen_i((nkfl,),CSS_KEYFRAMES_LETTERS)
			if (len(nm)<=len(k)):
				nkfl[k]=nm
		for k,v in kfdl:
			eo+=b"@keyframes "+nkfl[k]+v
		vm={}
		vml=[]
		for l,v,ns in tl:
			sc+=len(ns)
			fs=b""
			for i,k in enumerate(l):
				if (i>0):
					fs+=b";"
				if (k==b"animation"):
					nv=b""
					for j,e in enumerate(CSS_WHITESPACE_SPLIT_REGEX.split(v[k])):
						if (CSS_KEYFRAMES_NAME_REGEX.fullmatch(e) and e not in [b"running",b"paused",b"none",b"forwards",b"backwards",b"both",b"normal",b"reverse",b"alternate",b"alternate-reverse",b"infinite",b"linear",b"ease",b"ease-in",b"ease-out",b"ease-in-out",b"cubic-bezier",b"step-start",b"step-end",b"jump-start",b"jump-end",b"jump-none",b"jump-both",b"start",b"end"]):
							e=nkfl[e]
						if (j>0):
							nv+=b" "
						nv+=e
					v[k]=nv
				fs+=k+b":"+re.sub(CSS_SELECTOR_COMMA_REGEX,b",",v[k])
			fsh=hashlib.sha1(fs).hexdigest()
			if (fsh not in vm):
				vml.append(fsh)
				vm[fsh]=(fs,ns)
			else:
				vml.remove(fsh)
				vml.append(fsh)
				vm[fsh][1].extend(ns)
		return (sl,vm,vml,ml,eo,sc,pc,ec,gc)
	def _write_css_selector(k,tcm):
		o=b""
		for ss,st,sc,sp,se in k:
			o+=ss+st
			for c in sc.split(b".")[1:]:
				o+=b"."+tcm[c]
			o+=sp+se
		return o
	def _write_js(tl,cvm,cvma,cvms,sl,tcm):
		o=b""
		i=0
		while (i<len(tl)):
			if (i>=sl and tl[i][0]=="identifier"):
				idl=tl[i][1].split(b".")
				dpc=False
				if (idl[0]==b"document" and len(idl)>1 and b"document."+idl[1] in cvm):
					idl=[cvm[b"document."+idl[1]]]+idl[2:]
				if (idl[0] in cvm):
					idl[0]=cvm[idl[0]]
				for j,e in enumerate(idl[(2 if dpc==True else 1):]):
					if (e in cvma):
						idl[j+1]=b"["+cvma[e]+b"]"
					else:
						idl[j+1]=b"."+e
				o+=b"".join(idl)
			elif (tl[i][0]=="css_selector"):
				o+=b"\""+_write_css_selector(tl[i][1],tcm)+b"\""
			elif (tl[i][0]=="css_class"):
				o+=tcm[tl[i][1]]
			elif (tl[i][0]=="css_class_str"):
				o+=b"\""+tcm[tl[i][1]]+b"\""
			elif (tl[i][0]=="keyword" and tl[i][1] in [b"false",b"true"]):
				o+={b"false":b"!1",b"true":b"!0"}[tl[i][1]]
			elif (tl[i][0]=="stringS"):
				o+=tl[i][1]
				to,ti=_write_js(tl[i+1:],cvm,cvma,cvms,0,tcm)
				o+=to
				i+=ti+1
			elif (tl[i][0]=="stringE"):
				o+=tl[i][1]
				break
			else:
				if (tl[i][0]=="keyword" and tl[i][1] in [b"of",b"in"]):
					o+=b" "
				o+=tl[i][1]
				if (tl[i][0]=="keyword" and (tl[i][1] in [b"let",b"const",b"var",b"throw",b"new",b"of",b"in"] or (i+1<len(tl) and tl[i][1]==b"else" and tl[i+1][0]=="keyword" and tl[i+1][1]==b"if") or (tl[i][1]==b"return" and (i==len(tl)-2 or ((tl[i+1][0]!="operator" or tl[i+1][1] not in b")]}") and (tl[i+1][0] not in ["string","stringS"] or tl[i+1][1][:1] not in b"'\"`")))))):
					o+=b" "
			i+=1
		return (o,i)
	def _write_css(vm,vml,ml,eo):
		o=b""
		for k in vml:
			o+=b",".join([_write_css_selector(sk,ntcm) for sk in vm[k][1]])+b"{"+vm[k][0]+b"}"
		for k,v in ml:
			o+=b",".join(k)+b"{"+_write_css(*v)+b"}"
		return o+eo
	def _write_html(e,tcm,js_om):
		o=b"<"+e[0]
		for k,v in e[1].items():
			if (k==b"class"):
				nv=b""
				for c in v.split(b" "):
					nv+=(b" " if len(nv)>0 else b"")+tcm[c]
				v=nv
			elif (str(k,"utf-8") in HTML_TAG_JS_ATTRIBUTES):
				m=JS_REGEX_LIST["identifier"].match(v)
				if (m!=None):
					v,ev=m.group(0).split(b".")[0],v[m.end(0):]
					if (v in js_om[0]):
						v=js_om[0][v]
					for i in m.group(0).split(b".")[1:]:
						v+=(b"["+js_om[1][i]+b"]" if i in js_om[1] else b"."+i)
					v+=ev
			if (o[-1]!=b"\""):
				o+=b" "
			q=(b"\"" if re.search(HTML_QUOTED_ATTRIBUTE_REGEX,v)!=None else b"")
			o+=k+b"="+q+v+q
		o+=b">"
		for t in e[2]:
			if (t[0]=="__text__"):
				o+=t[1]
			else:
				o+=_write_html(t,tcm,js_om)
		return o+b"</"+e[0]+b">"
	print(f"Minifying File '{fp}'...")
	l=len(html)
	r=None
	c=None
	css_t=None
	js_t=None
	tcm={}
	stcm={}
	html=HTML_REMOVE_WHITEPSACE_REGEX.sub(br"",html)
	i=0
	ttc=0
	vfm={}
	vfma={}
	svg=0
	n_pm={}
	print("  Parsing HTML...")
	while (i<len(html)):
		m=HTML_TAG_REGEX.search(html[i:])
		if (m is None):
			break
		j=m.start(0)
		if (j!=0):
			if (r is None):
				raise RuntimeError("Text Before <html> Tag")
			c[-1][2].append(("__text__",html[i:i+j]))
		t_nm=m.group(1)
		if (svg==0 and str(t_nm.lower()[(1 if t_nm[:1]==b"/" else 0):],"utf-8") not in HTML_TAGS):
			raise RuntimeError(f"Unknown Tag '{str(t_nm,'utf-8')}'")
		if (t_nm==b"svg"):
			svg+=1
		elif (t_nm==b"/svg"):
			svg-=1
		e=m.group(3)
		v=None
		pm={**n_pm}
		n_pm.clear()
		if (len(m.group(2))>0):
			for k,v in re.findall(HTML_ATTRIBUTE_REGEX,m.group(2)):
				if (svg==0 and (not (k[:5]==b"data-" and len(k)>5) and str(k,"utf-8") not in HTML_TAG_ATTRIBUTE_MAP or (HTML_TAG_ATTRIBUTE_MAP[str(k,"utf-8")]!=None and str(t_nm,"utf-8") not in HTML_TAG_ATTRIBUTE_MAP[str(k,"utf-8")]))):
					raise RuntimeError(f"Tag <{str(t_nm,'utf-8')}> Contains an Invalid Attribute '{str(k,'utf-8')}'")
				elif (str(k,"utf-8") in HTML_TAG_JS_ATTRIBUTES):
					sm=JS_REGEX_LIST["identifier"].match(v)
					if (sm!=None):
						sm=sm.group(0).split(b".")
						if (len(sm)>1 and sm[0]==b"window" and str(sm[1],"utf-8") in JS_RESERVED_IDENTIFIERS and str(sm[1],"utf-8") not in JS_KEYWORDS):
							sm=sm[1:]
						if (sm[0] not in vfm):
							vfm[sm[0]]=1
						else:
							vfm[sm[0]]+=1
						for sme in sm[1:]:
							if (sme not in vfma):
								vfma[sme]=1
							else:
								vfma[sme]+=1
				pm[k]=v
		v=None
		if (t_nm==b"script" and b"type" in pm and pm[b"type"]==b"text/javascript" and b"src" in pm and b"async" not in pm and b"defer" not in pm):
			if (os.path.exists(fp_b+str(pm[b"src"],"utf-8"))):
				print(f"  Found Local JavaScript: '{fp_b+str(pm[b'src'],'utf-8')}'")
				with open(fp_b+str(pm[b"src"],"utf-8"),"rb") as rf:
					dt=rf.read()
					v=("__js__",dt)
			elif (HTML_URL_REGEX.match(pm[b"src"])):
				print(f"  Found External JavaScript: '{str(pm[b'src'],'utf-8')}'")
				v=("__js__",_get_url_cache(pm[b"src"]))
			else:
				raise RuntimeError(f"Unable to Decode <script> src: '{pm[b'src']}'")
			pm={b"type":b"text/javascript"}
		elif (t_nm==b"link" and b"rel" in pm and pm[b"rel"]==b"stylesheet" and b"href" in pm):
			if (os.path.exists(fp_b+str(pm[b"href"],"utf-8"))):
				print(f"  Found Local CSS: '{fp_b+str(pm[b'href'],'utf-8')}'")
				with open(fp_b+str(pm[b"href"],"utf-8"),"rb") as rf:
					dt=rf.read()
					v=("__css__",dt)
			elif (HTML_URL_REGEX.match(pm[b"href"])):
				print(f"  Found External CSS: '{str(pm[b'href'],'utf-8')}'")
				v=("__css__",_get_url_cache(pm[b"href"]))
			else:
				raise RuntimeError(f"Unable to Decode <link> href: '{pm[b'href']}'")
			t_nm=b"style"
			pm={b"type":b"text/css"}
		elif (t_nm==b"img" and b"src" in pm and pm[b"src"][:1]==b"/" and pm[b"src"][-4:]==b".svg"):
			print(f"  Found Local SVG Image: '{fp_b+str(pm[b'src'],'utf-8')}'")
			with open(fp_b+str(pm[b"src"],"utf-8"),"rb") as rf:
				dt=rf.read()
				l+=len(dt)
				html=html[:i]+HTML_REMOVE_WHITEPSACE_REGEX.sub(br"",dt)+html[i+m.end(0):]
				for k,v in pm.items():
					if (k!=b"src"):
						n_pm[k]=v
				continue
		elif (t_nm==b"svg"):
			svg+=1
		elif (t_nm==b"/svg"):
			svg-=1
		if (t_nm.lower()!=b"!doctype" and (v is None or ((v[0]=="__css__" and css_t is None) or (v[0]=="__js__" and js_t is None)))):
			if (b"class" in pm):
				cs=b""
				for ce in c:
					cs+=ce[0]+(b"#"+ce[1][b"id"] if b"id" in ce[1] else b"")+(b"."+ce[1][b"class"].replace(b" ",b".") if b"class" in ce[1] else b"")+b"$"
				cs+=t_nm+(b"#"+pm[b"id"] if b"id" in pm else b"")+(b"."+pm[b"class"].replace(b" ",b".") if b"class" in pm else b"")
				for tc in pm[b"class"].split(b" "):
					if (tc not in tcm):
						tcm[tc]=1
					else:
						tcm[tc]+=1
					if (tc not in stcm):
						stcm[tc]=[cs]
					elif (cs not in stcm[tc]):
						stcm[tc]+=[cs]
			if (r is None):
				ttc+=1
				r=(t_nm,pm,[])
				c=[r]
				if (len(e)!=0):
					raise RuntimeError("<html/>")
			else:
				if (t_nm[:1]==b"/"):
					if (t_nm[1:]!=c[-1][0]):
						raise RuntimeError(f"Expected '</{str(c[-1][0],'utf-8')}>', found '<{str(t_nm,'utf-8')}>'")
					c=c[:-1]
				else:
					ttc+=1
					c[-1][2].append((t_nm,pm,[]))
					if (len(e)==0 and str(t_nm,"utf-8") not in HTML_AUTO_CLOSE_TAGS):
						c+=[c[-1][2][-1]]
		if (v!=None):
			if (v[0]=="__js__"):
				if (js_t is None):
					c[-1][2].append((v[1],1))
					js_t=c[-1][2]
				else:
					js_t[0]=(v[1]+b"\n\n\n"+js_t[0][0],js_t[0][1]+1)
			else:
				if (css_t is None):
					c[-1][2].append((v[1],1))
					css_t=c[-1][2]
					c=c[:-1]
				else:
					css_t[0]=(v[1]+b"\n\n\n"+css_t[0][0],css_t[0][1]+1)
		i+=m.end(0)
	if (len(c)):
		raise RuntimeError(f"Unclosed Tags: ['{(chr(39)+'; '+chr(39)).join(['<'+str(e[0],'utf-8')+'>' for e in c[::-1]])}']")
	if (js_t!=None):
		print(f"  Parsing JS ({js_t[0][1]} script{('s' if js_t[0][1]!=1 else '')}, {len(js_t[0][0])} byte{('s' if len(js_t[0][0])!=1 else '')})...")
		l+=len(js_t[0][0])
		js_t[0]=(len(js_t[0][0]),*_parse_js(js_t[0][0],tcm,vfm,vfma))
	if (css_t!=None):
		print(f"  Parsing CSS ({css_t[0][1]} script{('s' if css_t[0][1]!=1 else '')}, {len(css_t[0][0])} byte{('s' if len(css_t[0][0])!=1 else '')})...")
		l+=len(css_t[0][0])
		css_t[0]=_parse_css(css_t[0][0],tcm)
		for k in css_t[0][8]:
			if (k in stcm):
				del stcm[k]
	print(f"  Generating Class Names ({len(tcm.keys())} item{('s' if len(tcm.keys())!=1 else '')})...")
	for k,v in stcm.items():
		if (len(v)>1):
			bl=v[0].count(b"$")
			b=v[0][:-len(v[0].split(b"$")[-1])-1]
			ok=True
			for e in v[1:]:
				if (e.count(b"$")!=bl or e[:-len(e.split(b"$")[-1])-1]!=b):
					ok=False
					break
			if (ok==True):
				continue
			raise RuntimeError(f"Duplicate Non-Global Multi-Level Class '{str(k,'utf-8')}' in {len(v)} Tags: ['{(chr(39)+'; '+chr(39)).join([str(e,'utf-8').replace('$',' > ') for e in v])}']")
	ntcm={}
	for k,v in sorted(tcm.items(),key=lambda e:-len(e[0])*e[1]):
		ntcm[k]=_gen_i([ntcm],CSS_CLASS_LETTERS)
	js_s="none"
	js_om=({},{})
	if (js_t!=None):
		print(f"  Regenerating JS ({len(js_t[0][1])} token{('s' if len(js_t[0][1])!=1 else '')})...")
		ssl=js_t[0][0]
		js_om=js_t[0][2:4]
		tl,cvm,cvma,vfms,vfms_f,sl=js_t[0][1:]
		for k,v,pl in vfms_f.values():
			s=b""
			for e in k:
				s+=(ntcm[e[0]] if e[1]==True else e[0])
			s=s[1:-1]
			if (s not in vfms):
				vfms[s]=(v,pl)
			else:
				vfms[s]=(vfms[s][0]+v,vfms[s][1]+pl)
		print(f"    Creaing String Constants ({len(vfms)} string{('s' if len(vfms)!=1 else '')})...")
		vfms_l=[]
		for k,v in vfms.items():
			if (v[0]>1):
				vfms_l+=[((len(k)+2)*v[0],k,v[0],v[1])]
		print("    Sorting String Constant List...")
		vfms_l=sorted(vfms_l,key=lambda e:-e[0])
		cvms={}
		for k in vfms_l:
			mv=_gen_i([cvm,cvma,cvms],JS_CONST_LETTERS)
			if (len(mv)+len(k[1])+len(mv)*k[2]+4<=(len(k[1])+2)*k[2]):
				for e in k[3]:
					cvms[e]=mv
				if (len(sl)==0):
					sl=[("keyword",b"let")]
				else:
					sl+=[("operator",b",")]
				sl+=[("identifier",mv),("operator",b"="),("string",b"`"+JS_HTML_STRING_UNESCAPE.sub(br"\1\2",k[1].replace(b"`",b"\\`"))+b"`")]
		for k,v in sorted(cvms.items(),key=lambda e:-e[0][1]):
			tl=tl[:k[0]]+[("identifier",v)]+tl[k[1]:]
		if (len(sl)>0):
			tl=sl+[("operator",b";")]+tl
		js_t[0]=("__text__",_write_js(tl,cvm,cvma,cvms,len(sl),ntcm)[0])
		js_s=f"{ssl} -> {len(js_t[0][1])} (-{round(10000-10000*len(js_t[0][1])/ssl)/100}%)"
	css_s="none"
	if (css_t!=None):
		sl,vm,vml,ml,eo,sc,pc,ec,_=css_t[0]
		print(f"  Regenerating CSS ({len(vml)} style{('s' if len(vml)!=1 else '')}, {sc} selector{('s' if sc!=1 else '')}, {pc} propert{('ies' if pc!=1 else 'y')}, {ec} @-rule{('s' if ec!=1 else '')})...")
		css_o=_write_css(vm,vml,ml,eo)
		css_t[0]=("__text__",css_o)
		css_s=f"{sl} -> {len(css_o)} (-{round(10000-10000*len(css_o)/sl)/100}%)"
	print(f"  Writing HTML ({ttc} tag{('s' if ttc!=1 else '')})...")
	o=b"<!DOCTYPE html>\n"+_write_html(r,ntcm,js_om)
	print(f"Minified HTML: {{\n  JS: {js_s}\n  CSS: {css_s}\n  HTML: {l} -> {len(o)} (-{round(10000-10000*len(o)/l)/100}%)\n}}")
	return o
