#define nn 81
#define nnn 128 \

#define pack(a,b,c,d) encode[(((a) *3+(b) ) *3+(c) ) *3+(d) ] \

#define dd 5 \

#define max_node_count 22000000 \

#define root &nodes[0] \

/*1:*/
#line 58 "setset-all.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <setjmp.h> 
jmp_buf restart_point;
/*5:*/
#line 133 "setset-all.w"

typedef char SETcard;


/*:5*//*20:*/
#line 373 "setset-all.w"

typedef struct node_struct{
SETcard val;

char level;
struct node_struct*par,*kid,*sib;
char*trans;
}node;

/*:20*/
#line 63 "setset-all.w"

/*6:*/
#line 144 "setset-all.w"

SETcard encode[nn]= {0,1,5,2,6,15,7,16,31,
3,8,17,9,18,32,19,33,50,
10,20,34,21,35,51,36,52,66,
4,11,22,12,23,37,24,38,53,
13,25,39,26,40,54,41,55,67,
27,42,56,43,57,68,58,69,76,
14,28,44,29,45,59,46,60,70,
30,47,61,48,62,71,63,72,77,
49,64,73,65,74,78,75,79,80};

/*:6*//*7:*/
#line 157 "setset-all.w"

int decimalform[nn]= {
0,1,2,10,11,12,20,21,22,
100,101,102,110,111,112,120,121,122,
200,201,202,210,211,212,220,221,222,
1000,1001,1002,1010,1011,1012,1020,1021,1022,
1100,1101,1102,1110,1111,1112,1120,1121,1122,
1200,1201,1202,1210,1211,1212,1220,1221,1222,
2000,2001,2002,2010,2011,2012,2020,2021,2022,
2100,2101,2102,2110,2111,2112,2120,2121,2122,
2200,2201,2202,2210,2211,2212,2220,2221,2222};
int decode[nn];

/*:7*//*9:*/
#line 178 "setset-all.w"

char z[3][3]= {{0,2,1},{2,1,0},{1,0,2}};
char third[nn][nnn];

/*:9*//*11:*/
#line 224 "setset-all.w"

char perm[dd][nnn][nnn];

/*:11*//*15:*/
#line 273 "setset-all.w"

char trit[4];
char aa[5][8];

/*:15*//*16:*/
#line 282 "setset-all.w"

char minp[dd][nnn][nnn+nnn];

/*:16*//*21:*/
#line 476 "setset-all.w"

char legal[nn+1];

/*:21*//*23:*/
#line 498 "setset-all.w"

node nodes[max_node_count];
node*node_ptr= &nodes[1];

/*:23*//*28:*/
#line 584 "setset-all.w"

SETcard x[22];
char forbidden[nn+1];
int auts;
int l;

/*:28*//*41:*/
#line 806 "setset-all.w"

char forbidden_back[22][nnn];
node*node_ptr_back[22];

/*:41*//*46:*/
#line 865 "setset-all.w"

int non_iso_count[30];
double total_count[30];
double multiplier[5]= {81.0,6480.0,505440.0,36391680.0,1965150720.0};

/*:46*/
#line 64 "setset-all.w"

/*22:*/
#line 484 "setset-all.w"

node*new_node()
{
register node*p= node_ptr++;
if(p>=&nodes[max_node_count]){
fprintf(stderr,"Node memory overflow!\n");
exit(-3);
}
p->kid= NULL;
return p;
}

/*:22*//*24:*/
#line 511 "setset-all.w"

void launch(node*,int,node*);
void new_terminal_kid(node*,SETcard);

void new_kid(node*p,int d,SETcard v)
{
register node*q;
/*29:*/
#line 611 "setset-all.w"

if(x[d]>=dd-1){
if(p->kid)new_terminal_kid(p,v);
else{
if(v<x[l])/*42:*/
#line 815 "setset-all.w"

longjmp(restart_point,1);

/*:42*/
#line 615 "setset-all.w"
;
q= new_node();
p->kid= q,q->val= v,q->sib= NULL,q->par= p;
if(v==x[l])q->level= l+1,auts++;
else q->level= l;
}
return;
}

/*:29*/
#line 518 "setset-all.w"
;
q= new_node();
q->val= v,q->par= p,q->level= 0;
q->sib= p->kid,p->kid= q;
launch(p,d+1,q);
for(q= q->sib;q;q= q->sib)if(q->trans)
new_kid(q,d+1,q->trans[v]);
}

/*:24*//*25:*/
#line 532 "setset-all.w"

void launch(node*p,int d,node*q)
{
register int v,w;
register node*r,*s,*t;
v= q->val;
w= minp[x[d-1]+1][v][v];
if(w<x[d])/*42:*/
#line 815 "setset-all.w"

longjmp(restart_point,1);

/*:42*/
#line 539 "setset-all.w"
;
if(w> x[d])q->trans= NULL;
else/*26:*/
#line 548 "setset-all.w"

{
w= x[d-1]+1;
q->trans= minp[w][v];
for(;w<x[d+1];w++)if(w!=x[d])/*27:*/
#line 578 "setset-all.w"

{
for(r= q,v= w;r!=root;r= r->par)v= r->trans[v+nnn];
forbidden[v]= 1;
}

/*:27*/
#line 552 "setset-all.w"
;
/*33:*/
#line 677 "setset-all.w"

if(x[d]>=dd-1&&d<l){
s= p->kid;if(s==q)s= s->sib;
v= q->trans[s->val];
if(v<x[d+1])/*42:*/
#line 815 "setset-all.w"

longjmp(restart_point,1);

/*:42*/
#line 681 "setset-all.w"
;
r= new_node();
q->kid= r,r->par= q;
r->val= v,r->sib= NULL;
if(v==x[d+1]){
r->level= d+2;
if(d+1==l)auts++;
}else r->level= d+1;
for(s= s->sib;s;s= s->sib)if(s!=q)
new_terminal_kid(q,q->trans[s->val]);
return;
}

/*:33*/
#line 553 "setset-all.w"
;
for(r= p->kid,s= NULL;r;r= r->sib)if(r!=q){
t= new_node();
t->par= q;
t->val= q->trans[r->val],t->level= 0;
if(s)s->sib= t;else q->kid= t;
s= t;
}
if(s)s->sib= NULL;
else auts++;
for(r= q->kid;r;r= r->sib)launch(q,d+1,r);
}

/*:26*/
#line 541 "setset-all.w"
;
}

/*:25*//*30:*/
#line 630 "setset-all.w"

void new_terminal_kid(node*q,SETcard v)
{
register node*r,*p;
register int k,w;
r= new_node();
r->val= v,r->sib= q->kid,q->kid= r;
/*31:*/
#line 645 "setset-all.w"

k= r->sib->level;
while(1){
if(v<x[k])/*42:*/
#line 815 "setset-all.w"

longjmp(restart_point,1);

/*:42*/
#line 648 "setset-all.w"
;
if(v> x[k])break;
/*32:*/
#line 664 "setset-all.w"

for(w= x[k-1]+1;w<x[k+1];w++)if(w!=x[k])
{
for(p= q,v= w;p!=root;p= p->par)v= p->trans[v+nnn];
forbidden[v]= 1;
}

/*:32*/
#line 650 "setset-all.w"
;
k++;
if(k> l){
auts++;
break;
}
for(p= r->sib,v= nn;p;p= p->kid)
if(p->val> x[k-1]&&p->val<v)v= p->val;
}

/*:31*/
#line 638 "setset-all.w"
;
r->level= k;
for(p= r->sib;p;p= p->kid)if(p->val> x[k])break;
r->kid= p;
}

/*:30*//*34:*/
#line 697 "setset-all.w"

void print_subtree(node*p,int d)
{
register node*r;
register int k;
for(k= 0;k<d;k++)printf(p->level?" ":".");
printf("%04d",decode[p->val]);
if(p->level){
printf(",%d",p->level);
if(p->kid)printf(" ->%04d",decode[p->kid->val]);
printf("\n");
}else if(p->trans){
/*35:*/
#line 717 "setset-all.w"

k= p->trans[0];
printf(" [%04d,%04d,%04d,%04d,%04d]\n",
decode[k],
decode[third[k][third[0][p->trans[4]]]],
decode[third[k][third[0][p->trans[3]]]],
decode[third[k][third[0][p->trans[2]]]],
decode[third[k][third[0][p->trans[1]]]]);

/*:35*/
#line 709 "setset-all.w"
;
for(r= p->kid;r;r= r->sib)print_subtree(r,d+1);
}else printf("\n");
}

/*:34*//*36:*/
#line 726 "setset-all.w"

void print_trees()
{
register node*r;
for(r= (root)->kid;r;r= r->sib)print_subtree(r,0);
}

/*:36*//*37:*/
#line 736 "setset-all.w"

node*nod(char*s)
{
register char*p;
register int j;
register node*q= root;
for(p= s;*p;p++){
if(!q)return NULL;
for(j= *p-'1',q= q->kid;j;j--){
if(!q)return NULL;
q= q->sib;
}
}
return q;
}
void dummy()
{
malloc(1);
}

/*:37*//*38:*/
#line 758 "setset-all.w"

void print_id(node*p)
{
register node*q= p->par,*r;
register char j;
if(q){
print_id(q);
for(r= q->kid,j= '1';r!=p;j++)
if(r)r= r->sib;
else{
printf("???");return;
}
printf("%c",j);
}
}

void whoami(node*p)
{
print_id(p);printf("\n");
}

/*:38*//*44:*/
#line 828 "setset-all.w"

void prune(node*p)
{
register node*q= p->kid;
register node*r;
if(q){
r= q;
if(q>=node_ptr)p->kid= q= q->sib;
if(!r->level)for(;q;q= q->sib)prune(q);
}
}

/*:44*/
#line 65 "setset-all.w"


main()
{
/*40:*/
#line 803 "setset-all.w"

register int i,j,k;

/*:40*/
#line 69 "setset-all.w"

/*8:*/
#line 170 "setset-all.w"

for(k= 0;k<nn;k++)
decode[encode[k]]= decimalform[k];

/*:8*//*10:*/
#line 184 "setset-all.w"

{
int a,b,c,d,e,f,g,h;
for(a= 0;a<3;a++)for(b= 0;b<3;b++)for(c= 0;c<3;c++)for(d= 0;d<3;d++)
for(e= 0;e<3;e++)for(f= 0;f<3;f++)for(g= 0;g<3;g++)for(h= 0;h<3;h++)
third[pack(a,b,c,d)][pack(e,f,g,h)]= pack(z[a][e],z[b][f],z[c][g],z[d][h]);
}

/*:10*//*12:*/
#line 243 "setset-all.w"

aa[4][4]= 1;
for(j= 0;j<5;j++){
int a,b,c,d,e,f,g,h,jj,kk;
for(jj= j+1;jj<5;jj++)for(k= 0;k<4;k++)aa[jj][k]= (k==jj?1:0);
for(a= 0;a<3;a++)for(b= 0;b<3;b++)for(c= 0;c<3;c++)for(d= 0;d<3;d++){
aa[j][0]= a,aa[j][1]= b,aa[j][2]= c,aa[j][3]= d;
for(kk= j;kk>=0;kk--)if(aa[j][kk])break;
if(kk<0)perm[4-j][pack(a,b,c,d)][0]= -1;
else{
/*13:*/
#line 259 "setset-all.w"

for(jj= 0;jj<j;jj++){
for(k= 0;k<4;k++)aa[jj][k]= 0;
aa[jj][(jj+kk+1)%(j+1)]= 1;
}

/*:13*/
#line 253 "setset-all.w"
;
/*14:*/
#line 265 "setset-all.w"

kk= pack(a,b,c,d);
for(e= 0;e<3;e++)for(f= 0;f<3;f++)for(g= 0;g<3;g++)for(h= 0;h<3;h++){
for(k= 0;k<4;k++)
trit[k]= (e*aa[0][k]+f*aa[1][k]+g*aa[2][k]+h*aa[3][k]+aa[4][k])%3;
perm[4-j][kk][pack(trit[0],trit[1],trit[2],trit[3])]= pack(e,f,g,h);
}

/*:14*/
#line 254 "setset-all.w"
;
}
}
}

/*:12*//*17:*/
#line 293 "setset-all.w"

for(j= dd-1;j>=0;j--)for(k= j;k<nn;k++){
if(perm[j][k][0]!=-1){
for(l= 0;l<nn;l++)minp[j][k][l]= perm[j][k][l];
}else if(j==dd-1){
for(l= 0;l<nn;l++)minp[j][k][l]= l;
for(i= j+1;i<nn;i++)
if(perm[j][i][0]!=-1&&perm[j][i][k]<minp[j][k][k])
for(l= 0;l<nn;l++)minp[j][k][l]= perm[j][i][l];
}else{
register int kk;
for(l= 0;l<nn;l++)minp[j][k][l]= minp[j+1][k][l];
for(i= j+1;i<nn;i++)if(perm[j][i][0]!=-1){
kk= perm[j][i][k];
if(minp[j+1][kk][kk]<minp[j][k][k])
for(l= 0;l<nn;l++)minp[j][k][l]= minp[j+1][kk][perm[j][i][l]];
}
}
for(l= 0;l<nn;l++)minp[j][k][minp[j][k][l]+nnn]= l;
}

/*:17*/
#line 70 "setset-all.w"
;
/*39:*/
#line 782 "setset-all.w"

l= 0;j= 0;
x[0]= -1;
if(setjmp(restart_point))goto backup;
moveup:while(forbidden[j])j++;
if(j==nn)goto backup;
for(k= 0;k<nn;k++)forbidden_back[l][k]= forbidden[k];
node_ptr_back[l]= node_ptr;
auts= 0;
l++,x[l]= x[l+1]= j;
new_kid(root,0,x[l]);
/*45:*/
#line 854 "setset-all.w"

for(j= 1;j<l;j++)printf(".");
non_iso_count[l]++;
for(k= 0;x[k+1]==k;k++);
total_count[l]+= multiplier[k-1]/(double)auts;
printf("%04d (%d:%d) %d\n",decode[x[l]],auts,k,node_ptr-nodes);

/*:45*/
#line 793 "setset-all.w"
;
/*43:*/
#line 818 "setset-all.w"

for(k= 1;k<l;k++)forbidden[third[x[k]][x[l]]]= 1;

/*:43*/
#line 794 "setset-all.w"
;
j= x[l]+1;goto moveup;
backup:l--;
node_ptr= node_ptr_back[l];
prune(root);
for(k= 0;k<nn;k++)forbidden[k]= forbidden_back[l][k];
j= x[l+1]+1;
if(l)goto moveup;

/*:39*/
#line 71 "setset-all.w"
;
/*47:*/
#line 870 "setset-all.w"

for(j= 1;j<=21;j++)
printf("%20.20g SETless %d-sets (%d cases)\n",
total_count[j],j,non_iso_count[j]);

/*:47*/
#line 72 "setset-all.w"
;
}

/*:1*/
