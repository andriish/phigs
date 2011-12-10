#ifndef _err_h
#define _err_h

typedef struct {
    Pint errnum;
    Pint funcnum;
} Err_msg;

typedef struct {
    char *fname;
} Err_local_data;

typedef struct {
    int  sfd;
    char *fname;
} Err_client_data;

typedef struct {
    int sfd;
} Err_remote_data;

typedef struct {
    Perr_mode mode;
    Perr_mode err_state;
    int       cur_func_num;
    int       errno;
    void      (*buf_func)();
    void      (*flush_func)();
    void      (*report_func)();
    void      (*destroy_func)();
    union {
	Err_local_data  local;
	Err_client_data	client;
	Err_remote_data remote;
    } data;
} Err_struct;

typedef Err_struct *Err_handle;

#define ERR_SET_CUR_FUNC(erh, funcnum) \
    ((erh)->cur_func_num = (funcnum))

#define ERR_BUF(erh, errnum) \
    if ((erh)->mode == PERR_ON) (*(erh)->buf_func)((erh), (errnum))

#define ERR_FLUSH(erh) \
    if ((erh)->flush_func) (*(erh)->flush_func)(erh)

#define ERR_HANDLE(errnum, funcnum, erf) \
    (*phg_errhandle)((errnum), (funcnum), (erf))

#define ERR_REPORT(erh, errnum) \
    if ((erh)->mode == PERR_ON && (erh)->report_func) \
       (*(erh)->report_func)((erh), (errnum))

#define ERR_DESTROY(erh) \
    if ((erh)->destroy_func) (*(erh)->destroy_func)(erh)

#endif
