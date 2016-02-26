#define sentinel 999
#define maxcodes 64 \

#define abort0(message,code) {fprintf(stderr,"%s!\n", \
message) ; \
exit(code) ;}
#define abort1(message,j,code) {fprintf(stderr,"Bad arg (%s): %s!\n", \
argv[j],message) ; \
exit(code) ;}
#define abort2(message,j,c,code) {fprintf(stderr,"Bad arg (%s): Node '%c' %s!\n", \
argv[j],c,message) ; \
exit(code) ;} \

#define root inputbud[1].parent \

#define offset 2 \

#define pip(u,i) ((u) <<2) +(i) 
#define pip_edge(p) ((p) >>2) 
#define pip_sub(p) ((p) &0x3) 
#define rot(p) (((p) +1) ^(((p) ^((p) +1) ) &-4) ) 
#define irot(p) (((p) -1) ^(((p) ^((p) -1) ) &-4) )  \

/*5:*/
#line 156 "skew-ternary-calc.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
/*7:*/
#line 193 "skew-ternary-calc.w"

typedef struct node_struct{
int left;
int middle;
int right;
int parent;
int rank;
}node;

typedef struct bud_struct{
int parent;
int rank;
int stepno;
}bud;

/*:7*//*14:*/
#line 371 "skew-ternary-calc.w"

typedef struct step_struct{
int rank;
int first;
int second;
int match;
}step;

/*:14*/
#line 160 "skew-ternary-calc.w"

/*8:*/
#line 208 "skew-ternary-calc.w"

node inputnode[256];
bud inputbud[512];
int buds;
int n;

/*:8*//*15:*/
#line 379 "skew-ternary-calc.w"

step chart[4*maxcodes];
int steps;
int stack[256];
int stacked;

/*:15*//*30:*/
#line 795 "skew-ternary-calc.w"

int alpha[4*256];

int alphainv[4*256];
int verts;

/*:30*//*41:*/
#line 1095 "skew-ternary-calc.w"

step chartstack[maxcodes][4*maxcodes];

step tmpchart[4*maxcodes];
int stk[maxcodes*maxcodes];
int curbud;

/*:41*/
#line 161 "skew-ternary-calc.w"

/*66:*/
#line 1571 "skew-ternary-calc.w"

void confusion(char*id){
fprintf(stderr,"This can't happen (%s)!\n",id);
exit(-666);
}

/*:66*/
#line 162 "skew-ternary-calc.w"

/*11:*/
#line 300 "skew-ternary-calc.w"

void setmate(int p){
register int q,d;
buds+= 2,q= 1-buds;
if(inputbud[buds-1].parent){
if(buds> 2)confusion("bud parent already set");
goto downward_mid;
}
inputbud[buds-1].parent= p;
upward:if(inputnode[p].middle==q){
q= p,p= inputnode[p].parent;
goto upward;
}
if(inputnode[p].left==q){
q= p,p= inputnode[p].right,d= 1;
goto downward;
}
if(inputnode[p].right==q){
q= p,p= inputnode[p].left,d= -1;
goto downward;
}
confusion("supposed parent node not apparent");
downward_mid:q= p,p= inputnode[p].middle,d= 0;
downward:if(p<0)abort0("Mate mixup",-25);
if(p> 0)goto downward_mid;
if(d> 0)inputnode[q].right= -buds;
else if(d<0)inputnode[q].left= -buds;
else inputnode[q].middle= -buds;
inputbud[buds].parent= q;
}

/*:11*//*12:*/
#line 334 "skew-ternary-calc.w"

void fillbuds(int p,int r){
if(r<0){
fprintf(stderr,"Not properly skewed: rank(%c)=-1!\n",p);
exit(-30);
}
inputnode[p].rank= r;
if(inputnode[p].left> 0)fillbuds(inputnode[p].left,r-1);
else{
if(inputnode[p].left==0)inputnode[p].left= -buds-1,setmate(p);
inputbud[-inputnode[p].left].rank= r-1;
}
if(inputnode[p].middle> 0)fillbuds(inputnode[p].middle,r);
else{
if(inputnode[p].middle==0)inputnode[p].middle= -buds-1,setmate(p);
inputbud[-inputnode[p].middle].rank= r;
}
if(inputnode[p].right> 0)fillbuds(inputnode[p].right,r+1);
else{
if(inputnode[p].right==0)inputnode[p].right= -buds-1,setmate(p);
inputbud[-inputnode[p].right].rank= r+1;
}
}

/*:12*//*17:*/
#line 393 "skew-ternary-calc.w"

void branch(int,int);
void budstep(int);
void createsteps(int p){
register int q;
q= inputnode[p].left;
if(q> 0)branch(p,q);
else budstep(-q);
q= inputnode[p].middle;
if(q> 0)branch(p,q);
else budstep(-q);
q= inputnode[p].right;
if(q> 0)branch(p,q);
else budstep(-q);
}

/*:17*//*18:*/
#line 411 "skew-ternary-calc.w"

void budstep(int b){
chart[steps].first= b,chart[steps].rank= stacked-offset;
if(chart[steps].rank!=inputbud[b].rank)confusion("rank offense b");
inputbud[b].stepno= steps;
steps++,stack[stacked++]= b;
}

/*:18*//*19:*/
#line 419 "skew-ternary-calc.w"

void branch(int p,int q){
chart[steps].first= p,chart[steps].second= q,chart[steps].rank= stacked-offset;
if(chart[steps].rank!=inputnode[q].rank)confusion("rank offense q");
chart[steps].match= stack[--stacked];
steps++;
createsteps(q);
chart[steps].first= q,chart[steps].second= p,chart[steps].rank= stacked-offset;
if(chart[steps].rank!=inputnode[q].rank+2)confusion("rank offense p");
chart[steps].match= stack[--stacked];
steps++;
}

/*:19*//*21:*/
#line 445 "skew-ternary-calc.w"

void printfam(int p){
register int q;
int l,m,r;
if(steps==4*n)steps= 0;
q= chart[steps++].second;
if(q==0)l= '-';
else l= q,printfam(q),steps++;
if(steps==4*n)steps= 0;
q= chart[steps++].second;
if(q==0)m= '-';
else m= q,printfam(q),steps++;
if(steps==4*n)steps= 0;
q= chart[steps++].second;
if(q==0)r= '-';
else r= q,printfam(q),steps++;
printf(" %c%c%c%c",
p,l,m,r);
}

/*:21*//*24:*/
#line 483 "skew-ternary-calc.w"

void print_tree(int p){
register int i;
for(i= 0;i<inputnode[p].rank+8;i++)printf(".");
printf(" %c:",
p);
if(inputnode[p].left<0)printf("%3d",
-inputnode[p].left);
else printf("  %c",
inputnode[p].left);
if(inputnode[p].middle<0)printf("%3d",
-inputnode[p].middle);
else printf(" %c ",
inputnode[p].middle);
if(inputnode[p].right<0)printf("%3d\n",
-inputnode[p].right);
else printf(" %c\n",
inputnode[p].right);
if(inputnode[p].left> 0)print_tree(inputnode[p].left);
if(inputnode[p].middle> 0)print_tree(inputnode[p].middle);
if(inputnode[p].right> 0)print_tree(inputnode[p].right);
}

/*:24*//*32:*/
#line 831 "skew-ternary-calc.w"

void splice(int p,int q){
register int r,s;
if((p&1)+(q&1))confusion("attempt to splice face pips");
r= alphainv[p],s= alphainv[q];
alphainv[p]= s,alphainv[q]= r;
alpha[s]= p,alpha[r]= q;
p= alpha[rot(p)],q= alpha[rot(q)];
r= alphainv[p],s= alphainv[q];
alphainv[p]= s,alphainv[q]= r;
alpha[s]= p,alpha[r]= q;
verts--;
}

/*:32*//*33:*/
#line 853 "skew-ternary-calc.w"

void print_alpha(void){
register int c,f,p,q,r,t,v;
/*34:*/
#line 868 "skew-ternary-calc.w"

printf("Vertices:\n");
v= 0,p= pip('*',0),t= 2*(n+1);
while(1){
for(;alpha[p]<=0&&t;p+= 2){
if(alpha[p]<0){
alpha[p]= -alpha[p],t--;
}
}
if(t==0)break;
for(q= p,r= alpha[q];r> 0;q= r,r= alpha[q]){
printf(" %c%d",
pip_edge(r),pip_sub(r));
alpha[q]= -r;
}
printf("\n");
v++;
}

/*:34*/
#line 856 "skew-ternary-calc.w"
;
if(v!=verts)confusion("vertex count");
/*35:*/
#line 889 "skew-ternary-calc.w"

printf("Faces:\n");
f= 0,p= pip('*',1),t= 2*(n+1);
while(1){
for(;alpha[p]<=0&&t;p+= 2){
if(alpha[p]<0){
alpha[p]= -alpha[p],t--;
}
}
if(t==0)break;
for(q= p,r= alpha[q];r> 0;q= r,r= alpha[q]){
printf(" %c%d",
pip_edge(r),pip_sub(r));
alpha[q]= -r;
}
printf("\n");
f++;
}

/*:35*/
#line 858 "skew-ternary-calc.w"
;
c= (v-(n+1)+f)>>1;
printf("(Altogether %d vertices, %d edges, %d faces, %d component%s.)\n",
v,n+1,f,c,c==1?"":"s");
}

/*:33*//*42:*/
#line 1112 "skew-ternary-calc.w"

void rnbpm_js(int s,int r,int h){
register int i,j,k,l,m,p,q,t,tt,apip,steps;
/*43:*/
#line 1132 "skew-ternary-calc.w"

if(chartstack[s][0].second)confusion("no root bud");
for(m= 1,steps= 1;;m++){
if(chartstack[s][steps++].second)confusion("non skew");
stk[h+m]= chartstack[s][steps++].second;
if(stk[h+m]==0)break;
}

/*:43*/
#line 1115 "skew-ternary-calc.w"
;
apip= pip(r,2);
while(m){
m--,t= stk[h+m];
/*44:*/
#line 1156 "skew-ternary-calc.w"

if(chartstack[s][steps].second==0)l= -1;
else{
tt= chartstack[s][steps].second;
if(chartstack[s][steps++].first!=t)confusion("wrong parent");
tmpchart[0].first= tmpchart[0].second= 0;
for(j= 1,q= l= 0;chartstack[s][steps].second!=t;steps++,j++){
tmpchart[j]= chartstack[s][steps];
if(q==2)k= j;
else if(q==-1)l++;
if(tmpchart[j].second)q--;else q++;
}
if(chartstack[s][steps].first!=tt)confusion("right bracket");
for(i= k;i<j;i++)chartstack[s+1][i-k]= tmpchart[i];
for(i= 0;i<k;i++)chartstack[s+1][i+j-k]= tmpchart[i];
}
steps++;

/*:44*/
#line 1119 "skew-ternary-calc.w"
;
if(m&&l>=0&&
(chartstack[s][steps].first!=t||
chartstack[s][steps].second!=stk[h+m-1]))
confusion("arc bracketing");
steps++;
if(l<0)/*45:*/
#line 1178 "skew-ternary-calc.w"

p= pip(t,1);

/*:45*/
#line 1125 "skew-ternary-calc.w"

else/*46:*/
#line 1185 "skew-ternary-calc.w"

{
stk[h+m]= (chartstack[s+1][2].second?chartstack[s+1][2].first:
chartstack[s+1][3].second?chartstack[s+1][3].first:
tt);
rnbpm_js(s+1,t,h+m);
for(p= alphainv[pip(t,3)];l;l--)
p= alphainv[p];
}

/*:46*/
#line 1126 "skew-ternary-calc.w"
;
/*47:*/
#line 1195 "skew-ternary-calc.w"

splice(irot(p),apip);
apip= pip(t,2);

/*:47*/
#line 1127 "skew-ternary-calc.w"
;
}
/*48:*/
#line 1199 "skew-ternary-calc.w"

splice(pip(r,0),apip);

/*:48*/
#line 1129 "skew-ternary-calc.w"
;
}

/*:42*//*55:*/
#line 1368 "skew-ternary-calc.w"

void rnbpm_ddp(int s,int r){
register int c,i,j,jj,k,p,q,rr,t,steps,parent;
/*56:*/
#line 1381 "skew-ternary-calc.w"

if(chartstack[s][0].second)confusion("no root bud");
for(steps= 1,q= -1;chartstack[s][steps].second!=sentinel;steps++){
if(q==-1)j= steps;
if(chartstack[s][steps].second==0)q++;else q--;
}
if(q!=2)confusion("bad rank at end");
c= chartstack[s][j-1].second;
if(c==0){
if(j!=1)confusion("parentless rank -1 bud not at beginning");
c= chartstack[s][steps].first,p= 0;
}else p= chartstack[s][j-1].first;
if(chartstack[s][j+1].second)confusion("not the last zero");

/*:56*/
#line 1371 "skew-ternary-calc.w"
;
/*57:*/
#line 1402 "skew-ternary-calc.w"

jj= j-1,steps= j+2;
rr= chartstack[s][steps].second;
if(rr){
if(chartstack[s][steps++].first!=c)confusion("wrong parent");
tmpchart[0].first= tmpchart[0].second= 0;
for(j= 1,q= 0;chartstack[s][steps].second!=c;steps++,j++){
tmpchart[j]= chartstack[s][steps];
if(q==2)k= j;
if(tmpchart[j].second)q--;else q++;
}
if(chartstack[s][steps].first!=rr)confusion("right bracket");
for(i= k;i<j;i++)chartstack[s+1][i-k]= tmpchart[i];
for(i= 0;i<k;i++)chartstack[s+1][i+j-k]= tmpchart[i];
chartstack[s+1][j].second= sentinel;
chartstack[s+1][j].first= (chartstack[s+1][2].second?chartstack[s+1][2].first:
chartstack[s+1][3].second?chartstack[s+1][3].first:
rr);
}
steps++;

/*:57*/
#line 1372 "skew-ternary-calc.w"
;
/*58:*/
#line 1423 "skew-ternary-calc.w"

if(rr)rnbpm_ddp(s+1,c);

/*:58*/
#line 1373 "skew-ternary-calc.w"
;
/*59:*/
#line 1432 "skew-ternary-calc.w"

if(p){
for(i= 0;i<jj;i++)chartstack[s+1][i]= chartstack[s][i];
chartstack[s+1][i].first= chartstack[s+1][i].second= 0,i++;
for(steps++;;steps++,i++){
chartstack[s+1][i]= chartstack[s][steps];
if(chartstack[s+1][i].second==sentinel)break;
}
}

/*:59*/
#line 1374 "skew-ternary-calc.w"
;
/*60:*/
#line 1442 "skew-ternary-calc.w"

if(p)rnbpm_ddp(s+1,r);

/*:60*/
#line 1375 "skew-ternary-calc.w"
;
/*61:*/
#line 1449 "skew-ternary-calc.w"

if(rr==0){
if(p)splice(pip(c,0),alpha[pip(p,0)]);
else splice(pip(c,0),pip(r,2));
}else{
if(p)splice(pip(c,2),alpha[pip(p,0)]);
else splice(pip(c,2),pip(r,2));
splice(pip(c,2),alpha[pip(c,2)]);
verts+= 2;
}
splice(pip(c,2),irot(alphainv[pip(r,3)]));

/*:61*/
#line 1376 "skew-ternary-calc.w"
;
}

/*:55*/
#line 163 "skew-ternary-calc.w"

main(int argc,char*argv[]){
register int c,i,j,k,p;
/*6:*/
#line 173 "skew-ternary-calc.w"

if(argc==1){
fprintf(stderr,"Usage: %s node_1 node_2 ... node_n\n",argv[0]);
exit(-1);
}
/*9:*/
#line 226 "skew-ternary-calc.w"

for(j= 1;j<argc;j++){
if(strlen(argv[j])!=4)
abort1("Must be four characters long",j,-10);
c= argv[j][0];
if(c<'@'||c> '~')
abort2("is not permitted",j,c,-15);
if(inputnode[c].rank)
abort2("has already been defined",j,c,-11);
inputnode[c].rank= sentinel;
p= argv[j][1];
if(p!='-'){
if(p<'@'||p> '~')
abort2("is not permitted",j,p,-16);
if(inputnode[p].parent)
abort2("already has a parent",j,p,-12);
inputnode[c].left= p,inputnode[p].parent= c;
}
p= argv[j][2];
if(p!='-'){
if(p<'@'||p> '~')
abort2("is not permitted",j,p,-17);
if(inputnode[p].parent)
abort2("already has a parent",j,p,-13);
inputnode[c].middle= p,inputnode[p].parent= c;
}
p= argv[j][3];
if(p!='-'){
if(p<'@'||p> '~')
abort2("is not permitted",j,p,-18);
if(inputnode[p].parent)
abort2("already has a parent",j,p,-14);
inputnode[c].right= p,inputnode[p].parent= c;
}
}
n= argc-1;
/*10:*/
#line 271 "skew-ternary-calc.w"

for(j= '@';j<='~';j++){
if(inputnode[j].rank&&!inputnode[j].parent){
if(root){
fprintf(stderr,"Nodes '%c' and '%c' cannot both be roots!\n",
root,j);
exit(-20);
}
root= j;
}
if(inputnode[j].parent&&!inputnode[j].rank){
fprintf(stderr,"No data was supplied for node `%c'!\n",
j);
exit(-21);
}
}
if(!root)abort0("There's no root",-21);
inputbud[1].rank= -2;
setmate(root);
fillbuds(root,0);
/*13:*/
#line 363 "skew-ternary-calc.w"

if(buds!=n+n+2)abort0("The input contains a cycle",-66);

/*:13*/
#line 291 "skew-ternary-calc.w"
;

/*:10*/
#line 262 "skew-ternary-calc.w"
;

/*:9*/
#line 179 "skew-ternary-calc.w"
;

/*:6*/
#line 166 "skew-ternary-calc.w"
;
/*16:*/
#line 385 "skew-ternary-calc.w"

/*20:*/
#line 432 "skew-ternary-calc.w"

chart[0].rank= -2,chart[0].first= 1,steps= 1;
stack[0]= 1,stacked= offset-1;
createsteps(root);
if(stacked!=2+offset)confusion("mismatched");
if(steps!=4*n)confusion("total steps");

/*:20*/
#line 386 "skew-ternary-calc.w"
;
/*23:*/
#line 479 "skew-ternary-calc.w"

print_tree(root);
printf("\n");

/*:23*/
#line 387 "skew-ternary-calc.w"
;
/*22:*/
#line 468 "skew-ternary-calc.w"

chart[4*n].second= sentinel,chart[4*n].first= root;
for(j= 1;j<4;j++){
for(i= 0;i<j;i++)printf("+");
printf(":");
for(k= steps= inputbud[stack[j+offset-2]].stepno+1;chart[k].second==0;k++);
printfam(chart[k].first);
printf("\n");
if(chart[steps].first!=stack[j+offset-2])confusion("bad end of cycle");
}

/*:22*/
#line 388 "skew-ternary-calc.w"
;

/*:16*/
#line 167 "skew-ternary-calc.w"
;
/*49:*/
#line 1206 "skew-ternary-calc.w"

for(j= 0;j<4;j++){
printf("--- JS map for T");
for(i= 0;i<j;i++)printf("+");
printf(" ---\n");
/*31:*/
#line 808 "skew-ternary-calc.w"

for(k= '*',verts= 0;k<='~';k++)if(k=='*'||inputnode[k].left){
alpha[pip(k,0)]= alphainv[pip(k,0)]= pip(k,0);
alpha[pip(k,1)]= alphainv[pip(k,1)]= pip(k,3);
alpha[pip(k,2)]= alphainv[pip(k,2)]= pip(k,2);
alpha[pip(k,3)]= alphainv[pip(k,3)]= pip(k,1);
verts+= 2;
}
if(verts!=2*(n+1))confusion("initial vertex count");

/*:31*/
#line 1211 "skew-ternary-calc.w"
;
for(i= inputbud[stack[j+offset-2]].stepno,k= 0;i<4*n;i++,k++)
chartstack[0][k]= chart[i];
for(i= 0;i<inputbud[stack[j+offset-2]].stepno;i++,k++)
chartstack[0][k]= chart[i];
stk[0]= inputbud[stack[j+offset-2]].parent;
rnbpm_js(0,'*',0);
print_alpha();
}

/*:49*//*62:*/
#line 1465 "skew-ternary-calc.w"

for(j= 0;j<4;j++){
printf("--- DDP map for T");
for(i= 0;i<j;i++)printf("+");
printf(" ---\n");
/*31:*/
#line 808 "skew-ternary-calc.w"

for(k= '*',verts= 0;k<='~';k++)if(k=='*'||inputnode[k].left){
alpha[pip(k,0)]= alphainv[pip(k,0)]= pip(k,0);
alpha[pip(k,1)]= alphainv[pip(k,1)]= pip(k,3);
alpha[pip(k,2)]= alphainv[pip(k,2)]= pip(k,2);
alpha[pip(k,3)]= alphainv[pip(k,3)]= pip(k,1);
verts+= 2;
}
if(verts!=2*(n+1))confusion("initial vertex count");

/*:31*/
#line 1470 "skew-ternary-calc.w"
;
for(i= inputbud[stack[j+offset-2]].stepno,k= 0;i<4*n;i++,k++)
chartstack[0][k]= chart[i];
for(i= 0;i<inputbud[stack[j+offset-2]].stepno;i++,k++)
chartstack[0][k]= chart[i];
chartstack[0][k].first= inputbud[stack[j+offset-2]].parent;
chartstack[0][k].second= sentinel;
rnbpm_ddp(0,'*');
print_alpha();
}

/*:62*/
#line 168 "skew-ternary-calc.w"
;
}

/*:5*/
