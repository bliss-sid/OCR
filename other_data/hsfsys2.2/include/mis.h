/************************************************************/
/*         Filename:     Mis.h                              */
/*         Adapted from: Calerahdr()                        */
/*         Author:    Michael D. Garris                     */
/*         Date:      7/18/90                               */
/************************************************************/
typedef struct misstruct{
   IHEAD *head;
   unsigned char *data;
   int misw;
   int mish;
   int misd;
   int entw;
   int enth;
   int ent_num;
   int ent_alloc;
} MIS;

/************************************************************/
/* Defines to facilitate reading/manipulating MIS data.     */
/************************************************************/
#define MIS_EXT	"mis"
#define WIDTH	8.5
#define HEIGHT	11.0
#define BYTE_SIZE 8.0
#define WD_SIZE	16.0
#define word_align(_t) \
       (int)((double)ceil((double)((_t)/(float)WD_SIZE))*WD_SIZE) 
#define PIX_WIDTH(bpi) 	((int)(word_align(bpi * WIDTH)))
#define PIX_HEIGHT(bpi)	((int)(bpi * HEIGHT))
#define BYTE_WIDTH(bpi)	((int)(PIX_WIDTH(bpi) / BYTE_SIZE))
#define FILE_SIZE(bpi)	(BYTE_WIDTH(bpi) * PIX_HEIGHT(bpi))

/************************************************************/
/* Defines to assign MIS specs into an ihead structure.     */
/************************************************************/
#define DEPTH		1	/* one bit per pixel */
#define COMP		UNCOMP	/* uncompressed image data */
#define COMPLEN		0	/* image uncompressed, so 0 length */
#define ALIGN		16	/* word aligned */
#define UNITSIZE	16	/* word fundamental image components */
#define SIGBIT		MSBF	/* most significant bit first */
#define BYTE_ORDER	HILOW   /* high byte - low byte order */
#define PIXOFF		0	/* raster data starts with first bit */
#define WHITEPIX	0	/* white pixels have value zero */
#define SGND		UNSIGNED/* fundamental components are unsigned */
#define R_M		ROW_MAJ	/* raster scanned row major */
#define T2B		TOP2BOT /* raster scanned top to bottom */
#define L2R		LEFT2RIGHT /* raster scanned left to right */
#define PARENT		""	/* original image (not a subimage) */
#define PAR_X		0
#define PAR_Y		0

extern MIS *allocmis();
extern MIS *readmisfile();
extern MIS *readmishdr();
extern void appendmis();
extern void changemis();
extern void deletemis();
extern void freemis();
extern IHEAD *buildmishdr();
