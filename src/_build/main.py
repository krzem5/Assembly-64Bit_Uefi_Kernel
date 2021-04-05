import sys
from create_docs import create_docs
from create_resource import create_resource
from create_kernel import create_kernel
from run import run



dbg=("--debug" in sys.argv)
if ("-a" in sys.argv or "--all" in sys.argv or "-R" in sys.argv or "--rsrc" in sys.argv):
	create_resource(dbg)
if ("-a" in sys.argv or "--all" in sys.argv or "-d" in sys.argv or "--docs" in sys.argv):
	create_docs(dbg)
if ("-a" in sys.argv or "--all" in sys.argv or "-k" in sys.argv or "--kernel" in sys.argv):
	create_kernel(dbg)
if ("-a" in sys.argv or "--all" in sys.argv or "-r" in sys.argv or "--run" in sys.argv):
	run(dbg)
