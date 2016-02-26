/*1:*/
#line 14 "sand.w"

#include "gb_graph.h"
#include "gb_save.h"

#line 17 "sand.w"

int vec[1000][1000];
int x[1000],d[1000],t[1000];
int n,r;
/*4:*/
#line 66 "sand.w"

void reduce()
{
register int j,k,h;
while(1){
h= 0;
for(j= 1;j<n;j++)if(x[j]>=d[j]){
h= 1;
for(k= 1;k<n;k++)x[k]-= vec[j][k];
}
if(h==0)break;
}
}

/*:4*/
#line 21 "sand.w"

main(int argc,char*argv[])
{
register int j,k;
Vertex*v;
Arc*a;
Graph*g;
/*2:*/
#line 33 "sand.w"

if(argc<2){
fprintf(stderr,"Usage: %s foo.gb [r]\n",argv[0]);
exit(1);
}
g= restore_graph(argv[1]);
if(!g){
fprintf(stderr,
"Sorry, can't create the graph from file %s! (error code %d)\n",
argv[1],panic_code);
exit(-1);
}
n= g->n;
if(argc> 2)sscanf(argv[2],"%d",&r);

/*:2*/
#line 28 "sand.w"
;
/*3:*/
#line 48 "sand.w"

for(j= 0;j<n;j++){
v= g->vertices+j;
for(a= v->arcs;a;a= a->next){
k= a->tip-g->vertices;
d[j]++;
vec[j][k]--;
}
vec[j][j]= d[j];
}
if(r){
for(j= 0;j<n;j++)k= vec[0][j],vec[0][j]= vec[r][j],vec[r][j]= k;
for(j= 0;j<n;j++)k= vec[j][0],vec[j][0]= vec[j][r],vec[j][r]= k;
k= d[0],d[0]= d[r],d[r]= k;
}

/*:3*/
#line 29 "sand.w"
;
/*5:*/
#line 80 "sand.w"

printf("The d vector is");
for(j= 1;j<n;j++){
x[j]= d[j];
printf(" %d",x[j]);
}
printf("\n and it reduces to");
reduce();
for(j= 1;j<n;j++){
printf(" %d",x[j]);
x[j]= d[j]-x[j];
}
printf("\nThe t vector is");
reduce();
for(j= 1;j<n;j++){
printf(" %d",x[j]);
x[j]= d[j]+d[j];
}
reduce();
printf("\nThe double-d vector reduces to");
for(j= 1;j<n;j++){
printf(" %d",x[j]);
x[j]= d[j]+d[j]-x[j];
}
reduce();
printf("\n and the zero vector is");
for(j= 1;j<n;j++){
printf(" %d",x[j]);
}
printf("\n");

/*:5*/
#line 30 "sand.w"
;
}

/*:1*/
