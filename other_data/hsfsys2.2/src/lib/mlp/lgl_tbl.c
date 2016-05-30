/*
# proc: lgl_tbl - Given a set of legal-parm-value-strings and coresponding
# proc:        code-number-strings, makes a neat table of the strings and codes.

Input args:
  n_legal: Number of legal-value string-code pairs.
  legal_valname_p: The legal-parm-value strings, as an array of
    pointers.
  legal_valcode_str: The corresponding code-numbers, as strings stored
    in an array whose second dimension is 2.  These numbers must be in
    the range 0 through 9.

Output arg:
  table: The table of string-code pairs, as a string.  Allocated by
    caller; allocate large enough for added spaces, to the left of
    the first col and between first and second col.
*/

#include <stdio.h>
#include <mlp/fmt_msgs.h>

void
lgl_tbl(n_legal, legal_valname_p, legal_valcode_str, table)
int n_legal;
char *legal_valname_p[], legal_valcode_str[][2], table[];
{
  char *op, *op_e, *ip;
  int i, j, maxlen, len;

  for(i = maxlen = 0; i < n_legal; i++)
    if((len = strlen(legal_valname_p[i])) > maxlen)
      maxlen = len;
  op = table;
  for(i = 0; i < n_legal; i++) {
    for(j = 0; j < MESSAGE_FIRSTCOL_TABLE; j++)
      *op++ = ' ';
    op_e = op + maxlen;
    for(ip = legal_valname_p[i]; *ip;)
      *op++ = *ip++;
    while(op <= op_e)
      *op++ = ' ';
    *op++ = legal_valcode_str[i][0];
    *op++ = '\n';
  }
  *op = '\0';
}
