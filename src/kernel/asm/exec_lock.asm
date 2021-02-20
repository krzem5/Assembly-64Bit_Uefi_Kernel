bits 64
global exec_lock
global exec_unlock



exec_lock:
	cli
	ret



exec_unlock:
	sti
	ret
