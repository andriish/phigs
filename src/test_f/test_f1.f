	program drawline

c	Line coordinates
	real pxa(2)
	real pya(2)

c	Open PHIGS and a workstation
	call popph(0, 0)
	call popwk(0, 0, 0)

c	Open structure
	call popst(0)

c	Draw line from lower left to top right
	pxa(1) = 0.0
	pya(1) = 0.0
	pxa(2) = 1.0
	pya(2) = 1.0
	call ppl(2, pxa, pya)

c	Draw line from lower right to top left
	pxa(1) = 1.0
	pya(1) = 0.0
	pxa(2) = 0.0
	pya(2) = 1.0
	call ppl(2, pxa, pya)

c	Close structure
	call pclst

c	Post structure to workstation
	call ppost(0, 0, 0.0)

c	Buisy loop
	do while (1 .gt. 0)
	end do

	stop
	end

