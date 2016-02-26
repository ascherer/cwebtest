/*1:*/
#line 19 "ran-prim.w"

#include <stdio.h> 
#include <math.h> 
#include "gb_flip.h"

int*perm;
int*list;
int seed;
int n;

main(argc,argv)
int argc;char*argv[];
{
register int i,j,k,t,x,y;
register double s;
/*2:*/
#line 40 "ran-prim.w"

if(argc!=3||sscanf(argv[1],"%d",&n)!=1||sscanf(argv[2],"%d",&seed)!=1){
fprintf(stderr,"Usage: %s n seed\n",argv[0]);
exit(-1);
}

/*:2*/
#line 34 "ran-prim.w"
;
/*3:*/
#line 49 "ran-prim.w"

gb_init_rand(seed);
perm= (int*)malloc(4*(n+2));
list= (int*)malloc(4*(n-1));
for(k= 1;k<=n;k++)perm[k]= n+1-k;
perm[0]= 0;perm[n+1]= n+1;
for(k= 1;k<n;k++)list[k-1]= k;
t= n-1;
s= 0.0;

/*:3*/
#line 35 "ran-prim.w"
;
while(t)/*4:*/
#line 59 "ran-prim.w"

{
s+= 1.0/(double)t;
j= gb_unif_rand(t);
i= list[j];
t--;
list[j]= list[t];
x= perm[i];y= perm[i+1];
perm[i]= y;perm[i+1]= x;
if(perm[i-1]> y&&perm[i-1]<x)list[t++]= i-1;
if(perm[i+2]<x&&perm[i+2]> y)list[t++]= i+1;
}

/*:4*/
#line 36 "ran-prim.w"
;
/*5:*/
#line 74 "ran-prim.w"

printf("%g = %gn\n",s,s/(double)n);

/*:5*/
#line 37 "ran-prim.w"
;
}

/*:1*/
