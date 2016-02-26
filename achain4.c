#define nmax 10000000 \

#define upbound(p,x) if(l[p]> x) { \
l[p]= x; \
if(link[p]==0) link[p]= top,top= p; \
} \

#define newa(j,y) undo[ptr++]= (j<<24) +a[j],a[j]= y
#define newb(j,y) undo[ptr++]= (1<<31) +(j<<24) +b[j],b[j]= y \

/*1:*/
#line 31 "achain4.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
unsigned char l[nmax+2];
int a[128],b[128];
unsigned int undo[128*128];
int ptr;
struct{
int lbp,ubp,lbq,ubq,r,ptrp,ptrq;
}stack[128];
int tail[128],outdeg[128],outsum[128],limit[128];
FILE*infile,*outfile;
int down[nmax];
int link[nmax];
int top;
int main(int argc,char*argv[])
{
register int i,j,n,p,q,r,s,ubp,ubq= 0,lbp,lbq,ptrp,ptrq;
int lb,ub,timer= 0;
/*2:*/
#line 68 "achain4.w"

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
#line 51 "achain4.w"
;
a[0]= b[0]= 1,a[1]= b[1]= 2;
/*5:*/
#line 115 "achain4.w"

for(n= 2,down[1]= 1;n<nmax;n++)down[n]= down[n>>1]+(n&1),l[n]= 127;
for(i= 1,p= 0;i*i<nmax;i++,p= (i==1<<(p+1)?p+1:p)){
for(j= i,q= p;i*j<nmax;j++,q= (j==1<<(q+1)?q+1:q)){
for(r= p+q;(1<<r)+i*j<nmax;r++)
if(l[(1<<r)+i*j]>=r+down[i]+down[j])
l[(1<<r)+i*j]= r+down[i]+down[j]-1;

}
}
for(n= 1;n<nmax;n++)down[n]= n-1;
/*6:*/
#line 143 "achain4.w"

top= 1;
for(i= 4;i<nmax;i++){
upbound(i,l[i-2]+1);
upbound(i,l[i-1]+1);
}
for(i= 2;i*i<nmax;i++)for(j= i;i*j<nmax;j++)upbound(i*j,l[i]+l[j]);
while(top> 1){
p= top,top= link[p],link[p]= 0;
upbound(p+1,l[p]+1);
upbound(p+2,l[p]+2);
for(i= 2;i*p<nmax;i++)upbound(i*p,l[i]+l[p]);
}

/*:6*/
#line 126 "achain4.w"
;

/*:5*/
#line 53 "achain4.w"
;
for(n= 1;n<nmax;n++){
/*4:*/
#line 98 "achain4.w"

for(q= n,i= -1,j= 0;q;q>>= 1,i++)j+= q&1;

if(j> 16)j= 16;
for(j--;j;j>>= 1)i++;
lb= fgetc(infile)-' ';
if(lb<i)lb= i;

/*:4*/
#line 55 "achain4.w"
;
/*8:*/
#line 183 "achain4.w"

ub= l[n],l[n]= lb;
while(lb<ub){
for(i= 0;i<=lb;i++)outdeg[i]= outsum[i]= 0;
a[lb]= b[lb]= n;
for(i= 2;i<lb;i++)a[i]= a[i-1]+1,b[i]= b[i-1]<<1;
for(i= lb-1;i>=2;i--){
if((a[i]<<1)<a[i+1])a[i]= (a[i+1]+1)>>1;
if(b[i]>=b[i+1])b[i]= b[i+1]-1;
}
/*12:*/
#line 241 "achain4.w"

ptr= 0;
for(r= s= lb;s> 2;s--){
if(outdeg[s]==1)
limit[s]= a[s]-tail[outsum[s]];else limit[s]= a[s]-1;

if(limit[s]> b[s-1])limit[s]= b[s-1];
/*13:*/
#line 285 "achain4.w"

if(a[s]&1){
unequal:if(outdeg[s-1]==0)q= a[s]/3;else q= a[s]>>1;
if(q> b[s-2])q= b[s-2];
p= a[s]-q;
if(l[p]>=s){
/*10:*/
#line 221 "achain4.w"

do{
if(down[p]> p)p= down[p];
else{
p= nmax;break;
}
}while(l[p]>=s);

/*:10*/
#line 291 "achain4.w"
;
q= a[s]-p;
}
}else{
p= q= a[s]>>1;
if(l[p]>=s)goto unequal;
}
if(p> limit[s])goto backup;
for(;r> 2&&a[r-1]==b[r-1];r--);
if(p> b[r-1]){
while(p> a[r])r++;
p= a[r],q= a[s]-p;
}else if(q<p&&q> b[r-2]){
if(a[r]<=a[s]-b[r-2])p= a[r],q= b[s]-p;
else q= b[r-2],p= a[s]-q;
}

/*:13*/
#line 248 "achain4.w"
;
while(p<=limit[s]){
/*17:*/
#line 359 "achain4.w"

if(l[p]>=s)goto failpq;
lbp= l[p];
while(b[lbp]<p)lbp++;
if((p&1)&&p> b[lbp-2]+b[lbp-1]){
if(++lbp>=s)goto failpq;
}
if(a[lbp]> p)goto failpq;
for(ubp= lbp;a[ubp+1]<=p;ubp++);
if(ubp==s-1)lbp= ubp;
if(p==q)lbq= lbp,ubq= ubp;
else{
lbq= l[q];
if(lbq>=ubp)goto failpq;
while(b[lbq]<q)lbq++;
if(a[lbq]<b[lbq]){
if((q&1)&&q> b[lbq-2]+b[lbq-1])lbq++;
if(lbq>=ubp)goto failpq;
if(a[lbq]> q)goto failpq;
if(lbp<=lbq)lbp= lbq+1;
while((q<<(lbp-lbq))<p)
if(++lbp> ubp)goto failpq;
}
for(ubq= lbq;a[ubq+1]<=q&&(q<<(ubp-ubq-1))>=p;ubq++);
}

/*:17*/
#line 252 "achain4.w"
;
ptrp= ptr;
for(;ubp>=lbp;ubp--){
/*19:*/
#line 401 "achain4.w"

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
/*20:*/
#line 449 "achain4.w"

i= p-(1<<(ubp-1));
if(i&&((i&(i-1))==0)&&(i<<4)<p){
for(j= ubp-2;(i&1)==0;i>>= 1,j--);
if(b[j]<(1<<j))goto failp;
for(;a[j]<(1<<j);j--)newa(j,1<<j);
}

/*:20*/
#line 428 "achain4.w"
;

/*:19*/
#line 256 "achain4.w"
;
if(p==q)goto happiness;
if(ubq>=ubp)ubq= ubp-1;
ptrq= ptr;
for(;ubq>=lbq;ubq--){
/*21:*/
#line 461 "achain4.w"

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
/*22:*/
#line 492 "achain4.w"

i= q-(1<<(ubq-1));
if(i&&((i&(i-1))==0)&&(i<<4)<q){
for(j= ubq-2;(i&1)==0;i>>= 1,j--);
if(b[j]<(1<<j))goto failq;
for(;a[j]<(1<<j);j--)newa(j,1<<j);
}


/*:22*/
#line 490 "achain4.w"
;

/*:21*/
#line 262 "achain4.w"
;
happiness:/*15:*/
#line 341 "achain4.w"

tail[s]= q,stack[s].r= r;
outdeg[ubp]++,outsum[ubp]+= s;
outdeg[ubq]++,outsum[ubq]+= s;
stack[s].lbp= lbp,stack[s].ubp= ubp;
stack[s].lbq= lbq,stack[s].ubq= ubq;
stack[s].ptrp= ptrp,stack[s].ptrq= ptrq;

/*:15*/
#line 263 "achain4.w"
;
goto onward;
backup:s++;
if(s> lb)goto impossible;
/*16:*/
#line 349 "achain4.w"

ptrq= stack[s].ptrq,ptrp= stack[s].ptrp;
lbq= stack[s].lbq,ubq= stack[s].ubq;
lbp= stack[s].lbp,ubp= stack[s].ubp;
outdeg[ubq]--,outsum[ubq]-= s;
outdeg[ubp]--,outsum[ubp]-= s;
q= tail[s],p= a[s]-q,r= stack[s].r;

/*:16*/
#line 267 "achain4.w"
;
if(p==q)goto failp;
failq:while(ptr> ptrq)/*18:*/
#line 392 "achain4.w"

{
i= undo[--ptr];
if(i>=0)a[i>>24]= i&0xffffff;
else b[(i&0x3fffffff)>>24]= i&0xffffff;
}

/*:18*/
#line 269 "achain4.w"
;
}
failp:while(ptr> ptrp)/*18:*/
#line 392 "achain4.w"

{
i= undo[--ptr];
if(i>=0)a[i>>24]= i&0xffffff;
else b[(i&0x3fffffff)>>24]= i&0xffffff;
}

/*:18*/
#line 271 "achain4.w"
;
}
failpq:/*14:*/
#line 308 "achain4.w"

if(p==q){
if(outdeg[s-1]==0)q= (a[s]/3)+1;
if(q> b[s-2])q= b[s-2];else q--;
p= a[s]-q;
if(l[p]>=s){
/*10:*/
#line 221 "achain4.w"

do{
if(down[p]> p)p= down[p];
else{
p= nmax;break;
}
}while(l[p]>=s);

/*:10*/
#line 314 "achain4.w"
;
q= a[s]-p;
}
}else{
/*9:*/
#line 214 "achain4.w"

{
p++;
if(l[p]==s)
p= (down[p]> p?down[p]:nmax);
}

/*:9*/
#line 318 "achain4.w"
;
q= a[s]-p;
}
if(q> 2){
if(a[s-1]==b[s-1]){
doublecheck:while(p<a[r]&&a[r-1]==b[r-1])r--;
if(p> b[r-1]){
while(p> a[r])r++;
p= a[r],q= a[s]-p;
}else if(q> b[r-2]){
if(a[r]<=a[s]-b[r-2])p= a[r],q= b[s]-p;
else q= b[r-2],p= a[s]-q;
}
}
if(ubq>=s)ubq= s-1;
while(q>=a[ubq+1])ubq++;
while(q<a[ubq])ubq--;
if(q> b[ubq]){
q= b[ubq],p= a[s]-q;
if(a[s-1]==b[s-1])goto doublecheck;
}
}

/*:14*/
#line 274 "achain4.w"
;
}
goto backup;
onward:continue;
}
/*7:*/
#line 167 "achain4.w"

for(j= 1;j*n<nmax;j++)upbound(j*n,l[j]+l[n]);
for(i= 0;i<lb;i++)for(j= 1;j*n+a[i]<nmax;j++)
upbound(j*n+a[i],l[j]+l[n]+1);
while(top> 1){
p= top,top= link[p],link[p]= 0;
upbound(p+1,l[p]+1);
upbound(p+2,l[p]+2);
for(i= 2;i*p<nmax;i++)upbound(i*p,l[i]+l[p]);
}

/*:7*/
#line 279 "achain4.w"
;
goto backtrackdone;
impossible:

/*:12*/
#line 193 "achain4.w"
;
l[n]= ++lb;
}
backtrackdone:

/*:8*/
#line 56 "achain4.w"
;
if(n%1000==0){
j= clock();
printf("%d..%d done in %.5g minutes\n",
n-999,n,(double)(j-timer)/(60*CLOCKS_PER_SEC));
timer= j;
}
done:/*11:*/
#line 231 "achain4.w"

if(l[n]<l[n-1]){
for(p= down[n];l[p]> l[n];p= q)
q= down[p],down[p]= n;
down[n]= p;
}

/*:11*/
#line 63 "achain4.w"
;
/*3:*/
#line 84 "achain4.w"

fprintf(outfile,"%c",l[n]+' ');
fflush(outfile);

/*:3*/
#line 64 "achain4.w"
;
}
}

/*:1*/
