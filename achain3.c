#define nmax (1<<20)  \

#define newa(j,y) undo[ptr++]= (j<<24) +a[j],a[j]= y
#define newb(j,y) undo[ptr++]= (1<<31) +(j<<24) +b[j],b[j]= y \

/*1:*/
#line 35 "achain3.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
char l[20][nmax];
int a[128],b[128];
unsigned int undo[128*128];
int ptr;
struct{
int lbp,lbq,ubq,r,ptrp,ptrq;
}stack[128];
int tail[128],outdeg[128],outsum[128],limit[128];
FILE*infile[64],*outfile[64];
char buf[100];
int main(int argc,char*argv[])
{
register int i,j,n,p,q,r,s,ubq,lbp,lbq,ptrp,ptrq;
int lg2n,kk,lb,ub,timer= 0;
/*2:*/
#line 71 "achain3.w"

if(argc!=3){
fprintf(stderr,"Usage: %s foo bar\n",argv[0]);
exit(-1);
}

/*:2*/
#line 53 "achain3.w"
;
a[0]= b[0]= 1,a[1]= b[1]= 2;
for(n= 2;n<nmax;n++){
/*5:*/
#line 105 "achain3.w"

for(q= n,i= -1,j= 0;q;q>>= 1,i++)
j+= q&1;
lg2n= i,ub= i+j-1;

/*:5*/
#line 56 "achain3.w"
;
for(kk= lg2n;kk;kk--){
/*4:*/
#line 92 "achain3.w"

if(!infile[kk]){
sprintf(buf,"%s-%d",argv[1],kk);
infile[kk]= fopen(buf,"r");
if(!infile[kk])infile[kk]= (FILE*)1;
}
if(infile[kk]!=(FILE*)1){
l[kk][n]= fgetc(infile[kk])-' ';
if(l[kk][n]<=0)infile[kk]= (FILE*)1;

goto done;
}

/*:4*/
#line 58 "achain3.w"
;
/*6:*/
#line 128 "achain3.w"

loop:l[kk][n]= ub;
if(kk==lg2n)goto done;
lb= ub-1;

if(lb<=kk+1)goto done;
for(i= 0;i<=lb;i++)outdeg[i]= outsum[i]= 0;
a[lb]= b[lb]= n;
for(i= 2;i<=kk;i++)a[i]= b[i]= 1<<i;
a[i]= a[kk]+1,b[i]= (1<<i)-1;
for(i++;i<lb;i++)a[i]= a[i-1]+1,b[i]= b[i-1]<<1;
for(i= lb-1;i> kk;i--){
if((a[i]<<1)<a[i+1])a[i]= (a[i+1]+1)>>1;
if(b[i]>=b[i+1])b[i]= b[i+1]-1;
}
if(a[lb-1]> b[lb-1])goto done;
/*7:*/
#line 155 "achain3.w"

ptr= 0;
for(r= s= lb;s> kk;s--){
if(outdeg[s]==0&&s<lb)goto backup;
if(outdeg[s]==1)limit[s]= tail[outsum[s]];else limit[s]= 1;
for(;r> 1&&a[r-1]==b[r-1];r--);
if(outdeg[s-1]==0&&(a[s]&1))q= a[s]/3;else q= a[s]>>1;
for(p= a[s]-q;p<=b[s-1];){
if(p> b[r-1]){
while(p> a[r])r++;
p= a[r],q= a[s]-p,r++;
}
if(q<limit[s])goto backup;
/*10:*/
#line 221 "achain3.w"

lbp= l[kk][p];
if(lbp>=lb)goto failpq;
while(b[lbp]<p)lbp++;
if(a[lbp]> p)goto failpq;
for(ubq= lbp;a[ubq+1]<=p;ubq++);
if(ubq==s-1)lbp= ubq;
if(p==q)lbq= lbp,ubq= ubq;
else{
lbq= l[kk][q];
if(lbq>=ubq)goto failpq;
while(b[lbq]<q)lbq++;
if(lbq>=ubq)goto failpq;
if(a[lbq]> q)goto failpq;
for(ubq= lbq;a[ubq+1]<=q&&ubq+1<ubq;ubq++);
if(lbp==lbq)lbp++;
}

/*:10*/
#line 170 "achain3.w"
;
ptrp= ptr;
for(;ubq>=lbp;ubq--){
/*12:*/
#line 255 "achain3.w"

if(a[ubq]!=p){
newa(ubq,p);
for(j= ubq-1;(a[j]<<1)<a[j+1];j--){
i= (a[j+1]+1)>>1;
if(i> b[j])goto failp;
newa(j,i);
}
for(j= ubq+1;a[j]<=a[j-1];j++){
i= a[j-1]+1;
if(i> b[j])goto failp;
newa(j,i);
}
}
if(b[ubq]!=p){
newb(ubq,p);
for(j= ubq-1;b[j]>=b[j+1];j--){
i= b[j+1]-1;
if(i<a[j])goto failp;
newb(j,i);
}
for(j= ubq+1;b[j]> b[j-1]<<1;j++){
i= b[j-1]<<1;
if(i<a[j])goto failp;
newb(j,i);
}
}
/*13:*/
#line 300 "achain3.w"

i= p-(1<<(ubq-1));
if(i&&((i&(i-1))==0)&&(i<<4)<p){
for(j= ubq-2;(i&1)==0;i>>= 1,j--);
if(b[j]<(1<<j))goto failp;
for(;a[j]<(1<<j);j--)newa(j,1<<j);
}

/*:13*/
#line 282 "achain3.w"
;

/*:12*/
#line 174 "achain3.w"
;
if(p==q)goto happiness;
if(ubq>=ubq)ubq= ubq-1;
ptrq= ptr;
for(;ubq>=lbq;ubq--){
/*14:*/
#line 312 "achain3.w"

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
/*15:*/
#line 343 "achain3.w"

i= q-(1<<(ubq-1));
if(i&&((i&(i-1))==0)&&(i<<4)<q){
for(j= ubq-2;(i&1)==0;i>>= 1,j--);
if(b[j]<(1<<j))goto failq;
for(;a[j]<(1<<j);j--)newa(j,1<<j);
}

/*:15*/
#line 341 "achain3.w"
;

/*:14*/
#line 180 "achain3.w"
;
happiness:/*8:*/
#line 203 "achain3.w"

tail[s]= q,stack[s].r= r;
outdeg[ubq]++,outsum[ubq]+= s;
outdeg[ubq]++,outsum[ubq]+= s;
stack[s].lbp= lbp,stack[s].ubq= ubq;
stack[s].lbq= lbq,stack[s].ubq= ubq;
stack[s].ptrp= ptrp,stack[s].ptrq= ptrq;

/*:8*/
#line 181 "achain3.w"
;
goto onward;
backup:s++;
if(s> lb)goto done;
/*9:*/
#line 211 "achain3.w"

ptrq= stack[s].ptrq,ptrp= stack[s].ptrp;
lbq= stack[s].lbq,ubq= stack[s].ubq;
lbp= stack[s].lbp,ubq= stack[s].ubq;
outdeg[ubq]--,outsum[ubq]-= s;
outdeg[ubq]--,outsum[ubq]-= s;
q= tail[s],p= a[s]-q,r= stack[s].r;

/*:9*/
#line 185 "achain3.w"
;
if(p==q)goto failp;
failq:while(ptr> ptrq)/*11:*/
#line 246 "achain3.w"

{
i= undo[--ptr];
if(i>=0)a[i>>24]= i&0xffffff;
else b[(i&0x3fffffff)>>24]= i&0xffffff;
}

/*:11*/
#line 187 "achain3.w"
;
}
failp:while(ptr> ptrp)/*11:*/
#line 246 "achain3.w"

{
i= undo[--ptr];
if(i>=0)a[i>>24]= i&0xffffff;
else b[(i&0x3fffffff)>>24]= i&0xffffff;
}

/*:11*/
#line 189 "achain3.w"
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
possible:

/*:7*/
#line 144 "achain3.w"
;
ub= lb;
goto loop;

/*:6*/
#line 59 "achain3.w"
;
done:/*3:*/
#line 77 "achain3.w"

if(!outfile[kk]){
sprintf(buf,"%s-%d",argv[2],kk);
outfile[kk]= fopen(buf,"w");
if(!outfile[kk]){
fprintf(stderr,"Can't open file `%s' for writing!\n",buf);
exit(-2);
}
}
fprintf(outfile[kk],"%c",l[kk][n]+' ');
fflush(outfile[kk]);

/*:3*/
#line 60 "achain3.w"
;
}
if(n%1000==0){
j= clock();
printf("%d..%d done in %.5g minutes\n",
n-999,n,(double)(j-timer)/(60*CLOCKS_PER_SEC));
timer= j;
}
}
}

/*:1*/
