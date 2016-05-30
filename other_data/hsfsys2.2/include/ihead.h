/************************************************************/
/*         File Name: IHead.h                               */
/*         Package:   NIST Internal Image Header            */
/*         Author:    Michael D. Garris                     */
/*         Date:      2/08/90                               */
/************************************************************/
/* Defines used by the ihead structure */
#define IHDR_SIZE	288	/* len of hdr record (always even bytes) */
#define SHORT_CHARS 	8	/* # of ASCII chars to represent a short */
#define BUFSIZE		80	/* default buffer size */
#define DATELEN		26	/* character length of date string */

typedef struct ihead{
   char id[BUFSIZE];			/* identification/comment field */
   char created[DATELEN];		/* date created */
   char width[SHORT_CHARS];		/* pixel width of image */
   char height[SHORT_CHARS];		/* pixel height of image */
   char depth[SHORT_CHARS];		/* bits per pixel */
   char density[SHORT_CHARS];		/* pixels per inch */
   char compress[SHORT_CHARS];		/* compression code */
   char complen[SHORT_CHARS];		/* compressed data length */
   char align[SHORT_CHARS];		/* scanline multiple: 8|16|32 */
   char unitsize[SHORT_CHARS];		/* bit size of image memory units */
   char sigbit;				/* 0->sigbit first | 1->sigbit last */
   char byte_order;			/* 0->highlow | 1->lowhigh*/
   char pix_offset[SHORT_CHARS];	/* pixel column offset */
   char whitepix[SHORT_CHARS];		/* intensity of white pixel */
   char issigned;			/* 0->unsigned data | 1->signed data */
   char rm_cm;				/* 0->row maj | 1->column maj */
   char tb_bt;				/* 0->top2bottom | 1->bottom2top */
   char lr_rl;				/* 0->left2right | 1->right2left */
   char parent[BUFSIZE];		/* parent image file */
   char par_x[SHORT_CHARS];		/* from x pixel in parent */
   char par_y[SHORT_CHARS];		/* from y pixel in parent */
}IHEAD;

/* General Defines */
#define UNCOMP		0
#define CCITT_G3	1
#define CCITT_G4	2
#define LZW		3
#define RL_LZW		4
#define RL		5
#define JPEG		6
#define WSQ		7
#define MSBF		'0'
#define LSBF		'1'
#define HILOW		'0'
#define LOWHI		'1'
#define UNSIGNED	'0'
#define SIGNED		'1'
#define ROW_MAJ		'0'
#define COL_MAJ		'1'
#define TOP2BOT		'0'
#define BOT2TOP		'1'
#define LEFT2RIGHT	'0'
#define RIGHT2LEFT	'1'

#define BYTE_SIZE	8.0

extern void nullihdr();
extern IHEAD *readihdr();
extern int valid_compression();
