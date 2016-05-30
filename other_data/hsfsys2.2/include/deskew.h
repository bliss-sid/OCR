#define HIGH_ANGLE  60   /* SCALE_ANGLE times the actual max angle in degrees */
#define SCALE_ANGLE 10.0 /* multiplier; convenient for array indexing         */
#define INCR_ANGLE   2.0 /* SCALE_ANGLE times the angle tics, smaller values  */
                         /* are beyond the resolution of the scan	      */
#define PIBY180 (M_PI / 180.0)

#define NUM_FUNCS        2
#define SUMLINESQUARES   0
#define SUMLINEEXPS      1
extern double sumlineexps();	/* at the global skew angle		*/
