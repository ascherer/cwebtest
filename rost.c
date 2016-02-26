/*1:*/
#line 9 "./rost.w"

#include <stdio.h> 
#include <math.h> 
#include "gb_flip.h"

char*bit;
int*list;
int seed;
int n;

main(argc,argv)
int argc;char*argv[];
{
register int i,j,k,l,t,u,r;
/*2:*/
#line 29 "./rost.w"

if(argc!=3||sscanf(argv[1],"%d",&n)!=1||sscanf(argv[2],"%d",&seed)!=1){
fprintf(stderr,"Usage: %s n seed >! output.ps\n",argv[0]);
exit(-1);
}
/*:2*/
#line 23 "./rost.w"
;
/*3:*/
#line 38 "./rost.w"

gb_init_rand(seed);
bit= (char*)malloc(2*n+2);
list= (int*)malloc(4*n+4);
for(k= 0;k<=n;k++)bit[k]= 1;
for(;k<=n+n+1;k++)bit[k]= 0;
l= u= n;
list[0]= n;
t= 1;

/*:3*/
#line 24 "./rost.w"
;
for(r= 0;r<n;r++)/*4:*/
#line 48 "./rost.w"

{
j= gb_unif_rand(t);
i= list[j];
t--;
list[j]= list[t];
bit[i]= 0;bit[i+1]= 1;
if(i==l)l--;
if(i==u)u++;
if(bit[i-1])list[t++]= i-1;
if(!bit[i+2])list[t++]= i+1;
}

/*:4*/
#line 25 "./rost.w"
;
/*5:*/
#line 61 "./rost.w"

/*6:*/
#line 67 "./rost.w"

printf("%%!PS\n");
printf("%%%%BoundingBox: -1 -1 361 361\n");
printf("%%%%Creator: %s %s %s\n",argv[0],argv[1],argv[2]);
printf("/d {0 s neg rlineto} bind def\n");
printf("/r {s 0 rlineto} bind def\n");

/*:6*/
#line 62 "./rost.w"
;
/*8:*/
#line 79 "./rost.w"

printf("/s %g def\n",360.0/sqrt(6.0*n));
printf("newpath %d %d s mul moveto\n",0,n-l);
for(k= l+1;k<=u;k++){
if(bit[k])printf(" d");else printf(" r");
if((k-l)%40==0)printf("\n");
}
printf("\n0 0 lineto closepath\n");
printf("1 setlinewidth stroke\n");

/*:8*/
#line 63 "./rost.w"
;
/*9:*/
#line 94 "./rost.w"

printf("newpath 0 360 moveto 0 120 120 0 360 0 curveto\n");
printf(" 0 0 lineto closepath\n");
printf(".3 setlinewidth stroke\n");

/*:9*/
#line 64 "./rost.w"
;
/*7:*/
#line 74 "./rost.w"

printf("showpage\n");

/*:7*/
#line 65 "./rost.w"
;

/*:5*/
#line 26 "./rost.w"
;
}

/*:1*/
