#define m 12
#define n 100000000 \

#define pack(a,b,c,d) (((a) *3+(b) ) *3+(c) ) *3+(d)  \

/*1:*/
#line 9 "./setset-random.w"

#include <stdio.h> 
#include "gb_flip.h" 
char deck[81];
char occ[81];
char z[3][3]= {{0,2,1},{2,1,0},{1,0,2}};
char third[81][128];

main()
{
register int j,k,t;
int reps,count;
/*2:*/
#line 34 "./setset-random.w"

gb_init_rand(0);
for(k= 0;k<81;k++)deck[k]= k;
for(k= 81-m;k<81;k++)occ[k]= 1;
{
int a,b,c,d,e,f,g,h;
for(a= 0;a<3;a++)for(b= 0;b<3;b++)for(c= 0;c<3;c++)for(d= 0;d<3;d++)
for(e= 0;e<3;e++)for(f= 0;f<3;f++)for(g= 0;g<3;g++)for(h= 0;h<3;h++)
third[pack(a,b,c,d)][pack(e,f,g,h)]= pack(z[a][e],z[b][f],z[c][g],z[d][h]);
}

/*:2*/
#line 21 "./setset-random.w"
;
while(1){
count= 0;
for(reps= 0;reps<n;reps++){
/*4:*/
#line 51 "./setset-random.w"

for(j= 81;j> 81-m;j--){
t= deck[j-1];
occ[t]= 0;
k= gb_unif_rand(j);
deck[j-1]= deck[k];
occ[deck[k]]= 1;
deck[k]= t;
}

/*:4*/
#line 25 "./setset-random.w"
;
/*3:*/
#line 45 "./setset-random.w"

for(j= 81-m;j<80;j++)for(k= j+1;k<81;k++)
if(occ[third[deck[j]][deck[k]]])goto set_found;
count++;
set_found:

/*:3*/
#line 26 "./setset-random.w"
;
}
printf("%d/%d\n",count,n);
}
}

/*:1*/
