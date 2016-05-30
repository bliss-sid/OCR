#define SMALL_THETA    0.01    /* too small of degrees to worry about rotation */
#define ERR_PIX_LIMIT  9       /* this is 1/4 inch at 300 bpi reduced by 8 */
#define ERR_INCH_LIMIT 0.25    /* if valid bpi then compute pixels per given inches */
#define REJ_PIX_LIMIT  38      /* this is 1 inch at 300 bpi reduced by 8 */
#define REJ_INCH_LIMIT 1.0     /* if valid bpi then compute pixels per given inches */
#define NUM_REG_PTS    4       /* 4 registration points (left,right,top,bottom) */
#define TOPN_RUNS      3       /* n-longest runs to be summed in histogram */
#define MAX_BIN_PRCT   0.50    /* used to threshold % of maximum bin value */
