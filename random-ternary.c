#define nmax 1000 \

#define sanity_checking 1 \

/*1:*/
#line 28 "./random-ternary.w"

#include <stdio.h> 
#include <stdlib.h> 
#include "gb_flip.h" 
int nn,seed;
int L[nmax],P[nmax];
main(int argc,char*argv[]){
register int i,j,k,n,nnn,p,q,r,x;
/*2:*/
#line 43 "./random-ternary.w"

if(argc!=3||sscanf(argv[1],"%d",&nn)!=1||
sscanf(argv[2],"%d",&seed)!=1){
fprintf(stderr,"Usage: %s n seed\n",argv[0]);
exit(-1);
}
gb_init_rand(seed);

/*:2*/
#line 36 "./random-ternary.w"
;
for(n= L[0]= P[0]= 0;n<nn;)
/*3:*/
#line 53 "./random-ternary.w"

{
n++;
nnn= 3*n;
x= gb_unif_rand(3*(nnn-1)*(nnn-2));
p= nnn-(x%3);
q= nnn-((x+1)%3);
r= nnn-((x+2)%3);
k= ((int)(x/3))%(nnn-2);
j= (int)(x/(9*n-6));
L[p]= nnn,P[nnn]= p;
L[q]= L[k],P[L[k]]= q,L[k]= nnn-1,P[nnn-1]= k;
/*5:*/
#line 90 "./random-ternary.w"

for(i= k+1-((k+2)%3);i> 0&&i!=j;i= P[i]+1-((P[i]+2)%3));
if(i> 0)/*6:*/
#line 104 "./random-ternary.w"

{
L[k]= nnn-2,P[nnn-2]= k;
i= P[j];
L[i]= nnn-1,P[nnn-1]= i,L[r]= j,P[j]= r;
}

/*:6*/
#line 92 "./random-ternary.w"

else{
if(j==L[q]){
L[r]= L[q],P[L[q]]= r,L[q]= nnn-2,P[nnn-2]= q;
}else if(j==nnn-2){
L[r]= nnn-2,P[nnn-2]= r;
}else L[P[j]]= nnn-2,P[nnn-2]= P[j],L[r]= j,P[j]= r;
}

/*:5*/
#line 65 "./random-ternary.w"
;
if(sanity_checking){
for(i= 0;i<=nnn;i++)if(P[L[i]]!=i){
fprintf(stderr,"(whoa---the links are fouled up!)\n");
exit(-2);
}
}
}

/*:3*/
#line 38 "./random-ternary.w"
;
for(k= 0;k<=3*nn;k++)printf(" %d",L[k]);
printf("\n");
}

/*:1*/
