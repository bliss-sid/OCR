#ifndef __INCLUDE_SEX_H__
#define __INCLUDE_SEX_H__


/*
 * Byte swaps for word and half words.
 */
#define swap_word(a) ( ((a) << 24) | \
		      (((a) << 8) & 0x00ff0000) | \
		      (((a) >> 8) & 0x0000ff00) | \
	((unsigned long)(a) >>24) )

#define swap_half(a) ( ((a & 0xff) << 8) | ((unsigned short)(a) >> 8) )

#endif	/* __INCLUDE_SEX_H__ */
