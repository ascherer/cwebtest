/*1:*/
#line 15 "hamdance.w"

#include "gb_graph.h" 
#include "gb_save.h" 
#define max_n 100
#define infty 1000000000 \

#define bare 2
#define outer 1
#define inner 0
#define type u.I
#define deg v.I
#define llink w.V
#define rlink x.V
#define mate y.V
#define comate z.V
#define head (&list_head)  \

#define dancing_delete(u) u->llink->rlink= u->rlink,u->rlink->llink= u->llink
#define decrease_deg(u,w)  \
if(u->type==bare) { \
u->deg--; \
if(u->deg==2) barelist[bcount++]= u; \
}else if(u!=w) u->deg-- \
 \

#define make_outer(u) { \
u->rlink= head->rlink,head->rlink->llink= u; \
u->llink= head,head->rlink= u; \
u->type= outer; \
} \

#define vprint() if(verbose)  \
printf(" %s--%s\n",source[ocount-1]->name,dest[ocount-1]->name)  \

#define dancing_undelete(v) v->llink->rlink= v->rlink->llink= v
#define make_bare(v) dancing_delete(v) ,v->type= bare,v->mate= NULL \

#define index(v) ((v) -g->vertices)  \


#line 18 "hamdance.w"

/*3:*/
#line 62 "hamdance.w"

int modulus;

/*:3*//*4:*/
#line 106 "hamdance.w"

Vertex list_head;
char*decode[3]= {"inner","outer","bare"};

/*:4*//*8:*/
#line 180 "hamdance.w"

Vertex*barelist[max_n];
int bcount;
int curb[max_n];
int curbb[max_n];
Vertex*bareback[max_n];

/*:8*//*10:*/
#line 209 "hamdance.w"

Vertex*source[max_n],*dest[max_n];
int ocount;
int curo[max_n];

/*:10*//*11:*/
#line 217 "hamdance.w"

Vertex*curv[max_n];
Arc*cura[max_n];
int curi[max_n];
int maxi[max_n];
int profile[max_n];
int l;
int maxl;
unsigned int total;

/*:11*//*35:*/
#line 709 "hamdance.w"

int v1[max_n],v2[max_n];
int path[max_n+1];

/*:35*/
#line 19 "hamdance.w"

Graph*g;
/*5:*/
#line 113 "hamdance.w"

void print_vert(Vertex*v)
{
printf("%s: %s, deg=%d",v->name,decode[v->type],v->deg);
if(v->llink)printf(", llink=%s",v->llink->name);
if(v->rlink)printf(", rlink=%s",v->rlink->name);
if(v->mate)printf(", mate=%s",v->mate->name);
if(v->comate)printf(", comate=%s",v->comate->name);
printf("\n");
}

/*:5*//*6:*/
#line 126 "hamdance.w"

void print_verts()
{
register Vertex*v;
for(v= g->vertices;v<g->vertices+g->n;v++)print_vert(v);
}

/*:6*//*7:*/
#line 139 "hamdance.w"

void sanity_check(Vertex*vv)
{
register Vertex*u,*v,*w;
register Arc*a;
register int c,d;
for(v= g->vertices,c= 0;v<g->vertices+g->n;v++){
w= v->mate;
if(v->type==bare&&w!=NULL)
printf("Bare vertex %s shouldn't have mate %s!\n",v->name,w->name);
if(v->type==outer)c++;
if(v->type==outer&&(w->mate!=v||w->type!=outer))
if(w!=vv||w->type!=inner)
printf("Outer vertex %s has mate problem vis-a-vis %s!\n",
v->name,w->name);
for(a= v->arcs,d= 0;a;a= a->next){
u= a->tip;
if(u->type!=inner&&u!=w)d++;
}
if(v->type!=inner&&v->deg!=d&&ocount!=g->n-1)
printf("Vertex %s should have degree %d, not %d!\n",v->name,d,v->deg);
if(v->type==bare&&d<3&&vv==NULL)
printf("Vertex %s (degree %d) should not be bare!\n",v->name,d);
}
for(v= head->rlink;c> 0;c--,v= v->rlink){
if(v->type!=outer)
printf("Vertex %s (%s) shouldn't be in the list!\n",
v->name,decode[v->type]);
if(v->llink->rlink!=v||v->rlink->llink!=v)
printf("Double-link failure at vertex %s!\n",v->name);
}
if(v!=head)
printf("The list doesn't contain all the outer vertices!\n");
}


/*:7*//*12:*/
#line 231 "hamdance.w"

void print_state()
{
register int i,j,k;
for(j= k= 0;k<=l;j++,k++){
while(j<curo[k]){
printf("      %s--%s\n",source[j]->name,dest[j]->name);
j++;
}
if(k){
if(j<g->n)printf(" %3d: %s--%s (%d of %d)\n",
k,source[j]->name,dest[j]->name,curi[k],maxi[k]);
}else/*39:*/
#line 780 "hamdance.w"

if(cura[0])
printf(" %3d: %s--%s (%d of %d)\n",
0,source[0]->name,dest[0]->name,curi[0],maxi[0]);
else{
j= -1;
if(maxi[0])
printf(" %3d: (%d of %d)\n",0,curi[0],maxi[0]);
}

/*:39*/
#line 243 "hamdance.w"
;
}
}

/*:12*/
#line 21 "hamdance.w"

int main(int argc,char*argv[])
{
register Vertex*u,*v,*w;
register Arc*a;
int k,d;
/*2:*/
#line 47 "hamdance.w"

if(argc> 1)g= restore_graph(argv[1]);else g= NULL;
if(argc<3||sscanf(argv[2],"%d",&modulus)!=1)modulus= infty;
if(!g||modulus==0){
fprintf(stderr,"Usage: %s foo.gb [[-]modulus] [verbose]\n",argv[0]);
exit(-1);
}
if(g->n> max_n){
fprintf(stderr,"Sorry, I'm set up to handle at most %d vertices!\n",max_n);
exit(-2);
}
if(argc> 3)verbose= 1;

/*:2*/
#line 27 "hamdance.w"
;
/*9:*/
#line 187 "hamdance.w"

d= infty;bcount= ocount= 0;
for(v= g->vertices;v<g->vertices+g->n;v++){
v->type= bare;
for(a= v->arcs,k= 0;a;a= a->next)k++;
v->deg= k;
if(k==2)barelist[bcount++]= v;
if(k<d)d= k,curv[0]= v;
v->llink= v->rlink= v->mate= v->comate= NULL;
}
head->rlink= head->llink= head;
head->name= "head";
if(d<2){
printf("There are no Hamiltonian circuits, because %s has degree %d!\n",
curv[0]->name,d);
exit(0);
}

/*:9*/
#line 28 "hamdance.w"
;
/*14:*/
#line 260 "hamdance.w"

/*36:*/
#line 729 "hamdance.w"

l= 0;
if(d> 2){
maxi[0]= d-1;
source[0]= v= curv[0];
make_outer(v);
force:cura[0]= a= v->arcs;
v->arcs= a->next;
curi[0]++;
dest[0]= u= a->tip;
ocount= 1;vprint();
make_outer(u);
v->deg--;
u->deg--;
/*37:*/
#line 747 "hamdance.w"

if(u->arcs->tip==v)u->arcs= u->arcs->next;
else{
for(a= u->arcs;a->next->tip!=v;a= a->next);
a->next= a->next->next;
}

/*:37*/
#line 743 "hamdance.w"
;
v->mate= u,u->mate= v;
}

/*:36*/
#line 261 "hamdance.w"
;
advance:/*18:*/
#line 384 "hamdance.w"

for(k= curb[l];k<bcount;k++){
v= barelist[k];
if(v->type!=bare)bareback[k]= v,barelist[k]= NULL;
else{
if(v->deg!=2){
if(verbose)printf("(oops, low degree; backing up)\n");
goto emergency_backup;
}
/*19:*/
#line 410 "hamdance.w"

for(a= v->arcs;;a= a->next){
u= a->tip;
if(u->type!=inner)break;
}
for(a= a->next;;a= a->next){
w= a->tip;
if(w->type!=inner)break;
}

/*:19*/
#line 393 "hamdance.w"
;
if(u->mate==w&&ocount!=g->n-2){
if(verbose)printf("(oops, short cycle; backing up)\n");
goto emergency_backup;
}
v->mate= u,v->comate= w;
v->type= inner;
source[ocount]= u,dest[ocount++]= v;vprint();
source[ocount]= v,dest[ocount++]= w;vprint();
if(u->type==bare)
if(w->type==bare)/*20:*/
#line 425 "hamdance.w"

{
u->deg--,w->deg--;
make_outer(u);
make_outer(w);
u->mate= w,w->mate= u;
for(a= u->arcs;a;a= a->next)if(a->tip==w){
u->deg--,w->deg--;
break;
}
}

/*:20*/
#line 403 "hamdance.w"

else/*21:*/
#line 437 "hamdance.w"

{
u->deg--;
make_outer(u);
u->mate= w->mate,w->mate->mate= u;
for(a= u->arcs;a;a= a->next)if(a->tip==w->mate){
u->deg--,w->mate->deg--;
break;
}
for(a= w->arcs;a;a= a->next){
v= a->tip;
if(v->type!=inner)decrease_deg(v,w->mate);
}
w->type= inner;
dancing_delete(w);
}

/*:21*/
#line 404 "hamdance.w"

else if(w->type==bare)/*22:*/
#line 454 "hamdance.w"

{
w->deg--;
make_outer(w);
w->mate= u->mate,u->mate->mate= w;
for(a= w->arcs;a;a= a->next)if(a->tip==u->mate){
w->deg--,u->mate->deg--;
break;
}
for(a= u->arcs;a;a= a->next){
v= a->tip;
if(v->type!=inner)decrease_deg(v,u->mate);
}
u->type= inner;
dancing_delete(u);
}

/*:22*/
#line 405 "hamdance.w"

else/*23:*/
#line 471 "hamdance.w"

{
for(a= u->arcs;a;a= a->next){
v= a->tip;
if(v->type!=inner)decrease_deg(v,u->mate);
}
u->type= inner;
dancing_delete(u);
for(a= w->arcs;a;a= a->next){
v= a->tip;
if(v->type!=inner)decrease_deg(v,w->mate);
}
w->type= inner;
dancing_delete(w);
if(u->mate!=w){
u->mate->mate= w->mate,w->mate->mate= u->mate;
for(a= u->mate->arcs;a;a= a->next)if(a->tip==w->mate){
u->mate->deg--,w->mate->deg--;
break;
}
}
}

/*:23*/
#line 406 "hamdance.w"
;
}
}

/*:18*/
#line 262 "hamdance.w"
;

l++;
if(verbose){
if(l> maxl)maxl= l;
printf("Entering level %d:",l);
profile[l]++;
}
if(ocount>=g->n-1)/*32:*/
#line 655 "hamdance.w"

{
if(ocount<g->n)
/*33:*/
#line 675 "hamdance.w"

{
u= head->llink,v= head->rlink;
for(a= u->arcs;a;a= a->next)if(a->tip==v)goto yes;
goto backup;
yes:;
}

/*:33*/
#line 658 "hamdance.w"
;
total++;
if(total%abs(modulus)==0||verbose){
curo[l]= ocount;
source[ocount]= head->rlink,dest[ocount]= head->llink;
curi[l]= maxi[l]= 1;
if(modulus<0){
printf("\n%d:\n",total);print_state();
}else/*34:*/
#line 685 "hamdance.w"

{
register int i,j,k;
for(k= 0;k<g->n;k++)v1[k]= -1;
for(k= 0;k<g->n;k++){
i= index(source[k]);
j= index(dest[k]);
if(v1[i]<0)v1[i]= j;
else v2[i]= j;
if(v1[j]<0)v1[j]= i;
else v2[j]= i;
}
path[0]= 0,path[1]= v1[0];
for(k= 2;;k++){
if(v1[path[k-1]]==path[k-2])path[k]= v2[path[k-1]];
else path[k]= v1[path[k-1]];
if(path[k]==0)break;
}
if(verbose)printf("\n");
printf("%d:",total);
for(k= 0;k<=g->n;k++)printf(" %s",(g->vertices+path[k])->name);
printf("\n");
}

/*:34*/
#line 666 "hamdance.w"
;
}
goto backup;
}

/*:32*/
#line 270 "hamdance.w"
;
/*15:*/
#line 305 "hamdance.w"

for(u= head->rlink,d= infty;u!=head;u= u->rlink){
if(verbose)printf(" %s(%d)",u->name,u->deg);
if(u->deg<d)d= u->deg,v= u;
}

/*:15*/
#line 271 "hamdance.w"
;
if(verbose)printf(" choosing %s(%d)\n",v->name,d);
if(d==0)goto backup;
curv[l]= v,curi[l]= 1,maxi[l]= d,curb[l]= bcount,curo[l]= ocount;
source[ocount]= v;
w= v->mate;
/*16:*/
#line 323 "hamdance.w"

for(a= v->arcs;a;a= a->next){
u= a->tip;
if(u->type> inner)decrease_deg(u,w);
}
v->type= inner;
dancing_delete(v);
curbb[l]= bcount;

/*:16*/
#line 277 "hamdance.w"
;
a= v->arcs;
try_move:for(;;a= a->next){
u= a->tip;
if(u->type!=inner&&u!=w)break;
}
cura[l]= a;
/*17:*/
#line 348 "hamdance.w"

dest[ocount++]= u;vprint();
if(u->type==outer){
for(a= w->arcs;a;a= a->next)
if(a->tip==u->mate){
u->mate->deg--,w->deg--;
break;
}
w->mate= u->mate,u->mate->mate= w;
dancing_delete(u);
u->type= inner;
for(a= u->arcs;a;a= a->next){
w= a->tip;
if(w->type> inner)decrease_deg(w,u->mate);
}
}else{
for(a= w->arcs;a;a= a->next)
if(a->tip==u){
u->deg--,w->deg--;
break;
}
w->mate= u,u->mate= w;
make_outer(u);
}

/*:17*/
#line 284 "hamdance.w"
;
goto advance;
backup:l--;
if(verbose)printf(" back to level %d:\n",l);
/*25:*/
#line 514 "hamdance.w"

for(k= bcount-1;k>=curb[l];k--){
v= barelist[k];
if(!v)barelist[k]= bareback[k];
else{
u= v->mate,w= v->comate;
v->type= bare,v->mate= NULL;
v->comate= NULL;
if(u->type==outer)
if(w->type==outer)/*26:*/
#line 531 "hamdance.w"

{
u->deg++,w->deg++;
make_bare(u);
make_bare(w);
for(a= u->arcs;a;a= a->next)if(a->tip==w){
u->deg++,w->deg++;
break;
}
}

/*:26*/
#line 523 "hamdance.w"

else/*27:*/
#line 546 "hamdance.w"

{
v->deg--;
w->mate->mate= w;
dancing_undelete(w);
w->type= outer;
for(a= u->arcs;a;a= a->next)if(a->tip==w->mate){
u->deg++,w->mate->deg++;
break;
}
for(a= w->arcs;a;a= a->next){
v= a->tip;
if(v->type!=inner&&v!=w->mate)v->deg++;
}
u->deg++;
make_bare(u);
}

/*:27*/
#line 524 "hamdance.w"

else if(w->type==outer)/*28:*/
#line 564 "hamdance.w"

{
v->deg--;
u->mate->mate= u;
dancing_undelete(u);
u->type= outer;
for(a= w->arcs;a;a= a->next)if(a->tip==u->mate){
w->deg++,u->mate->deg++;
break;
}
for(a= u->arcs;a;a= a->next){
v= a->tip;
if(v->type!=inner&&v!=u->mate)v->deg++;
}
w->deg++;
make_bare(w);
}

/*:28*/
#line 525 "hamdance.w"

else/*29:*/
#line 582 "hamdance.w"

{
v->deg-= 2;
if(u->mate!=w){
u->mate->mate= u,w->mate->mate= w;
for(a= u->mate->arcs;a;a= a->next)if(a->tip==w->mate){
u->mate->deg++,w->mate->deg++;
break;
}
}
dancing_undelete(w);
w->type= outer;
for(a= w->arcs;a;a= a->next){
v= a->tip;
if(v->type!=inner&&v!=w->mate)v->deg++;
}
dancing_undelete(u);
u->type= outer;
for(a= u->arcs;a;a= a->next){
v= a->tip;
if(v->type!=inner&&v!=u->mate)v->deg++;
}
}

/*:29*/
#line 526 "hamdance.w"
;
}
emergency_backup:;
}

/*:25*/
#line 288 "hamdance.w"
;
if(l){
/*30:*/
#line 611 "hamdance.w"

v= curv[l];
ocount= curo[l];
u= dest[ocount];
if(u->type==inner){
for(a= u->arcs;a;a= a->next){
w= a->tip;
if(w->type!=inner&&w!=u->mate)w->deg++;
}
u->type= outer;
dancing_undelete(u);
w= v->mate;
u->mate->mate= u,w->mate= v;
for(a= w->arcs;a;a= a->next)
if(a->tip==u->mate){
u->mate->deg++,w->deg++;
break;
}
}else{
make_bare(u);
w= v->mate;
w->mate= v;
for(a= w->arcs;a;a= a->next)
if(a->tip==u){
u->deg++,w->deg++;
break;
}
}
bcount= curbb[l];

/*:30*/
#line 290 "hamdance.w"
;

if(curi[l]<maxi[l]){
curi[l]++;
w= v->mate;a= cura[l]->next;
goto try_move;
}
/*31:*/
#line 641 "hamdance.w"

bcount= curb[l];
dancing_undelete(v);
v->type= outer;
for(a= v->arcs;a;a= a->next){
u= a->tip;
if(u->type!=inner&&u!=w)u->deg++;
}

/*:31*/
#line 297 "hamdance.w"
;
if(l> 1)goto backup;
}
/*38:*/
#line 758 "hamdance.w"

if(curi[0]<maxi[0]){
if(verbose)printf(" back to level 0:\n");
l= 0;
ocount= 0;
u= dest[0];
dancing_delete(u);
u->type= bare;
if(u->deg==2)barelist[0]= u,bcount= 1;
else bcount= 0;
v= source[0];
if(v->deg==2){
v->type= bare;
dancing_delete(v);
barelist[bcount++]= v;
}
if(bcount==0)goto force;
maxi[0]= curi[0]= curi[0]+1;
cura[0]= NULL;
goto advance;
}

/*:38*/
#line 300 "hamdance.w"
;

/*:14*/
#line 29 "hamdance.w"
;
/*13:*/
#line 247 "hamdance.w"

printf("Altogether %u solutions.\n",total);
if(verbose){
for(k= 1;k<=maxl;k++)
printf("%3d: %d\n",k,profile[k]);
}

/*:13*/
#line 30 "hamdance.w"
;
exit(0);
}

/*:1*/
