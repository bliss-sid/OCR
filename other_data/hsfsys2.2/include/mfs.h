#define MAXMFSLENGTH 512

typedef struct mfsstruct{
   int alloc;
   int num;
   char **values;
} MFS;

extern MFS *allocmfs();
extern MFS *reallocmfs();
extern void appendmfs();
extern void freemfs();
extern MFS *readmfsfile();
extern void mfs2int2();
extern void writemfsfile();
