#define size 100000
#define o mems++
#define oo mems+= 2 \

#define deBruijn 0x03f79d71b4ca8b09 \

/*1:*/
#line 49 "maxcliques.w"

#include <stdio.h> 
#include "gb_graph.h"
#include "gb_save.h"
unsigned long long rho[64];
unsigned long long work[size];
unsigned int mems;
int space;
char table[64];
main(int argc,char*argv[])
{
register int i,j,k,l,m,n,p,q,r;
register Graph*g;
register Arc*a;
unsigned long long u,v,w;
/*2:*/
#line 71 "maxcliques.w"

if(argc!=2){
fprintf(stderr,"Usage: %s inputgraph.gb\n",argv[0]);
exit(-1);
}
g= restore_graph(argv[1]);
if(!g){
fprintf(stderr,"I can't input the graph %s (panic code %ld)!\n",
argv[1],panic_code);
exit(-2);
}
n= g->n;
if(n> 64){
fprintf(stderr,"Sorry, that graph has %d vertices; ",n);
fprintf(stderr,"I can't handle more than 64!\n");
exit(-3);
}
/*3:*/
#line 90 "maxcliques.w"

for(j= 0;j<n;j++){
w= 1LL<<j;
for(a= (g->vertices+j)->arcs;a;a= a->next)w|= 1LL<<(a->tip-g->vertices);
rho[j]= w;
}

/*:3*/
#line 88 "maxcliques.w"
;

/*:2*/
#line 64 "maxcliques.w"
;
/*6:*/
#line 131 "maxcliques.w"

w= 1LL<<(n-1);
oo,work[1]= work[size-1]= (w<<1)-1;
m= 1,space= 4;
for(i= 0;i<n;i++)if(oo,rho[i]!=work[size-1]){
v= 1LL<<i;
/*7:*/
#line 156 "maxcliques.w"

j= 1,k= m;
while(1){
while((o,work[j]&v)==0)j++;
while((o,work[k]&v)!=0)k--;
if(j> k)break;
oo,u= work[j],work[j]= work[k],work[k]= u;
j++,k--;
}

/*:7*/
#line 137 "maxcliques.w"
;
/*8:*/
#line 201 "maxcliques.w"

for(l= k,p= size-1;j<=m;j++){
o,u= work[j],q= size-2;
w= u&rho[i];
if(u!=w){
for(;q>=p;q--){
if((o,w&work[q])==w)goto second_entry;
if((w&work[q])==work[q])goto absorb;
}
o,work[--p]= w;
if(space<m+2+size-p)space= m+2+size-p;
goto second_entry;
}
absorb:/*9:*/
#line 234 "maxcliques.w"

o,r= p,work[p-1]= 0;
while(1){
while(o,(w|work[q])!=w)q--;
while(o,(w|work[r])==w)r++;
if(q<r)break;
oo,work[q]= work[r],work[r]= 0,q--,r++;
}
o,work[q]= w,p= q;

/*:9*/
#line 214 "maxcliques.w"
;
if(u==w)continue;
second_entry:w= u&~v;
for(q= 1;q<=k;q++)if((o,w&work[q])==w)goto done_with_u;
o,work[++l]= w;
done_with_u:continue;
}
for(m= l;p<size-1;p++)oo,work[++m]= work[p];

/*:8*/
#line 139 "maxcliques.w"
;
}

/*:6*/
#line 65 "maxcliques.w"
;
/*4:*/
#line 99 "maxcliques.w"

printf("Graph %s has %d maximal cliques:\n",g->id,m);
/*5:*/
#line 118 "maxcliques.w"

for(j= 0,v= 1;v;j++,v<<= 1){
u= v*deBruijn;
table[u>>58]= j;
}

/*:5*/
#line 101 "maxcliques.w"
;
for(k= 1;k<=m;k++){
for(w= work[k];w;w= w^v){
v= w&-w;
u= v*deBruijn;
j= table[u>>58];
printf(" %s",(g->vertices+j)->name);
}
printf("\n");
}

/*:4*/
#line 66 "maxcliques.w"
;
printf("(The computation took %u mems, using %d words of workspace.)\n",
mems,space);
}

/*:1*/
