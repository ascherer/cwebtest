@x
With change files I can adapt it to other uses.
@y
With change files I can adapt it to other uses.

Here I have a sequence of 0s and 1s on the command line, and I compute
the function value.
@z
@x
node* mem[bdds];
@y
node* mem[bdds];
int val[varsize];
@z
@x
@ @<Do our thing@>=
@y
@ @<Do our thing@>=
if (argc!=3) {
  fprintf(stderr,"Usage: %s foo.bdd <booleanstring>\n",
                  argv[0]);
  exit(-1);
}
for (j=k=0;k<varsize;k++)
  if (present[k]) val[k]=argv[2][j++]-'0';
for (k=root[0];k>1;)
  if (val[mem[0][k].v]) k=mem[0][k].hi;
  else k=mem[0][k].lo;
printf("Value is %d.\n",k);
@z
