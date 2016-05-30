/*
# proc: valid_compression - determines if the compression code passed is
# proc:                     supported by installed software.
*/

#include <ihead.h>
#include<defs.h>

int valid_compression(code)
int code;
{
  switch (code){
    case UNCOMP:
         return(True);
         break;
    case CCITT_G3:
         return(True);
         break;
    case CCITT_G4:
         return(True);
         break;
    case LZW:
         return(True);
         break;
    case RL_LZW:
         return(True);
         break;
    case RL:
         return(True);
         break;
    case JPEG:
         return(True);
         break;
    default:
         return(False);
         break;
  }
}
