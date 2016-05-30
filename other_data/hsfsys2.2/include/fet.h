#ifndef True
#define True		1
#define False		0
#endif
#define FET_EXT		"fet"
#define MAXFETS		100
#define MAXFETLENGTH	512

typedef struct fetstruct{
   int alloc;
   int num;
   char **names;
   char **values;
} FET;

extern FET *allocfet();
extern FET *reallocfet();
extern void freefet();
extern void updatefet();
extern void writefetfile();
