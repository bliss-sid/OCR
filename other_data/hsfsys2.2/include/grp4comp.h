/*********************************************************************/
/* grp4comp.h                                                        */
/* Originally compression.h                                          */
/*********************************************************************/

#include <stdio.h>


/* WARNING */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* !!!!!!!!!!!!!!!!!! Change Here. Redeclaration of Type. !!!!!!!!!!!!!!!! */
#define SHORT int   /* this type was just a regular old C "short".         */
                    /* In images with > 2^15 rows the 2 byte definition    */
                    /* gave garbage output because short overflowed.       */
                    /* Increasing all variables from 2 to 4 bytes seems    */
                    /* to fix it. I have used the macro SHORT here to show */
                    /* where this change applies, so that it can be undone */
                    /* if desired. Some variables of type "int" existed in */
                    /* the code before this change, and the SHORT macro    */
                    /* allows reversal of just the correct ones.           */
                    /* Patrick Grother Dec 9 1994                          */
/* !!!!!!!!!!!!!!!!!! Change Here. Redeclaration of Type. !!!!!!!!!!!!!!!! */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


#define True					1
#define False					0
#define Debug					False

#define White					0
#define Black 					1

#define Largest_code						2560
#define Size_of_make_up_code_increments		64
#define Max_terminating_length				63	/* longest terminating code*/
#define Number_of_different_bytes			256  

#define Pixels_per_byte			8  
#define Bits_per_byte			8  
#define Last_bit_in_a_byte		7		/* assumes bits numbered from 0 - 7 */	
#define Last_bit_mask 			1		/* masks the last (low magnitude) bit */ 
#define Default_width_in_pixels	2560 	/* default width of a scan line */
#define Default_number_of_lines	3300 	/* default length of an image */

#define Invalid				   -1 
#define None					0 
#define Extra_positions			25		/* ensures extra room in allocations */
#define Not_done_yet			0

#define VL3 				   -3  		/* Vertical Left 3 mode */
#define VL2 				   -2  		/* Vertical Left 2 mode */
#define VL1 				   -1 		/* Vertical Left 1 mode */
#define V0						0		/* Vertical mode */	  
#define VR1						1 		/* Vertical Right 1 mode */  
#define VR2						2  		/* Vertical Right 2 mode */ 
#define VR3						3  		/* Vertical Right 3 mode */ 
#define P						4 		/* Pass mode */ 
#define H						5		/* Horizontal mode */
#define EOFB					6		/* End Of File Buffer */

#define No_offset				0		/* no offset during fseek() */
#define End_of_file				2		/* start at EOF during fseek() */
#define Start_of_file			0		/* start at SOF during fseek() */

/*
unsigned char 	*calloc();
SHORT 			*malloc();
*/

struct parameters {
	SHORT previous_color;	/* color of last run of pixels */
	SHORT index;			/* indicates current position in "coding_line" */
	SHORT max_pixel;		/* the number of pixels in a scan line */
	SHORT pixel;			/* pixel number of the last changing element */
	SHORT *reference_line;	/* array of changing elements on reference line */
	SHORT *coding_line;		/* array of changing elements on coding line */
};

struct compressed_descriptor {
	unsigned char  *data;		/* pointer to compressed image */
	SHORT  pixels_per_line;		/* the number of pixels in a scan line */
	SHORT  number_of_lines;		/* the number of scan lines in the image */
	int    length_in_bytes;		/* length of the compressed image in bytes */
};

struct uncompressed_descriptor {
	unsigned char  *data;		/* pointer to uncompressed image */
	SHORT  pixels_per_line;		/* the number of pixels in a scan line */
	SHORT  number_of_lines;		/* the number of scan lines in the image */
};


/*****************************************************************************

	declarations of all the procedures in the group4 compression routines
	follow.  The names of the files that contain the procedures are enclosed
	in comments above the declarations.
	
******************************************************************************/

/* compress.c */
void control_compression();
struct uncompressed_descriptor process_arguments();
void set_up_first_and_last_changing_elements_c();
void set_up_first_and_last_changing_elements_d();
void read_uncompressed_file_into_memory();
void write_compressed_data_into_a_file();
void prepare_to_compress();
void compress_image();
void make_array_of_changing_elements();
void prepare_to_compress_next_line();
void set_up_first_line_c();
void set_up_first_line_d();
void crash_c();

/* table.c */
void process_char();


/* mode.c */
void compress_line();
void initialize_b1();
void pass_mode_c();
void pass_mode_d();
void vertical_mode_c();
void vertical_mode_d();
void horizontal_mode_c();
void horizontal_mode_d();


/* write_run.c */
void write_run_length();


/* write_bits.c */
void prepare_to_write_bits_c();
void prepare_to_write_bits_d();
void write_bits_c();
void write_bits_d();
unsigned int flush_buffer();
