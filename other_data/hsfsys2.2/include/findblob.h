/* Header file for the findblob routine. */

/* Manifest constant values of some input flags. */

/* Values for erase_flag */
#define ERASE      0
#define NO_ERASE   1

/* Values for alloc_flag */
#define ALLOC      0
#define NO_ALLOC   1

/* Values for out_flag */
#define ORIG_BLOB  0
#define W_H_BLOB   1
#define BOUND_BLOB 2

/* connectivity codes */
#define CONNECT4 44
#define CONNECT8 88

/* Starting, growth-increment, and maximum number of elts for the
   internal list used by findblob.  (Each list elt occupies 12 bytes.)	*/
#define LIST_STARTSIZE  6144		/* might as well keep mults of	*/
#define LIST_INCR       2048		/* 2 so that max list size	*/
#define LIST_MAXSIZE    8388608         /* is 12*8*1024*1024 = 96Mb	*/

typedef struct { /* info about one run of pixels */
  unsigned short y;
  unsigned char *w_on, *e_off;
} RUN;
