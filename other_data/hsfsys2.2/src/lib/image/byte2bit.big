/*
# proc: bytes2bits - converts a grayscale image into a binary image.
# proc:
*/

/* LINTLIBRARY */

#include <stdio.h>
#include <values.h>
#include <sys/param.h>
#include <sys/types.h>

bytes2bits(p,q,pixels)
u_char *p, *q;
register u_int pixels;
{
register u_char *r, a;
register u_long *l;
register u_char b;

if (p == (u_char *) NULL)
	fatalerr("bytes2bits", "null src pointer", NULL);

if (q == (u_char *) NULL)
	fatalerr("bytes2bits", "null dst pointer", NULL);

pixels = howmany(pixels,BITSPERBYTE);
r = q;
l = (u_long *) p;

while (pixels--) {

	switch (*l++) {
		case 0x00000000:	a = 0x00; break;
		case 0x00000001:	a = 0x10; break;
		case 0x00000100:	a = 0x20; break;
		case 0x00000101:	a = 0x30; break;

		case 0x00010000:	a = 0x40; break;
		case 0x00010001:	a = 0x50; break;
		case 0x00010100:	a = 0x60; break;
		case 0x00010101:	a = 0x70; break;

		case 0x01000000:	a = 0x80; break;
		case 0x01000001:	a = 0x90; break;
		case 0x01000100:	a = 0xa0; break;
		case 0x01000101:	a = 0xb0; break;

		case 0x01010000:	a = 0xc0; break;
		case 0x01010001:	a = 0xd0; break;
		case 0x01010100:	a = 0xe0; break;
		case 0x01010101:	a = 0xf0; break;

		default:
		  (void) fprintf(stderr,"bytes2bits: bad value %x\n",*--l);
		  exit(1);
	}

	switch (*l++) {
		case 0x00000000:	b = 0x00; break;
		case 0x00000001:	b = 0x01; break;
		case 0x00000100:	b = 0x02; break;
		case 0x00000101:	b = 0x03; break;

		case 0x00010000:	b = 0x04; break;
		case 0x00010001:	b = 0x05; break;
		case 0x00010100:	b = 0x06; break;
		case 0x00010101:	b = 0x07; break;

		case 0x01000000:	b = 0x08; break;
		case 0x01000001:	b = 0x09; break;
		case 0x01000100:	b = 0x0a; break;
		case 0x01000101:	b = 0x0b; break;

		case 0x01010000:	b = 0x0c; break;
		case 0x01010001:	b = 0x0d; break;
		case 0x01010100:	b = 0x0e; break;
		case 0x01010101:	b = 0x0f; break;

		default:
		  (void) fprintf(stderr,"bytes2bits: bad value %x\n",*--l);
		  exit(1);
	}
	*r++ = a | b;
}
}
