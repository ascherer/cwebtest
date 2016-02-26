#define maxn 90
#define maxm 2000
#define logmemsize 27
#define memsize (1<<logmemsize) 
#define loghtsize 24
#define htsize (1<<loghtsize)  \

#define num z.I
#define invarcs y.A \

#define trunc(addr) ((addr) &(memsize-1) )  \

/*1:*/
#line 25 "simpath-directed-cycles.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include "gb_graph.h"
#include "gb_save.h"
char mem[memsize];
unsigned long long tail,boundary,head;
unsigned int htable[htsize];
unsigned int htid;
int htcount;
Vertex*vert[maxn+1];
int arcto[maxm];
int firstarc[maxn+2];
char mate[maxn+3];
int serial,newserial;
/*12:*/
#line 293 "simpath-directed-cycles.w"

void printstate(int j,int jj,int ll){
register int h,hh,ss,t,tt,hash;
for(t= j;t<jj;t++)
if(mate[t]&&mate[t]!=t)break;
if(t<jj)printf("0");
else if(ll<jj)printf("0");
else{
ss= ll+1-jj;
if(head+ss-tail> memsize){
fprintf(stderr,"Oops, I'm out of memory (memsize=%d, serial=%d)!\n",
memsize,serial);
exit(-69);
}
/*13:*/
#line 314 "simpath-directed-cycles.w"

for(t= jj,h= trunc(head),hash= 0;t<=ll;t++,h= trunc(h+1)){
mem[h]= mate[t];
hash= hash*31415926525+mate[t];
}

/*:13*/
#line 307 "simpath-directed-cycles.w"
;
/*14:*/
#line 323 "simpath-directed-cycles.w"

for(hash= hash&(htsize-1);;hash= (hash+1)&(htsize-1)){
hh= htable[hash];
if((hh^htid)>=memsize)/*15:*/
#line 335 "simpath-directed-cycles.w"

{
if(++htcount> (htsize>>1)){
fprintf(stderr,"Sorry, the hash table is full (htsize=%d, serial=%d)!\n",
htsize,serial);
exit(-96);
}
hh= trunc(head);
htable[hash]= htid+hh;
head+= ss;
goto found;
}

/*:15*/
#line 326 "simpath-directed-cycles.w"
;
hh= trunc(hh);
for(t= hh,h= trunc(head),tt= trunc(t+ss-1);;t= trunc(t+1),h= trunc(h+1)){
if(mem[t]!=mem[h])break;
if(t==tt)goto found;
}
}
found:

/*:14*/
#line 308 "simpath-directed-cycles.w"
;
h= trunc(hh-boundary)/ss;
printf("%x",newserial+h);
}
}

/*:12*/
#line 41 "simpath-directed-cycles.w"


main(int argc,char*argv[]){
register int i,j,jj,jm,k,km,l,ll,m,n,t;
register Graph*g;
register Arc*a,*b;
register Vertex*u,*v;
/*2:*/
#line 54 "simpath-directed-cycles.w"

if(argc!=2){
fprintf(stderr,"Usage: %s foo.gb\n",argv[0]);
exit(-1);
}
g= restore_graph(argv[1]);
if(!g){
fprintf(stderr,"I can't input the graph %s (panic code %ld)!\n",
argv[1],panic_code);
exit(-2);
}
n= g->n;
if(n> maxn){
fprintf(stderr,"Sorry, that graph has %d vertices; ",n);
fprintf(stderr,"I can't handle more than %d!\n",maxn);
exit(-3);
}
if(g->m> maxm){
fprintf(stderr,"Sorry, that graph has %d arcs; ",(g->m+1)/2);
fprintf(stderr,"I can't handle more than %d!\n",maxm);
exit(-3);
}

/*:2*/
#line 48 "simpath-directed-cycles.w"
;
/*3:*/
#line 84 "simpath-directed-cycles.w"

for(v= g->vertices;v<g->vertices+n;v++)v->num= 0,v->invarcs= NULL;
for(v= g->vertices;v<g->vertices+n;v++){
for(a= v->arcs;a;a= a->next){
register Arc*b= gb_virgin_arc();
u= a->tip;
b->tip= v;
b->next= u->invarcs;
u->invarcs= b;
}
}
vert[1]= g->vertices,g->vertices->num= 1;
for(j= 0,k= 1;j<k;j++){
v= vert[j+1];
for(a= v->arcs;a;a= a->next){
u= a->tip;
if(u->num==0)u->num= ++k,vert[k]= u;
}
for(a= v->invarcs;a;a= a->next){
u= a->tip;
if(u->num==0)u->num= ++k,vert[k]= u;
}
}

/*:3*/
#line 49 "simpath-directed-cycles.w"
;
/*4:*/
#line 120 "simpath-directed-cycles.w"

for(m= 0,k= 1;k<=n;k++){
firstarc[k]= m;
v= vert[k];
printf("%d(%s)\n",k,v->name);
for(a= v->arcs;a;a= a->next){
u= a->tip;
if(u->num> k){
arcto[m++]= u->num;
if(a->len==1)printf(" -> %d(%s) #%d\n",u->num,u->name,m);
else printf(" -> %d(%s,%d) #%d\n",u->num,u->name,a->len,m);
}
}
for(a= v->invarcs;a;a= a->next){
u= a->tip;
if(u->num> k){
arcto[m++]= -u->num;
if(a->len==1)printf(" <- %d(%s) #%d\n",u->num,u->name,m);
else printf(" <- %d(%s,%d) #%d\n",u->num,u->name,a->len,m);
}
}
}
firstarc[k]= m;

/*:4*/
#line 50 "simpath-directed-cycles.w"
;
/*5:*/
#line 187 "simpath-directed-cycles.w"

for(t= 1;t<=n;t++)mate[t]= t;
/*6:*/
#line 198 "simpath-directed-cycles.w"

jj= ll= 1;
mem[0]= mate[1];
tail= 0,head= 1;
serial= 2;

/*:6*/
#line 189 "simpath-directed-cycles.w"
;
for(i= 0;i<m;i++){
printf("#%d:\n",i+1);
fprintf(stderr,"Beginning arc %d (serial=%d,head-tail=%ld)\n",
i+1,serial,head-tail);
fflush(stderr);
/*7:*/
#line 214 "simpath-directed-cycles.w"

boundary= head,htcount= 0,htid= (i+1)<<logmemsize;
newserial= serial+((head-tail)/(ll+1-jj));
j= jj,k= arcto[i],l= ll;
while(jj<=n&&firstarc[jj+1]==i+1)jj++;
ll= (k> l?k:-k> l?-k:l);
while(tail<boundary){
printf("%x:",serial);
serial++;
/*8:*/
#line 233 "simpath-directed-cycles.w"

for(t= j;t<=l;t++,tail++){
mate[t]= mem[trunc(tail)];
}

/*:8*/
#line 223 "simpath-directed-cycles.w"
;
/*10:*/
#line 270 "simpath-directed-cycles.w"

printstate(j,jj,ll);

/*:10*/
#line 224 "simpath-directed-cycles.w"
;
printf(",");
/*9:*/
#line 241 "simpath-directed-cycles.w"

if(k> 0){
jm= mate[j],km= mate[k];
if(jm==j)jm= -j;
if(jm>=0||km<=0)printf("0");
else if(jm==-k)
/*11:*/
#line 276 "simpath-directed-cycles.w"

{
for(t= j+1;t<=ll;t++)if(t!=(k> 0?k:-k)){
if(mate[t]&&mate[t]!=t)break;
}
if(t> ll)printf("1");
else printf("0");
}

/*:11*/
#line 247 "simpath-directed-cycles.w"

else{
mate[j]= 0,mate[k]= 0;
mate[-jm]= km,mate[km]= jm;
printstate(j,jj,ll);
mate[-jm]= j,mate[km]= k,mate[j]= jm,mate[k]= km;
if(mate[j]==-j)mate[j]= j;
}
}else{
jm= mate[j],km= mate[-k];
if(km==-k)km= k;
if(jm<=0||km>=0)printf("0");
else if(km==-j)
/*11:*/
#line 276 "simpath-directed-cycles.w"

{
for(t= j+1;t<=ll;t++)if(t!=(k> 0?k:-k)){
if(mate[t]&&mate[t]!=t)break;
}
if(t> ll)printf("1");
else printf("0");
}

/*:11*/
#line 260 "simpath-directed-cycles.w"

else{
mate[j]= 0,mate[-k]= 0;
mate[jm]= km,mate[-km]= jm;
printstate(j,jj,ll);
mate[jm]= j,mate[km]= -k,mate[j]= jm,mate[-k]= km;
if(mate[-k]==k)mate[-k]= -k;
}
}

/*:9*/
#line 226 "simpath-directed-cycles.w"
;
printf("\n");
}

/*:7*/
#line 195 "simpath-directed-cycles.w"
;
}

/*:5*/
#line 51 "simpath-directed-cycles.w"
;
}

/*:1*/
