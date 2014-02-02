C *****************************************************************************
C * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
C *
C * This file is part of Open PHIGS
C * Copyright (C) 2014 Surplus Users Ham Society
C *
C * Open PHIGS is free software: you can redistribute it and/or modify
C * it under the terms of the GNU Lesser General Public License as published by
C * the Free Software Foundation, either version 2.1 of the License, or
C * (at your option) any later version.
C *
C * Open PHIGS is distributed in the hope that it will be useful,
C * but WITHOUT ANY WARRANTY; without even the implied warranty of
C * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
C * GNU Lesser General Public License for more details.
C *
C * You should have received a copy of the GNU Lesser General Public License
C * along with Open PHIGS. If not, see <http://www.gnu.org/licenses/>.
C *****************************************************************************

	PROGRAM DRAWLINE

C	Include PHIGS enumeration file
	INCLUDE 'phigsf77.h'

C	Delcare arrays
	REAL COLR(1:3)
	REAL PXA(1:2)
	REAL PYA(1:2)

C	Open PHIGS and a workstation
	CALL POPPH(0, 0)
	CALL POPWK(0, 0, 0)

C	Define line colour in workstation table
	COLR(1) = 1.0
	COLR(2) = 1.0
	COLR(3) = 0.0
	CALL PSCR(0, 1, 3, COLR)

C	Open structure
	CALL POPST(0)

C	Set text attributes
	CALL PSCHH(0.055)

C	Draw text
	CALL PTX(0.22, 0.48, 'Hello Fortran!')

C	Set line attributes
	CALL PSPLCI(1)
	CALL PSLN(PLDASD)
	CALL PSLWSC(2.0)

C	Draw line from bottom-left to top-right
	PXA(1) = 0.0
	PYA(1) = 0.0
	PXA(2) = 1.0
	PYA(2) = 1.0
	CALL PPL(2, PXA, PYA)

C	Draw line from bottom-right to top-left
	PXA(1) = 1.0
	PYA(1) = 0.0
	PXA(2) = 0.0
	PYA(2) = 1.0
	CALL PPL(2, PXA, PYA)

C	Close structure
	CALL PCLST

C	Post structure to workstation
	CALL PPOST(0, 0, 0.0)

C	Buisy loop
	DO WHILE (1 .GT. 0)
	END DO

	STOP
	END
