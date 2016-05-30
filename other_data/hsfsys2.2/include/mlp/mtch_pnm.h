/* Names of the values of the a_type parm of mtch_pnm. */
#define MP_FILENAME ((char)0)
#define MP_INT      ((char)1)
#define MP_FLOAT    ((char)2)
#define MP_SWITCH   ((char)3)

/* Bundles together some parms for mtch_pnm, to reduce the verbosity
of the (many) calls of it by st_nv_ok. */
typedef struct {
  char *namestr, *valstr, *errstr, ok;
  int linenum;
} NVEOL;
