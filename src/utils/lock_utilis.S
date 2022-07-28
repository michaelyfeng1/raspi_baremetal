.global __lock_acq_mem
__lock_acq_mem:
	mov	w2, #1
	sevl
l1:	wfe
l2:	ldaxr	w1, [x0]
	cbnz	w1, l1
	stxr	w1, w2, [x0]
	//mov     w3, #1
	//stlr	w3, [x0]
	cbnz	w1, l2
	ret

.global __lock_rel_mem
__lock_rel_mem:
	stlr	wzr, [x0]
	ret

//Atomic Memory Access Notes

/**
	LDXR : check exclusive tag, label esclusive tag and GM will be noticed

	STXR : check exclusive tag, if tag is not set, will fail the memory write, and return non-zeor value
			                    if tag is set, will write memory with value and clear the tag to non-exclusive

	LDAXR


	LDAR

	STLR
 */