import sys



if ("-a" in sys.argv or "--all" in sys.argv or "-R" in sys.argv or "--rsrc" in sys.argv):
	import create_resource
if ("-a" in sys.argv or "--all" in sys.argv or "-d" in sys.argv or "--docs" in sys.argv):
	import create_docs
if ("-a" in sys.argv or "--all" in sys.argv or "-k" in sys.argv or "--kernel" in sys.argv):
	import create_kernel
if ("-a" in sys.argv or "--all" in sys.argv or "-r" in sys.argv or "--run" in sys.argv):
	import run
