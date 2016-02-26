#define maxm 7
#define maxmmmm (maxm*maxm*maxm*maxm) 
#define maxclasses ((maxmmmm-maxm*maxm) /4) 
#define ustacksize 1000000
#define o mems++
#define oo mems+= 2 \

#define green 2
#define red 0
#define blue 1 \

#define p1(x) (o,hexconv[(x) &0xf000]) 
#define p2(x) (o,hexconv[(x) &0xff00]) 
#define p3(x) (o,hexconv[(x) &0xfff0]) 
#define s1(x) (o,hexconv[((x) &0x000f) <<12]) 
#define s2(x) (o,hexconv[((x) &0x00ff) <<8]) 
#define s3(x) (o,hexconv[((x) &0x0fff) <<4])  \

#define insert(x,alpha,del,off) { \
ploc= (del) +(off) ; \
o,p= mem[ploc+mmmm]; \
o,mem[p]= x; \
o,mem[(off) -mmmm+alpha]= p; \
o,mem[ploc+mmmm]= p+1; \
} \

#define sanity_checking 0 \

#define bluecheck(loc,off) { \
for(p= loc+off,q= mem[loc+mmmm];p<q;p++) { \
x= mem[p],alpha= hexconv[x]; \
if(mem[alpha]!=blue) fprintf(stderr," %04x in list %d is %s!\n", \
x,loc+off,mem[alpha]==green?"green":"red") ; \
if(mem[off-mmmm+alpha]!=p)  \
fprintf(stderr, \
" inverse index for %04x in list %d should be %d, not %d!\n", \
x,loc+off,p,mem[off-mmmm+alpha]) ; \
} \
} \

#define appcheck(x,alpha,del,off) { \
p= (del) +(off) ; \
q= mem[p+mmmm]; \
if(q!=p-1) { \
if(mem[alpha]==green) { \
fprintf(stderr," list ") ;decode_list(p) ; \
fprintf(stderr," for %x isn't closed!\n", \
x) ; \
}else{ \
r= mem[off-mmmm+alpha]; \
if(r<p||r>=q) { \
fprintf(stderr," %x isn't in list ", \
x) ; \
decode_list(p) ;fprintf(stderr,"!\n") ; \
} \
} \
} \
} \

#define sthresh 1 \

#define closelist(del,off) { \
p= (del) +(off) ; \
o,q= mem[p+mmmm]; \
if(q!=p-1) store(p+mmmm,p-1) ; \
} \

#define rem(alpha,del,off) { \
p= (del) +(off) ; \
o,q= mem[p+mmmm]-1; \
if(q>=p) { \
store(p+mmmm,q) ; \
o,t= mem[(off) -mmmm+alpha]; \
if(t!=q) { \
o,y= mem[q]; \
store(t,y) ; \
o,store((off) -mmmm+hexconv[y],t) ; \
} \
} \
} \

/*1:*/
#line 45 "./back-commafree4.w"

#include <stdio.h> 
#include <stdlib.h> 
int m,goal;
typedef unsigned long long ull;
/*28:*/
#line 507 "./back-commafree4.w"

typedef struct frame_struct{
int cls;
int x;
int slack;
int uptr;
}frame;

/*:28*/
#line 50 "./back-commafree4.w"
;
ull mems;
ull nodes;
ull thresh= 10000000000;
ull profile[maxclasses+3];
int count;
int maxstack,maxpoison;
int vbose;
/*6:*/
#line 134 "./back-commafree4.w"

unsigned mem[1<<16];
unsigned int undo[ustacksize];
int uptr;
unsigned int stamp[1<<16];
unsigned int curstamp;

/*:6*//*9:*/
#line 177 "./back-commafree4.w"

int hexconv[1<<16];
int mconv[maxmmmm];

/*:9*//*14:*/
#line 268 "./back-commafree4.w"

int mmmm;
int maxl;
int p1offset,p2offset,p3offset,s1offset,s2offset,s3offset,cloffset;
char aa[5];
int cl[maxmmmm];
int clrep[maxclasses];
int classes;

/*:14*//*24:*/
#line 479 "./back-commafree4.w"

int poison;

/*:24*//*29:*/
#line 521 "./back-commafree4.w"

frame move[maxclasses+1];
int clfree[maxclasses];
int freeptr;
int ifree[maxclasses];

/*:29*/
#line 58 "./back-commafree4.w"
;
/*4:*/
#line 91 "./back-commafree4.w"

void confusion(char*s){
fprintf(stderr,"I'm confused (%s)!\n",s);
exit(-666);
}

/*:4*//*5:*/
#line 110 "./back-commafree4.w"

inline void store(register short a,register short x){
if(o,stamp[a]!=curstamp){
o,stamp[a]= curstamp;
oo,undo[uptr++]= (a<<16)+mem[a];
if(uptr> maxstack){
maxstack= uptr;
if(maxstack>=ustacksize){
fprintf(stderr,"Stack overflow (ustacksize=%d)!\n",ustacksize);
exit(-9);
}
}
}
o,mem[a]= x;
}

inline void unstore(register int ptr){
register unsigned int x;
while(uptr> ptr){
o,x= undo[--uptr];
o,mem[x>>16]= x&0xffff;
}
}

/*:5*//*18:*/
#line 330 "./back-commafree4.w"

void decode_list(int p){
register int x,alpha;
alpha= p%mmmm,x= mconv[alpha];
switch(p/mmmm){
case 2:fprintf(stderr,"%x...",x>>12);break;
case 5:fprintf(stderr,"%02x..",x>>8);break;
case 8:fprintf(stderr,"%03x.",x>>4);break;
case 11:fprintf(stderr,"...%x",x>>12);break;
case 14:fprintf(stderr,"..%02x",x>>8);break;
case 17:fprintf(stderr,".%03x",x>>4);break;
case 20:fprintf(stderr,"[%04x]",clrep[alpha/4]);break;
default:fprintf(stderr,"???");
}
}

/*:18*//*19:*/
#line 353 "./back-commafree4.w"

void print_list(int p){
register int q,r;
q= mem[p+mmmm];
fprintf(stderr,"List ");
decode_list(p);
fprintf(stderr,":");
if(q<p)fprintf(stderr," .");
else for(r= p;r<q;r++)fprintf(stderr," %04x",mem[r]);
fprintf(stderr,"\n");
}

/*:19*//*20:*/
#line 365 "./back-commafree4.w"

void print_all_lists(void){
register int a,b,c,k;
for(a= 0;a<m;a++){
print_list(p1offset+a*m*m*m);
print_list(s1offset+a*m*m*m);
for(b= 0;b<m;b++){
print_list(p2offset+(a*m+b)*m*m);
print_list(s2offset+(a*m+b)*m*m);
for(c= 0;c<m;c++){
print_list(p3offset+((a*m+b)*m+c)*m);
print_list(s3offset+((a*m+b)*m+c)*m);
}
}
}
for(k= 0;k<maxl;k++)print_list(cloffset+4*k);
}

/*:20*//*21:*/
#line 390 "./back-commafree4.w"

void sanity(void){
register int a,b,c,j,k,p,q,r,x,alpha;
/*22:*/
#line 411 "./back-commafree4.w"

for(a= 0;a<m;a++){
bluecheck(a*m*m*m,p1offset);
bluecheck(a*m*m*m,s1offset);
for(b= 0;b<m;b++){
bluecheck((a*m+b)*m*m,p2offset);
bluecheck((a*m+b)*m*m,s2offset);
for(c= 0;c<m;c++){
bluecheck(((a*m+b)*m+c)*m,p3offset);
bluecheck(((a*m+b)*m+c)*m,s3offset);
}
}
}
for(k= 0;k<maxl;k++)bluecheck(4*k,cloffset);

/*:22*/
#line 393 "./back-commafree4.w"
;
/*23:*/
#line 448 "./back-commafree4.w"

for(alpha= 0;alpha<mmmm;alpha++)if(mem[alpha]!=red){
x= mconv[alpha];
appcheck(x,alpha,p1(x),p1offset);
appcheck(x,alpha,p2(x),p2offset);
appcheck(x,alpha,p3(x),p3offset);
appcheck(x,alpha,s1(x),s1offset);
appcheck(x,alpha,s2(x),s2offset);
appcheck(x,alpha,s3(x),s3offset);
appcheck(x,alpha,4*cl[alpha],cloffset);
}

/*:23*/
#line 394 "./back-commafree4.w"
;
}

/*:21*//*31:*/
#line 530 "./back-commafree4.w"

void print_moves(int lev){
register int l;
for(l= 1;l<=lev;l++)
if(move[l].x>=0)printf(" %04x",move[l].x);
else printf(" (%04x)",clrep[move[l].cls]);
printf("\n");
}

/*:31*//*42:*/
#line 835 "./back-commafree4.w"

inline void redden(register int x){
register int alpha,p,q,t,y;
if(vbose> 2)fprintf(stderr," red %04x\n",
x);
o,alpha= hexconv[x];
if(mem[alpha]!=blue)confusion("redden");
store(alpha,red);
rem(alpha,p1(x),p1offset);
rem(alpha,p2(x),p2offset);
rem(alpha,p3(x),p3offset);
rem(alpha,s1(x),s1offset);
rem(alpha,s2(x),s2offset);
rem(alpha,s3(x),s3offset);
o,t= cl[alpha];
rem(alpha,4*t,cloffset);
}

/*:42*/
#line 59 "./back-commafree4.w"
;
main(int argc,char*argv[]){
register int a,b,c,d,i,j,k,l,p,q,r,s,pp,x,y,yy,z,zz,alpha,ploc,cls,slack;
/*3:*/
#line 79 "./back-commafree4.w"

if(argc<3||sscanf(argv[1],"%d",&m)!=1||sscanf(argv[2],"%d",&goal)!=1){
fprintf(stderr,
"Usage: %s m goal [profile] [verbose] [extraverbose]\n",argv[0]);
exit(-1);
}
if(m<2||m> maxm){
fprintf(stderr,"m should be between 2 and %d, not %d!\n",maxm,m);
exit(-2);
}
vbose= argc-3;

/*:3*/
#line 62 "./back-commafree4.w"
;
/*10:*/
#line 181 "./back-commafree4.w"

for(a= 0;a<m;a++)for(b= 0;b<m;b++)for(c= 0;c<m;c++)for(d= 0;d<m;d++)
o,hexconv[(a<<12)+(b<<8)+(c<<4)+d]= ((a*m+b)*m+c)*m+d,
mconv[((a*m+b)*m+c)*m+d]= (a<<12)+(b<<8)+(c<<4)+d;


/*:10*//*17:*/
#line 312 "./back-commafree4.w"

mmmm= m*m*m*m;
for(k= 0;k<mmmm;k++)o,mem[k]= red;
maxl= (mmmm-m*m)>>2;
if(goal<maxl-m*(m-1)||goal> maxl){
fprintf(stderr,"The goal should be between %d and %d, not %d!\n",
maxl-m*(m-1),maxl,goal);
exit(-3);
}
p1offset= 2*mmmm,p2offset= 5*mmmm,p3offset= 8*mmmm;
s1offset= 11*mmmm,s2offset= 14*mmmm,s3offset= 17*mmmm;
cloffset= 20*mmmm;
/*12:*/
#line 223 "./back-commafree4.w"

for(a= 0;a<m;a++){
o,mem[p1offset+mmmm+a*m*m*m]= p1offset+a*m*m*m;
o,mem[s1offset+mmmm+a*m*m*m]= s1offset+a*m*m*m;
for(b= 0;b<m;b++){
o,mem[p2offset+mmmm+(a*m+b)*m*m]= p2offset+(a*m+b)*m*m;
o,mem[s2offset+mmmm+(a*m+b)*m*m]= s2offset+(a*m+b)*m*m;
for(c= 0;c<m;c++){
o,mem[p3offset+mmmm+((a*m+b)*m+c)*m]= p3offset+((a*m+b)*m+c)*m;
o,mem[s3offset+mmmm+((a*m+b)*m+c)*m]= s3offset+((a*m+b)*m+c)*m;
}
}
}

/*:12*/
#line 324 "./back-commafree4.w"
;
/*15:*/
#line 279 "./back-commafree4.w"

f1:o,aa[0]= -1,j= 1,classes= 0;
f2:if(j==4)/*16:*/
#line 290 "./back-commafree4.w"

{
for(i= 0;i<4;i++){
for(x= 0,k= 0;k<4;k++)
x= (x<<4)+aa[1+((i+k)%4)];
o,alpha= hexconv[x];
if(i==0)clrep[classes]= x;
if(x!=0x0100&&x!=0x1000){
o,mem[alpha]= blue;
o,mem[i+4*classes+cloffset]= x;
o,mem[cloffset-mmmm+alpha]= i+4*classes+cloffset;
/*13:*/
#line 250 "./back-commafree4.w"

insert(x,alpha,p1(x),p1offset);
insert(x,alpha,p2(x),p2offset);
insert(x,alpha,p3(x),p3offset);
insert(x,alpha,s1(x),s1offset);
insert(x,alpha,s2(x),s2offset);
insert(x,alpha,s3(x),s3offset);

/*:13*/
#line 301 "./back-commafree4.w"
;
}
o,cl[alpha]= classes;
}
o,mem[cloffset+mmmm+4*classes]= cloffset+4*classes+(classes?4:2);
classes++;
}

/*:16*/
#line 281 "./back-commafree4.w"
;
f3:for(j= 4;o,aa[j]==m-1;j--);
f4:if(j){
o,aa[j]++;
f5:for(k= j+1;k<=4;k++)oo,aa[k]= aa[k-j];
goto f2;
}
if(classes!=maxl)confusion("classes");

/*:15*/
#line 325 "./back-commafree4.w"
;

/*:17*//*25:*/
#line 482 "./back-commafree4.w"

poison= 22*mmmm+1;
mem[poison-1]= poison;

/*:25*//*30:*/
#line 527 "./back-commafree4.w"

for(k= 0,freeptr= maxl;k<maxl;k++)oo,clfree[k]= ifree[k]= k;

/*:30*/
#line 63 "./back-commafree4.w"
;
/*32:*/
#line 541 "./back-commafree4.w"

l= 1;
cls= 0,x= 0x0001,slack= maxl-goal;
nodes= profile[1]= 1;
uptr= 0;
goto tryit;
enter:profile[l]++,nodes++;
/*33:*/
#line 581 "./back-commafree4.w"

if(mems>=thresh){
thresh+= 10000000000;
fprintf(stderr,"After %lld mems:",mems);
for(k= 2;k<=l;k++)fprintf(stderr," %lld",profile[k]);
fprintf(stderr,"\n");
}

/*:33*/
#line 548 "./back-commafree4.w"
;
if(l> maxl)/*46:*/
#line 872 "./back-commafree4.w"

{
count++;
printf("%d:",
count);
print_moves(maxl);
goto backup;
}

/*:46*/
#line 549 "./back-commafree4.w"
;
choose:if(sanity_checking)sanity();
/*34:*/
#line 607 "./back-commafree4.w"

/*35:*/
#line 620 "./back-commafree4.w"

if(vbose> 3)fprintf(stderr," class sizes");
for(r= 5,k= 0;k<freeptr;k++){
c= clfree[k];
o,j= mem[4*c+cloffset+mmmm]-(4*c+cloffset);
if(vbose> 3)fprintf(stderr," %04x:%d",
clrep[c],j);
if(j<r){
r= j,cls= c;
if(r==0)break;
}
}
if(vbose> 3)fprintf(stderr,"\n");

/*:35*/
#line 608 "./back-commafree4.w"
;
if(r> 1){
/*40:*/
#line 736 "./back-commafree4.w"

s= 0;
if(vbose> 3)fprintf(stderr," kills");
o,pp= mem[poison-1];
for(p= poison;p<pp;){
o,y= mem[p],z= mem[p+1];
o,yy= mem[y+mmmm];
if(yy==y)goto deleet;
o,zz= mem[z+mmmm];
if(zz==z)goto deleet;
if(yy<y||zz<z)confusion("poison remnant");
if(vbose> 3){
fprintf(stderr," %d",
yy-y);
decode_list(y);
fprintf(stderr,"|");
decode_list(z);
fprintf(stderr,"%d",
zz-z);
}
if(yy-y>=zz-z){
if(yy-y> s)o,s= yy-y,x= mem[z];
}else if(zz-z> s)o,s= zz-z,x= mem[y];
p+= 2;
continue;
deleet:/*26:*/
#line 489 "./back-commafree4.w"

pp-= 2;
if(p!=pp){
o,store(p,mem[pp]);
store(p+1,mem[pp+1]);
}

/*:26*/
#line 761 "./back-commafree4.w"
;
}
if(vbose> 3)fprintf(stderr,"\n");
store(poison-1,pp);

/*:40*/
#line 610 "./back-commafree4.w"
;
if(s>=sthresh){
oo,cls= cl[hexconv[x]];
goto done;
}
}
if(r==0)x= -1;
else o,x= mem[4*cls+cloffset];
done:

/*:34*/
#line 551 "./back-commafree4.w"
;
tryit:if(vbose> 1){
if(x>=0)fprintf(stderr,"Level %d, trying %04x (%lld mems)\n",
l,x,mems);
else fprintf(stderr,"Level %d, trying (%04x) (%lld mems)\n",
l,clrep[cls],mems);
}
o,move[l].uptr= uptr;/*7:*/
#line 154 "./back-commafree4.w"

if(++curstamp==0){
for(k= 0;k<(1<<16);k++)o,stamp[k]= 0;
curstamp= 1;
}

/*:7*/
#line 558 "./back-commafree4.w"
;
if(x>=0)/*36:*/
#line 637 "./back-commafree4.w"

{
/*37:*/
#line 652 "./back-commafree4.w"

o,alpha= hexconv[x];
store(alpha,green);
closelist(p1(x),p1offset);
closelist(p2(x),p2offset);
closelist(p3(x),p3offset);
closelist(s1(x),s1offset);
closelist(s2(x),s2offset);
closelist(s3(x),s3offset);
o,k= cl[alpha];
closelist(4*k,cloffset);
for(r= cloffset+4*k;r<q;r++)if(o,mem[r]!=x)redden(mem[r]);

/*:37*/
#line 639 "./back-commafree4.w"
;
o,pp= mem[poison-1];
/*38:*/
#line 665 "./back-commafree4.w"

y= p1(x)+s1offset;
z= s3(x)+p3offset;
/*27:*/
#line 496 "./back-commafree4.w"

store(pp,y),store(pp+1,z),mems--;
pp+= 2;
if(pp> maxpoison)maxpoison= pp;

/*:27*/
#line 668 "./back-commafree4.w"
;
y= p2(x)+s2offset;
z= s2(x)+p2offset;
/*27:*/
#line 496 "./back-commafree4.w"

store(pp,y),store(pp+1,z),mems--;
pp+= 2;
if(pp> maxpoison)maxpoison= pp;

/*:27*/
#line 671 "./back-commafree4.w"
;
y= p3(x)+s3offset;
z= s1(x)+p1offset;
/*27:*/
#line 496 "./back-commafree4.w"

store(pp,y),store(pp+1,z),mems--;
pp+= 2;
if(pp> maxpoison)maxpoison= pp;

/*:27*/
#line 674 "./back-commafree4.w"
;

/*:38*/
#line 641 "./back-commafree4.w"
;
/*39:*/
#line 690 "./back-commafree4.w"

for(p= poison;p<pp;){
o,y= mem[p],z= mem[p+1];
o,yy= mem[y+mmmm];
if(yy==y)goto delete;
o,zz= mem[z+mmmm];
if(zz==z)goto delete;
if(yy<y&&zz<z)goto try_again;
if(yy> y&&zz> z){
p+= 2;continue;
}
if(yy<y){
if(vbose> 3){
fprintf(stderr," killing ");decode_list(z);fprintf(stderr,"\n");
}
store(z+mmmm,z);
for(r= z;r<zz;r++)o,redden(mem[r]);
}else{
if(vbose> 3){
fprintf(stderr," killing ");decode_list(y);fprintf(stderr,"\n");
}
store(y+mmmm,y);
for(r= y;r<yy;r++)o,redden(mem[r]);
}
delete:/*26:*/
#line 489 "./back-commafree4.w"

pp-= 2;
if(p!=pp){
o,store(p,mem[pp]);
store(p+1,mem[pp+1]);
}

/*:26*/
#line 714 "./back-commafree4.w"
;
}

/*:39*/
#line 642 "./back-commafree4.w"
;
store(poison-1,pp);
}

/*:36*/
#line 559 "./back-commafree4.w"

else if(slack==0||l==1)goto backup;
else slack--;
o,move[l].x= x,move[l].cls= cls;
o,move[l].slack= slack;
/*44:*/
#line 856 "./back-commafree4.w"

freeptr--;
o,p= ifree[cls];
if(p!=freeptr){
o,y= clfree[freeptr];
o,clfree[p]= y;
o,ifree[y]= p;
o,clfree[freeptr]= cls;
o,ifree[cls]= freeptr;
}

/*:44*/
#line 564 "./back-commafree4.w"
;
l++;goto enter;
try_again:o,unstore(move[l].uptr);
if(vbose> 1)
fprintf(stderr,"Level %d, forbidding %04x (%lld mems)\n",
l,x,mems);
/*7:*/
#line 154 "./back-commafree4.w"

if(++curstamp==0){
for(k= 0;k<(1<<16);k++)o,stamp[k]= 0;
curstamp= 1;
}

/*:7*/
#line 570 "./back-commafree4.w"
;
/*43:*/
#line 853 "./back-commafree4.w"

redden(x);

/*:43*/
#line 571 "./back-commafree4.w"
;
goto choose;
backup:if(--l){
o,x= move[l].x,cls= move[l].cls;
/*45:*/
#line 867 "./back-commafree4.w"

freeptr++;

/*:45*/
#line 575 "./back-commafree4.w"
;
if(x<0)goto backup;
slack= move[l].slack;
goto try_again;
}

/*:32*/
#line 64 "./back-commafree4.w"
;
fprintf(stderr,
"Altogether %d solutions (%lld mems, %lld nodes)\n",
count,mems,nodes);
fprintf(stderr,"(maxstack=%d,maxpoison=%d)\n",
maxstack,(maxpoison-poison)/2);
if(vbose)/*2:*/
#line 73 "./back-commafree4.w"

{
fprintf(stderr,"Profile:          1\n");
for(k= 2;k<=maxl+1;k++)fprintf(stderr,"%19lld\n",profile[k]);
}

/*:2*/
#line 70 "./back-commafree4.w"
;
}

/*:1*/
