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

