#ifndef _phigsfunc_h
#define _phigsfunc_h

void popen_phigs(char *error_file, size_t memory);
void popen_ws(Pint ws_id, void *conn_id, Pint ws_type);
void popen_struct(Pint struct_id);
void pclose_struct(void);
void pset_elem_ptr(Pint elem_ptr_value);
void poffset_elem_ptr(Pint elem_ptr_offset);
void pset_elem_ptr_label(Pint label_id);
void pdel_elem(void);
void pdel_elem_range(Pint elem_ptr1_value, Pint elem_ptr2_value);
void pdel_elems_labels(Pint label1_id, Pint label2_id);
void pinq_elem_ptr(Pint *err_ind, Pint *elem_ptr_value);
void pcopy_all_elems_struct(Pint struct_id);
void padd_names_set(Pint_list *names);
void premove_names_set(Pint_list *names);
void pset_local_tran(Pmatrix local_tran, Pcompose_type compose_type);
void pset_local_tran3(Pmatrix3 local_tran, Pcompose_type compose_type);
void pset_view_ind3(Pint index);
void ptext(Ppoint *text_pos, char *char_string);
void ppolyline(Ppoint_list *point_list);
void ppolyline3(Ppoint_list3 *point_list);
void ppolymarker(Ppoint_list *point_list);
void ppolymarker3(Ppoint_list3 *point_list);
void pfill_area(Ppoint_list *point_list);
void pfill_area3(Ppoint_list3 *point_list);
void plabel(Pint label_id);
void pset_pick_id(Pint pick_id);
void pset_int_colr_ind(Pint colr_ind);
void pset_int_style(Pint_style int_style);
void pset_line_colr_ind(Pint colr_ind);
void pset_linewidth(Pfloat linewidth);
void pset_linetype(Pint linetype);
void pset_marker_colr_ind(Pint colr_ind);
void pset_marker_size(Pfloat marker_size);
void pset_marker_type(Pint marker_type);
void pset_edge_colr_ind(Pint colr_ind);
void pset_edgetype(Pint edgetype);
void pset_edge_flag(Pedge_flag edge_flag);
void pset_edgewidth(Pfloat edgewidth);
void pset_text_font(Pint font);
void pset_text_colr_ind(Pint colr_ind);
void pexec_struct(Pint struct_id);
void pset_colr_rep(Pint ws, Pint index, Pcolr_rep *rep);

#endif

