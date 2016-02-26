#define n 12
#define memsize 36000000
#define thresh (1<<7) 
#define hashsize (1<<21)  \

#define wt(p) *p
#define aux(p) *(p+1) 
#define bits(p) *(p+2) 
#define link(p) *(p+3)  \

/*1:*/
#line 32 "./posets.w"

#include <stdio.h> 
#include "gb_flip.h"
typedef unsigned int uint;
uint mem[memsize];
int*hash[1<<21];
short uni[4][256];
uint*top,*start,*ostart;
int offset[1<<(n-1)];
int t,x,y,z,mask,hmask;
int count,curn;

main()
{
register int j,k,l,curbits,curaux;
uint*p;
/*2:*/
#line 55 "./posets.w"

gb_init_rand(0);
for(j= 0;j<4;j++)for(k= 1;k<256;k++)uni[j][k]= gb_next_rand();

/*:2*//*4:*/
#line 62 "./posets.w"

for(j= 0,k= 1,l= 2;l<=thresh;k++,l<<= 1)offset[l-1]= j,j+= k;
for(j= 0;l<1<<n;k++,l<<= 1)offset[l-1]= j,j+= k;

/*:4*/
#line 48 "./posets.w"
;
/*5:*/
#line 75 "./posets.w"

start= &mem[0];
wt(start)= 1,aux(start)= bits(start)= 0,link(start)= (uint)NULL;
top= start+4;
for(l= (1<<(n-1))-1,curn= n;l> thresh;l>>= 1,curn--){
hmask= (1<<offset[l])-1;
for(j= 0;j<hashsize;j++)hash[j]= NULL;
count= 0;
for(p= start,start= top;p!=start;p= (p==&mem[memsize-4]?&mem[0]:p+4)){
count++;
mask= (aux(p)>>offset[l])&l;
for(x= 0;x<=l;x= ((x|mask)+1)&~mask){
curbits= bits(p);
curaux= aux(p);
for(y= x&(x+1),t= x^-1;y;y-= z){
z= y&-y;
if(z<=thresh)curbits|= (t&(z-1))<<offset[z-1];
else curaux|= (t&(z-1))<<offset[z-1];
}
/*6:*/
#line 100 "./posets.w"

{register int h;register uint*q;
curaux&= hmask;
h= uni[0][curbits&0xff]+uni[1][(curbits>>8)&0xff]+
uni[2][(curbits>>16)&0xff]+uni[3][curbits>>24];
h+= uni[0][curaux&0xff]+uni[1][(curaux>>8)&0xff]+
uni[2][(curaux>>16)&0xff]+uni[3][curaux>>24];
h&= hashsize-1;
for(q= hash[h];q;q= (uint*)link(q))
if(bits(q)==curbits&&aux(q)==curaux)goto found;
q= top;
if(q==p){
fprintf(stderr,"Sorry, I need more memory!\n");
exit(-1);
}
bits(q)= curbits,aux(q)= curaux,wt(q)= 0;
link(q)= (uint)hash[h],hash[h]= q;
top= q+4;
if(top==&mem[memsize])top= &mem[0];
found:wt(q)+= wt(p);
}

/*:6*/
#line 94 "./posets.w"
;
}
}
printf(" %d item%s on list %d;\n",count,count> 1?"s":"",curn);
}

/*:5*/
#line 49 "./posets.w"
;
/*7:*/
#line 125 "./posets.w"

/*9:*/
#line 166 "./posets.w"

ostart= top;
x= (top-mem)%3;
if(x)top+= 3-x;
for(p= start,start= top;p!=ostart;p= (p==&mem[memsize-4]?&mem[0]:p+4)){
wt(top)= wt(p),aux(top)= 0,bits(top)= bits(p);
top+= 3;
if(top==&mem[memsize])top= &mem[0];
}

/*:9*/
#line 126 "./posets.w"
;
for(;l;l>>= 1,curn--){
hmask= (1<<offset[l])-1;
for(j= 0;j<=hmask;j++)hash[j]= NULL;
count= 0;
for(p= start,start= top;p!=start;p= (p==&mem[memsize-3]?&mem[0]:p+3)){
count++;
mask= (bits(p)>>offset[l])&l;
for(x= 0;x<=l;x= ((x|mask)+1)&~mask){
curbits= bits(p);
for(y= x&(x+1),t= x^-1;y;y-= z){
z= y&-y;
curbits|= (t&(z-1))<<offset[z-1];
}
/*8:*/
#line 147 "./posets.w"

{register uint*q;
y= curbits&hmask;
q= hash[y];
if(!q){
q= top;
if(q==p){
fprintf(stderr,"Sorry, I need more memory!\n");
exit(-2);
}
bits(q)= curbits,wt(q)= aux(q)= 0;
hash[y]= q;
top= q+3;
if(top==&mem[memsize])top= &mem[0];
}
wt(q)+= wt(p),aux(q)+= aux(p);
if(wt(q)>=1000000000)aux(q)+= 1,wt(q)-= 1000000000;
}

/*:8*/
#line 140 "./posets.w"
;
}
}
printf(" %d items on list %d;\n",count,curn);
}
printf("...and the solution for %d is %d%09d.\n",n,aux(start),wt(start));

/*:7*/
#line 50 "./posets.w"
;
}

/*:1*/
