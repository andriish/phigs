/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2011 Surplus Users Ham Society
*
*   Open PHIGS is free software: you can redistribute it and/or modify
*   it under the terms of the GNU Lesser General Public License as published by
*   the Free Software Foundation, either version 2.1 of the License, or
*   (at your option) any later version.
*
*   Open PHIGS is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU Lesser General Public License for more details.
*
*   You should have received a copy of the GNU Lesser General Public License
*   along with Open PHIGS. If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/

#ifndef _phg_el_h
#define _phg_el_h

#include <stdio.h>
#include <stdlib.h>
#include <phigs/phg.h>
#include <phigs/private/cssP.h>

int phg_handle_names_set(Css_handle cssh,
                         El_handle elmt,
                         caddr_t argdata,
                         Css_el_op op);
int phg_handle_local_tran(Css_handle cssh,
                          El_handle elmt,
                          caddr_t argdata,
                          Css_el_op op);
int phg_handle_local_tran3(Css_handle cssh,
                           El_handle elmt,
                           caddr_t argdata,
                           Css_el_op op);
int phg_handle_point_list(Css_handle cssh,
                          El_handle elmt,
                          caddr_t argdata,
                          Css_el_op op);
int phg_handle_point_list3(Css_handle cssh,
                           El_handle elmt,
                           caddr_t argdata,
                           Css_el_op op);
int phg_handle_text(Css_handle cssh,
                    El_handle elmt,
                    caddr_t argdata,
                    Css_el_op op);
int phg_handle_int(Css_handle cssh,
                   El_handle elmt,
                   caddr_t argdata,
                   Css_el_op op);
int phg_handle_float(Css_handle cssh,
                     El_handle elmt,
                     caddr_t argdata,
                     Css_el_op op);
int phg_handle_edge_flag(Css_handle cssh,
                         El_handle elmt,
                         caddr_t argdata,
                         Css_el_op op);
int phg_handle_int_style(Css_handle cssh,
                         El_handle elmt,
                         caddr_t argdata,
                         Css_el_op op);

#endif

