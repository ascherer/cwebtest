#define nmax 10000000 \

#define harmless 128 \

/*1:*/
#line 32 "achain0.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
char l[nmax];
int a[129];
struct{
int lbp,ubp,lbq,ubq,savep;
}stack[128];
FILE*infile,*outfile;
int prime[1000];
int pr;
char x[64];
int main(int argc,char*argv[])
{
register int i,j,p,q,n,s,ubp,ubq,lbp,lbq;
int lb,ub,timer= 0;
/*2:*/
#line 66 "achain0.w"

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
#line 49 "achain0.w"
;
prime[0]= 2,pr= 1;
a[0]= 1,a[1]= 2;
for(n= 1;n<nmax;n++){
/*4:*/
#line 90 "achain0.w"

for(q= n,i= -1,j= 0;q;q>>= 1,i++)
if(q&1)x[j++]= i;

lb= fgetc(infile)-' ';
if(lb<i+3)lb= i+3;

/*:4*/
#line 53 "achain0.w"
;
/*5:*/
#line 112 "achain0.w"

ub= i+j-1;
if(ub> l[n-1]+1)ub= l[n-1]+1;
/*6:*/
#line 131 "achain0.w"

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
#line 115 "achain0.w"
;
l[n]= ub;
if(j<=3)goto done;
if(j==4){
p= x[3]-x[2],q= x[1]-x[0];
if(p==q||p==q+1||(q==1&&(p==3||(p==5&&x[2]==x[1]+1))))l[n]= i+2;
goto done;
}

/*:5*/
#line 54 "achain0.w"
;
/*7:*/
#line 154 "achain0.w"

while(lb<ub){
a[ub-1]= n;
for(i= 2;i<ub-1;i++)a[i]= 0;
/*8:*/
#line 204 "achain0.w"

for(s= ub-1;s> 1;s--)if(a[s]){
for(q= a[s]>>1,p= a[s]-q;q;p++,q--){
/*9:*/
#line 243 "achain0.w"

lbp= l[p];
if(lbp<=1)goto p_ready;
if(lbp>=ub)goto p_hopeless;
p_search:while(a[lbp]<p){
if(a[lbp]==0)goto p_empty_slot;
lbp++;
}
if(a[lbp]==p)goto p_ready;
p_hopeless:ubp= lbp-1;goto p_done;
p_empty_slot:for(j= lbp-1;a[j]==0;j--);
i= a[j];
if(p<i)goto p_hopeless;
for(i+= i,j++;j<lbp;j++)i+= i;
while(p> i){
lbp++;
if(a[lbp])goto p_search;
i+= i;
}
for(j= lbp+1;a[j]==0;j++);
i= a[j];
if(i<=p){
if(i<p){
lbp= j+1;goto p_search;
}
p_ready:ubp= lbp= harmless;goto tryq;
}
for(ubp= j-1,i= (i+1)>>1;p<i;ubp--)i= (i+1)>>1;
p_done:

/*:9*/
#line 208 "achain0.w"
;
if(ubp==s-1&&lbp<ubp)lbp= ubp;
for(;ubp>=lbp;ubp--){
a[ubp]= p;
tryq:/*10:*/
#line 277 "achain0.w"

lbq= l[q];
if(lbq>=ub)goto q_hopeless;
if(lbq<=1)goto q_ready;
q_search:while(a[lbq]<q){
if(a[lbq]==0)goto q_empty_slot;
lbq++;
}
if(a[lbq]==q)goto q_ready;
q_hopeless:ubq= lbq-1;goto q_done;
q_empty_slot:for(j= lbq-1;a[j]==0;j--);
i= a[j];
if(q<i)goto q_hopeless;
for(i+= i,j++;j<lbq;j++)i+= i;
while(q> i){
lbq++;
if(a[lbq])goto q_search;
i+= i;
}
for(j= lbq+1;a[j]==0;j++);
i= a[j];
if(i<=q){
if(i<q){
lbq= j+1;goto q_search;
}
q_ready:ubq= lbq= harmless;goto happiness;
}
for(ubq= j-1,i= (i+1)>>1;q<i;ubq--)i= (i+1)>>1;
q_done:


/*:10*/
#line 213 "achain0.w"
;
for(;ubq>=lbq;ubq--){
a[ubq]= q;
happiness:stack[s].savep= p;
stack[s].lbp= lbp,stack[s].ubp= ubp;
stack[s].lbq= lbq,stack[s].ubq= ubq;
goto onward;
backup:s++;
if(s==ub)goto done;
if(a[s]==0)goto backup;
lbq= stack[s].lbq,ubq= stack[s].ubq;
lbp= stack[s].lbp,ubp= stack[s].ubp;
p= stack[s].savep,q= a[s]-p;
a[ubq]= 0;
}
a[ubp]= 0;
}
}
goto backup;
onward:continue;
}

/*:8*/
#line 158 "achain0.w"
;
l[n]= --ub;
}

/*:7*/
#line 55 "achain0.w"
;
done:/*3:*/
#line 82 "achain0.w"

fprintf(outfile,"%c",l[n]+' ');
fflush(outfile);

/*:3*/
#line 56 "achain0.w"
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
