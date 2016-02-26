#define nmax 10000000 \

#define newa(j,y) undo[ptr++]= (j<<24) +a[j],a[j]= y
#define newb(j,y) undo[ptr++]= (1<<31) +(j<<24) +b[j],b[j]= y \

/*1:*/
#line 24 "achain2.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
char l[nmax];
int a[128],b[128];
unsigned int undo[128*128];
int ptr;
struct{
int lbp,ubp,lbq,ubq,r,ptrp,ptrq;
}stack[128];
int tail[128],outdeg[128],outsum[128],limit[128];
FILE*infile,*outfile;
int prime[1000];
int pr;
char x[64];
int main(int argc,char*argv[])
{
register int i,j,n,p,q,r,s,ubp,ubq,lbp,lbq,ptrp,ptrq;
int lb,ub,timer= 0;
/*2:*/
#line 61 "achain2.w"

if(argc!=3){
fprintf(stderr,"Usage: %s infile outfile\n",argv[0]);
exit(-1);
}
infile= fopen(argv[1],"r");
if(!infile){
fprintf(stderr,"I couldn't open `%s' for reading!\n",argv[1]);
exit(-2);
}
outfile= fopen(argv[2],"w");
if(!outfile){
fprintf(stderr,"I couldn't open `%s' for writing!\n",argv[2]);
exit(-3);
}

/*:2*/
#line 44 "achain2.w"
;
prime[0]= 2,pr= 1;
a[0]= b[0]= 1,a[1]= b[1]= 2;
for(n= 1;n<nmax;n++){
/*4:*/
#line 85 "achain2.w"

for(q= n,i= -1,j= 0;q;q>>= 1,i++)
if(q&1)x[j++]= i;

lb= fgetc(infile)-' ';
if(lb<i+3)lb= i+3;

/*:4*/
#line 48 "achain2.w"
;
/*5:*/
#line 107 "achain2.w"

ub= i+j-1;
if(ub> l[n-1]+1)ub= l[n-1]+1;
/*6:*/
#line 126 "achain2.w"

if(n> 2)for(s= 0;;s++){
p= prime[s];
q= n/p;
if(n==p*q){
if(l[p]+l[q]<ub)ub= l[p]+l[q];
break;
}
if(q<=p){
if(pr<1000)prime[pr++]= n;
break;
}
}

/*:6*/
#line 110 "achain2.w"
;
l[n]= ub;
if(j<=3)goto done;
if(j==4){
p= x[3]-x[2],q= x[1]-x[0];
if(p==q||p==q+1||(q==1&&(p==3||(p==5&&x[2]==x[1]+1))))l[n]= i+2;
goto done;
}

/*:5*/
#line 49 "achain2.w"
;
/*7:*/
#line 145 "achain2.w"

l[n]= lb;
while(lb<ub){
for(i= 0;i<=lb;i++)outdeg[i]= outsum[i]= 0;
a[lb]= b[lb]= n;
for(i= 2;i<lb;i++)a[i]= a[i-1]+1,b[i]= b[i-1]<<1;
for(i= lb-1;i>=2;i--){
if((a[i]<<1)<a[i+1])a[i]= (a[i+1]+1)>>1;
if(b[i]>=b[i+1])b[i]= b[i+1]-1;
}
/*9:*/
#line 206 "achain2.w"

ptr= 0;
for(r= s= lb;s> 2;s--){
if(outdeg[s]==1)limit[s]= tail[outsum[s]];else limit[s]= 1;
for(;r> 1&&a[r-1]==b[r-1];r--);
if(outdeg[s-1]==0&&(a[s]&1))q= a[s]/3;else q= a[s]>>1;
for(p= a[s]-q;p<=b[s-1];){
if(p> b[r-1]){
while(p> a[r])r++;
p= a[r],q= a[s]-p,r++;
}
if(q<limit[s])goto backup;
/*12:*/
#line 272 "achain2.w"

lbp= l[p];
if(lbp>=s)goto failpq;
while(b[lbp]<p)lbp++;
if(a[lbp]> p)goto failpq;
for(ubp= lbp;a[ubp+1]<=p;ubp++);
if(ubp==s-1)lbp= ubp;
if(p==q)lbq= lbp,ubq= ubp;
else{
lbq= l[q];
if(lbq>=ubp)goto failpq;
while(b[lbq]<q)lbq++;
if(lbq>=ubp)goto failpq;
if(a[lbq]> q)goto failpq;
for(ubq= lbq;a[ubq+1]<=q&&ubq+1<ubp;ubq++);
if(lbp==lbq)lbp++;
}

/*:12*/
#line 220 "achain2.w"
;
ptrp= ptr;
for(;ubp>=lbp;ubp--){
/*14:*/
#line 306 "achain2.w"

if(a[ubp]!=p){
newa(ubp,p);
for(j= ubp-1;(a[j]<<1)<a[j+1];j--){
i= (a[j+1]+1)>>1;
if(i> b[j])goto failp;
newa(j,i);
}
for(j= ubp+1;a[j]<=a[j-1];j++){
i= a[j-1]+1;
if(i> b[j])goto failp;
newa(j,i);
}
}
if(b[ubp]!=p){
newb(ubp,p);
for(j= ubp-1;b[j]>=b[j+1];j--){
i= b[j+1]-1;
if(i<a[j])goto failp;
newb(j,i);
}
for(j= ubp+1;b[j]> b[j-1]<<1;j++){
i= b[j-1]<<1;
if(i<a[j])goto failp;
newb(j,i);
}
}
/*15:*/
#line 354 "achain2.w"

i= p-(1<<(ubp-1));
if(i&&((i&(i-1))==0)&&(i<<4)<p){
for(j= ubp-2;(i&1)==0;i>>= 1,j--);
if(b[j]<(1<<j))goto failp;
for(;a[j]<(1<<j);j--)newa(j,1<<j);
}

/*:15*/
#line 333 "achain2.w"
;

/*:14*/
#line 224 "achain2.w"
;
if(p==q)goto happiness;
if(ubq>=ubp)ubq= ubp-1;
ptrq= ptr;
for(;ubq>=lbq;ubq--){
/*16:*/
#line 366 "achain2.w"

if(a[ubq]!=q){
if(a[ubq]> q)goto failq;
newa(ubq,q);
for(j= ubq-1;(a[j]<<1)<a[j+1];j--){
i= (a[j+1]+1)>>1;
if(i> b[j])goto failq;
newa(j,i);
}
for(j= ubq+1;a[j]<=a[j-1];j++){
i= a[j-1]+1;
if(i> b[j])goto failq;
newa(j,i);
}
}
if(b[ubq]!=q){
if(b[ubq]<q)goto failq;
newb(ubq,q);
for(j= ubq-1;b[j]>=b[j+1];j--){
i= b[j+1]-1;
if(i<a[j])goto failq;
newb(j,i);
}
for(j= ubq+1;b[j]> b[j-1]<<1;j++){
i= b[j-1]<<1;
if(i<a[j])goto failq;
newb(j,i);
}
}
/*17:*/
#line 397 "achain2.w"

i= q-(1<<(ubq-1));
if(i&&((i&(i-1))==0)&&(i<<4)<q){
for(j= ubq-2;(i&1)==0;i>>= 1,j--);
if(b[j]<(1<<j))goto failq;
for(;a[j]<(1<<j);j--)newa(j,1<<j);
}


/*:17*/
#line 395 "achain2.w"
;

/*:16*/
#line 230 "achain2.w"
;
happiness:/*10:*/
#line 254 "achain2.w"

tail[s]= q,stack[s].r= r;
outdeg[ubp]++,outsum[ubp]+= s;
outdeg[ubq]++,outsum[ubq]+= s;
stack[s].lbp= lbp,stack[s].ubp= ubp;
stack[s].lbq= lbq,stack[s].ubq= ubq;
stack[s].ptrp= ptrp,stack[s].ptrq= ptrq;

/*:10*/
#line 231 "achain2.w"
;
goto onward;
backup:s++;
if(s> lb)goto impossible;
/*11:*/
#line 262 "achain2.w"

ptrq= stack[s].ptrq,ptrp= stack[s].ptrp;
lbq= stack[s].lbq,ubq= stack[s].ubq;
lbp= stack[s].lbp,ubp= stack[s].ubp;
outdeg[ubq]--,outsum[ubq]-= s;
outdeg[ubp]--,outsum[ubp]-= s;
q= tail[s],p= a[s]-q,r= stack[s].r;

/*:11*/
#line 235 "achain2.w"
;
if(p==q)goto failp;
failq:while(ptr> ptrq)/*13:*/
#line 297 "achain2.w"

{
i= undo[--ptr];
if(i>=0)a[i>>24]= i&0xffffff;
else b[(i&0x3fffffff)>>24]= i&0xffffff;
}

/*:13*/
#line 237 "achain2.w"
;
}
failp:while(ptr> ptrp)/*13:*/
#line 297 "achain2.w"

{
i= undo[--ptr];
if(i>=0)a[i>>24]= i&0xffffff;
else b[(i&0x3fffffff)>>24]= i&0xffffff;
}

/*:13*/
#line 239 "achain2.w"
;
}
failpq:if(p==q){
if(outdeg[s-1]==0)q= a[s]/3+1;
if(q> b[s-2])q= b[s-2];
else q--;
p= a[s]-q;
}else p++,q--;
}
goto backup;
onward:continue;
}
goto done;
impossible:

/*:9*/
#line 155 "achain2.w"
;
l[n]= ++lb;
}

/*:7*/
#line 50 "achain2.w"
;
done:/*3:*/
#line 77 "achain2.w"

fprintf(outfile,"%c",l[n]+' ');
fflush(outfile);

/*:3*/
#line 51 "achain2.w"
;
if(n%1000==0){
j= clock();
printf("%d..%d done in %.5g minutes\n",
n-999,n,(double)(j-timer)/(60*CLOCKS_PER_SEC));
timer= j;
}
}
}

/*:1*/
