	PROGRAM DRAWLINE

C	Line coordinates
	REAL PXA(2)
	REAL PYA(2)

C	Open PHIGS and a workstation
	CALL POPPH(0, 0)
	CALL POPWK(0, 0, 0)

C	Open structure
	CALL POPST(0)

C	Draw line from lower left to top right
	PXA(1) = 0.0
	PYA(1) = 0.0
	PXA(2) = 1.0
	PYA(2) = 1.0
	CALL PPL(2, pxa, pya)

C	Draw line from lower right to top left
	PXA(1) = 1.0
	PYA(1) = 0.0
	PXA(2) = 0.0
	PYA(2) = 1.0
	CALL PPL(2, pxa, pya)

C	Close structure
	CALL PCLST

C	Post structure to workstation
	CALL PPOST(0, 0, 0.0)

C	Buisy loop
	DO WHILE (1 .GT. 0)
	END DO

	STOP
	END

