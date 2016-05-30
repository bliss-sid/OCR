/*
# proc: neverset - a standard error message wrapper.
*/

void
neverset(parmname_str)
char parmname_str[];
{
  char str[100];

  sprintf(str, "%s is never set", parmname_str);
  eb_cat_e(str);
}
