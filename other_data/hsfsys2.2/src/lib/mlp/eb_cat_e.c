/*
# proc: eb_cat_e - Puts "ERROR " in front of provided string, formats the result,
# proc:            catenates that onto the hidden error-messages buffer, and sets
# proc:            the hidden error-exists flag.
*/

void
eb_cat_e(str)
char str[];
{
  char errstr[500], errstr2[500];

  sprintf(errstr2, "ERROR: %s", str);
  strm_fmt(errstr2, errstr);
  eb_cat(errstr);
  is_e_set();
}
