#ifndef _phgargs_h
#define _phgargs_h

typedef enum {
    PHG_ARGS_LNREP,
    PHG_ARGS_EXTLNREP,
    PHG_ARGS_MKREP,
    PHG_ARGS_EXTMKREP,
    PHG_ARGS_TXREP,
    PHG_ARGS_EXTTXREP,
    PHG_ARGS_EFREP,
    PHG_ARGS_INTERREP,
    PHG_ARGS_EXTINTERREP,
    PHG_ARGS_EDGEREP,
    PHG_ARGS_EXTEDGEREP,
    PHG_ARGS_PTREP,
    PHG_ARGS_EXTPTREP,
    PHG_ARGS_COREP,
    PHG_ARGS_VIEWREP,
    PHG_ARGS_DCUEREP,
    PHG_ARGS_LIGHTSRCREP,
    PHG_ARGS_COLRMAPREP
} Phg_args_rep_type;

typedef enum {
    PHG_ARGS_SETEP_ABS,
    PHG_ARGS_SETEP_REL,
    PHG_ARGS_SETEP_LABEL,
    PHG_ARGS_SETEP_PICK_ID
} Phg_args_set_ep_op;

typedef enum {
    PHG_ARGS_DEL_CURRENT,
    PHG_ARGS_DEL_RANGE,
    PHG_ARGS_DEL_LABEL,
    PHG_ARGS_EMPTY_STRUCT
} Phg_args_del_el_op;

typedef enum {
   PHG_ARGS_HIER_ANCESTORS,
   PHG_ARGS_HIER_DESCENDANTS
} Phg_args_hierarchy_dir;

typedef struct {
   Pint       el_size;
   Pelem_type el_type;
   Pelem_data el_data;
} Phg_args_add_el;

typedef struct {
   Pint orig_id;
   Pint new_id;
   int  posted;
} Phg_args_change_struct;

typedef struct {
   int empty;
} Phg_args_open_ws;

typedef union {
    struct {
        Pint    ep1;
        Pint    ep2;
    } ep_values;
    struct {
        Pint    label1;
        Pint    label2;
    } label_range;
    Pint        struct_id;
} Phg_args_del_el_data;

#endif

