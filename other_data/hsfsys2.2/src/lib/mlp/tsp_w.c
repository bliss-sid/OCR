/*
# proc: tsp_w - standard wrapper to generate a warning message.
*/

void
tsp_w(parmname_str, linenum)
char parmname_str[];
int linenum;
{
  char str[200];

  sprintf(str, "training-stopping parameter %s has been set \
(line %d); its value will not be used.", parmname_str, linenum);
  eb_cat_w(str);
}
