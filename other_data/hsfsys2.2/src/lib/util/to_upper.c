/*
# proc: to_upper - takes the integer value of an ASCII character, and if the
# proc:            character is lowercase, converts it to uppercase.
*/

#include <stdio.h>

/*************************************************************/
to_upper(c)
int c;
{
   if((c >= 'a') && (c <= 'z'))
      c -= 0x20;
   return(c);
}
