@x l.1
\datethis
@s try x
@y
\def\title{HAM}
@z

@x l.21
main(int argc,char *argv[])
@y
int main(int argc,char *argv[])
@z

@x l.33
  for (v=g->vertices;v<g->vertices+g->n;v++) printf(" %d",v->deg);
@y
  for (v=g->vertices;v<g->vertices+g->n;v++) printf(" %ld",v->deg);
@z

@x l.36
    printf("The minimum degree is %d (vertex %s)!\n",x->deg,x->name);
@y
    printf("The minimum degree is %ld (vertex %s)!\n",x->deg,x->name);
@z

@x l.46
  for (v=g->vertices;v<g->vertices+g->n;v++) printf(" %d",v->deg);
@y
  for (v=g->vertices;v<g->vertices+g->n;v++) printf(" %ld",v->deg);
@z

@x l.85
I may learn my lesson.
@y
I may learn my lesson.
@-k@>
@z

@x l.97
try: @<Look at edge |a| and its successors, advancing if it is a valid move@>;
@y
try_next: @<Look at edge |a| and its successors, advancing if it is a valid move@>;
@z

@x l.148
  goto try;
@y
  goto try_next;
@z

@x l.163
@*Index.
@y
@z
