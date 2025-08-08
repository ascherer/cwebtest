@x
@i gb_types.w
@y
@i gb_types.w
\input epsf
@z
@x
ullng count; /* solutions found so far */
@y
ullng count; /* solutions found so far */
unsigned int max1,max2,max3;
unsigned int min1=-1,min2=-1;
unsigned int count1,count2,count3,count4,count5,count6,count7;
@z
@x
  fprintf(stderr,"Altogether "O"llu solution"O"s, "O"llu nodes,",@|
             count,count==1?"":"s",nodes);
  fprintf(stderr," "O"llu+"O"llu mems.\n",imems,mems);
@y
  fprintf(stderr,"Altogether "O"llu solution"O"s, "O"llu nodes,",@|
             count,count==1?"":"s",nodes);
  fprintf(stderr," "O"llu+"O"llu mems.\n",imems,mems);
  fprintf(stderr,"mins "O"u,"O"u; maxs "O"u,"O"u,"O"u;\n",
             min1,min2,max1,max2,max3);
  fprintf(stderr,"counts "O"u,"O"u,"O"u,"O"u,"O"u,"O"u,"O"u.\n",
             count1,count2,count3,count4,count5,count6,count7);
@z
@x
  count++;
  if (spacing && count mod spacing==0) {
    nd[level].i=0,nd[level].d=1;
    nd[level].m=eptr;
    if (vbose&show_raw_sols) {
      printf("\n"O"llu:\n",count);@+print_state(stdout);
    }@+else @<Unscramble and print the current solution@>;
    fflush(stdout);
  }
@y
  count++;
  nd[level].i=0,nd[level].d=1;
  nd[level].m=eptr;
  @<Unscramble the current solution@>;
  @<Analyze the solution@>;
  if (spacing && count mod spacing==0) {
    @<Print the current solution and its stats@>;
    fflush(stdout);
  }
@z
@x
int path[maxn+1]; /* the Hamiltonian cycle, in order */
@y
int path[maxn+2]; /* the Hamiltonian cycle, in order */
int pathi[maxn+2],pathj[maxn+2]; /* the same, separated into coordinates */
int odd_area,swept_area,zeros,spread,check_area;
int winding[8][8][21]; /* internal winding numbers */
int magic0[21]={0,0,-1,-1,-1,-1,-1,-1,-2,1,0,0,0,0,0,0,0,0,-1,0,-1}; /* blue */
int magic1[21]={2,2,1,2,2,2,2,2,2,1,0,0,1,1,1,1,1,2,1,2,1}; /* green */
int magic2[21]={1,0,0,1,0,0,0,0,0,1,0,0,1,1,0,0,0,1,0,0,0}; /* red hi */
int magic3[21]={0,0,-1,0,0,0,-1,-1,-1,0,0,-1,0,0,0,0,-1,0,0,0,0}; /* red lo */
int magic4[21]={1,1,1,1,1,1,1,0,0,1,0,0,1,1,1,0,0,1,0,1,1}; /* orange hi */
int magic5[21]={2,2,1,2,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1}; /* orange lo */
int area[21]={20,1,1,1,8,12,8,1,3,12,12,3,1,8,12,8,1,1,1,3,3}; /* region size */
@z
@x
@*Index.
@y
@ @<Unscramble the current solution@>=
{
  register int i,j,k;
  for (k=0;k<nn;k++) v1[k]=-1;
  for (k=0;k<nn;k++) {
    i=e[k].u,j=e[k].v;
    if (v1[i]<0) v1[i]=j;
    else v2[i]=j;
    if (v1[j]<0) v1[j]=i;
    else v2[j]=i;
  }
  path[1]=path[nn+1]=v1[0]; /* |path[0]=path[nn]=0| */
  for (k=2;;k++) {
    if (v1[path[k-1]]==path[k-2]) path[k]=v2[path[k-1]];
    else path[k]=v1[path[k-1]];
    if (path[k]==0) break;
  }
}

@ In this variant I study the ``winding number'' aspects of knight's tours.

Each cell of the board is partitioned into 21 regions, numbered from 0 to~20
according to the following pattern:
$$\vcenter{\epsfbox{winding.1}}$$

My eventual goal is to count how many tours have the min and max
odd area; also the maximum absolute value of the swept area.
Also the min and max of interior corners with zero winding number.
And the min and max of odd area among tours whose swept area is zero.
And the min and max of the ``spread,'' which is the difference between
the smallest and largest winding numbers at corners.

But I have no idea what the mins and maxs of the various areas will be.
I do know that the minimum of interior zeros is 0, and the minimum spread is~1.
Furthermore the maximum number of interior zeros is probably 40,
and the maximum spread is probably~9. So I'll simply count the number of
times those extreme occur. And I'll cover my bases by also counting
how often times the maximum of interior zeros exceeds 40, and/or the
maximum spread exceeds~9. Then a second census will only have to look
at comparatively few bunches that match one of the rare extremes.
(That second census will count the number of times the true extremes occur.)

@d rows 6 /* this code is for $m=6$ */
@d cols 6 /* this code is for $n=6$ */
@d row(x) ((x) mod rows)
@d col(x) ((x) / rows)

@<Analyze the solution@>=
{
  register int i,j,k,ii,jj,kk,s,wmin,wmax,w,q;
  for (i=0;i<rows;i++) for (j=0;j<cols;j++) for (k=0;k<21;k++)
    winding[i][j][k]=0;
  for (kk=0;kk<nn;kk++) {
    i=pathi[kk],j=pathj[kk];  /* |pathi[0]=pathj[0]=0| */
    ii=pathi[kk+1]=row(path[kk+1]), jj=pathj[kk+1]=col(path[kk+1]);
    if (i<ii) s=+1;
    else s=-1,i=pathi[kk+1],j=pathj[kk+1],ii=pathi[kk],jj=pathj[kk];
    if (ii==i+1) {
      if (jj<j) @<Handle a move from $(i,j)$ to $(i+1,j-2)$@>@;
      else @<Handle a move from $(i,j)$ to $(i+1,j+2)$@>;
    }@+else if (jj<j) @<Handle a move from $(i,j)$ to $(i+2,j-1)$@>@;
    else @<Handle a move from $(i,j)$ to $(i+2,j+1)$@>;
  }
  @<Compute the stats@>;
  @<Update the mins and maxs@>;
}

@ @<Handle a move from $(i,j)$ to $(i+1,j-2)$@>=
{
  for (k=0;k<21;k++) {
    for (q=j+magic0[k];q<cols;q++) {
      if (k<9) winding[i+1][q][k]+=s;
      else winding[i][q][k]+=s;
    }
  }
}

@ @<Handle a move from $(i,j)$ to $(i+1,j+2)$@>=
{
  for (k=0;k<21;k++) {
    for (q=j+magic1[k];q<cols;q++) {
      if (k<9) winding[i+1][q][k]+=s;
      else winding[i][q][k]+=s;
    }
  }
}

@ @<Handle a move from $(i,j)$ to $(i+2,j-1)$@>=
{
  for (k=0;k<21;k++) {
    for (q=j+magic2[k];q<cols;q++) {
      if (k<9) winding[i+1][q][k]+=s;
      else winding[i][q][k]+=s;
    }
    for (q=j+magic3[k];q<cols;q++) {
      if (k<9) winding[i+2][q][k]+=s;
      else winding[i+1][q][k]+=s;
    }
  }
}

@ @<Handle a move from $(i,j)$ to $(i+2,j+1)$@>=
{
  for (k=0;k<21;k++) {
    for (q=j+magic4[k];q<cols;q++) {
      if (k<9) winding[i+1][q][k]+=s;
      else winding[i][q][k]+=s;
    }
    for (q=j+magic5[k];q<cols;q++) {
      if (k<9) winding[i+2][q][k]+=s;
      else winding[i+1][q][k]+=s;
    }
  }
}

@ The area of each region is a multiple of 1/120. We prefer to work
with integers; so we multiply the areas by 120.

@d debugging 1

@<Compute the stats@>=
for (zeros=swept_area=0,wmin=infty,wmax=-infty,i=1;i<rows;i++)
 for (j=1;j<cols;j++) {
  w=winding[i][j][0];
  if (w==0) zeros++;
  if (w>wmax) wmax=w;
  if (w<wmin) wmin=w;
  swept_area+=w;
}
spread=wmax-wmin;
for (odd_area=check_area=i=0;i<rows;i++) for (j=0;j<cols;j++)
 for (k=0;k<21;k++) {
  w=winding[i][j][k];
  if (w&1) odd_area+=area[k];
  if (debugging) check_area+=area[k]*w;
}
if (debugging) @<Compute the swept area by the special formula and check it@>;

@ Long ago I discovered a surprisingly simple formula for the
swept area of a polygonal path that never touches the corner points
of a grid. (See {\sl The American Mathematical Monthly\/ \bf101} (1994),
682--683; {\bf104} (1997), 669.) Namely, the swept area always turns
out to be an integer, equal to ${1\over2}\bigl(i_0j_1-i_1j_0+i_1j_2-i_2j_1+
\cdots+i_{mn-1}j_{mn}-i_{mn}j_{mn-1}\bigr)$.

@<Compute the swept area by the special formula and check it@>=
{
  if (check_area!=120*swept_area)
    fprintf(stderr,"This can't happen.\n");
  for (check_area=k=0;k<nn;k++)
    check_area+=pathi[k]*pathj[k+1]-pathi[k+1]*pathj[k];
  if (check_area!=2*swept_area)
    fprintf(stderr,"This can't happen either.\n");
}

@ @<Update the mins and maxs@>=
if (odd_area<min1) min1=odd_area;
if (odd_area>max1) max1=odd_area;
if (!swept_area) {
  count1++;
  if (odd_area<min2) min2=odd_area;
  if (odd_area>max2) max2=odd_area;
}
else {
  if (swept_area<0) swept_area=-swept_area;
  if (swept_area>max3) max3=swept_area;
}
if (!zeros) count2++;
else if (zeros==40) count4++;
else if (zeros>40) count6++;
if (spread==1) count3++;
else if (spread==9) count5++;
else if (spread>9) count7++;

@ @<Print the current solution and its stats@>=
for (k=0;k<=nn;k++) printf(""O"s ",name(path[k]));
printf(""O"u,"O"u,"O"u,"O"u",
         odd_area,swept_area,zeros,spread);
printf(" #"O"llu\n",count);

@ @<Sub...@>=
void print_windings(void) {
  register int i,j,k;
  for (i=0;i<rows;i++) for (j=0;j<cols;j++) {
    fprintf(stderr,"["O"d,"O"d]",i,j);
    for (k=0;k<21;k++) fprintf(stderr,""O"2d",winding[i][j][k]);
    fprintf(stderr,"\n");
  }
}

@*Index.
@z
