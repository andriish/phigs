/******************************************************************************
*   DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS HEADER
*
*   This file is part of Open PHIGS
*   Copyright (C) 2011 - 2012 Surplus Users Ham Society
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

#ifndef _phigs_h
#define _phigs_h

/* Max name length */
#define PHIGS_MAX_NAME_LEN   255

/* Line types */
#define PLINE_SOLID            1
#define PLINE_DASH             2
#define PLINE_DOT              3
#define PLINE_DASH_DOT         4

/* Marker types */
#define PMARKER_DOT            1
#define PMARKER_PLUS           2
#define PMARKER_ASTERISK       3
#define PMARKER_CIRLCE         4
#define PMARKER_CROSS          5

/* Color model */
#define PINDIRECT              0
#define PMODEL_RGB             1

/* HLHSR constants */
#define PHIGS_HLHSR_MODE_NONE  0
#define PHIGS_HLHSR_MODE_ZBUFF 1

#define PHIGS_HLHSR_ID_OFF     0
#define PHIGS_HLHSR_ID_ON      1

/* PHIGS states */
typedef enum {
   PSYS_ST_PHCL,
   PSYS_ST_PHOP
} Psys_st;

typedef enum {
   PWS_ST_WSCL,
   PWS_ST_WSOP
} Pws_st;

typedef enum {
   PSTRUCT_ST_STCL,
   PSTRUCT_ST_STOP
} Pstruct_st;

/* Workstation related */
typedef enum {
   PCLASS_VEC,
   PCLASS_RASTER,
   PCLASS_OTHER
} Pws_class;

typedef enum {
   PCAT_OUT,
   PCAT_IN,
   PCAT_OUTIN,
   PCAT_MO,
   PCAT_MI
} Pws_cat;

typedef enum {
   PELEM_ALL,
   PELEM_NIL,
   PELEM_ADD_NAMES_SET,
   PELEM_REMOVE_NAMES_SET,
   PELEM_FILL_AREA,
   PELEM_FILL_AREA3,
   PELEM_POLYLINE,
   PELEM_POLYLINE3,
   PELEM_POLYMARKER,
   PELEM_POLYMARKER3,
   PELEM_TEXT,
   PELEM_INT_IND,
   PELEM_INT_COLR_IND,
   PELEM_INT_STYLE,
   PELEM_INT_STYLE_IND,
   PELEM_LINE_COLR_IND,
   PELEM_LINEWIDTH,
   PELEM_LINETYPE,
   PELEM_LINE_IND,
   PELEM_MARKER_IND,
   PELEM_MARKER_COLR_IND,
   PELEM_MARKER_SIZE,
   PELEM_MARKER_TYPE,
   PELEM_EDGE_IND,
   PELEM_EDGE_COLR_IND,
   PELEM_EDGEWIDTH,
   PELEM_EDGETYPE,
   PELEM_EDGE_FLAG,
   PELEM_TEXT_IND,
   PELEM_TEXT_FONT,
   PELEM_TEXT_COLR_IND,
   PELEM_LOCAL_MODEL_TRAN,
   PELEM_LOCAL_MODEL_TRAN3,
   PELEM_VIEW_IND,
   PELEM_EXEC_STRUCT,
   PELEM_LABEL,
   PELEM_PICK_ID,
   PELEM_HLHSR_ID,
   PELEM_INT_COLR,
   PELEM_LINE_COLR,
   PELEM_MARKER_COLR,
   PELEM_EDGE_COLR,
   PELEM_TEXT_COLR,
   PELEM_NUM_EL_TYPES
} Pelem_type;

typedef enum {
   PDIR_BACKWARD,
   PDIR_FORWARD
} Psearch_dir;

typedef enum {
   PSEARCH_STATUS_FAILURE,
   PSEARCH_STATUS_SUCCESS
} Psearch_status;

typedef enum {
   PEDIT_INSERT,
   PEDIT_REPLACE
} Pedit_mode;

typedef enum {
   PFLAG_DEL,
   PFLAG_KEEP
} Pref_flag;

typedef enum {
   PERR_OFF,
   PERR_ON
} Perr_mode;

typedef enum {
   PSTRUCT_NONE,
   PSTRUCT_OPEN
} Popen_struct_status;

typedef enum {
   PSTRUCT_STATUS_NON_EXISTENT,
   PSTRUCT_STATUS_EMPTY,
   PSTRUCT_STATUS_NOT_EMPTY
} Pstruct_status;

typedef enum {
   PORDER_TOP_FIRST,
   PORDER_BOTTOM_FIRST
} Ppath_order;

typedef enum {
   PEDGE_OFF,
   PEDGE_ON
} Pedge_flag;

typedef enum {
   PSTYLE_HOLLOW,
   PSTYLE_SOLID,
   PSTYLE_PAT,
   PSTYLE_HATCH
} Pint_style;

typedef enum {
   PTYPE_PRECONCAT,
   PTYPE_POSTCONCAT,
   PTYPE_REPLACE
} Pcompose_type;

typedef enum {
   PFLAG_COND,
   PFLAG_ALWAYS
} Pctrl_flag;

typedef enum {
   PFLAG_POSTPONE,
   PFLAG_PERFORM
} Pregen_flag;

typedef enum {
   PIND_NO_CLIP,
   PIND_CLIP
} Pclip_ind;

typedef enum {
   PPRI_HIGHER,
   PPRI_LOWER
} Prel_pri;

typedef enum {
   PVISUAL_ST_CORRECT,
   PVISUAL_ST_DEFER,
   PVISUAL_ST_SIMULATED
} Pvisual_st;

typedef enum {
   PSURF_NOT_EMPTY,
   PSURF_EMPTY
} Pdisp_surf_empty;

typedef enum {
   PDEFER_ASAP,
   PDEFER_BNIG,
   PDEFER_BNIL,
   PDEFER_ASTI,
   PDEFER_WAIT
} Pdefer_mode;

typedef enum {
   PMODE_NIVE,
   PMODE_UWOR,
   PMODE_UQUM
} Pmod_mode;

typedef enum {
   PUPD_NOT_PEND,
   PUPD_PEND
} Pupd_st;

typedef enum {
   PINQ_SET,
   PINQ_REALIZED
} Pinq_type;

typedef enum {
   PTYPE_PARAL,
   PTYPE_PERSPECT
} Pproj_type;

typedef enum {
   PASF_BUNDLED,
   PASF_INDIV
} Pasf;

typedef enum {
   PFLAG_LINE,
   PFLAG_FILL,
   PFLAG_FILL_SET
} Pline_fill_ctrl_flag;

typedef enum {
   PPR_OFF,
   PPR_ON
} Ppr_switch;

typedef enum {
   PDC_METRES,
   PDC_OTHER
} Pdc_units;

typedef enum {
   PIN_STATUS_NONE,
   PIN_STATUS_OK,
   PIN_STATUS_NO_IN
} Pin_status;

typedef enum {
   PIN_NONE,
   PIN_LOC,
   PIN_STROKE,
   PIN_VAL,
   PIN_CHOICE,
   PIN_PICK,
   PIN_STRING
} Pin_class;

typedef enum {
   POP_REQ,
   POP_SAMPLE,
   POP_EVENT
} Pop_mode;

typedef enum {
   PSWITCH_NO_ECHO,
   PSWITCH_ECHO
} Pecho_switch;

typedef int Pint;
typedef long Plong;
typedef float Pfloat;

typedef Pfloat Pmatrix3[4][4];
typedef Pfloat Pmatrix[3][3];

typedef struct {
   Pint       num_elem_types;
   Pelem_type *elem_types;
} Pelem_type_list;

typedef struct {
   Pint struct_id;
   Pint elem_pos;
} Pelem_ref;

typedef struct {
   Pint      num_elem_refs;
   Pelem_ref *elem_refs;
} Pelem_ref_list;

typedef struct {
   Pint         type;
   union {
      Pint      ind;
      struct {
         Pfloat x;
         Pfloat y;
         Pfloat z;
      }         general;
   }            val;
} Pgcolr;

typedef struct {
   Pint   type;
   Pfloat width;
   Pint   colr_ind;
} Pline_bundle;

typedef struct {
   Pint   type;
   Pfloat width;
   Pgcolr colr;
} Pline_bundle_plus;

typedef struct {
   Pint   type;
   Pfloat size;
   Pint   colr_ind;
} Pmarker_bundle;

typedef struct {
   Pint   type;
   Pfloat size;
   Pgcolr colr;
} Pmarker_bundle_plus;

typedef struct {
   Pedge_flag flag;
   Pint       type;
   Pfloat     width;
   Pint       colr_ind;
} Pedge_bundle;

typedef struct {
   Pedge_flag flag;
   Pint       type;
   Pfloat     width;
   Pgcolr     colr;
} Pedge_bundle_plus;

typedef struct {
   Pint_style style;
   Pint       style_ind;
   Pint       colr_ind;
} Pint_bundle;

typedef struct {
   Pint_style style;
   Pint       style_ind;
   Pgcolr     colr;
} Pint_bundle_plus;

typedef struct {
   Pint font;
   Pint colr_ind;
} Ptext_bundle;

typedef struct {
   Pint   font;
   Pgcolr colr;
} Ptext_bundle_plus;

typedef struct {
   Pint num_ints;
   Pint *ints;
} Pint_list;

typedef struct {
   Pfloat x;
   Pfloat y;
} Ppoint;

typedef struct {
   Pfloat x;
   Pfloat y;
   Pfloat z;
} Ppoint3;

typedef struct {
   Pfloat x;
   Pfloat y;
   Pfloat z;
   Pfloat w;
} Ppoint4;

typedef struct {
   Pfloat delta_x;
   Pfloat delta_y;
} Pvec;

typedef struct {
   Pfloat delta_x;
   Pfloat delta_y;
   Pfloat delta_z;
} Pvec3;

typedef struct {
   Ppoint p;
   Ppoint q;
} Prect;

typedef struct {
   Pfloat x_min, x_max;
   Pfloat y_min, y_max;
} Plimit;

typedef struct {
   Pfloat x_min, x_max;
   Pfloat y_min, y_max;
   Pfloat z_min, z_max;
} Plimit3;

typedef struct {
   Pint num_points;
   Ppoint *points;
} Ppoint_list;

typedef struct {
   Pint num_points;
   Ppoint3 *points;
} Ppoint_list3;

typedef struct {
   Ppoint pos;
   char   *char_string;
} Ptext;

typedef struct {
   Pcompose_type compose_type;
   Pmatrix       matrix;
} Plocal_tran;

typedef struct {
   Pcompose_type compose_type;
   Pmatrix3      matrix;
} Plocal_tran3;

typedef struct {
   Pmatrix3  ori_matrix;
   Pmatrix3  map_matrix;
   Plimit3   clip_limit;
   Pclip_ind xy_clip;
   Pclip_ind back_clip;
   Pclip_ind front_clip;
} Pview_rep3;

typedef struct {
   Pfloat red;
   Pfloat green;
   Pfloat blue;
} Prgb;

typedef struct {
   Pfloat hue;
   Pfloat satur;
   Pfloat value;
} Phsv;

typedef union {
   Prgb rgb;
   Phsv hsv;
} Pcolr_rep;

typedef struct {
   Pint size_x;
   Pint size_y;
} Pint_size;

typedef struct {
   Pint size_x;
   Pint size_y;
   Pint size_z;
} Pint_size3;

typedef struct {
   Pfloat size_x;
   Pfloat size_y;
} Pfloat_size;

typedef struct {
   Pfloat size_x;
   Pfloat size_y;
   Pfloat size_z;
} Pfloat_size3;

typedef struct {
   Pdc_units   dc_units;
   Pfloat_size size_dc;
   Pint_size   size_raster;
} Pdisp_space_size;

typedef struct {
   Pdc_units    dc_units;
   Pfloat_size3 size_dc;
   Pint_size3   size_raster;
} Pdisp_space_size3;

typedef struct {
   Pint   id;
   Pfloat disp_pri;
} Pposted_struct;

typedef struct {
   Pint           num_postings;
   Pposted_struct *postings;
} Pposted_struct_list;

typedef union {
   Pint         int_data;
   Pfloat       float_data;
   Ppoint_list  point_list;
   Ppoint_list3 point_list3;
   Ptext        text;
   Plocal_tran  local_tran;
   Plocal_tran3 local_tran3;
   Pint_list    names;
   Pedge_flag   edge_flag;
   Pint_style   int_style;
   Pgcolr       colr;
} Pelem_data;

typedef struct {
   Plimit win;
   Plimit proj_vp;
} Pview_map;

typedef struct {
   Plimit3    win;
   Plimit3    proj_vp;
   Pproj_type proj_type;
   Ppoint3    proj_ref_point;
   Pfloat     view_plane;
   Pfloat     back_plane;
   Pfloat     front_plane;
} Pview_map3;

typedef struct {
   Pasf         type_asf;
   Pasf         width_asf;
   Pasf         colr_ind_asf;
   Pint         ind;
   Pline_bundle bundle;
} Pline_attrs;

typedef struct {
   Pasf           type_asf;
   Pasf           size_asf;
   Pasf           colr_ind_asf;
   Pint           ind;
   Pmarker_bundle bundle;
} Pmarker_attrs;

typedef struct {
   Pasf        style_asf;
   Pasf        style_ind_asf;
   Pasf        colr_ind_asf;
   Pint        ind;
   Pint_bundle bundle;
} Pint_attrs;

typedef struct {
   Pasf         flag_asf;
   Pasf         type_asf;
   Pasf         widthasf;
   Pasf         colr_ind_asf;
   Pint         ind;
   Pedge_bundle bundle;
} Pedge_attrs;

typedef struct {
   Pint struct_id;
   Pint pick_id;
   Pint elem_pos;
} Ppick_path_elem;

typedef struct {
   Pint            depth;
   Ppick_path_elem *path_list;
} Ppick_path;

typedef struct {
   Pint loc;
   Pint stroke;
   Pint val;
   Pint choice;
   Pint pick;
   Pint string;
} Pnum_in;

typedef struct {
   union {

      struct {
         Pint unused;
      } pet_r1;

      struct {
         Pint unused;
      } pet_r2;

      struct {
         Pint unused;
      } pet_r3;

      struct {
         Pline_attrs line_attrs;
      } pet_r4;

      struct {
         Pline_fill_ctrl_flag line_fill_ctrl_flag;
         union {
            Pline_attrs line_attrs;
            Pint_attrs  int_attrs;
            struct {
               Pint_attrs  int_attrs;
               Pedge_attrs edge_attrs;
            } fill_set;
         } attrs;
      } pet_r5;

   } pets;
} Ploc_data;

typedef Ploc_data Ploc_data3;

typedef struct {
   union {
      struct {
         Pint unused;
      } pet_r1;
   } pets;
} Ppick_data;

typedef Ppick_data Ppick_data3;

typedef struct {
   Pint   buffer_size;
   Pint   init_pos;
   Pfloat x_interval;
   Pfloat y_interval;
   Pfloat time_interval;

   union {
      struct {
         Pint unused;
      } pet_r1;

      struct {
         Pint unused;
      } pet_r2;

      struct {
         Pmarker_attrs marker_attrs;
      } pet_r3;

      struct {
         Pline_attrs line_attrs;
      } pet_r4;
   } pets;

} Pstroke_data;

typedef struct {
   Pint   buffer_size;
   Pint   init_pos;
   Pfloat x_interval;
   Pfloat y_interval;
   Pfloat z_interval;
   Pfloat time_interval;

   union {
      struct {
         Pint unused;
      } pet_r1;

      struct {
         Pint unused;
      } pet_r2;

      struct {
         Pmarker_attrs marker_attrs;
      } pet_r3;

      struct {
         Pline_attrs line_attrs;
      } pet_r4;
   } pets;

} Pstroke_data3;

typedef struct {
   Pfloat low;
   Pfloat high;

   union {

      struct {
         Pint unused;
      } pet_r1;

      struct {
         char *label;
         char *format;
         char *low_label;
         char *high_label;
      } pet_u1;

   } pets;
} Pval_data;

typedef Pval_data Pval_data3;

typedef struct {

   union {
      struct {
         Pint unused;
      } pet_r1;

      struct {
         Pint       num_prompts;
         Ppr_switch *prompts;
      } pet_r2;

      struct {
         Pint       num_strings;
         char       **strings;
      } pet_r3;

      struct {
         Pint       num_strings;
         char       **strings;
      } pet_r4;

      struct {
         Pint       struct_id;
         Pint       num_pick_ids;
         Pint       *pick_ids;
      } pet_r5;
   } pets;

} Pchoice_data;

typedef Pchoice_data Pchoice_data3;

typedef struct {
   Pint buffer_size;
   Pint init_pos;

   union {
      struct {
         Pint unused;
      } pet_r1;
   } pets;
} Pstring_data;

typedef Pstring_data Pstring_data3;

#endif

