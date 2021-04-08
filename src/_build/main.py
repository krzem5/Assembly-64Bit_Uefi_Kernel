import ctypes
import ctypes.wintypes
import os
import sys



class StdStreamWrapper(object):
	def __init__(self,s):
		self.__std_s=s
	def __getattr__(self,k):
		return getattr(self.__std_s,k)
	def write(self,dt):
		ctypes.windll.kernel32.SetConsoleMode(ctypes.windll.kernel32.GetStdHandle(-11),ctypes.wintypes.DWORD(7))
		self.__std_s.write(dt)



if (os.name=="nt"):
	ctypes.windll.kernel32.SetConsoleMode.argtypes=(ctypes.wintypes.HANDLE,ctypes.wintypes.DWORD)
	ctypes.windll.kernel32.SetConsoleMode.restype=ctypes.wintypes.BOOL
	ctypes.windll.kernel32.GetStdHandle.argtypes=(ctypes.wintypes.DWORD,)
	ctypes.windll.kernel32.GetStdHandle.restype=ctypes.wintypes.HANDLE
	sys.stdout=StdStreamWrapper(sys.stdout)
	sys.stderr=StdStreamWrapper(sys.stderr)
dbg=("--debug" in sys.argv)
if ("-a" in sys.argv or "--all" in sys.argv or "-R" in sys.argv or "--rsrc" in sys.argv):
	from create_resource import create_resource
	create_resource(dbg)
if ("-a" in sys.argv or "--all" in sys.argv or "-d" in sys.argv or "--docs" in sys.argv):
	from create_docs import create_docs
	create_docs(dbg)
if ("-a" in sys.argv or "--all" in sys.argv or "-k" in sys.argv or "--kernel" in sys.argv):
	from create_kernel import create_kernel
	create_kernel(dbg)
if ("-a" in sys.argv or "--all" in sys.argv or "-r" in sys.argv or "--run" in sys.argv):
	from run import run
	run(dbg)
