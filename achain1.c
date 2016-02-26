#define nmax 10000000 \

#define newa(j,y) undo[ptr++]= (j<<24) +a[j],a[j]= y
#define newb(j,y) undo[ptr++]= (1<<31) +(j<<24) +b[j],b[j]= y \

/*1:*/
#line 28 "achain1.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
char l[nmax];
int a[128],b[128];
unsigned int undo[128*128];
int ptr;
struct{
int lbp,ubp,lbq,ubq,p,r,ptrp,ptrq;
}stack[128];
FILE*infile,*outfile;
int prime[1000];
int pr;
char x[64];
int main(int argc,char*argv[])
{
register int i,j,n,p,q,r,s,ubp,ubq,lbp,lbq,ptrp,ptrq;
int lb,ub,timer= 0;
/*2:*/
#line 64 "achain1.w"

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
#line 47 "achain1.w"
;
prime[0]= 2,pr= 1;
a[0]= b[0]= 1,a[1]= b[1]= 2;
for(n= 1;n<nmax;n++){
/*4:*/
#line 88 "achain1.w"

for(q= n,i= -1,j= 0;q;q>>= 1,i++)
if(q&1)x[j++]= i;

lb= fgetc(infile)-' ';
if(lb<i+3)lb= i+3;

/*:4*/
#line 51 "achain1.w"
;
/*5:*/
#line 110 "achain1.w"

ub= i+j-1;
if(ub> l[n-1]+1)ub= l[n-1]+1;
/*6:*/
#line 129 "achain1.w"

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
#line 113 "achain1.w"
;
l[n]= ub;
if(j<=3)goto done;
if(j==4){
p= x[3]-x[2],q= x[1]-x[0];
if(p==q||p==q+1||(q==1&&(p==3||(p==5&&x[2]==x[1]+1))))l[n]= i+2;
goto done;
}

/*:5*/
#line 52 "achain1.w"
;
/*7:*/
#line 157 "achain1.w"

l[n]= lb;
while(lb<ub){
a[lb]= b[lb]= n;
for(i= 2;i<lb;i++)a[i]= a[i-1]+1,b[i]= b[i-1]<<1;
for(i= lb-1;i>=2;i--){
if((a[i]<<1)<a[i+1])a[i]= (a[i+1]+1)>>1;
if(b[i]>=b[i+1])b[i]= b[i+1]-1;
}
/*8:*/
#line 184 "achain1.w"

ptr= 0;
for(r= s= lb;s> 2;s--){
for(;r> 1&&a[r-1]==b[r-1];r--);
for(q= a[s]>>1,p= a[s]-q;p<=b[s-1];){
if(p> b[r-1]){
while(p> a[r])r++;
p= a[r],q= a[s]-p,r++;
}
/*9:*/
#line 232 "achain1.w"

lbp= l[p];
if(lbp>=lb)goto failpq;
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

/*:9*/
#line 195 "achain1.w"
;
ptrp= ptr;
for(;ubp>=lbp;ubp--){
/*11:*/
#line 266 "achain1.w"

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

/*:11*/
#line 199 "achain1.w"
;
if(p==q)goto happiness;
if(ubq>=ubp)ubq= ubp-1;
ptrq= ptr;
for(;ubq>=lbq;ubq--){
/*12:*/
#line 298 "achain1.w"

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

/*:12*/
#line 205 "achain1.w"
;
happiness:stack[s].p= p,stack[s].r= r;
stack[s].lbp= lbp,stack[s].ubp= ubp;
stack[s].lbq= lbq,stack[s].ubq= ubq;
stack[s].ptrp= ptrp,stack[s].ptrq= ptrq;
goto onward;
backup:s++;
if(s> lb)goto impossible;
ptrq= stack[s].ptrq,ptrp= stack[s].ptrp;
lbq= stack[s].lbq,ubq= stack[s].ubq;
lbp= stack[s].lbp,ubp= stack[s].ubp;
p= stack[s].p,q= a[s]-p,r= stack[s].r;
if(p==q)goto failp;
failq:while(ptr> ptrq)/*10:*/
#line 257 "achain1.w"

{
i= undo[--ptr];
if(i>=0)a[i>>24]= i&0xffffff;
else b[(i&0x3fffffff)>>24]= i&0xffffff;
}

/*:10*/
#line 218 "achain1.w"
;
}
failp:while(ptr> ptrp)/*10:*/
#line 257 "achain1.w"

{
i= undo[--ptr];
if(i>=0)a[i>>24]= i&0xffffff;
else b[(i&0x3fffffff)>>24]= i&0xffffff;
}

/*:10*/
#line 220 "achain1.w"
;
}
failpq:if(p==q&&q> b[s-2])q= b[s-2],p= a[s]-q;else p++,q--;
}
goto backup;
onward:continue;
}
goto done;
impossible:

/*:8*/
#line 166 "achain1.w"
;
l[n]= ++lb;
}

/*:7*/
#line 53 "achain1.w"
;
done:/*3:*/
#line 80 "achain1.w"

fprintf(outfile,"%c",l[n]+' ');
fflush(outfile);

/*:3*/
#line 54 "achain1.w"
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
