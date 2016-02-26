#define ll 4
#define mm 4
#define nn 4
#define ss 48 \

#define ox(j1,k1,j2,k2) {optx[i][j][k].neighbor[kk++]= &cells[i][j1][k1]; \
optx[i][j][k].neighbor[kk++]= &cells[i][j2][k2]; \
optx[i][j][k].neighbor[kk++]= NULL;}
#define oxx(i1) {optx[i][j][k].neighbor[kk++]= &cells[i1][j][k]; \
optx[i][j][k].neighbor[kk++]= &cells[i1][j][k+1]; \
optx[i][j][k].neighbor[kk++]= &cells[i1][j+1][k]; \
optx[i][j][k].neighbor[kk++]= &cells[i1][j+1][k+1]; \
optx[i][j][k].neighbor[kk++]= NULL;} \

#define oy(i1,k1,i2,k2) {opty[i][j][k].neighbor[kk++]= &cells[i1][j][k1]; \
opty[i][j][k].neighbor[kk++]= &cells[i2][j][k2]; \
opty[i][j][k].neighbor[kk++]= NULL;}
#define oyy(j1) {opty[i][j][k].neighbor[kk++]= &cells[i][j1][k]; \
opty[i][j][k].neighbor[kk++]= &cells[i][j1][k+1]; \
opty[i][j][k].neighbor[kk++]= &cells[i+1][j1][k]; \
opty[i][j][k].neighbor[kk++]= &cells[i+1][j1][k+1]; \
opty[i][j][k].neighbor[kk++]= NULL;} \

#define oz(i1,j1,i2,j2) {optz[i][j][k].neighbor[kk++]= &cells[i1][j1][k]; \
optz[i][j][k].neighbor[kk++]= &cells[i2][j2][k]; \
optz[i][j][k].neighbor[kk++]= NULL;}
#define ozz(k1) {optz[i][j][k].neighbor[kk++]= &cells[i][j][k1]; \
optz[i][j][k].neighbor[kk++]= &cells[i][j+1][k1]; \
optz[i][j][k].neighbor[kk++]= &cells[i+1][j][k1]; \
optz[i][j][k].neighbor[kk++]= &cells[i+1][j+1][k1]; \
optz[i][j][k].neighbor[kk++]= NULL;} \

/*1:*/
#line 59 "./antislide3.w"

#include <stdio.h> 

/*3:*/
#line 115 "./antislide3.w"

typedef struct node_struct{
struct node_struct*left,*right;
struct node_struct*up,*down;
struct node_struct*sym[ss];
struct row_struct*row;
struct col_struct*col;
}node;

/*:3*//*4:*/
#line 132 "./antislide3.w"

typedef struct col_struct{
node head;
int len;
int init_len;
struct col_struct*prev,*next;
node*filled;
int empty;

int nonempty;
char name[4];
struct col_struct*invsym[ss];
}cell;

/*:4*//*6:*/
#line 158 "./antislide3.w"

typedef struct row_struct{
cell*neighbor[22];
int neighbor_ptr;
}option;

/*:6*/
#line 62 "./antislide3.w"

/*2:*/
#line 82 "./antislide3.w"

int verbose= 0;
int count= 0;
int spacing= 1;
int profile[ll*mm*nn+1],prof_syms[ll*mm*nn+1],
prof_cons[ll*mm*nn+1],prof_frcs[ll*mm*nn+1];

/*:2*//*5:*/
#line 150 "./antislide3.w"

cell root;

/*:5*//*7:*/
#line 168 "./antislide3.w"

cell cells[ll][mm][nn];
option opt[ll][mm][nn],
optx[ll][mm-1][nn-1],opty[ll-1][mm][nn-1],optz[ll-1][mm-1][nn];
node blockx[ll][mm-1][nn-1][4],blocky[ll-1][mm][nn-1][4],
blockz[ll-1][mm-1][nn][4];

/*:7*//*23:*/
#line 408 "./antislide3.w"

int symcheck_sig[(ll*mm*nn+1)*(ss-1)],
symcheck_j[(ll*mm*nn+1)*(ss-1)];
int symcheck_ptr[ll*mm*nn+2];
cell*constraint[ll*mm*nn*22];
int constraint_ptr[ll*mm*nn+2];
cell*force[ll*mm*nn];
int force_ptr[ll*mm*nn+1];
cell*best_cell[ll*mm*nn+1];
node*move[ll*mm*nn+1];

/*:23*/
#line 63 "./antislide3.w"

/*19:*/
#line 327 "./antislide3.w"

make_syms(pp)
node pp[];
{
register char*q;
register int s,t,imax,imin,jmax,jmin,kmax,kmin,i,j,k;
for(s= 0;s<ss;s++){
imax= jmax= kmax= -1;imin= jmin= kmin= 1000;
for(t= 0;t<4;t++){
q= pp[t].col->head.sym[s]->col->name;i= q[0]-'0';j= q[1]-'0';k= q[2]-'0';
if(i<imin)imin= i;if(i> imax)imax= i;
if(j<jmin)jmin= j;if(j> jmax)jmax= j;
if(k<kmin)kmin= k;if(k> kmax)kmax= k;
}
if(imin==imax)/*20:*/
#line 347 "./antislide3.w"

for(t= 0;t<4;t++){
q= pp[t].col->head.sym[s]->col->name;i= q[0]-'0';j= q[1]-'0';k= q[2]-'0';
pp[t].sym[s]= &blockx[i][jmin][kmin][(j-jmin)*2+k-kmin];
}

/*:20*/
#line 341 "./antislide3.w"

else if(jmin==jmax)/*21:*/
#line 353 "./antislide3.w"

for(t= 0;t<4;t++){
q= pp[t].col->head.sym[s]->col->name;i= q[0]-'0';j= q[1]-'0';k= q[2]-'0';
pp[t].sym[s]= &blocky[imin][j][kmin][(i-imin)*2+k-kmin];
}

/*:21*/
#line 342 "./antislide3.w"

else/*22:*/
#line 359 "./antislide3.w"

for(t= 0;t<4;t++){
q= pp[t].col->head.sym[s]->col->name;i= q[0]-'0';j= q[1]-'0';k= q[2]-'0';
pp[t].sym[s]= &blockz[imin][jmin][k][(i-imin)*2+j-jmin];
}

/*:22*/
#line 343 "./antislide3.w"
;
}
}

/*:19*//*27:*/
#line 477 "./antislide3.w"

cover(c)
cell*c;
{register cell*l,*r;
register node*rr,*pp,*uu,*dd;
l= c->prev;r= c->next;
l->next= r;r->prev= l;
for(rr= c->head.down;rr!=&(c->head);rr= rr->down)
for(pp= rr->right;pp!=rr;pp= pp->right){
uu= pp->up;dd= pp->down;
uu->down= dd;dd->up= uu;
pp->col->len--;
}
}

/*:27*//*28:*/
#line 497 "./antislide3.w"

uncover(c)
cell*c;
{register cell*l,*r;
register node*rr,*pp,*uu,*dd;
for(rr= c->head.up;rr!=&(c->head);rr= rr->up)
for(pp= rr->left;pp!=rr;pp= pp->left){
uu= pp->up;dd= pp->down;
uu->down= dd->up= pp;
pp->col->len++;
}
l= c->prev;r= c->next;
l->next= r->prev= c;
}

/*:28*//*43:*/
#line 652 "./antislide3.w"

print_move(p)
node*p;
{
register node*q;
for(q= p->right;q!=p;q= q->right)printf("%s-",q->col->name);
printf("%s ",q->col->name);
}

/*:43*/
#line 64 "./antislide3.w"


main(argc,argv)
int argc;
char*argv[];
{
/*24:*/
#line 419 "./antislide3.w"

register int i,j,k,s;
register int l;
register cell*c;
register node*p;
register cell*q;
register option*r;
int ii,jj,kk,t;
node*pp;

/*:24*/
#line 70 "./antislide3.w"
;
if(argc> 1){
verbose= argc-1;
sscanf(argv[1],"%d",&spacing);
}
/*8:*/
#line 175 "./antislide3.w"

/*9:*/
#line 180 "./antislide3.w"

q= &root;
for(i= 0;i<ll;i++)for(j= 0;j<mm;j++)for(k= 0;k<nn;k++){
c= &cells[i][j][k];q->next= c;c->prev= q;q= c;p= &(c->head);
p->left= p->right= p->up= p->down= p;
p->row= &opt[i][j][k];p->col= c;
/*10:*/
#line 192 "./antislide3.w"

for(s= 0;s<ss;s++){
switch(s>>3){
case 0:ii= i;jj= j;kk= k;break;
case 1:ii= j;jj= i;kk= k;break;
case 2:ii= k;jj= j;kk= i;break;
case 3:ii= i;jj= k;kk= j;break;
case 4:ii= j;jj= k;kk= i;break;
case 5:ii= k;jj= i;kk= j;break;
}
if(s&4)ii= ll-1-ii;
if(s&2)jj= mm-1-jj;
if(s&1)kk= nn-1-kk;
p->sym[s]= &(cells[ii][jj][kk].head);
cells[ii][jj][kk].invsym[s]= c;
}

/*:10*/
#line 186 "./antislide3.w"
;
c->name[0]= i+'0';c->name[1]= j+'0';c->name[2]= k+'0';
c->len= 1;
}
q->next= &root;root.prev= q;

/*:9*/
#line 176 "./antislide3.w"
;
/*11:*/
#line 209 "./antislide3.w"

/*12:*/
#line 223 "./antislide3.w"

for(i= 0;i<ll;i++)for(j= 0;j<mm-1;j++)for(k= 0;k<nn-1;k++){
kk= 0;
if(j)ox(j-1,k,j-1,k+1);
if(j<mm-2)ox(j+2,k,j+2,k+1);
if(k)ox(j,k-1,j+1,k-1);
if(k<nn-2)ox(j,k+2,j+1,k+2);
if(i)oxx(i-1);
if(i<ll-1)oxx(i+1);
optx[i][j][k].neighbor_ptr= kk;
}

/*:12*/
#line 210 "./antislide3.w"
;
/*13:*/
#line 244 "./antislide3.w"

for(i= 0;i<ll-1;i++)for(j= 0;j<mm;j++)for(k= 0;k<nn-1;k++){
kk= 0;
if(i)oy(i-1,k,i-1,k+1);
if(i<ll-2)oy(i+2,k,i+2,k+1);
if(k)oy(i,k-1,i+1,k-1);
if(k<nn-2)oy(i,k+2,i+1,k+2);
if(j)oyy(j-1);
if(j<mm-1)oyy(j+1);
opty[i][j][k].neighbor_ptr= kk;
}

/*:13*/
#line 211 "./antislide3.w"
;
/*14:*/
#line 265 "./antislide3.w"

for(i= 0;i<ll-1;i++)for(j= 0;j<mm-1;j++)for(k= 0;k<nn;k++){
kk= 0;
if(i)oz(i-1,j,i-1,j+1);
if(i<ll-2)oz(i+2,j,i+2,j+1);
if(j)oz(i,j-1,i+1,j-1);
if(j<mm-2)oz(i,j+2,i+1,j+2);
if(k)ozz(k-1);
if(k<nn-1)ozz(k+1);
optz[i][j][k].neighbor_ptr= kk;
}

/*:14*/
#line 212 "./antislide3.w"
;

/*:11*/
#line 177 "./antislide3.w"
;
/*15:*/
#line 277 "./antislide3.w"

/*16:*/
#line 282 "./antislide3.w"

for(i= 0;i<ll;i++)for(j= 0;j<mm-1;j++)for(k= 0;k<nn-1;k++){
for(t= 0;t<4;t++){
p= &blockx[i][j][k][t];
p->right= &blockx[i][j][k][(t+1)&3];
p->left= &blockx[i][j][k][(t+3)&3];
c= &cells[i][j+((t&2)>>1)][k+(t&1)];
pp= c->head.up;
pp->down= c->head.up= p;p->up= pp;p->down= &(c->head);
p->row= &optx[i][j][k];
p->col= c;c->len++;
}
make_syms(blockx[i][j][k]);
}

/*:16*/
#line 278 "./antislide3.w"
;
/*17:*/
#line 297 "./antislide3.w"

for(i= 0;i<ll-1;i++)for(j= 0;j<mm;j++)for(k= 0;k<nn-1;k++){
for(t= 0;t<4;t++){
p= &blocky[i][j][k][t];
p->right= &blocky[i][j][k][(t+1)&3];
p->left= &blocky[i][j][k][(t+3)&3];
c= &cells[i+((t&2)>>1)][j][k+(t&1)];
pp= c->head.up;
pp->down= c->head.up= p;p->up= pp;p->down= &(c->head);
p->row= &opty[i][j][k];
p->col= c;c->len++;
}
make_syms(blocky[i][j][k]);
}

/*:17*/
#line 279 "./antislide3.w"
;
/*18:*/
#line 312 "./antislide3.w"

for(i= 0;i<ll-1;i++)for(j= 0;j<mm-1;j++)for(k= 0;k<nn;k++){
for(t= 0;t<4;t++){
p= &blockz[i][j][k][t];
p->right= &blockz[i][j][k][(t+1)&3];
p->left= &blockz[i][j][k][(t+3)&3];
c= &cells[i+((t&2)>>1)][j+(t&1)][k];
pp= c->head.up;
pp->down= c->head.up= p;p->up= pp;p->down= &(c->head);
p->row= &optz[i][j][k];
p->col= c;c->len++;
}
make_syms(blockz[i][j][k]);
}

/*:18*/
#line 280 "./antislide3.w"
;

/*:15*/
#line 178 "./antislide3.w"
;

/*:8*/
#line 75 "./antislide3.w"
;
/*25:*/
#line 432 "./antislide3.w"

/*46:*/
#line 687 "./antislide3.w"

for(i= 0;i<ll;i++)for(j= 0;j<mm;j++)for(k= 0;k<nn;k++){
c= &cells[i][j][k];
c->init_len= c->len;
}
for(k= 0;k<ss;k++)symcheck_sig[k]= k+1;
symcheck_ptr[1]= ss-1;

/*:46*/
#line 433 "./antislide3.w"
;
l= 1;goto choose;
advance:/*29:*/
#line 512 "./antislide3.w"

for(p= move[l]->right;p!=move[l];p= p->right)cover(p->col);

/*:29*/
#line 435 "./antislide3.w"
;
if(verbose)/*44:*/
#line 661 "./antislide3.w"

{
profile[l]++;
prof_syms[l]+= symcheck_ptr[l+1]-symcheck_ptr[l]+1;
prof_cons[l]+= constraint_ptr[l+1]-constraint_ptr[l];
prof_frcs[l]+= force_ptr[l]-force_ptr[l-1];
if(verbose> 1){
printf("Level %d, ",l);
print_move(move[l]);
printf("(%d,%d,%d)\n",
symcheck_ptr[l+1]-symcheck_ptr[l]+1,
constraint_ptr[l+1]-constraint_ptr[l],
force_ptr[l]-force_ptr[l-1]);
}
}

/*:44*/
#line 436 "./antislide3.w"
;
l++;
choose:/*26:*/
#line 454 "./antislide3.w"

/*31:*/
#line 518 "./antislide3.w"

q= root.next;
if(q==&root)goto solution;
for(c= q,j= q->len,q= q->next;q!=&root;q= q->next)
if(q->len<j)c= q,j= q->len;
best_cell[l]= c;

/*:31*/
#line 455 "./antislide3.w"
;
force_ptr[l]= force_ptr[l-1];
cover(c);
c->empty= 1;
/*41:*/
#line 634 "./antislide3.w"

move[l]= &(c->head);
if(c->nonempty)goto try_again;

/*:41*/
#line 460 "./antislide3.w"
;
try:/*32:*/
#line 525 "./antislide3.w"

for(p= move[l]->right;p!=move[l];p= p->right){
p->col->filled= p;
p->col->nonempty++;
}
p->col->filled= p;
if(p->right!=p)p->col->nonempty++;

/*:32*/
#line 461 "./antislide3.w"
;
/*34:*/
#line 542 "./antislide3.w"

j= constraint_ptr[l-1];k= constraint_ptr[l];
if(p->right==p)/*35:*/
#line 553 "./antislide3.w"

{
c= p->col;
while(j<constraint_ptr[l]){
kk= k;
while((q= constraint[j])){
if(q!=c)constraint[k++]= q;
j++;
}
j++;
if(k==kk+1)/*36:*/
#line 568 "./antislide3.w"

{
k= kk;
q= constraint[k];
if(!q->nonempty){
q->nonempty= 1;q->len--;force[force_ptr[l]++]= q;
}
}

/*:36*/
#line 563 "./antislide3.w"

else constraint[k++]= NULL;
}
}

/*:35*/
#line 545 "./antislide3.w"

else{
/*37:*/
#line 577 "./antislide3.w"

r= p->row;
for(i= 0;i<r->neighbor_ptr;i++){
kk= k;
while((q= r->neighbor[i])){
if(q->nonempty){
do i++;while(r->neighbor[i]);
goto no_problem;
}else if(!q->empty)constraint[k++]= q;
i++;
}
if(k> kk+1){
constraint[k++]= NULL;
continue;
}
if(k==kk)goto unmark;
q= constraint[kk];q->nonempty= 1;q->len--;force[force_ptr[l]++]= q;
no_problem:k= kk;
}

/*:37*/
#line 547 "./antislide3.w"
;
/*38:*/
#line 597 "./antislide3.w"

while(j<constraint_ptr[l]){
kk= k;
while((q= constraint[j])){
if(q->nonempty)goto flush;
constraint[k++]= q;
j++;
}
constraint[k++]= NULL;
j++;continue;
flush:do j++;while(constraint[j]);
k= kk;j++;
}

/*:38*/
#line 548 "./antislide3.w"
;
}
constraint_ptr[l+1]= k;

/*:34*/
#line 463 "./antislide3.w"
;
/*40:*/
#line 619 "./antislide3.w"

for(k= symcheck_ptr[l-1],kk= symcheck_ptr[l];k<symcheck_ptr[l];k++){
for(i= symcheck_sig[k],j= symcheck_j[k]+1;j<=l;j++){
c= best_cell[j]->invsym[i];
if(!c->filled)break;
p= c->filled->sym[i];
if(p<move[j])goto unmark;
if(p> move[j])goto okay;
}
symcheck_sig[kk]= i;symcheck_j[kk]= j-1;kk++;
okay:;
}
symcheck_ptr[l+1]= kk;

/*:40*/
#line 465 "./antislide3.w"
;
goto advance;
unmark:/*33:*/
#line 533 "./antislide3.w"

for(p= move[l]->left;p!=move[l];p= p->left){
p->col->filled= NULL;
p->col->nonempty--;
}
p->col->filled= NULL;
if(p->right!=p)p->col->nonempty--;

/*:33*/
#line 467 "./antislide3.w"
;
/*39:*/
#line 611 "./antislide3.w"

while(force_ptr[l]!=force_ptr[l-1]){
q= force[--force_ptr[l]];
q->len++;
q->nonempty= 0;
}

/*:39*/
#line 468 "./antislide3.w"
;
try_again:move[l]= move[l]->up;best_cell[l]->empty= 0;
if(move[l]->right!=move[l])goto try;
c= best_cell[l];
uncover(c);

/*:26*/
#line 438 "./antislide3.w"
;
backup:l--;
if(l==0)goto done;
/*30:*/
#line 515 "./antislide3.w"

for(p= move[l]->left;p!=move[l];p= p->left)uncover(p->col);

/*:30*/
#line 441 "./antislide3.w"
;
goto unmark;
solution:/*42:*/
#line 638 "./antislide3.w"

count++;
if(verbose){
if(count%spacing==0){
printf("%d: ",count);
for(j= 1;j<l;j++)print_move(move[j]);
if(symcheck_ptr[l]==symcheck_ptr[l-1])
printf("(1 sym, %d blks)\n",(ll*mm*nn+1-l)/3);
else printf("(%d syms, %d blks)\n",
symcheck_ptr[l]-symcheck_ptr[l-1]+1,
(ll*mm*nn+1-l)/3);
}
}

/*:42*/
#line 443 "./antislide3.w"
;goto backup;
done:

/*:25*/
#line 76 "./antislide3.w"
;
/*47:*/
#line 695 "./antislide3.w"

q= &root;
for(i= 0;i<ll;i++)for(j= 0;j<mm;j++)for(k= 0;k<nn;k++){
c= &cells[i][j][k];
if(c->nonempty||c->len!=c->init_len||c->prev!=q||q->next!=c)
printf("Trouble at cell %s!\n",c->name);
q= c;
}

/*:47*/
#line 77 "./antislide3.w"
;
printf("Altogether %d solutions.\n",count);
if(verbose)/*45:*/
#line 677 "./antislide3.w"

{
for(j= 1;j<=ll*mm*nn;j++)
printf(" Level %d: %d sols, %#.1f syms, %#.1f cons, %#.1f frcs\n",
j,profile[j],
(double)prof_syms[j]/(double)profile[j],
(double)prof_cons[j]/(double)profile[j],
(double)prof_frcs[j]/(double)profile[j]);
}

/*:45*/
#line 79 "./antislide3.w"
;
}

/*:1*/
