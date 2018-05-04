@x in search for best_col, give pref to items whose name begins with #
  if (o,nd[k].len<=t) {
    if (nd[k].len<t) best_itm=k,t=nd[k].len,p=1;
    else {
@y NB: sharp items should come FIRST in the list
  if (t==max_nodes || (o,nd[k].len<=1) || (nd[k].len<=t && cl[k].name[0]=='#')) {
    if (nd[k].len<t) best_itm=k,t=nd[k].len,p=1;
    else if (t)  { /* need this in case nd[k].len=1 and t=0 */
@z
