/* For use by strm_fmt() and lgl_tbl(), which format the warning and
error messages that may be written as the result of scanning a
specfile.  Columns are numbered starting at 0. */

#define MESSAGE_FIRSTCOL_FIRSTLINE   6 /* for first line of a msg */
#define MESSAGE_FIRSTCOL_LATERLINES  8 /* later lines indented */
#define MESSAGE_LASTCOL             70
#define MESSAGE_FIRSTCOL_TABLE      12 /* table indented even more */
