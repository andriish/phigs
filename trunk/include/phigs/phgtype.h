#ifndef _phgtype_h
#define _phgtype_h

#ifndef TRUE
#define TRUE     1
#endif

#ifndef FALSE
#define FALSE    0
#endif

#define PHG_ZERO_TOLERANCE      1.0e-30

typedef struct {
   char     *buf;
   unsigned size;
} Phg_scratch;

typedef struct _Css_structel *El_handle;
typedef struct _Css_ssl      *Struct_handle;
typedef struct _Css_struct   *Css_handle;
typedef struct _Ws           *Ws_handle;

#define PHG_NEAR_ZERO(s) \
   (_ABS(s) < PHG_ZERO_TOLERANCE)

#define PHG_SCRATCH_SPACE(_sc, _size) \
    ((_sc)->size >= (_size) ? (_sc)->buf \
        : phg_grow_scratch( (_sc), (unsigned)(_size) ))

#endif

