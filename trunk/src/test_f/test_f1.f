	program drawline

	real pxa(2)
	real pya(2)

	call popph(0, 0)
	call popwk(0, 0, 0)

	call popst(0)

	pxa(1) = 0.0
	pya(1) = 0.0
	pxa(2) = 1.0
	pya(2) = 1.0
	call ppl(2, pxa, pya)

	pxa(1) = 1.0
	pya(1) = 0.0
	pxa(2) = 0.0
	pya(2) = 1.0
	call ppl(2, pxa, pya)

	call pclst

	call ppost(0, 0, 0.0)

	do while (1 .gt. 0)
	end do

	stop
	end
