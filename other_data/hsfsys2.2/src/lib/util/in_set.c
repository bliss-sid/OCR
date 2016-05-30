/*
# proc: is_in_int_set - determines if the specified integer is a member
# proc:                 of the given set represetned as a string of integers.
*/

is_in_int_set(i, set)
int i;
char *set;
{
   int j, c;

   for(j = 0; j < strlen(set); j++){
      c = set[j] - 0x30;
      if(i == c)
         return(1);
   }
   return(0);
}
