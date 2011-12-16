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

#ifndef _phigs_h
#define _phigs_h

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
   PELEM_INT_COLR_IND,
   PELEM_INT_STYLE,
   PELEM_LINE_COLR_IND,
   PELEM_LINEWIDTH,
   PELEM_LINETYPE,
   PELEM_MARKER_COLR_IND,
   PELEM_MARKER_SIZE,
   PELEM_MARKER_TYPE,
   PELEM_EDGE_COLR_IND,
   PELEM_EDGEWIDTH,
   PELEM_EDGETYPE,
   PELEM_EDGE_FLAG,
   PELEM_TEXT_FONT,
   PELEM_TEXT_COLR_IND,
   PELEM_LOCAL_MODEL_TRAN,
   PELEM_LOCAL_MODEL_TRAN3,
   PELEM_VIEW_IND,
   PELEM_EXEC_STRUCT,
   PELEM_LABEL,
   PELEM_PICK_ID,
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
   PSTYLE_SOLID
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
   Pfloat size;
   Pint   colr_ind;
} Pmarker_bundle;

typedef struct {
   Pint font;
   Pint colr_ind;
} Ptext_bundle;

typedef struct {
   Pedge_flag flag;
   Pint       type;
   Pfloat     width;
   Pint       colr_ind;
} Pedge_bundle;

typedef struct {
   Pint_style style;
   Pint       colr_ind;
} Pint_bundle;

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
} Pelem_data;

extern Pint phigs_ws_type_glx_drawable;

#endif

