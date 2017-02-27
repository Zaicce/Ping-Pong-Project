

.align 2
.text

.global enInterrupt

enInterrupt:
	ei
	jr	$ra
	nop
		