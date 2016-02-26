#define n 100
#define memsize 1000000 \

#define grandtotal 0
#define subtotal 1
#define zero 2
#define one 3 \

/*1:*/
#line 24 "hwb-fast.w"

#include <stdio.h> 
#include <stdlib.h> 
char mem[memsize];
int memptr;
int numptr;
int start[n*n];
int bico[n][n];
int addedA[n],addedB[n],addedC[n],addedD[n];
unsigned char rev[256];
int perm[n+1];
int nonbeads;
int tnonbeads;
/*3:*/
#line 88 "hwb-fast.w"

void printnum(int k){
register int j;
for(j= start[k+1]-1;j> start[k];j--)if(mem[j])break;
for(;j>=start[k];j--)printf("%d",mem[j]);
}

/*:3*//*5:*/
#line 101 "hwb-fast.w"

void clearnum(int k){
register int j;
for(j= start[k];j<start[k+1];j++)mem[j]= 0;
}

/*:5*//*6:*/
#line 112 "hwb-fast.w"

int add(int k,int l){
register int c,i,j;
if(memptr+n>=memsize){
fprintf(stderr,"Out of memory (memsize=%d)!\n",memsize);
exit(-1);
}
for(c= 0,i= start[k],j= start[l];;i++,j++,memptr++){
if(i<start[k+1]){
if(j<start[l+1])mem[memptr]= mem[i]+mem[j]+c;
else mem[memptr]= mem[i]+c;
}else{
if(j<start[l+1])mem[memptr]= mem[j]+c;
else break;
}
if(mem[memptr]>=10)c= 1,mem[memptr]-= 10;
else c= 0;
}
if(c)mem[memptr++]= 1;
numptr++;
start[numptr+1]= memptr;
return numptr;
}

/*:6*//*7:*/
#line 140 "hwb-fast.w"

void addto(int k,int l){
register c,i,j;
for(c= 0,i= start[k],j= start[l];i<start[k+1];i++,j++){
mem[i]+= (j<start[l+1]?mem[j]:0)+c;
if(mem[i]>=10)c= 1,mem[i]-= 10;
else c= 0;
}

}

/*:7*//*9:*/
#line 168 "hwb-fast.w"

int binom(int m,int k){
if(k<0||k> m)return zero;
if(k==0||k==m)return one;
if(!bico[m][k])bico[m][k]= add(binom(m-1,k),binom(m-1,k-1));
return bico[m][k];
}

/*:9*/
#line 37 "hwb-fast.w"


main(int argc){
register int i,j,k,m,p,s,ss,t,tt,w,ww;
/*2:*/
#line 63 "hwb-fast.w"

for(j= 0x80,m= 1;j;j>>= 1,m<<= 1)
for(k= 0;k<0x100;k+= j+j)rev[k+j]= rev[k]+m;
for(j= m= 1,k= n;j<=n/10;j++,k--,m+= 2)
perm[k]= m,perm[j]= m+1;
for(i= 0;m<=n;i++,m++){
while(rev[i]> k-j)i++;
perm[k-rev[i]]= m;
}
printf("Starting from perm");
for(j= 1;j<=n;j++)printf(" %d",perm[j]);
printf("\n");
perm[0]= n+1;

/*:2*/
#line 41 "hwb-fast.w"
;
/*8:*/
#line 158 "hwb-fast.w"

start[grandtotal]= 0;mem[0]= 2;
start[subtotal]= start[grandtotal]+n;
start[zero]= start[subtotal]+n,start[zero+1]= start[zero]+1;
mem[start[one]]= 1,start[one+1]= start[one]+1;
numptr= one,memptr= start[numptr+1];

/*:8*/
#line 42 "hwb-fast.w"
;
for(k= 0;k<n;k++){
/*10:*/
#line 178 "hwb-fast.w"

clearnum(subtotal);
nonbeads= 0;
m= n-k;
/*14:*/
#line 271 "hwb-fast.w"

for(j= 0;j<m;j++)addedA[j]= addedB[j]= addedC[j]= addedD[j]= 0;

/*:14*/
#line 182 "hwb-fast.w"
;
for(s= 0;s<=k;s++){
/*12:*/
#line 232 "hwb-fast.w"

for(w= ww= j= 0;j<=m;j++)if(perm[s+j]<=k){
w++;
if(j> 0&&j<m)ww++;
}
if(ww==m-1)/*15:*/
#line 276 "hwb-fast.w"

{
for(t= k-s,ss= s,tt= w-ss;tt<=t;ss--,tt++)if(ss>=0&&tt>=0){
if(perm[s+m]<=k){
if(!addedA[ss])
addedA[ss]= 1,addto(subtotal,binom(m-1,ss));
if(ss> 0&&!addedB[ss-1])
addedB[ss-1]= 1,addto(subtotal,binom(m-1,ss-1));
}else if(!addedB[ss])
addedB[ss]= 1,addto(subtotal,binom(m-1,ss));
if(ss> 0&&perm[s]<=k){
if(perm[s+m]<=k){
if(!addedC[ss-1])
addedC[ss-1]= 1,addto(subtotal,binom(m-1,ss-1));
if(ss> 1&&!addedD[ss-2])
addedD[ss-2]= 1,addto(subtotal,binom(m-1,ss-2));
}else if(!addedD[ss-1])
addedD[ss-1]= 1,addto(subtotal,binom(m-1,ss-1));
}
}
}

/*:15*/
#line 237 "hwb-fast.w"

else{
/*13:*/
#line 252 "hwb-fast.w"

p= n+1;
if(ww==m-2){
if(m==2&&perm[s+1]==n)
p= (perm[s+2]<=k?1:0);
else if(w==m){
for(p= 1;;p++)if(perm[s+p]> k)break;
if(perm[s+p]!=k+1)p= n+1;
}
}

/*:13*/
#line 239 "hwb-fast.w"
;
for(t= k-s,ss= s,tt= w-ss;tt<=t;ss--,tt++){
addto(subtotal,binom(w,ss));
if(ss==p)nonbeads++;
}
}

/*:12*/
#line 184 "hwb-fast.w"
;
}
/*16:*/
#line 298 "hwb-fast.w"

if(addedA[0])nonbeads++;
if(addedD[m-1])nonbeads++;

/*:16*/
#line 186 "hwb-fast.w"
;

/*:10*/
#line 44 "hwb-fast.w"
;
/*11:*/
#line 188 "hwb-fast.w"

printf("height %d: ",m);
printnum(subtotal);
if(nonbeads)printf("-%d\n",nonbeads);
else printf("\n");
addto(grandtotal,subtotal);
tnonbeads+= nonbeads;

/*:11*/
#line 45 "hwb-fast.w"
;
}
printf("height 0: 2\n");
/*4:*/
#line 95 "hwb-fast.w"

printf("Altogether ");
printnum(0);
printf("-%d nodes; I used %d bytes of memory for %d numbers.\n",
tnonbeads,memptr,numptr);

/*:4*/
#line 48 "hwb-fast.w"
;
}

/*:1*/
