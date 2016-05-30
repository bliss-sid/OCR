#define CHAR_H_FCTR   0.3   /* threshold factor for determining larger strokes */
#define BIG_CHAR_H_FCTR 0.6 /* threshold factor for determining very large strokes */
#define SMALL_A_FCTR  0.5   /* threshold factor for determining small noise blobs */
#define DOT_X_FCTR    2.0   /* threshold factors for determining dot-sized blobs */
#define DOT_Y_FCTR    3.0
#define OVER_FCTR     1.6   /* overlap factor biasing left nbr */
#define LARGE_FCTR    0.3   /* merge distance factor between large blobs based on */
                            /* estimated character height */
#define SAME_PRCT     0.9   /* tolerance for two things being nearly the same */
                            /* estimated character height */
#define DOT_DIST      2.0   /* distance threshold normalized on line width and */
                            /* used to reconnect dots */
#define HVW_PT        2.25  /* X1 for linear detector of touching characters */
#define HVB_PT        4.75  /* Y2 for linear detector of touching characters */
#define ALPHA_HVW_PT  4.0   /* X1 for linear detector of touching alphas */
#define ALPHA_HVB_PT  5.0   /* Y2 for linear detector of touching alphas */
#define CUT_FACTOR    1.5   /* factor for likely cutting multiple characters */
                            /* from left to right in field image */
#define ONE_HLIMIT    0.4   /* min % of char height to be considered a 1 */

#define FRAG_HEIGHT   0.3   /* % of char height less than which a cut piece */
                            /* is considered a fragment */
#define HEIGHT_PRCT   0.9   /* top percentile for selecting tall character height */

#define DASH_ASPECT   1.5   /* aspect ratio of w to h (2/3) for top of alpha */

#define TALL_FACTOR   1.5   /* factor of estimated character for determining */
                            /* tallness */

#define MIN_2ND_OVER  0.125 /* minimum 2nd largest overlap of a character with */
                            /* a line in the phrase map to consider split */

#define PRCT_OVERLAP  0.75  /* bottom line overlap must be larger than this % */
                            /* of map's line width to consider split */

#define TOO_SHORT     2     /* phrase too short to be a line */
