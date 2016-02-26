#define verbose Verbose \

#define bdry 999999
#define obst 999998
#define maxsize 256
#define boardsize (maxsize*3+2)  \

#define boardover() { \
fprintf(stderr,"Sorry, I can't handle that large a board;\n") ; \
fprintf(stderr," please recompile me with more maxsize.\n") ; \
exit(-3) ; \
} \

#define bufsize 1024 \

#define cell(j,k) board[ul+(j) *colsp+k] \

#define hashsize (1<<13) 
#define hashcode(x) (uni[0][x&0xff]+uni[1][(x>>8) &0xff]+ \
uni[2][(x>>16) &0xff]+uni[3][x>>24])  \

#define memsize (1<<25) 
#define maxmoves 1000 \

/*4:*/
#line 105 "sliding.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <setjmp.h> 
#include "gb_flip.h" 
typedef unsigned int uint;
jmp_buf success_point;
int style;
int verbose;
/*6:*/
#line 160 "sliding.w"

int board[boardsize];
int aboard[boardsize];
int rows;
int cols;
int colsp;
int ul,lr;
int delta[4]= {1,-1};

/*:6*//*8:*/
#line 207 "sliding.w"

int off[maxsize];
int offstart[16];
int curo;
char buf[bufsize];

/*:8*//*11:*/
#line 267 "sliding.w"

int bcount;
int piece[maxsize],apiece[maxsize];
int place[maxsize],aplace[maxsize];


/*:11*//*18:*/
#line 400 "sliding.w"

int balance[16];

/*:18*//*21:*/
#line 463 "sliding.w"

char xboard[boardsize];
uint config[maxsize/8];

/*:21*//*25:*/
#line 523 "sliding.w"

short uni[4][256];
uint hash[hashsize];
uint hashh[hashsize];

/*:25*//*27:*/
#line 564 "sliding.w"

uint pos[memsize+maxsize/8+1];
uint cutoff;
uint cutoffh;
uint curpos;
uint curposh;
uint source;
uint sourceh;
uint nextsource,nextsourceh;
uint maxpos;
uint maxposh;
uint configs;
uint configsh;
uint oldconfigs;
uint milestone[maxmoves];
uint milestoneh[maxmoves];
uint shortcut;
int goalhash;
uint goal[maxsize/8];
uint start[maxsize/8];

/*:27*//*49:*/
#line 956 "sliding.w"

int head[maxsize+1],out[maxsize+1],in[maxsize+1];
int link[boardsize],olink[boardsize],ilink[boardsize];

/*:49*//*53:*/
#line 1043 "sliding.w"

int perm[maxsize+1],iperm[maxsize+1];
char decision[maxsize];
int inx[maxsize],lstart[maxsize];
int super[maxsize];

/*:53*/
#line 114 "sliding.w"

/*10:*/
#line 236 "sliding.w"

int fill_board(int board[],int piece[],int place[])
{
register int j,c,k,t;
register char*p;
for(j= 0;j<ul;j++)board[j]= bdry;
for(j= ul;j<=lr;j++)board[j]= -1;
for(j= ul+cols;j<=lr;j+= colsp)board[j]= bdry;
for(;j<=lr+colsp;j++)board[j]= bdry;
for(p= &buf[0],j= ul,bcount= c= 0;*p!='\n';p++){
while(board[j]>=0)if(++j> lr)return-1;
if(*p=='0')board[j]= t= 0;
else if(*p>='1'&&*p<='9')t= *p-'0';
else if(*p>='a'&&*p<='f')t= *p-('a'-10);
else if(*p=='x')t= 0,board[j]= obst;
else return-2;
if(t){
bcount++;
piece[bcount]= t;
place[bcount]= j;
board[j]= bcount;
for(k= offstart[t];off[k];k++)
if(j+off[k]<ul||j+off[k]> lr||board[j+off[k]]>=0)c++;
else board[j+off[k]]= bcount;
}
j++;
}
for(;j<=lr;j++)if(board[j]<0)board[j]= 0;
return c;
}

/*:10*//*12:*/
#line 292 "sliding.w"

void print_board(int board[],int piece[])
{
register int j,k;
for(j= 0;j<rows;j++){
for(k= 0;k<cols;k++)
printf(" %c",
cell(j,k)==cell(j-1,k)&&cell(j,k)&&cell(j,k)<obst?'|':' ');
printf("\n");
for(k= 0;k<cols;k++)
if(cell(j,k)<0)printf(" ?");
else if(cell(j,k)<obst)printf("%c%x",
cell(j,k)==cell(j,k-1)&&cell(j,k)&&cell(j,k)<obst?'-':' ',
piece[cell(j,k)]);
else printf("  ");
printf("\n");
}
}

/*:12*//*20:*/
#line 445 "sliding.w"

int pack(int board[],int piece[])
{
register int i,j,k,p,s,t;
for(j= ul;j<=lr;j++)xboard[j]= 0;
for(i= s= 0,p= 28,j= ul,t= bcount;t;j++)if(board[j]<obst&&!xboard[j]){
k= piece[board[j]];
if(k){
t--,s+= k<<p;
for(k= offstart[k];off[k];k++)xboard[j+off[k]]= 1;
}
if(!p)config[i++]= s,s= 0,p= 28;
else p-= 4;
}
if(p!=28)config[i++]= s;
return i;
}

/*:20*//*22:*/
#line 467 "sliding.w"

void print_config(uint config[],int n)
{
register int j,t;
for(j= 0;j<n-1;j++)printf("%08x",config[j]);
for(t= config[n-1],j= 8;(t&0xf)==0;j--)t>>= 4;
printf("%0*x",j,t);
}

/*:22*//*23:*/
#line 478 "sliding.w"

int unpack(int board[],int piece[],int place[],uint config[])
{
register int i,j,k,p,s,t;
for(j= ul;j<=lr;j++)xboard[j]= 0;
for(p= i= 0,j= ul,t= bcount;t;j++)
if(board[j]<obst&&!xboard[j]){
if(!p)s= config[i++],p= 28;
else p-= 4;
k= (s>>p)&0xf;
if(k){
board[j]= t,piece[t]= k,place[t]= j;
for(k= offstart[k];off[k];k++)xboard[j+off[k]]= 1,board[j+off[k]]= t;
t--;
}else board[j]= 0;
}
for(;j<=lr;j++)if(board[j]<obst&&!xboard[j])board[j]= 0;
return i;
}

/*:23*//*26:*/
#line 528 "sliding.w"

void print_big(uint hi,uint lo)
{
printf("%.15g",((double)hi)*4294967296.0+(double)lo);
}

void print_bigx(uint hi,uint lo)
{
if(hi)printf("%x%08x",hi,lo);
else printf("%x",lo);
}

/*:26*//*28:*/
#line 591 "sliding.w"

int hashin(int trick)
{
register int h,j,k,n,bound;
n= pack(board,piece);
for(h= hashcode(config[0]),j= 1;j<n;j++)h^= hashcode(config[j]);
h&= hashsize-1;
if(hashh[h]==cutoffh){
if(hash[h]<cutoff)goto newguy;
}else if(hashh[h]<cutoffh)goto newguy;
bound= hash[h]-cutoff;
for(j= hash[h]&(memsize-1);;j= (j-pos[j])&(memsize-1)){
for(k= 0;k<n;k++)if(config[k]!=pos[j+2+k])goto nope;
if(trick)/*44:*/
#line 850 "sliding.w"

{
if(bound<shortcut)return 0;
n= (j-source)%(memsize-1);
if(pos[j+1]==n)return 0;
pos[j+1]= n;
return 1;
}

/*:44*/
#line 604 "sliding.w"
;
return 0;
nope:bound-= pos[j];
if(bound<0)break;
}
newguy:/*31:*/
#line 628 "sliding.w"

j= curpos&(memsize-1);
pos[j]= curpos-hash[h];
if(pos[j]> memsize||curposh> hashh[h]+(pos[j]> curpos))
pos[j]= memsize;
pos[j+1]= curpos-source;
for(k= 0;k<n;k++)pos[j+2+k]= config[k];
hash[h]= curpos,hashh[h]= curposh;
/*32:*/
#line 642 "sliding.w"

if(configs==oldconfigs||verbose> 0){
print_config(config,n);
if(verbose> 0){
printf(" (");
print_big(configsh,configs);
printf("=#");
print_bigx(curposh,curpos);
printf(", from #");
print_bigx(sourceh,source);
printf(")\n");
}
}
configs++;
if(configs==0)configsh++;

/*:32*/
#line 636 "sliding.w"
;
/*33:*/
#line 658 "sliding.w"

curpos+= n+2;
if(curpos<n+2)curposh++;
if((curpos&(memsize-1))<n+2)curpos&= -memsize;
if(curposh==maxposh){
if(curpos<=maxpos)goto okay;
}else if(curposh<maxposh)goto okay;
fprintf(stderr,"Sorry, my memsize isn't big enough for this puzzle.\n");
exit(-13);
okay:

/*:33*/
#line 637 "sliding.w"
;

/*:31*/
#line 609 "sliding.w"
;
if(h==goalhash)/*29:*/
#line 618 "sliding.w"

{
for(k= 0;k<n;k++)if(config[k]!=goal[k])goto not_yet;
longjmp(success_point,1);
not_yet:;
}

/*:29*/
#line 610 "sliding.w"
;
return trick;
}

/*:28*//*43:*/
#line 775 "sliding.w"

void move(int k,int del,int delo)
{
register int j,s,t;
s= place[k],t= piece[k];
for(j= offstart[t];;j++){
board[s+off[j]]= 0;
if(!off[j])break;
}
for(j= offstart[t];;j++){
if(board[s+del+off[j]])goto illegal;
if(!off[j])break;
}
for(j= offstart[t];;j++){
board[s+del+off[j]]= k;
if(!off[j])break;
}
if(hashin(style==2)||style==1)/*45:*/
#line 864 "sliding.w"

{
for(j= offstart[t];;j++){
board[s+del+off[j]]= 0;
if(!off[j])break;
}
for(j= offstart[t];;j++){
board[s+off[j]]= k;
if(!off[j])break;
}
if(style==1)move(k,del+delo,delo);
else for(j= 0;j<4;j++)if(delta[j]!=-delo)move(k,del+delta[j],delta[j]);
}

/*:45*/
#line 792 "sliding.w"

else{
for(j= offstart[t];;j++){
board[s+del+off[j]]= 0;
if(!off[j])break;
}
illegal:for(j= offstart[t];;j++){
board[s+off[j]]= k;
if(!off[j])break;
}
}
}

/*:43*//*50:*/
#line 985 "sliding.w"

void supermove(int,int);
void ideals(int del)
{
register int j,k,l,p,u,v,t;
for(j= 0;j<=bcount;j++)perm[j]= iperm[j]= j;
l= p= 0;
excl:decision[l]= 0,lstart[l]= p;
for(j= inx[l],t= j+1;j<t;j++)
/*51:*/
#line 1013 "sliding.w"

{
v= perm[j];
for(k= in[v];k>=0;k= ilink[k]){
u= aboard[k];
if(iperm[u]>=t){
register int uu= perm[t],tt= iperm[u];
perm[t]= u,perm[tt]= uu,iperm[u]= t,iperm[uu]= tt;
t++;
}
}
if(decision[0]==1)
for(v= head[v];v>=0;v= link[v])super[p++]= v;
}

/*:51*/
#line 994 "sliding.w"
;
if(t> bcount){
/*54:*/
#line 1049 "sliding.w"

if(p){
super[p]= 0;
if(decision[0]==0)supermove(del,del);
else supermove(-del,-del);
}

/*:54*/
#line 996 "sliding.w"
;goto incl;
}
inx[++l]= t;goto excl;
incl:decision[l]= 1,p= lstart[l];
for(j= inx[l],t= j+1;j<t;j++)
/*52:*/
#line 1028 "sliding.w"

{
u= perm[j];
for(k= out[u];k>=0;k= olink[k]){
v= aboard[k];
if(iperm[v]>=t){
register int vv= perm[t],tt= iperm[v];
perm[t]= v,perm[tt]= vv,iperm[v]= t,iperm[vv]= tt;
t++;
}
}
if(decision[0]==0)
for(u= head[u];u>=0;u= link[u])super[p++]= u;
}

/*:52*/
#line 1001 "sliding.w"
;
if(t> bcount){
/*54:*/
#line 1049 "sliding.w"

if(p){
super[p]= 0;
if(decision[0]==0)supermove(del,del);
else supermove(-del,-del);
}

/*:54*/
#line 1003 "sliding.w"
;goto backup;
}
inx[++l]= t;goto excl;
backup:if(l){
l--;
if(decision[l])goto backup;
goto incl;
}
}

/*:50*//*55:*/
#line 1059 "sliding.w"

void supermove(int del,int delo)
{
register int j,s,t;
for(j= 0;super[j];j++){
board[super[j]]= 0;
}
for(j= 0;super[j];j++){
if(board[del+super[j]])goto illegal;
}
for(j= 0;super[j];j++){
board[del+super[j]]= aboard[super[j]];
}
if(hashin(style==5)||style==4)
/*56:*/
#line 1088 "sliding.w"

{
for(j= 0;super[j];j++){
board[del+super[j]]= 0;
}
for(j= 0;super[j];j++){
board[super[j]]= aboard[super[j]];
}
if(style==4)supermove(del+delo,delo);
else for(j= 0;j<4;j++)
if(delta[j]!=-delo)supermove(del+delta[j],delta[j]);
}

/*:56*/
#line 1073 "sliding.w"

else{
for(j= 0;super[j];j++){
board[del+super[j]]= 0;
}
illegal:for(j= 0;super[j];j++){
board[super[j]]= aboard[super[j]];
}
}
}

/*:55*/
#line 115 "sliding.w"

main(int argc,char*argv[])
{
register int j,k,t;
int d;
/*5:*/
#line 131 "sliding.w"

if(!(argc>=2&&sscanf(argv[1],"%d",&style)==1&&
(argc==2||sscanf(argv[2],"%d",&verbose)==1))){
fprintf(stderr,"Usage: %s style [verbose]\n",argv[0]);
exit(-1);
}
if(style<0||style> 5){
fprintf(stderr,
"Sorry, the style should be between 0 and 5, not %d!\n",style);
exit(-2);
}

/*:5*/
#line 120 "sliding.w"
;
/*13:*/
#line 314 "sliding.w"

/*14:*/
#line 320 "sliding.w"

fgets(buf,bufsize,stdin);
if(sscanf(buf,"%d x %d",&rows,&cols)!=2||rows<=0||cols<=0){
fprintf(stderr,"Bad specification of rows x cols!\n");
exit(-6);
}
if(rows*cols> maxsize)boardover();
colsp= cols+1;
delta[2]= colsp,delta[3]= -colsp;
ul= colsp;
lr= (rows+1)*colsp-2;

/*:14*/
#line 315 "sliding.w"
;
/*15:*/
#line 332 "sliding.w"

for(j= 1;j<16;j++)offstart[j]= -1;
while(1){
if(!fgets(buf,bufsize,stdin)){
buf[0]= '\n';break;
}
if(buf[0]=='\n')continue;
if(buf[1]!=' '||buf[2]!='='||buf[3]!=' ')break;
if(buf[0]>='1'&&buf[0]<='9')t= buf[0]-'0';
else if(buf[0]>='a'&&buf[0]<='f')t= buf[0]-('a'-10);
else{
printf("Bad piece name (%c)!\n",buf[0]);
exit(-7);
}
if(offstart[t]>=0)
printf("Warning: Redefinition of piece %c is being ignored.\n",buf[0]);
else{
offstart[t]= curo;
/*7:*/
#line 184 "sliding.w"

{
register char*p;
for(t= -1,j= k= 0,p= &buf[4];;p++)
switch(*p){
case'1':if(t<0)t= k;else off[curo++]= k-t;
if(curo>=maxsize)boardover();
case'0':j++,k++;break;
case'/':k+= colsp-j,j= 0;break;
case'\n':goto offsets_done;
default:fprintf(stderr,
"Bad character `%c' in definition of piece %c!\n",*p,buf[0]);
exit(-4);
}
offsets_done:if(t<0){
fprintf(stderr,"Piece %c is empty!\n",buf[0]);exit(-5);
}
off[curo++]= 0;
if(curo>=maxsize)boardover();
}

/*:7*/
#line 350 "sliding.w"
;
}
}

/*:15*/
#line 316 "sliding.w"
;
/*16:*/
#line 354 "sliding.w"

t= fill_board(board,piece,place);
printf("Starting configuration:\n");
print_board(board,piece);
if(t){
if(t> 0)
if(t==1)fprintf(stderr,"Oops, you filled a cell twice!\n");
else fprintf(stderr,"Oops, you overfilled %d cells!\n",t);
else fprintf(stderr,"Oops, %s!\n",
t==-1?"your board wasn't big enough":
"the configuration contains an illegal character");
exit(-8);
}
if(bcount==0){
fprintf(stderr,"The puzzle doesn't have any pieces!\n");
exit(-9);
}

/*:16*/
#line 317 "sliding.w"
;
/*17:*/
#line 372 "sliding.w"

fgets(buf,bufsize,stdin);
t= fill_board(aboard,apiece,aplace);
printf("\nStopping configuration:\n");
print_board(aboard,apiece);
if(t){
if(t> 0)
if(t==1)fprintf(stderr,"Oops, you filled a cell twice!\n");
else fprintf(stderr,"Oops, you overfilled %d cells!\n",t);
else fprintf(stderr,"Oops, %s!\n",
t==-1?"your board wasn't big enough":
"the configuration contains an illegal character");
exit(-10);
}
for(j= 0;j<16;j++)balance[j]= 0;
for(j= ul;j<=lr;j++){
if((board[j]<obst)!=(aboard[j]<obst)){
fprintf(stderr,"The dead cells (x's) are in different places!\n");
exit(-11);
}
if(board[j]<obst)
balance[piece[board[j]]]++,balance[apiece[aboard[j]]]--;
}
for(j= 0;j<16;j++)if(balance[j]){
fprintf(stderr,"Wrong number of pieces in the stopping configuration!\n");
exit(-12);
}

/*:17*/
#line 318 "sliding.w"
;

/*:13*/
#line 121 "sliding.w"
;
/*24:*/
#line 501 "sliding.w"

gb_init_rand(0);
for(j= 0;j<4;j++)for(k= 1;k<256;k++)uni[j][k]= gb_next_rand();

/*:24*//*30:*/
#line 625 "sliding.w"

if(setjmp(success_point))goto hurray;

/*:30*/
#line 122 "sliding.w"
;
/*34:*/
#line 672 "sliding.w"

printf("\n(using moves of style %d)\n",style);
/*36:*/
#line 696 "sliding.w"

t= pack(board,piece);
for(k= 0;k<t;k++)start[k]= config[k];

/*:36*/
#line 674 "sliding.w"
;
restart:/*35:*/
#line 688 "sliding.w"

t= pack(aboard,apiece);
for(k= goalhash= 0;k<t;k++)goal[k]= config[k],goalhash^= hashcode(config[k]);
goalhash&= hashsize-1;

/*:35*/
#line 675 "sliding.w"
;
/*37:*/
#line 700 "sliding.w"

curpos= cutoff= milestone[0]= 1,curposh= cutoffh= milestoneh[0]= 0;
source= sourceh= configs= configsh= oldconfigs= d= 0;
maxposh= 1;
printf("*** Distance 0:\n");
hashin(0);
if(verbose<=0)printf(".\n");

/*:37*/
#line 676 "sliding.w"
;
for(d= 1;d<maxmoves;d++){
printf("*** Distance %d:\n",d);
milestone[d]= curpos,milestoneh[d]= curposh;
oldconfigs= configs;
/*38:*/
#line 708 "sliding.w"

if(d> 1)cutoff= milestone[d-2],cutoffh= milestoneh[d-2];
shortcut= curpos-cutoff;
maxpos= cutoff+memsize,maxposh= cutoffh+(maxpos<memsize);
for(source= milestone[d-1],sourceh= milestoneh[d-1];
source!=milestone[d]||sourceh!=milestoneh[d];
source= nextsource,sourceh= nextsourceh){
j= unpack(board,piece,place,&pos[(source&(memsize-1))+2])+2;
nextsource= source+j,nextsourceh= sourceh+(nextsource<j);
if((nextsource&(memsize-1))<j)nextsource&= -memsize;
/*42:*/
#line 765 "sliding.w"

if(style<3)
for(j= 0;j<4;j++)
for(k= 1;k<=bcount;k++)move(k,delta[j],delta[j]);
else/*57:*/
#line 1104 "sliding.w"

{
/*46:*/
#line 894 "sliding.w"

for(j= 0;j<=bcount;j++)head[j]= -1;
for(j= 0;j<=lr+colsp;j++){
k= board[j];
if(k){
if(k>=obst)k= 0;
aboard[j]= k;
link[j]= head[k];
head[k]= j;
}else aboard[j]= -1;
}

/*:46*/
#line 1106 "sliding.w"
;
/*48:*/
#line 945 "sliding.w"

for(j= 0;j<=bcount;j++)out[j]= in[j]= -1;
for(j= 0;j<=bcount;j++)
for(k= head[j];k>=ul;k= link[k]){
t= aboard[k-colsp];
if(t!=j&&t>=0&&(out[t]<0||aboard[out[t]]!=j)){
olink[k]= out[t],out[t]= k;
ilink[k-colsp]= in[j],in[j]= k-colsp;
}
}

/*:48*/
#line 1107 "sliding.w"
;
ideals(colsp);
head[0]= lr+1;
/*47:*/
#line 931 "sliding.w"

for(j= 0;j<=bcount;j++)out[j]= in[j]= -1;
for(j= 0;j<=bcount;j++)
for(k= head[j];k>=ul;k= link[k]){
t= aboard[k-1];
if(t!=j&&t>=0&&(out[t]<0||aboard[out[t]]!=j)){
olink[k]= out[t],out[t]= k;
ilink[k-1]= in[j],in[j]= k-1;
}
}

/*:47*/
#line 1110 "sliding.w"
;
ideals(1);
}

/*:57*/
#line 769 "sliding.w"
;

/*:42*/
#line 718 "sliding.w"
;
}

/*:38*/
#line 681 "sliding.w"
;
if(configs==oldconfigs)exit(0);
if(verbose<=0)printf(" and %d more.\n",configs-oldconfigs-1);
}
printf("No solution found yet (maxmoves=%d)!\n",maxmoves);
exit(0);

/*:34*/
#line 123 "sliding.w"
;
hurray:/*39:*/
#line 723 "sliding.w"

if(d==0){
printf("\nYou're joking: That puzzle is solved in zero moves!\n");
exit(0);
}
printf("... Solution!\n");
if(verbose<0)exit(0);
/*40:*/
#line 737 "sliding.w"

if(curposh||curpos> memsize){
maxpos= curpos-memsize;
maxposh= curposh-(maxpos> curpos);
}else maxpos= maxposh= 0;
for(j= 0;j<=lr+colsp;j++)aboard[j]= board[j];
while(sourceh> maxposh||(sourceh==maxposh&&source>=maxpos)){
d--;
if(d==0)exit(0);
printf("\n%d:\n",d);
k= source&(memsize-1);
unpack(aboard,apiece,place,&pos[k+2]);
print_board(aboard,apiece);
if(source<pos[k+1])sourceh--;
source= source-pos[k+1];
}

/*:40*/
#line 730 "sliding.w"
;
/*41:*/
#line 754 "sliding.w"

printf("(Unfortunately I've forgotten how to get to level %d,\n",d);
printf(" so I'll have to reconstruct that part. Please bear with me.)\n");
for(j= 0;j<hashsize;j++)hash[j]= hashh[j]= 0;
unpack(board,piece,place,start);
goto restart;

/*:41*/
#line 732 "sliding.w"
;

/*:39*/
#line 124 "sliding.w"
;
}

/*:4*/
