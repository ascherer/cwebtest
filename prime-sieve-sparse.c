#define del ((long long) (1<<23) ) 
#define qsize (1<<6) 
#define kmax 35000000
#define ksmall 156000
#define bestgap 1000
#define lsize (1<<21)  \

#define ddel (del/4)  \

#define KK 100
#define LL 37
#define MM (1L<<30) 
#define mod_diff(x,y) (((x) -(y) ) &(MM-1) )  \

#define QUALITY 1009
#define TT 70
#define is_odd(x) ((x) &1)  \

#define ran_arr_next() (*ran_arr_ptr>=0?*ran_arr_ptr++:ran_arr_cycle() )  \

#define magic ((3LL*5LL*7LL*11LL*13LL*17LL*19LL*23LL*29LL*31LL*37LL*41LL*43LL \
*47LL*(unsigned long long) 53) >>1)  \

/*2:*/
#line 102 "./prime-sieve-sparse.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
FILE*infile,*outfile;
unsigned int prime[kmax];
unsigned int start[ksmall];
struct{
unsigned int p;
unsigned int s;
}list[qsize][lsize];
int count[qsize];
int countmax;
unsigned long long sieve[2+del/256];
unsigned long long s0;
int tt;
unsigned long long st;
unsigned long long lastprime;
unsigned long long sv[11];
int rem[11];
char nu[0x10000];
int timer,starttime;
/*22:*/
#line 499 "./prime-sieve-sparse.w"

long ran_x[KK];
void ran_array(long aa[],int n)
{
register int i,j;
for(j= 0;j<KK;j++)aa[j]= ran_x[j];
for(;j<n;j++)aa[j]= mod_diff(aa[j-KK],aa[j-LL]);
for(i= 0;i<LL;i++,j++)ran_x[i]= mod_diff(aa[j-KK],aa[j-LL]);
for(;i<KK;i++,j++)ran_x[i]= mod_diff(aa[j-KK],ran_x[i-LL]);
}

/*:22*//*23:*/
#line 514 "./prime-sieve-sparse.w"

long ran_arr_buf[QUALITY];
long ran_arr_dummy= -1,ran_arr_started= -1;
long*ran_arr_ptr= &ran_arr_dummy;
void ran_start(long seed)
{
register int t,j;
long x[KK+KK-1];
register long ss= (seed+2)&(MM-2);
for(j= 0;j<KK;j++){
x[j]= ss;
ss<<= 1;if(ss>=MM)ss-= MM-2;
}
x[1]++;
for(ss= seed&(MM-1),t= TT-1;t;){
for(j= KK-1;j> 0;j--)x[j+j]= x[j],x[j+j-1]= 0;
for(j= KK+KK-2;j>=KK;j--)
x[j-(KK-LL)]= mod_diff(x[j-(KK-LL)],x[j]),
x[j-KK]= mod_diff(x[j-KK],x[j]);
if(is_odd(ss)){
for(j= KK;j> 0;j--)x[j]= x[j-1];
x[0]= x[KK];
x[LL]= mod_diff(x[LL],x[KK]);
}
if(ss)ss>>= 1;else t--;
}
for(j= 0;j<LL;j++)ran_x[j+KK-LL]= x[j];
for(;j<KK;j++)ran_x[j-LL]= x[j];
for(j= 0;j<10;j++)ran_array(x,KK+KK-1);
ran_arr_ptr= &ran_arr_started;
}

/*:23*//*25:*/
#line 554 "./prime-sieve-sparse.w"

long ran_arr_cycle()
{
if(ran_arr_ptr==&ran_arr_dummy)
ran_start(314159L);
ran_array(ran_arr_buf,QUALITY);
ran_arr_buf[KK]= -1;
ran_arr_ptr= ran_arr_buf+1;
return ran_arr_buf[0];
}

/*:25*//*26:*/
#line 570 "./prime-sieve-sparse.w"

unsigned long long acc_hi,acc_lo;
void mult(unsigned long long x,unsigned long long y)
{
register unsigned int xhi,xlo,yhi,ylo;
unsigned long long t;
xhi= x>>32,xlo= x&0xffffffff;
yhi= y>>32,ylo= y&0xffffffff;
t= ((unsigned long long)xlo)*ylo,acc_lo= t&0xffffffff;
t= ((unsigned long long)xhi)*ylo+(t>>32),acc_hi= t>>32;
t= ((unsigned long long)xlo)*yhi+(t&0xffffffff);
acc_hi+= ((unsigned long long)xhi)*yhi+(t>>32);acc_lo+= (t&0xffffffff)<<32;
}

/*:26*//*27:*/
#line 603 "./prime-sieve-sparse.w"

int isprime(unsigned long long z)
{
register int k,lgz,rep;
long long x,y,q;
unsigned long long m,zp,goal;
/*32:*/
#line 688 "./prime-sieve-sparse.w"

{
long long u,v,t;
t= magic-(z>>1);
v= z;
B4:while((t&1)==0)t>>= 1;
B5:if(t> 0)u= t;else v= -t;
B6:t= (u-v)/2;
if(t)goto B4;
if(u> 1)return 0;
}

/*:32*/
#line 609 "./prime-sieve-sparse.w"
;
/*28:*/
#line 637 "./prime-sieve-sparse.w"

for(lgz= 63,m= 0x8000000000000000;(m&z)==0;m>>= 1,lgz--);
for(k= lgz,goal= m;k<64;k++){
goal+= goal;
if(goal>=z)goal-= z;
}
/*29:*/
#line 648 "./prime-sieve-sparse.w"

{
zp= (z&4?z:z^8);
zp= (2-zp*z)*zp;
zp= (2-zp*z)*zp;
zp= (2-zp*z)*zp;
zp= (2-zp*z)*zp;
}

/*:29*/
#line 643 "./prime-sieve-sparse.w"
;

/*:28*/
#line 610 "./prime-sieve-sparse.w"
;
for(rep= 0;rep<32;rep++){
P1:x= ran_arr_next();
P2:q= z>>1;
for(y= x,m= 1LL<<(lgz-2);m;m>>= 1){
/*30:*/
#line 661 "./prime-sieve-sparse.w"

{
mult(y,y);
y= acc_hi;
mult(zp*acc_lo,z);
if(y<acc_hi)y+= z-acc_hi;
else y-= acc_hi;
}

/*:30*/
#line 615 "./prime-sieve-sparse.w"
;
if(m&q)/*31:*/
#line 670 "./prime-sieve-sparse.w"

{
mult(x,y);
y= acc_hi;
mult(zp*acc_lo,z);
if(y<acc_hi)y+= z-acc_hi;
else y-= acc_hi;
}

/*:31*/
#line 616 "./prime-sieve-sparse.w"
;
}
if(y!=goal&&y!=z-goal){
if(rep){
fprintf(outfile,"(%lld is a pseudoprime of rank %d)\n",z,rep);
fflush(outfile);
}
return 0;
}
}
return 1;
}

/*:27*/
#line 124 "./prime-sieve-sparse.w"

main(int argc,char*argv[])
{
register j,jj,k;
unsigned long long x,xx,y,z,s,ss;
int d,dd,ii,kk,qq;
starttime= timer= time(0);
/*18:*/
#line 442 "./prime-sieve-sparse.w"

for(j= 0;j<=16;j++)nu[((1<<j)-1)]= j;

/*:18*/
#line 131 "./prime-sieve-sparse.w"
;
/*24:*/
#line 546 "./prime-sieve-sparse.w"

ran_start(314159L);

/*:24*/
#line 132 "./prime-sieve-sparse.w"
;
/*3:*/
#line 142 "./prime-sieve-sparse.w"

if(argc!=5||sscanf(argv[1],"%llu",&s0)!=1||
sscanf(argv[2],"%d",&tt)!=1){
fprintf(stderr,"Usage: %s s[0] t inputfile outputfile\n",argv[0]);
exit(-1);
}
infile= fopen(argv[3],"rb");
if(!infile){
fprintf(stderr,"I can't open %s for binary input!\n",argv[3]);
exit(-2);
}
outfile= fopen(argv[4],"w");
if(!outfile){
fprintf(stderr,"I can't open %s for text output!\n",argv[4]);
exit(-3);
}
st= s0+tt*del;
if(del%256){
fprintf(stderr,"Oops: The sieve size %d isn't a multiple of 256!\n",del);
exit(-4);
}
if(s0&3){
fprintf(stderr,"The starting point %llu isn't a multiple of 4!\n",s0);
exit(-5);
}
if(s0*s0<del){
fprintf(stderr,"The starting point %llu is less than sqrt(%llu)!\n",s0,del);
exit(-6);
}
/*4:*/
#line 185 "./prime-sieve-sparse.w"

for(k= 0;;k++){
if(k>=kmax){
fprintf(stderr,"Oops: Please recompile me with kmax>%d!\n",kmax);
exit(-7);
}
if(fread(&prime[k],sizeof(unsigned int),1,infile)!=1){
fprintf(stderr,"The input file ended prematurely (%d^2<%llu)!\n",
k?prime[k-1]:0,st);
exit(-8);
}
if(k==0&&prime[0]!=2){
fprintf(stderr,"The input file begins with %d, not 2!\n",prime[0]);
exit(-9);
}else if(k> 0&&prime[k]<=prime[k-1]){
fprintf(stderr,"The input file has consecutive entries %d,%d!\n",
prime[k-1],prime[k]);
exit(-10);
}
if(prime[k]<ddel){
if(k>=ksmall){
fprintf(stderr,"Oops: Please recompile me with ksmall>%d!\n",ksmall);
exit(-11);
}
dd= k+1;
}
if(((unsigned long long)prime[k])*prime[k]> st)break;
}
printf("%d primes successfully loaded from %s\n",k,argv[3]);

/*:4*/
#line 171 "./prime-sieve-sparse.w"
;
printf("Sieving between s[0]=%llu and s[t]=%llu:\n",s0,st);

/*:3*/
#line 133 "./prime-sieve-sparse.w"
;
/*7:*/
#line 265 "./prime-sieve-sparse.w"

/*5:*/
#line 233 "./prime-sieve-sparse.w"

for(k= 1;((unsigned long long)prime[k])*prime[k]<s0;k++){
j= (((long long)(prime[k]&3)*prime[k])>>2)-(long long)((s0>>2)%prime[k]);
if(j<0)j+= prime[k];
if(k<dd)start[k]= j;
else{
jj= (j/ddel)%qsize;
if(count[jj]==countmax){
countmax++;
if(countmax>=lsize){
fprintf(stderr,"Oops: Please recompile me with lsize>%d!\n",lsize);
exit(-12);
}
}
list[jj][count[jj]].p= prime[k],list[jj][count[jj]].s= j;
count[jj]++;
}
}
kk= k;
/*6:*/
#line 258 "./prime-sieve-sparse.w"

for(k= 1;prime[k]<32&&k<kk;k++){
for(x= 0,y= 1LL<<start[k];x!=y;x= y,y|= y<<prime[k]);
sv[k]= x,rem[k]= 64%prime[k];
}
d= k;

/*:6*/
#line 252 "./prime-sieve-sparse.w"
;

/*:5*/
#line 266 "./prime-sieve-sparse.w"
;
ss= s0;
sieve[1+del/256]= -1;

/*:7*/
#line 134 "./prime-sieve-sparse.w"
;
for(ii= 0;ii<tt;ii++)/*8:*/
#line 270 "./prime-sieve-sparse.w"

{
s= ss,ss= s+del,qq= ii%qsize;
if(qq==0){
j= time(0);
printf("Beginning segment %llu (after %d sec)\n",s,j-timer);
fflush(stdout);
timer= j;
}
/*9:*/
#line 285 "./prime-sieve-sparse.w"

for(j= 0;j<del/256;j++){
for(z= 0,k= 1;k<d;k++){
z|= sv[k];
sv[k]= (sv[k]<<(prime[k]-rem[k]))|(sv[k]>>rem[k]);
}
sieve[j]= z;
}

/*:9*/
#line 279 "./prime-sieve-sparse.w"
;
/*10:*/
#line 299 "./prime-sieve-sparse.w"

if(dd>=kk){
for(k= d;k<kk;k++){
for(j= start[k];j<ddel;j+= prime[k])sieve[j>>6]|= 1LL<<(j&0x3f);
start[k]= j-ddel;
}
}else{
for(k= d;k<dd;k++){
for(j= start[k];j<ddel;j+= prime[k])sieve[j>>6]|= 1LL<<(j&0x3f);
start[k]= j-ddel;
}
/*11:*/
#line 318 "./prime-sieve-sparse.w"

for(j= k= 0;k<count[qq];k++){
if(list[qq][k].s>=(qq+1)*ddel)
list[qq][j].p= list[qq][k].p,list[qq][j].s= list[qq][k].s-qsize*ddel,j++;
else{
register unsigned int nstart;
jj= list[qq][k].s%ddel;
sieve[jj>>6]|= 1LL<<(jj&0x3f);
nstart= list[qq][k].s+list[qq][k].p;
jj= (nstart/ddel)%qsize;
if(count[jj]==countmax){
countmax++;
if(countmax>=lsize){
fprintf(stderr,"Oops: Please recompile me with lsize>%d!\n",lsize);
exit(-13);
}
}
list[jj][count[jj]].p= list[qq][k].p;
list[jj][count[jj]].s= (jj>=qq?nstart:nstart-qsize*ddel);
count[jj]++;
}
}
count[qq]= j;

/*:11*/
#line 310 "./prime-sieve-sparse.w"
;
}

/*:10*/
#line 280 "./prime-sieve-sparse.w"
;
/*12:*/
#line 344 "./prime-sieve-sparse.w"

for(k= kk;((unsigned long long)prime[k])*prime[k]<ss;k++){
for(j= (((unsigned long long)prime[k])*prime[k]-s-1)>>2;j<ddel;j+= prime[k])
sieve[j>>6]|= 1LL<<(j&0x3f);
if(k<dd)start[k]= j-ddel;
else{
j+= qq*ddel;
jj= (j/ddel)%qsize;
if(count[jj]==countmax){
countmax++;
if(countmax>=lsize){
fprintf(stderr,"Oops: Please recompile me with lsize>%d!\n",lsize);
exit(-14);
}
}
list[jj][count[jj]].p= prime[k];
list[jj][count[jj]].s= (jj> qq?j:j-qsize*ddel);
count[jj]++;
}
}
kk= k;

/*:12*/
#line 281 "./prime-sieve-sparse.w"
;
/*13:*/
#line 381 "./prime-sieve-sparse.w"

j= 0,k= -100;
while(1){
for(;sieve[j]==-1;j++);
if(j==del/256)x= ss;
else/*15:*/
#line 416 "./prime-sieve-sparse.w"

{
y= ~sieve[j];
y= y&-y;
/*17:*/
#line 435 "./prime-sieve-sparse.w"

y--;
y= nu[y&0xffff]+nu[(y>>16)&0xffff]+nu[(y>>32)&0xffff]+nu[(y>>48)&0xffff];

/*:17*/
#line 420 "./prime-sieve-sparse.w"
;
x= s+(j<<8)+(y<<2)+1;
}

/*:15*/
#line 386 "./prime-sieve-sparse.w"
;
if(k>=0)/*16:*/
#line 424 "./prime-sieve-sparse.w"

{
for(y= ~sieve[k],z= y&(y-1);z;y= z,z= y&(y-1));
/*17:*/
#line 435 "./prime-sieve-sparse.w"

y--;
y= nu[y&0xffff]+nu[(y>>16)&0xffff]+nu[(y>>32)&0xffff]+nu[(y>>48)&0xffff];

/*:17*/
#line 427 "./prime-sieve-sparse.w"
;
lastprime= s+(k<<8)+(y<<2)+1;
}

/*:16*/
#line 387 "./prime-sieve-sparse.w"

else if(lastprime==0)/*14:*/
#line 404 "./prime-sieve-sparse.w"

{
for(z= s+3;z<x;z+= 4)if(isprime(z)){
lastprime= z;goto got_it;
}
if(x==ss)goto donewithseg;
lastprime= x;
got_it:fprintf(outfile,"The first prime is %llu = s[0]+%d\n",
lastprime,lastprime-s0);
fflush(outfile);
}

/*:14*/
#line 388 "./prime-sieve-sparse.w"
;
/*19:*/
#line 456 "./prime-sieve-sparse.w"

if(j>=k+bestgap/256){
xx= x;
zloop:if(x-lastprime<bestgap)goto done_here;
y= (k>=0?lastprime:s);
for(z= ((lastprime&~2)+bestgap-2);z> y;z-= 4)if(isprime(z)){
lastprime= z,k= 0;goto zloop;
}
z= (lastprime&~2)+bestgap+2;
if(z<s)z= s+3;
for(;z<x;z+= 4)if(isprime(z)){
x= z;break;
}
if(x==ss)goto donewithseg;

/*20:*/
#line 476 "./prime-sieve-sparse.w"

{
if(x-lastprime>=bestgap){
fprintf(outfile,"%llu is followed by a gap of length %d\n",
lastprime,x-lastprime);
fflush(outfile);
}
}

/*:20*/
#line 471 "./prime-sieve-sparse.w"
;
lastprime= x,x= xx;goto zloop;
}
done_here:

/*:19*/
#line 389 "./prime-sieve-sparse.w"
;
if(j==del/256)break;
for(j++;sieve[j]!=-1;j++);
if(j<del/256)k= j-1;
else{
k= del/256-1;
/*16:*/
#line 424 "./prime-sieve-sparse.w"

{
for(y= ~sieve[k],z= y&(y-1);z;y= z,z= y&(y-1));
/*17:*/
#line 435 "./prime-sieve-sparse.w"

y--;
y= nu[y&0xffff]+nu[(y>>16)&0xffff]+nu[(y>>32)&0xffff]+nu[(y>>48)&0xffff];

/*:17*/
#line 427 "./prime-sieve-sparse.w"
;
lastprime= s+(k<<8)+(y<<2)+1;
}

/*:16*/
#line 395 "./prime-sieve-sparse.w"
;
break;
}
}
for(z= ss-1;z> lastprime;z-= 4)if(isprime(z)){
lastprime= z;break;
}
donewithseg:

/*:13*/
#line 282 "./prime-sieve-sparse.w"
;
}

/*:8*/
#line 135 "./prime-sieve-sparse.w"
;
/*21:*/
#line 485 "./prime-sieve-sparse.w"

if(lastprime){
fprintf(outfile,"The final prime is %llu = s[t]-%d.\n",
lastprime,st-lastprime);
}else fprintf(outfile,"No prime numbers exist between s[0] and s[t].\n");

/*:21*/
#line 136 "./prime-sieve-sparse.w"
;
printf("(Finished; the last segment took %d sec; total time %.6g hours.)\n",
time(0)-timer,((double)(time(0)-starttime))/3600.0);
printf("(The maximum list size needed was %d.)\n",countmax);
}

/*:2*/
