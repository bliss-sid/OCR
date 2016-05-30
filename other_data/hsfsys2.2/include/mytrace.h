/* mytrace.h */

#ifdef MY_TRACE
#define my_trace(_msg_) \
        fprintf(stdout, _msg_)
#define my_trace1(_cntrl_, _v1_) \
        fprintf(stdout, _cntrl_, _v1_)
#define my_trace2(_cntrl_, _v1_, _v2_) \
        fprintf(stdout, _cntrl_, _v1_, _v2_)
#define my_trace3(_cntrl_, _v1_, _v2_, _v3_) \
        fprintf(stdout, _cntrl_, _v1_, _v2_, _v3_)
#define my_trace4(_cntrl_, _v1_, _v2_, _v3_, _v4_) \
        fprintf(stdout, _cntrl_, _v1_, _v2_, _v3_, _v4_)
#else
#define my_trace(_msg_)
#define my_trace1(_cntrl_, _v1_)
#define my_trace2(_cntrl_, _v1_, _v2_)
#define my_trace3(_cntrl_, _v1_, _v2_, _v3_)
#define my_trace4(_cntrl_, _v1_, _v2_, _v3_, _v4_)
#endif
