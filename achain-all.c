#define nmax 10000000 \

#define newa(j,y) undo[ptr++]= (j<<24) +a[j],a[j]= y
#define newb(j,y) undo[ptr++]= (1<<31) +(j<<24) +b[j],b[j]= y \

/*1:*/
#line 11 "achain-all.w"

#include <stdio.h> 
unsigned char l[nmax];
int a[128],b[128];
unsigned int undo[128*128];
int ptr;
struct{
int lbp,ubp,lbq,ubq,r,ptrp,ptrq;
}stack[128];
int tail[128],outdeg[128],outsum[128],limit[128];
int down[nmax];
FILE*infile;
main(int argc,char*argv[])
{
register int i,j,n,p,q,r,s,ubp,ubq= 0,lbp,lbq,ptrp,ptrq;
int lb,nn;
/*2:*/
#line 36 "achain-all.w"

if(argc!=3){
fprintf(stderr,"Usage: %s infile n\n",argv[0]);
exit(-1);
}
infile= fopen(argv[1],"r");
if(!infile){
fprintf(stderr,"I couldn't open `%s' for reading!\n",argv[1]);
exit(-2);
}
if(sscanf(argv[2],"%d",&nn)!=1||nn<3||nn>=nmax){
fprintf(stderr,"The number `%s' was supposed to be between 3 and %d!\n",
argv[2],nmax-1);
exit(-3);
}

/*:2*/
#line 27 "achain-all.w"
;
/*7:*/
#line 105 "achain-all.w"

for(n= 1;n<=nn;n++)down[n]= n-1;

/*:7*/
#line 28 "achain-all.w"
;
for(n= 1;n<=nn;n++){
/*3:*/
#line 52 "achain-all.w"

lb= fgetc(infile)-' ';
if(lb<0||(n> 1&&lb> l[n-1]+1)){
fprintf(stderr,"Input file has the wrong value (%d) for l[%d]!\n",lb,n);
exit(-4);
}
l[n]= lb;

/*:3*/
#line 30 "achain-all.w"
;
/*8:*/
#line 110 "achain-all.w"

if(l[n]<l[n-1]){
for(p= down[n];l[p]> l[n];p= q)
q= down[p],down[p]= n;
down[n]= p;
}

/*:8*/
#line 31 "achain-all.w"
;
}
/*4:*/
#line 62 "achain-all.w"

a[0]= b[0]= 1,a[1]= b[1]= 2;
n= nn,lb= l[n];
for(i= 0;i<=lb;i++)outdeg[i]= outsum[i]= 0;
a[lb]= b[lb]= n;
for(i= 2;i<lb;i++)a[i]= a[i-1]+1,b[i]= b[i-1]<<1;
for(i= lb-1;i>=2;i--){
if((a[i]<<1)<a[i+1])a[i]= (a[i+1]+1)>>1;
if(b[i]>=b[i+1])b[i]= b[i+1]-1;
}
/*9:*/
#line 117 "achain-all.w"

ptr= 0;
for(r= s= lb;s> 2;s--){
if(outdeg[s]==1)
limit[s]= a[s]-tail[outsum[s]];else limit[s]= a[s]-1;

if(limit[s]> b[s-1])limit[s]= b[s-1];
/*10:*/
#line 161 "achain-all.w"

if(a[s]&1){
unequal:if(outdeg[s-1]==0)q= a[s]/3;else q= a[s]>>1;
if(q> b[s-2])q= b[s-2];
p= a[s]-q;
if(l[p]>=s){
/*6:*/
#line 97 "achain-all.w"

do{
if(down[p]> p)p= down[p];
else{
p= nmax;break;
}
}while(l[p]>=s);

/*:6*/
#line 167 "achain-all.w"
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

/*:10*/
#line 124 "achain-all.w"
;
while(p<=limit[s]){
/*14:*/
#line 235 "achain-all.w"

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

/*:14*/
#line 128 "achain-all.w"
;
ptrp= ptr;
for(;ubp>=lbp;ubp--){
/*16:*/
#line 277 "achain-all.w"

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
/*17:*/
#line 325 "achain-all.w"

i= p-(1<<(ubp-1));
if(i&&((i&(i-1))==0)&&(i<<4)<p){
for(j= ubp-2;(i&1)==0;i>>= 1,j--);
if(b[j]<(1<<j))goto failp;
for(;a[j]<(1<<j);j--)newa(j,1<<j);
}

/*:17*/
#line 304 "achain-all.w"
;

/*:16*/
#line 132 "achain-all.w"
;
if(p==q)goto happiness;
if(ubq>=ubp)ubq= ubp-1;
ptrq= ptr;
for(;ubq>=lbq;ubq--){
/*18:*/
#line 337 "achain-all.w"

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
/*19:*/
#line 368 "achain-all.w"

i= q-(1<<(ubq-1));
if(i&&((i&(i-1))==0)&&(i<<4)<q){
for(j= ubq-2;(i&1)==0;i>>= 1,j--);
if(b[j]<(1<<j))goto failq;
for(;a[j]<(1<<j);j--)newa(j,1<<j);
}

/*:19*/
#line 366 "achain-all.w"
;

/*:18*/
#line 138 "achain-all.w"
;
happiness:/*12:*/
#line 217 "achain-all.w"

tail[s]= q,stack[s].r= r;
outdeg[ubp]++,outsum[ubp]+= s;
outdeg[ubq]++,outsum[ubq]+= s;
stack[s].lbp= lbp,stack[s].ubp= ubp;
stack[s].lbq= lbq,stack[s].ubq= ubq;
stack[s].ptrp= ptrp,stack[s].ptrq= ptrq;

/*:12*/
#line 139 "achain-all.w"
;
goto onward;
backup:s++;
if(s> lb)goto impossible;
/*13:*/
#line 225 "achain-all.w"

ptrq= stack[s].ptrq,ptrp= stack[s].ptrp;
lbq= stack[s].lbq,ubq= stack[s].ubq;
lbp= stack[s].lbp,ubp= stack[s].ubp;
outdeg[ubq]--,outsum[ubq]-= s;
outdeg[ubp]--,outsum[ubp]-= s;
q= tail[s],p= a[s]-q,r= stack[s].r;

/*:13*/
#line 143 "achain-all.w"
;
if(p==q)goto failp;
failq:while(ptr> ptrq)/*15:*/
#line 268 "achain-all.w"

{
i= undo[--ptr];
if(i>=0)a[i>>24]= i&0xffffff;
else b[(i&0x3fffffff)>>24]= i&0xffffff;
}

/*:15*/
#line 145 "achain-all.w"
;
}
failp:while(ptr> ptrp)/*15:*/
#line 268 "achain-all.w"

{
i= undo[--ptr];
if(i>=0)a[i>>24]= i&0xffffff;
else b[(i&0x3fffffff)>>24]= i&0xffffff;
}

/*:15*/
#line 147 "achain-all.w"
;
}
failpq:/*11:*/
#line 184 "achain-all.w"

if(p==q){
if(outdeg[s-1]==0)q= (a[s]/3)+1;
if(q> b[s-2])q= b[s-2];else q--;
p= a[s]-q;
if(l[p]>=s){
/*6:*/
#line 97 "achain-all.w"

do{
if(down[p]> p)p= down[p];
else{
p= nmax;break;
}
}while(l[p]>=s);

/*:6*/
#line 190 "achain-all.w"
;
q= a[s]-p;
}
}else{
/*5:*/
#line 90 "achain-all.w"

{
p++;
if(l[p]==s)
p= (down[p]> p?down[p]:nmax);
}

/*:5*/
#line 194 "achain-all.w"
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

/*:11*/
#line 150 "achain-all.w"
;
}
goto backup;
onward:continue;
}
/*20:*/
#line 376 "achain-all.w"

for(j= 0;j<=lb;j++)printf(" %d",a[j]);
printf("\n");

/*:20*/
#line 155 "achain-all.w"
;
goto backup;
impossible:

/*:9*/
#line 72 "achain-all.w"
;

/*:4*/
#line 33 "achain-all.w"
;
}

/*:1*/
