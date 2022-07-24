.global __lock_acq_mem
__lock_acq_mem:
	mov	w2, #1
	sevl
l1:	wfe
l2:	ldaxr	w1, [x0]
	cbnz	w1, l1
	stxr	w1, w2, [x0]
	cbnz	w1, l2
	ret

.global __lock_rel_mem
__lock_rel_mem:
	stlr	wzr, [x0]
	ret