import os
import re
import sys
if (os.name=="nt"):
	import ctypes
	import ctypes.wintypes



class StdStreamWrapper(object):
	def __init__(self,s,f_cl):
		self.__std_s=s
		self.__f_cl=f_cl
	def __getattr__(self,k):
		return getattr(self.__std_s,k)
	def write(self,dt):
		if (self.__f_cl):
			if (os.name=="nt"):
				ctypes.windll.kernel32.SetConsoleMode(ctypes.windll.kernel32.GetStdHandle(-11),ctypes.wintypes.DWORD(7))
		else:
			dt=re.sub(r"\x1b\[[^m]+?m","",dt)
		self.__std_s.write(dt)



if (os.name=="nt"):
	ctypes.windll.kernel32.SetConsoleMode.argtypes=(ctypes.wintypes.HANDLE,ctypes.wintypes.DWORD)
	ctypes.windll.kernel32.SetConsoleMode.restype=ctypes.wintypes.BOOL
	ctypes.windll.kernel32.GetStdHandle.argtypes=(ctypes.wintypes.DWORD,)
	ctypes.windll.kernel32.GetStdHandle.restype=ctypes.wintypes.HANDLE
sys.stdout=StdStreamWrapper(sys.stdout,("--no-color" not in sys.argv))
sys.stderr=StdStreamWrapper(sys.stderr,("--no-color" not in sys.argv))
dbg=("--debug" in sys.argv)
if ("--all" in sys.argv or "--rsrc" in sys.argv):
	from create_resource import create_resource
	create_resource(dbg)
if ("--all" in sys.argv or "--docs" in sys.argv):
	from create_docs import create_docs
	create_docs(dbg)
if ("--all" in sys.argv or "--kernel" in sys.argv):
	from create_kernel import create_kernel
	create_kernel(dbg)
if ("--all" in sys.argv or "--run" in sys.argv):
	from run import run
	run(dbg)
