@x in search for best_itm, give pref to items whose name begins with #
    if (t<=score) {
      if (t<score || s<best_s || (s==best_s && nd[k].len>best_l))
        score=t,best_itm=k,best_s=s,best_l=nd[k].len,p=1;
      else if (s==best_s && nd[k].len==best_l) {
@y
    if (score==infty || t<=1 || (t<=score && cl[k].name[0]=='#')) {
      if (t<score || s<best_s || (score && s==best_s && nd[k].len>best_l))
        score=t,best_itm=k,best_s=s,best_l=nd[k].len,p=1;
      else if (score && s==best_s && nd[k].len==best_l) {
@z
