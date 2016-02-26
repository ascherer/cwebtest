#define maxn 32
#define maxd 7
#define bufsize maxn+5 \

#define place(i,j) ((i) *maxn+(j) )  \

#define pack(c,p) (((c) <<16) +(p) )  \

#define root col_array[0] \

#define max_cols (2*maxn*maxn) 
#define max_nodes (maxn*maxn*maxn*maxn*maxd)  \

/*1:*/
#line 22 "back-dissect.w"

#include <stdio.h> 
#include <stdlib.h> 
/*25:*/
#line 549 "back-dissect.w"

typedef struct node_struct{
struct node_struct*left,*right;
struct node_struct*up,*down;
struct col_struct*col;
int info;
}node;

/*:25*//*26:*/
#line 570 "back-dissect.w"

typedef struct col_struct{
node head;
int len;
char*name;
struct col_struct*prev,*next;
}column;

/*:26*/
#line 25 "back-dissect.w"

int d;
char buf[bufsize];
int maxrow;
int maxcol;
char aname[maxn*maxn][8];
char bname[maxn*maxn][8];
int site[maxn*maxn];
int vbose;
/*11:*/
#line 318 "back-dissect.w"

char alen[maxn*maxn];
char blen[maxn*maxn];
int aa[maxn*maxn][maxd];
int bb[maxn*maxn][maxd];
int shift[4*maxn*maxn];
int complement;
int bcover[4*maxn*maxn][maxn*maxn];
int bcovered[4*maxn*maxn];
int s[maxd+1];
int t[maxd+1];

/*:11*//*22:*/
#line 488 "back-dissect.w"

int alist[maxn*maxn],blist[maxn*maxn];
int apos[maxn*maxn],bpos[maxn*maxn];
int acount,bcount;
int acolor[maxn*maxn],bcolor[maxn*maxn];
unsigned long long count;
unsigned long long counta,countb,countc,countd,counte;


/*:22*//*28:*/
#line 591 "back-dissect.w"

column col_array[max_cols+2];
node node_array[max_nodes];
column*acol[maxn*maxn],*bcol[maxn*maxn];
node*choice[maxn*maxn];

/*:28*/
#line 34 "back-dissect.w"
;
/*33:*/
#line 680 "back-dissect.w"

cover(c)
column*c;
{register column*l,*r;
register node*rr,*nn,*uu,*dd;
register k= 1;
l= c->prev;r= c->next;
l->next= r;r->prev= l;
for(rr= c->head.down;rr!=&(c->head);rr= rr->down)
for(nn= rr->right;nn!=rr;nn= nn->right){
uu= nn->up;dd= nn->down;
uu->down= dd;dd->up= uu;
k++;
nn->col->len--;
}
}

/*:33*//*34:*/
#line 701 "back-dissect.w"

uncover(c)
column*c;
{register column*l,*r;
register node*rr,*nn,*uu,*dd;
for(rr= c->head.up;rr!=&(c->head);rr= rr->up)
for(nn= rr->left;nn!=rr;nn= nn->left){
uu= nn->up;dd= nn->down;
uu->down= dd->up= nn;
nn->col->len++;
}
l= c->prev;r= c->next;
l->next= r->prev= c;
}

/*:34*//*42:*/
#line 781 "back-dissect.w"

void debug(char*s){
fflush(stdout);
fprintf(stderr,"***%s!\n",
s);
}

/*:42*/
#line 35 "back-dissect.w"
;
main(int argc,char*argv[]){
register int a,b,dd,i,j,k,l,ll,lll,m,n,nn,slack;
/*2:*/
#line 44 "back-dissect.w"

if(argc<2||sscanf(argv[1],"%d",
&d)!=1){
fprintf(stderr,"Usage: %s d [verbose] [extra verbose] < foo.dots\n",
argv[0]);
exit(-1);
}
if(d<2||d> maxd){
fprintf(stderr,"The number of pieces should be between 2 and %d, not %d!\n",
maxd,d);
exit(-2);
}
vbose= argc-2;

/*:2*/
#line 38 "back-dissect.w"
;
/*3:*/
#line 60 "back-dissect.w"

for(i= nn= 0;;i++){
if(!fgets(buf,bufsize,stdin))break;
if(i>=maxn){
fprintf(stderr,"Recompile me: I allow at most %d lines of input!\n",
maxn);
exit(-3);
}
/*4:*/
#line 87 "back-dissect.w"

for(j= 0;buf[j]&&buf[j]!='\n';j++){
if(buf[j]=='*'){
if(j> maxcol){
maxcol= j;
if(j>=maxn){
fprintf(stderr,"Recompile me: I allow at most %d columns of input!\n",
maxn);
exit(-5);
}
}
site[nn++]= place(i,j);
sprintf(bname[place(i,j)],"%02db%02d",
i,j);
}
}

/*:4*/
#line 68 "back-dissect.w"
;
}
maxrow= i-1;
if(maxrow<0){
fprintf(stderr,"There was no input!\n");
exit(-666);
}
fprintf(stderr,"OK, I've got a shape with %d lines and %d cells.\n",
i,nn);
for(n= 1;n*n<nn;n++);
if(n*n!=nn){
fprintf(stderr,"The number of cells should be a positive perfect square!\n");
exit(-4);
}
for(i= 0;i<n;i++)for(j= 0;j<n;j++)
sprintf(aname[place(i,j)],"%02da%02d",
i,j);
complement= place(n-1,n-1);

/*:3*/
#line 39 "back-dissect.w"
;
/*6:*/
#line 236 "back-dissect.w"

/*8:*/
#line 261 "back-dissect.w"

for(m= 0,a= 1-n;a<=maxrow;a++)for(b= 1-n;b<=maxcol;b++){
for(k= 0,i= (a<0?-a:0);i<n&&a+i<=maxrow;i++)
for(j= (b<0?-b:0);j<n&&b+j<=maxcol;j++)
if(bname[place(a+i,b+j)][0])bcover[m][k++]= place(a+i,b+j);
if(k){
if(vbose> 1)fprintf(stderr," S[%d]=(%d,%d)\n",
m,a,b);
shift[m]= place(a,b),bcovered[m++]= k;
}
}
if(vbose)fprintf(stderr,"There are %d legal shifts.\n",
m);

/*:8*/
#line 237 "back-dissect.w"
;
while(1){
/*9:*/
#line 275 "back-dissect.w"

for(slack= -nn,k= 1;k<=d;k++)slack+= bcovered[s[k]];
if(slack<0)goto shapenot;
for(k= 0;k<nn;k++)blen[site[k]]= 0;
for(k= 1;k<=d;k++){
for(i= 0,j= s[k];i<bcovered[j];i++){
l= bcover[j][i];
if(!blen[l])blen[l]= 1;
else{
if(!slack)goto shapenot;
slack--;
blen[l]++;
}
}
}

/*:9*/
#line 239 "back-dissect.w"
;
counta++;
/*7:*/
#line 247 "back-dissect.w"

for(k= 2;k<=d;k++)t[k]= 0;
while(1){
for(k= 2;k<=d;k++)
if(s[k]==s[k-1]&&t[k]==t[k-1])goto squarenot;
/*10:*/
#line 296 "back-dissect.w"

for(k= 0;k<nn;k++)blen[site[k]]= 0;
for(i= 0;i<n;i++)for(j= 0;j<n;j++)alen[place(i,j)]= 0;
for(slack= -nn,k= 1;k<=d;k++)slack+= bcovered[s[k]];
for(k= 1;k<=d;k++){
for(i= 0,j= s[k];i<bcovered[j];i++){
l= bcover[j][i];
ll= l-shift[j];
if(t[k]&1){
register int q= ll/maxn,r= ll%maxn;
ll= place(r,n-1-q);
}
if(t[k]&2)ll= complement-ll;
if(alen[ll]){
if(!slack)goto squarenot;
slack--;
}
aa[ll][alen[ll]++]= pack(k,l);
bb[l][blen[l]++]= pack(k,ll);
}
}

/*:10*/
#line 253 "back-dissect.w"
;
countb++;
/*12:*/
#line 340 "back-dissect.w"

if(vbose> 1)/*13:*/
#line 350 "back-dissect.w"

{
fprintf(stderr," Trying to match");
for(k= 1;k<=d;k++)fprintf(stderr," %d^%d",
s[k],t[k]);
fprintf(stderr,":\n");
for(i= 0;i<n;i++)for(j= 0;j<n;j++){
fprintf(stderr,"  %s --",
aname[place(i,j)]);
for(k= 0;k<alen[place(i,j)];k++)fprintf(stderr," %s.%d",
bname[aa[place(i,j)][k]&0xffff],aa[place(i,j)][k]>>16);
fprintf(stderr,"\n");
}
}

/*:13*/
#line 341 "back-dissect.w"
;
/*14:*/
#line 365 "back-dissect.w"

for(acount= i= 0;i<n;i++)for(j= 0;j<n;j++){
if(alen[place(i,j)]> 1)apos[place(i,j)]= acount,alist[acount++]= place(i,j);
else{
l= aa[place(i,j)][0]&0xffff;
if(!blen[l])goto done;
acolor[place(i,j)]= bcolor[l]= aa[place(i,j)][0]>>16;
if(blen[l]==1)blen[l]= 0;
else/*15:*/
#line 384 "back-dissect.w"

{
for(k= 0;k<blen[l];k++){
ll= bb[l][k]&0xffff;
if(ll!=place(i,j)){
register int opp= (bb[l][k]&0xffff0000)+l;

dd= alen[ll]-1,alen[ll]= dd;
if(!dd)goto done;
for(a= 0;aa[ll][a]!=opp;a++);
if(a> dd)debug("ahi");
if(a!=dd)aa[ll][a]= aa[ll][dd];
}
}
blen[l]= 0;
}

/*:15*/
#line 373 "back-dissect.w"
;
}
}

/*:14*/
#line 342 "back-dissect.w"
;
countc++;
/*16:*/
#line 401 "back-dissect.w"

if(acount){
for(bcount= i= 0;i<nn;i++){
l= site[i];
if(!blen[l])continue;
if(blen[l]> 1)bpos[l]= bcount,blist[bcount++]= l;
else{
ll= bb[l][0]&0xffff;
if(!alen[ll])goto done;
acolor[ll]= bcolor[l]= bb[l][0]>>16;
acount--;
/*17:*/
#line 418 "back-dissect.w"

j= apos[ll];
if(j!=acount)
lll= alist[acount],alist[j]= lll,apos[lll]= j;
if(alen[ll]!=1){
for(k= 0;k<alen[ll];k++){
lll= aa[ll][k]&0xffff;
if(lll!=l){
register int opp= (aa[ll][k]&0xffff0000)+ll;

dd= blen[lll]-1,blen[lll]= dd;
if(!dd)goto done;
for(b= 0;bb[lll][b]!=opp;b++);
if(b> dd)debug("bhi");
if(b!=dd)bb[lll][b]= bb[lll][dd];
}
}
alen[ll]= 0;
}

/*:17*/
#line 412 "back-dissect.w"
;
}
}
if(acount!=bcount)debug("count mismatch");
}

/*:16*/
#line 344 "back-dissect.w"
;
/*18:*/
#line 442 "back-dissect.w"

while(acount){
for(i= 0;i<acount;i++)if(alen[ll= alist[i]]==1)
/*19:*/
#line 452 "back-dissect.w"

{
acount--;
if(i<acount)lll= alist[acount],alist[i]= lll,apos[lll]= i--;
l= aa[ll][0]&0xffff;
acolor[ll]= bcolor[l]= aa[ll][0]>>16;
/*20:*/
#line 461 "back-dissect.w"

j= bpos[l];
if(j<acount)lll= blist[acount],blist[j]= lll,bpos[lll]= j;
if(blen[l]!=1){
for(k= 0;k<blen[l];k++){
lll= bb[l][k]&0xffff;
if(lll!=ll){
register int opp= (bb[l][k]&0xffff0000)+l;

dd= alen[lll]-1,alen[lll]= dd;
if(!dd)goto done;
for(a= 0;aa[lll][a]!=opp;a++);
if(a> dd)debug("chi");
if(a!=dd)aa[lll][a]= aa[lll][dd];
}
}
}

/*:20*/
#line 458 "back-dissect.w"
;
}

/*:19*/
#line 445 "back-dissect.w"
;
for(i= 0;i<acount;i++)if(blen[l= blist[i]]==1)
/*21:*/
#line 479 "back-dissect.w"

{
acount--;
if(i<acount)lll= blist[acount],blist[i]= lll,bpos[lll]= i--;
ll= bb[l][0]&0xffff;
acolor[ll]= bcolor[l]= bb[l][0]>>16;
/*17:*/
#line 418 "back-dissect.w"

j= apos[ll];
if(j!=acount)
lll= alist[acount],alist[j]= lll,apos[lll]= j;
if(alen[ll]!=1){
for(k= 0;k<alen[ll];k++){
lll= aa[ll][k]&0xffff;
if(lll!=l){
register int opp= (aa[ll][k]&0xffff0000)+ll;

dd= blen[lll]-1,blen[lll]= dd;
if(!dd)goto done;
for(b= 0;bb[lll][b]!=opp;b++);
if(b> dd)debug("bhi");
if(b!=dd)bb[lll][b]= bb[lll][dd];
}
}
alen[ll]= 0;
}

/*:17*/
#line 485 "back-dissect.w"
;
}

/*:21*/
#line 447 "back-dissect.w"
;
if(acount==bcount)break;
bcount= acount;
}

/*:18*/
#line 345 "back-dissect.w"
;
countd++;
/*23:*/
#line 506 "back-dissect.w"

if(acount==0)/*40:*/
#line 754 "back-dissect.w"

{
register int OK= 1;
if(OK){
count++;
printf("Solution %lld, from",
count);
for(k= 1;k<=d;k++)printf(" %d^%d",
s[k],t[k]);
printf(":\n");
for(i= 0;i<n||i<=maxrow;i++){
for(j= 0;j<n;j++)printf("%c",
i<n?acolor[place(i,j)]+'0':' ');
if(i<=maxrow){
printf("  ");
for(j= 0;j<=maxcol;j++)printf("%c",
bname[place(i,j)][0]?bcolor[place(i,j)]+'0':'.');
}
printf("\n");
}
}
}

/*:40*/
#line 507 "back-dissect.w"

else{
/*27:*/
#line 584 "back-dissect.w"

register column*cur_col;
register node*cur_node;

/*:27*//*32:*/
#line 672 "back-dissect.w"

register int level;
register column*best_col;

/*:32*//*38:*/
#line 739 "back-dissect.w"

register int minlen;
register int j,k,x;

/*:38*/
#line 509 "back-dissect.w"
;
counte++;
if(vbose> 1)/*24:*/
#line 516 "back-dissect.w"

{
fprintf(stderr," which reduces to:\n");
for(i= 0;i<acount;i++){
fprintf(stderr,"  %s --",
aname[alist[i]]);
for(k= 0;k<alen[alist[i]];k++)fprintf(stderr," %s.%d",
bname[aa[alist[i]][k]&0xffff],aa[alist[i]][k]>>16);
fprintf(stderr,"\n");
}
}

/*:24*/
#line 511 "back-dissect.w"
;
/*29:*/
#line 597 "back-dissect.w"

for(i= 0;i<acount;i++){
ll= alist[i],l= blist[i];
acol[ll]= &col_array[i+i+1],col_array[i+i+1].name= aname[ll];
bcol[l]= &col_array[i+i+2],col_array[i+i+2].name= bname[l];
}
root.prev= &col_array[acount+acount];
root.prev->next= &root;
for(cur_col= col_array+1;cur_col<=root.prev;cur_col++){
cur_col->head.up= cur_col->head.down= &cur_col->head;
cur_col->len= 0;
cur_col->prev= cur_col-1,(cur_col-1)->next= cur_col;
}
for(cur_node= node_array,i= 0;i<acount;i++){
ll= alist[i];
for(k= 0;k<alen[ll];k++)/*30:*/
#line 615 "back-dissect.w"

{
register column*ccol;
l= aa[ll][k]&0xffff;
j= ((aa[ll][k]>>16)<<24)+(l<<12)+ll;
ccol= acol[ll];
cur_node->left= cur_node->right= cur_node+1;
cur_node->col= ccol,cur_node->info= j;
cur_node->up= ccol->head.up,ccol->head.up->down= cur_node;
ccol->head.up= cur_node,cur_node->down= &ccol->head;
ccol->len++;
cur_node++;
ccol= bcol[l];
cur_node->left= cur_node->right= cur_node-1;
cur_node->col= ccol,cur_node->info= j;
cur_node->up= ccol->head.up,ccol->head.up->down= cur_node;
ccol->head.up= cur_node,cur_node->down= &ccol->head;
ccol->len++;
cur_node++;
}

/*:30*/
#line 612 "back-dissect.w"
;
}

/*:29*/
#line 512 "back-dissect.w"
;
/*31:*/
#line 652 "back-dissect.w"

level= 0;
forward:/*37:*/
#line 730 "back-dissect.w"

minlen= max_nodes;
if(vbose> 2)fprintf(stderr,"Level %d:",level);
for(cur_col= root.next;cur_col!=&root;cur_col= cur_col->next){
if(vbose> 2)fprintf(stderr," %s(%d)",cur_col->name,cur_col->len);
if(cur_col->len<minlen)best_col= cur_col,minlen= cur_col->len;
}
if(vbose> 2)fprintf(stderr," branching on %s(%d)\n",best_col->name,minlen);

/*:37*/
#line 654 "back-dissect.w"
;
cover(best_col);
cur_node= choice[level]= best_col->head.down;
advance:if(cur_node==&(best_col->head))goto backup;
if(vbose> 1)
fprintf(stderr,"L%d: %s %s\n",
level,cur_node->col->name,cur_node->right->col->name);
/*35:*/
#line 716 "back-dissect.w"

cover(cur_node->right->col);

/*:35*/
#line 661 "back-dissect.w"
;
if(root.next==&root)/*39:*/
#line 743 "back-dissect.w"

{
if(vbose> 1)fprintf(stderr,"(a good dance)\n");
for(k= 0;k<=level;k++){
j= choice[k]->info;
acolor[j&0xfff]= bcolor[(j>>12)&0xfff]= j>>24;
}
/*40:*/
#line 754 "back-dissect.w"

{
register int OK= 1;
if(OK){
count++;
printf("Solution %lld, from",
count);
for(k= 1;k<=d;k++)printf(" %d^%d",
s[k],t[k]);
printf(":\n");
for(i= 0;i<n||i<=maxrow;i++){
for(j= 0;j<n;j++)printf("%c",
i<n?acolor[place(i,j)]+'0':' ');
if(i<=maxrow){
printf("  ");
for(j= 0;j<=maxcol;j++)printf("%c",
bname[place(i,j)][0]?bcolor[place(i,j)]+'0':'.');
}
printf("\n");
}
}
}

/*:40*/
#line 750 "back-dissect.w"
;
goto recover;
}

/*:39*/
#line 662 "back-dissect.w"
;
level++;
goto forward;
backup:uncover(best_col);
if(level==0)goto done;
level--;
cur_node= choice[level];best_col= cur_node->col;
recover:/*36:*/
#line 727 "back-dissect.w"

uncover(cur_node->left->col);

/*:36*/
#line 669 "back-dissect.w"
;
cur_node= choice[level]= cur_node->down;goto advance;

/*:31*/
#line 513 "back-dissect.w"
;
}

/*:23*/
#line 347 "back-dissect.w"
;
done:

/*:12*/
#line 255 "back-dissect.w"
;
squarenot:for(k= d;t[k]==3;k--)t[k]= 0;
if(k==1)break;
t[k]++;
}

/*:7*/
#line 241 "back-dissect.w"
;
shapenot:for(k= d;s[k]==m-1;k--);
if(k==0)break;
for(j= s[k]+1;k<=d;k++)s[k]= j;
}

/*:6*/
#line 40 "back-dissect.w"
;
/*41:*/
#line 777 "back-dissect.w"

fprintf(stderr,"%lld solutions; run stats %d,%lld,%lld,%lld,%lld,%lld.\n",
count,m,counta,countb,countc,countd,counte);

/*:41*/
#line 41 "back-dissect.w"
;
}

/*:1*/
