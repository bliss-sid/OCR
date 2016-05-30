#ifndef __align_h
#define __align_h

#define CHAR_OK		0
#define CHAR_SUB	1
#define CHAR_INS	2
#define CHAR_DEL	3

#endif
#ifndef __align_params_h
#define __align_params_h

#include <values.h>

#define NALIGNCHARS	( 1 << BITSPERBYTE )

struct align_params_t {
	int case_sensitive;
	int insv[ NALIGNCHARS ];
	int delv[ NALIGNCHARS ];
	int *subv;	/*
			 * Need to malloc() 256^2 integers for this
			 *	struct -- it's too big to declare
			 *	as "int subv[256][256];"
			 */
};

#endif
