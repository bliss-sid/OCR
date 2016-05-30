/* rmline.h */

#define SW_FCTR            2.0           /* median slice width factor used to detect    */
                                         /* jumps that are too big to be part of a line */
#define SW2_FCTR           1.5           /* median slice width factor used to detect    */
                                         /* smaller jumps that are too big to be part   */
                                         /* of a line */
#define SD2_THRESH         1             /* summed second derivative threshold used to  */
                                         /* detect an inflection point (elbow) along a  */
                                         /* pixel contour */
#define SMALL_NOISE        2             /* small amount of tollerable pixel noise or   */
                                         /* scanner jitter */
#define SMALLER_NOISE      1             /* smaller amount of tollerable pixel noise or */
                                         /* scanner jitter */
#define BIGGER_NOISE       3             /* larger amount of tollerable pixel noise or  */
                                         /* scanner jitter */
#define UNERASED           0             /* slice erased flag */
#define ERASED             1             /* slice unerased flag */
#define WHITE_PIX          0             /* white pixel value */
#define BLACK_PIX          1             /* black pixel value */
#define RUN_DYS            6             /* maximum length of character contour traces  */
                                         /* and measurements */
#define TOPN               5             /* top n-choices from the hough transform used  */
                                         /* to pick the next line to process */
#define MIN_SHORT_VOID     8             /* miminum allowable distance below which a     */
                                         /* "short" void may be filled in. */
#define SHORT_VOID_FCTR    2.0           /* median slice width factor for determining    */
                                         /* the allowable distance below which a "curved"*/
                                         /* void may be filled in */
#define MIN_CURVE_VOID     10            /* minimum allowable distance below which a     */
                                         /* "curved" void may be filled in */
#define CURVE_VOID_FCTR    5.0           /* median slice width factor for determining    */
                                         /* the allowable distance below which a "curved"*/
                                         /* void may be filled in */
#define SLICES_FOR_LINE    3             /* minimum number of slices need for computing  */
                                         /* reliable line statistics */
#define SLICE_FCTR         3.0           /* median slice width factor for requiring an   */
                                         /* even longer number of slices for computing   */
                                         /* reliable line statistics */

/* macros for computing line jump limits */
#define step_limit(_msw_)     max(_msw_+BIGGER_NOISE,sround(_msw_ * SW_FCTR))
#define step_limit2(_msw_)     max(_msw_+BIGGER_NOISE,sround(_msw_ * SW2_FCTR))

/* character piece types */
#define RUN_UNKNOWN 0
#define RUN_BOOT    1
#define RUN_HAT     2
#define RUN_NCROSS  3
#define RUN_PCROSS  4
#define RUN_THRU_DL 5
#define RUN_THRU_DR 6
#define RUN_THRU_UL 7
#define RUN_THRU_UR 8

/* define 2 points on linear translation function  */
/* for thresholding on line length */
#define PRCT1          0.70  /* require 70%         */
#define PIX1         100.0  /* when at 100 pixels  */
#define PRCT2          0.35 /* require only 35%    */
#define PIX2        2000.0  /* when at 2000 pixels */

#define FILTER_WIN_1   0.5  /* use a filter window of 0.5 inch */
#define FILTER_WIN_2   1.0  /* use a filter window of 1 inch */
