#define file_given (argc==2)  \

#define o mems++
#define oo mems+= 2
#define ooo mems+= 3
#define oooo mems+= 4 \

#define addr_(p) ((addr) (size_t) (p) )  \

#define logpagesize 12
#define memsize (1<<29)  \

#define pagesize (1<<logpagesize) 
#define pagemask (pagesize-1) 
#define pageints (pagesize/sizeof(int) ) 
#define node_(a) ((node*) (size_t) (a) ) 
#define page_(a) ((page*) (size_t) (a) )  \

#define topofmem ((page*) &mem[memsize])  \

#define logmaxhashsize 21
#define slotsperpage (pagesize/sizeof(addr) ) 
#define maxhashpages (((1<<logmaxhashsize) +slotsperpage-1) /slotsperpage)  \

#define logvarsize 10
#define varsize (1<<logvarsize) 
#define varpart(x) ((x) >>(32-logvarsize) ) 
#define initnewnode(p,v,l,h) oo,p->lo= addr_(l) ,p->hi= addr_(h) ,p->xref= 0, \
oooo,p->index= ((v) <<(32-logvarsize) ) +(gb_next_rand() >>(logvarsize-1) )  \

#define topofvars &varhead[varsize] \

#define hashcode(l,h) ((addr*) (size_t) (oo,((l) ->index<<3) ^((h) ->index<<2) ) ) 
#define hashedcode(p) hashcode(node_(p->lo) ,node_(p->hi) ) 
#define addr__(x) (*((addr*) (size_t) (x) ) ) 
#define fetchnode(v,k) node_(addr__(v->base[(k) >>logpagesize]+((k) &pagemask) ) ) 
#define storenode(v,k,p) o,addr__(v->base[(k) >>logpagesize]+((k) &pagemask) ) = addr_(p)  \

#define storenulls(k) *(long long*) (size_t) (k) = 0LL; \

#define timerinterval 1024
#define deadfraction 8 \

#define maxmask ((1<<logmaxhashsize) *sizeof(addr) -1)  \
 \

#define memo_(a) ((memo*) (size_t) (a) )  \

#define logmaxcachepages 15
#define maxcachepages (1<<logmaxcachepages) 
#define cacheslotsperpage (pagesize/sizeof(memo) ) 
#define maxbinop 15 \

#define id(a) (((size_t) (a) -(size_t) mem) /sizeof(node) )  \

#define cachehash(f,g,h)  \
((f) ->index<<4) ^(((h) ?(g) ->index:addr_(g) ) <<5) ^(addr_(h) <<6) 
#define thememo(s) memo_(cachepage[((s) &cachemask) >>logpagesize]+((s) &pagemask) )  \

#define extsize 1000 \

#define thevar(p) (&varhead[varpart((p) ->index) ]) 
#define print_node(p)  \
printf("%x: (~%d?%x:%x)",id(p) ,thevar(p) ->name,id((p) ->lo) ,id((p) ->hi) ) 
#define print_node_unmapped(p)  \
printf("%x: (~%d?%x:%x)",id(p) ,thevar(p) -varhead,id((p) ->lo) ,id((p) ->hi) )  \

#define includesanity 1 \

#define ghost(p) node_((size_t) (p) -(size_t) mem+(size_t) smem)  \

#define complain(complaint)  \
{printf("! %s in node ",complaint) ; \
print_node(p) ;printf("\n") ;}
#define legit(p) (((size_t) (p) &(sizeof(node) -1) ) ==0&&(p) <nodeptr&& \
(p) >=botsink&&ghost(p) ->xref!=-1) 
#define superlegit(p) (((size_t) (p) &(sizeof(node) -1) ) ==0&&(p) <nodeptr&& \
(p) > topsink&&ghost(p) ->xref!=-1)  \

#define badpage(p) ((p) <pageptr||(p) >=topofmem)  \

#define deref(p) if(o,(p) ->xref==0) recursively_kill(p) ;else o,(p) ->xref-- \

#define debugging 1 \

#define bufsize 100 \

#define getk for(k= 0;isdigit(*c) ;c++) k= 10*k+*c-'0'
#define reporterror {printf("Sorry; `%c' confuses me %s%s", \
*(c-1) ,infile?"in this command: ":"in that command.",infile?buf:"\n") ; \
goto nextcommand;} \

#define passblanks for(;*c==' ';c++)  \

#define getkf  \
getk;if(k>=extsize) {printf("f%d is out of range.\n",k) ;continue;}
#define getkv  \
getk;if(k>=varsize) {printf("x%d is out of range.\n",k) ;continue;} \

#define checknull(p) if(!p) {printf("f%d is null!\n",k) ;continue;} \
 \

/*2:*/
#line 82 "bdd14.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include "gb_flip.h" 
#define verbose Verbose 
/*10:*/
#line 204 "bdd14.w"

typedef unsigned int addr;

/*:10*//*11:*/
#line 237 "bdd14.w"

typedef struct node_struct{
addr lo,hi;
int xref;
unsigned int index;
}node;
typedef struct page_struct{
addr dat[pageints];
}page;

/*:11*//*20:*/
#line 399 "bdd14.w"

typedef struct var_struct{
addr proj;
addr repl;
int free;
int mask;
addr base[maxhashpages];
int name;
unsigned int timestamp;
int aux;
struct var_struct*up,*down;
}var;

/*:20*//*39:*/
#line 819 "bdd14.w"

typedef struct memo_struct{
addr f;
addr g;
addr h;
addr r;
}memo;

/*:39*/
#line 88 "bdd14.w"

/*4:*/
#line 111 "bdd14.w"

FILE*infile;
int verbose= -1;

/*:4*//*8:*/
#line 159 "bdd14.w"

unsigned long long mems,rmems,zmems;

/*:8*//*13:*/
#line 271 "bdd14.w"

char mem[memsize];
node*nodeptr;
page*pageptr;
node*nodeavail;
page*pageavail;
node*botsink,*topsink;
int totalnodes;
int deadnodes;
int leasesonlife= 10;

/*:13*//*22:*/
#line 438 "bdd14.w"

var varhead[varsize];
var*tvar= topofvars;
int varmap[varsize];

/*:22*//*30:*/
#line 585 "bdd14.w"

unsigned long long timer;

/*:30*//*40:*/
#line 841 "bdd14.w"

addr cachepage[maxcachepages];
int cachepages;
int cacheinserts;
int threshold;
int cachemask;

/*:40*//*42:*/
#line 876 "bdd14.w"

char*binopname[]= 
{"","&",">","!","<","@","^","|","_","A","N","#","Y","$","D","E"};
char*ternopname1[]= 
{"?",".","&","&","@","#","$","%","*","<","-","+","|","/","\\","~"};
char*ternopname2[]= 
{":",".","&","E","@","#","$","%","*","<","-","+","|","/","\\","~"};

/*:42*//*51:*/
#line 1084 "bdd14.w"

node*f[extsize];

/*:51*//*59:*/
#line 1247 "bdd14.w"

#if includesanity
char smem[memsize];
#endif

/*:59*//*101:*/
#line 2158 "bdd14.w"

int timestamp;
int timestamp_changed;


/*:101*//*112:*/
#line 2361 "bdd14.w"

char buf[bufsize];

/*:112*//*132:*/
#line 2692 "bdd14.w"

int totalvars;
var*firstvar;
int oldleases;

/*:132*//*136:*/
#line 2830 "bdd14.w"

addr savebase[maxhashpages];

/*:136*//*154:*/
#line 3144 "bdd14.w"

double trigger;
int toobig= memsize;

/*:154*//*158:*/
#line 3196 "bdd14.w"

int last_ditch;

/*:158*//*161:*/
#line 3222 "bdd14.w"

FILE*outfile;
int outcount;

/*:161*/
#line 89 "bdd14.w"

/*25:*/
#line 475 "bdd14.w"

node*unique_find(var*v,node*l,node*h);

/*:25*//*27:*/
#line 542 "bdd14.w"

void recursively_revive(node*p);
void recursively_kill(node*p);
void collect_garbage(int level);

/*:27*//*90:*/
#line 1893 "bdd14.w"

node*mux_rec(node*f,node*g,node*h);

/*:90*//*107:*/
#line 2269 "bdd14.w"

void attempt_repairs(void);

/*:107*/
#line 90 "bdd14.w"

/*7:*/
#line 143 "bdd14.w"

void show_stats(void){
printf("stats: %d/%d nodes, %d dead, %d pages,",
totalnodes,nodeptr-botsink,deadnodes,topofmem-pageptr);
printf(" %llu mems, %llu rmems, %llu zmems\n",mems,rmems,zmems);
}

/*:7*//*14:*/
#line 290 "bdd14.w"

node*reserve_node(void){
register node*r= nodeavail;
if(r)o,nodeavail= node_(nodeavail->xref);
else{
r= nodeptr;
if(r<(node*)pageptr)nodeptr++;
else{
leasesonlife--;
fprintf(stderr,"NULL node forced (%d pages, %d nodes, %d dead)\n",
topofmem-pageptr,nodeptr-botsink,deadnodes);
fprintf(stderr,"(I will try %d more times)\n",leasesonlife);
if(leasesonlife==0){
show_stats();exit(-98);
}
return NULL;
}
}
totalnodes++;
return r;
}

/*:14*//*15:*/
#line 315 "bdd14.w"

void free_node(register node*p){
o,p->xref= addr_(nodeavail);
nodeavail= p;
totalnodes--;
}

/*:15*//*16:*/
#line 325 "bdd14.w"

page*reserve_page(void){
register page*r= pageavail;
if(r)o,pageavail= page_(pageavail->dat[0]);
else{
r= pageptr-1;
if((node*)r>=nodeptr)pageptr= r;
else{
leasesonlife--;
fprintf(stderr,"NULL page forced (%d pages, %d nodes, %d dead)\n",
topofmem-pageptr,nodeptr-botsink,deadnodes);
fprintf(stderr,"(I will try %d more times)\n",leasesonlife);
if(leasesonlife==0){
show_stats();exit(-97);
}
return NULL;
}
}
return r;
}

void free_page(register page*p){
o,p->dat[0]= addr_(pageavail);
pageavail= p;
}

/*:16*//*24:*/
#line 455 "bdd14.w"

node*projection(int v){
register node*p;
register var*hv= &varhead[v];
o,p= node_(hv->proj);
if(p)return p;
o,hv->base[0]= addr_(reserve_page());
/*28:*/
#line 552 "bdd14.w"

o,hv->free= 2,hv->mask= 7;
storenulls(hv->base[0]);
zmems++;

/*:28*/
#line 462 "bdd14.w"
;
p= unique_find(&varhead[v],botsink,topsink);
oooo,botsink->xref++,topsink->xref++;
o,hv->proj= addr_(p);
if(verbose&2)printf(" %x=x%d\n",id(p),v);
o,hv->name= v;
return p;
}

/*:24*//*26:*/
#line 517 "bdd14.w"

node*unique_find(var*v,node*l,node*h){
register int j,k,mask,free;
register addr*hash;
register node*p,*r;
if(l==h){
return oo,l->xref--,l;
}
restart:o,mask= v->mask,free= v->free;
for(hash= hashcode(l,h);;hash++){
k= addr_(hash)&mask;
oo,p= fetchnode(v,k);
if(!p)goto newnode;
if(node_(p->lo)==l&&node_(p->hi)==h)break;
}
if(o,p->xref<0){
deadnodes--,o,p->xref= 0;
return p;
}
oooo,l->xref--,h->xref--;
return o,p->xref++,p;
newnode:/*29:*/
#line 577 "bdd14.w"

if((++timer%timerinterval)==0){
if(deadnodes> totalnodes/deadfraction){
collect_garbage(0);
goto restart;
}
}

/*:29*/
#line 538 "bdd14.w"
;
/*31:*/
#line 590 "bdd14.w"

p= reserve_node();
if(!p)goto cramped;
if(--free<=mask>>4){
free_node(p);
/*32:*/
#line 608 "bdd14.w"

{
register int newmask= mask+mask+1,kk= newmask>>logpagesize;
if(verbose&256)
printf("doubling the hash table for level %d(x%d) (%d slots)\n",
v-varhead,v->name,(newmask+1)/sizeof(addr));
if(kk)/*33:*/
#line 629 "bdd14.w"

{
if(newmask> maxmask){
if(verbose&(2+256+512))
printf("profile limit reached for level %d(x%d)\n",v-varhead,v->name);
goto cramped;
}
for(k= (mask>>logpagesize)+1;k<=kk;k++){
o,v->base[k]= addr_(reserve_page());
if(!v->base[k]){
for(k--;k> mask>>logpagesize;k--){
o,free_page(page_(v->base[k]));
}
goto cramped;
}
for(j= v->base[k];j<v->base[k]+pagesize;j+= sizeof(long long))
storenulls(j);
zmems+= pagesize/sizeof(long long);
}
}

/*:33*/
#line 614 "bdd14.w"

else{
for(k= v->base[0]+mask+1;k<v->base[0]+newmask;k+= sizeof(long long))
storenulls(k);
zmems+= (newmask-mask)/sizeof(long long);
}
/*34:*/
#line 663 "bdd14.w"

for(k= 0;k<newmask;k+= sizeof(addr)){
oo,r= fetchnode(v,k);
if(r){
storenode(v,k,NULL);
for(o,hash= hashedcode(r);;hash++){
j= addr_(hash)&newmask;
oo,p= fetchnode(v,j);
if(!p)break;
}
storenode(v,j,r);
}else if(k> mask)break;
}

/*:34*/
#line 620 "bdd14.w"
;
v->mask= newmask;
v->free= free+1+(newmask-mask)/sizeof(addr);
goto restart;
}

/*:32*/
#line 595 "bdd14.w"
;
}
storenode(v,k,p);o,v->free= free;
initnewnode(p,v-varhead,l,h);
return p;
cramped:
deref(l);
deref(h);
return NULL;

/*:31*/
#line 539 "bdd14.w"
;
}

/*:26*//*35:*/
#line 682 "bdd14.w"

void table_purge(var*v){
register int free,i,j,jj,k,kk,mask,newmask,oldtotal;
register node*p,*r;
register addr*hash;
o,mask= v->mask,free= v->free;
if(o,v->proj){
oldtotal= totalnodes;
for(k= 0;k<mask;k+= sizeof(addr)){
oo,p= fetchnode(v,k);
if(p&&p->xref<0){
free_node(p);
/*36:*/
#line 710 "bdd14.w"

do{
for(kk= k,j= k+sizeof(addr),k= 0;;j+= sizeof(addr)){
jj= j&mask;
oo,p= fetchnode(v,jj);
if(!p)break;
if(p->xref>=0){
o,i= addr_(hashedcode(p))&mask;
if((i<=kk)+(jj<i)+(kk<jj)> 1)storenode(v,kk,p),kk= jj;
}else if(!k)
k= j,free_node(p);
}
storenode(v,kk,NULL);
}while(k);
k= j;

/*:36*/
#line 694 "bdd14.w"
;
}
}
deadnodes-= oldtotal-totalnodes,free+= oldtotal-totalnodes;
/*37:*/
#line 728 "bdd14.w"

k= (mask>>2)+1-free;
for(newmask= mask;(newmask>>5)>=k;newmask>>= 1);
if(newmask!=mask){
if(verbose&256)
printf("downsizing the hash table for level %d(x%d) (%d slots)\n",
v-varhead,v->name,(newmask+1)/sizeof(addr));
free-= (mask-newmask)>>2;
/*38:*/
#line 748 "bdd14.w"

for(k= newmask+1;k<mask;k+= sizeof(addr)){
oo,r= fetchnode(v,k);
if(r){
for(o,hash= hashedcode(r);;hash++){
j= addr_(hash)&newmask;
oo,p= fetchnode(v,j);
if(!p)break;
}
storenode(v,j,r);
}
}

/*:38*/
#line 736 "bdd14.w"
;
for(k= mask>>logpagesize;k> newmask>>logpagesize;k--)
o,free_page(page_(v->base[k]));
v->mask= newmask;
}

/*:37*/
#line 698 "bdd14.w"
;
o,v->free= free;
}
}

/*:35*//*41:*/
#line 855 "bdd14.w"

void print_memo(memo*m){
printf("%x",id(m->f));
if(m->h==0)printf("[%d]",m->g);
else if(m->h<=maxbinop)printf("%s%x",binopname[m->h],id(m->g));
else printf("%s%x%s%x",
ternopname1[m->h&0xf],id(m->g),ternopname2[m->h&0xf],id(m->h));
printf("=%x\n",id(m->r));
}

void print_cache(void){
register int k;
register memo*m;
for(k= 0;k<cachepages;k++)
for(m= memo_(cachepage[k]);m<memo_(cachepage[k])+cacheslotsperpage;m++)
if(m->r)print_memo(m);
}

/*:41*//*43:*/
#line 891 "bdd14.w"

int choose_cache_size(int items){
register int k,slots;
k= 1,slots= cacheslotsperpage;
while(4*slots<totalnodes-deadnodes&&k<maxcachepages)k<<= 1,slots<<= 1;
while(slots<4*items&&k<maxcachepages)k<<= 1,slots<<= 1;
return k;
}

void cache_init(void){
register int k;
register memo*m;
cachepages= choose_cache_size(0);
if(verbose&(8+16+32+512))
printf("initializing the cache (%d page%s)\n",
cachepages,cachepages==1?"":"s");
for(k= 0;k<cachepages;k++){
o,cachepage[k]= addr_(reserve_page());
if(!cachepage[k]){
fprintf(stderr,"(trouble allocating cache pages!)\n");
for(k--;(k+1)&k;k--)o,free_page(page_(cachepage[k]));
cachepages= k+1;
break;
}
for(m= memo_(cachepage[k]);m<memo_(cachepage[k])+cacheslotsperpage;m++)
m->r= 0;
zmems+= cacheslotsperpage;
}
cachemask= (cachepages<<logpagesize)-1;
cacheinserts= 0;
threshold= 1+(cachepages*cacheslotsperpage)/2;
}

/*:43*//*45:*/
#line 941 "bdd14.w"

node*cache_lookup(node*f,node*g,node*h){
register node*r;
register memo*m;
register addr slot= cachehash(f,g,h);
o,m= thememo(slot);
o,r= node_(m->r);
if(!r)return NULL;
if(o,node_(m->f)==f&&node_(m->g)==g&&node_(m->h)==h){
if(verbose&8){
printf("hit %x: ",(slot&cachemask)/sizeof(memo));
print_memo(m);
}
if(o,r->xref<0){
recursively_revive(r);
return r;
}
return o,r->xref++,r;
}
return NULL;
}

/*:45*//*46:*/
#line 966 "bdd14.w"

void cache_insert(node*f,node*g,node*h,node*r){
register memo*m,*mm;
register int k;
register int slot= cachehash(f,g,h);
if(h)oo;else o;
if(++cacheinserts>=threshold)/*47:*/
#line 985 "bdd14.w"

if(cachepages<maxcachepages){
if(verbose&(8+16+32+512))
printf("doubling the cache (%d pages)\n",cachepages<<1);
for(k= cachepages;k<cachepages+cachepages;k++){
o,cachepage[k]= addr_(reserve_page());
if(!cachepage[k]){
fprintf(stderr,"(trouble doubling cache pages!)\n");
for(k--;k>=cachepages;k--)o,free_page(page_(cachepage[k]));
goto done;
}
for(m= memo_(cachepage[k]);m<memo_(cachepage[k])+cacheslotsperpage;m++)
m->r= 0;
zmems+= cacheslotsperpage;
}
cachepages<<= 1;
cachemask+= cachemask+1;
threshold= 1+(cachepages*cacheslotsperpage)/2;
/*48:*/
#line 1007 "bdd14.w"

for(k= cachepages>>1;k<cachepages;k++){
for(o,m= memo_(cachepage[k]);m<memo_(cachepage[k])+cacheslotsperpage;m++)
if(o,m->r){
if(m->h)oo;else o;
oo,mm= thememo(cachehash(node_(m->f),node_(m->g),node_(m->h)));
if(m!=mm){
oo,*mm= *m;
o,m->r= 0;
}
}
}

/*:48*/
#line 1003 "bdd14.w"
;
}
done:

/*:47*/
#line 972 "bdd14.w"
;
o,m= thememo(slot);
if((verbose&16)&&m->r){
printf("lose %x: ",(slot&cachemask)/sizeof(memo));
print_memo(m);
}
oo,m->f= addr_(f),m->g= addr_(g),m->h= addr_(h),m->r= addr_(r);
if(verbose&32){
printf("set %x: ",(slot&cachemask)/sizeof(memo));
print_memo(m);
}
}

/*:46*//*49:*/
#line 1024 "bdd14.w"

void cache_purge(void){
register int k,items,newcachepages;
register memo*m,*mm;
for(k= items= 0;k<cachepages;k++){
for(m= memo_(cachepage[k]);m<memo_(cachepage[k])+cacheslotsperpage;m++)
if(o,m->r){
if((o,node_(m->r)->xref<0)||(oo,node_(m->f)->xref<0))goto purge;
if(m->h==0){
if(m->g!=thevar(node_(m->f))->timestamp)goto purge;
}else{
if(o,node_(m->g)->xref<0)goto purge;
if(m->h> maxbinop&&(o,node_(m->h&-0x10)->xref<0))goto purge;
}
items++;continue;
purge:o,m->r= 0;
}
}
if(verbose&(8+16+32+512))
printf("purging the cache (%d items left)\n",items);
/*50:*/
#line 1048 "bdd14.w"

newcachepages= choose_cache_size(items);
if(newcachepages<cachepages){
if(verbose&(8+16+32+512))
printf("downsizing the cache (%d page%s)\n",
newcachepages,newcachepages==1?"":"s");
cachemask= (newcachepages<<logpagesize)-1;
for(k= newcachepages;k<cachepages;k++){
for(o,m= memo_(cachepage[k]);m<memo_(cachepage[k])+cacheslotsperpage;m++)
if(o,m->r){
if(m->h)oo;else o;
oo,mm= thememo(cachehash(node_(m->f),node_(m->g),node_(m->h)));
if(m!=mm){
oo,*mm= *m;
}
}
free_page(page_(cachepage[k]));
}
cachepages= newcachepages;
threshold= 1+(cachepages*cacheslotsperpage)/2;
}

/*:50*/
#line 1044 "bdd14.w"
;
cacheinserts= items;
}

/*:49*//*52:*/
#line 1091 "bdd14.w"

void mark(node*p){
rmems++;
restart:if(o,p->xref>=0){
o,p->xref^= 0x80000000;
ooo,mark(node_(p->lo));
o,p= node_(p->hi);
goto restart;
}
}

/*:52*//*53:*/
#line 1105 "bdd14.w"

void unmark(node*p){
rmems++;
restart:if(o,p->xref<0){
o,p->xref^= 0x80000000;
ooo,unmark(node_(p->lo));
o,p= node_(p->hi);
goto restart;
}
}

/*:53*//*54:*/
#line 1127 "bdd14.w"

void print_base(int marked,int unmapped){
register int j,k;
register node*p;
register var*v;
for(v= varhead;v<topofvars;v++)if(v->proj){
for(k= 0;k<v->mask;k+= sizeof(addr)){
p= fetchnode(v,k);
if(p&&(!marked||(p->xref+1)<0)){
if(unmapped)print_node_unmapped(p);else print_node(p);
if(marked||p->xref==0)printf("\n");
else printf(" (%d)\n",p->xref);
}
}
if(!marked&&v->repl)printf("y%d=%x\n",v->name,id(v->repl));
}
if(!marked){
for(j= 0;j<extsize;j++)if(f[j])
printf("f%d=%x\n",j,id(f[j]));
}
}

/*:54*//*55:*/
#line 1151 "bdd14.w"

void print_function(node*p,int unmapped){
unsigned long long savemems= mems,savermems= rmems;

if(p==botsink||p==topsink)printf("%d\n",p-botsink);
else if(p){
mark(p);
print_base(1,unmapped);
unmark(p);
}
mems= savemems,rmems= savermems;
}

/*:55*//*56:*/
#line 1164 "bdd14.w"

void print_profile(node*p){
unsigned long long savemems= mems,savermems= rmems;
register int j,k,tot;
register var*v;
if(!p)printf(" 0\n");
else if(p<=topsink)printf(" 1\n");
else{
tot= 0;
mark(p);
for(v= varhead;v<topofvars;v++)if(v->proj){
/*57:*/
#line 1183 "bdd14.w"

for(j= k= 0;k<v->mask;k+= sizeof(addr)){
register node*q= fetchnode(v,k);
if(q&&(q->xref+1)<0)j++;
}
printf(" %d",j);
tot+= j;

/*:57*/
#line 1175 "bdd14.w"
;
}
unmark(p);
printf(" 2 (total %d)\n",tot+2);
}
mems= savemems,rmems= savermems;
}

/*:56*//*58:*/
#line 1214 "bdd14.w"

#if includesanity
unsigned int sanitycount;
void sanity_check(void){
register node*p,*q;
register int j,k,count,extra;
register var*v;
unsigned long long savemems= mems;
sanitycount++;
/*60:*/
#line 1260 "bdd14.w"

for(p= botsink;p<nodeptr;p++)ghost(p)->xref= 0,ghost(p)->index= -1;
/*64:*/
#line 1316 "bdd14.w"

extra= nodeptr-botsink;
for(p= nodeavail;p;p= node_(p->xref)){
if(!superlegit(p))
printf("! illegal node %x in the list of free nodes\n",id(p));
else extra--,ghost(p)->xref= -1;
}

/*:64*/
#line 1262 "bdd14.w"
;
/*65:*/
#line 1324 "bdd14.w"

ghost(botsink)->index= ghost(topsink)->index= 0;
for(v= varhead;v<topofvars;v++)if(v->proj){
if(!superlegit(node_(v->proj)))
printf("! illegal projection function for level %d\n",v-varhead);
else ghost(v->proj)->index++;
if(v->repl){
if(!legit(node_(v->repl)))
printf("! illegal replacement function for level %d\n",v-varhead);
else ghost(v->repl)->index++;
}
}
for(j= 0;j<extsize;j++)if(f[j]){
if(f[j]> topsink&&!superlegit(f[j]))
printf("! illegal external pointer f%d\n",j);
else ghost(f[j])->index++;
}

/*:65*/
#line 1263 "bdd14.w"
;
for(count= 2,p= topsink+1;p<nodeptr;p++)if(ghost(p)->xref!=-1){
count++;
if(!legit(node_(p->lo))||!legit(node_(p->hi)))
complain("bad pointer")
else if(node_(thevar(p)->proj)==NULL)
complain("bad var")
else if(p->lo==p->hi)
complain("lo=hi")
else{
/*63:*/
#line 1299 "bdd14.w"

{
register addr*hash;
register var*v= thevar(p);
j= v->mask;
for(hash= hashcode(node_(p->lo),node_(p->hi));;hash++){
k= addr_(hash)&j;
q= fetchnode(v,k);
if(!q)break;
if(q->lo==p->lo&&q->hi==p->hi)break;
}
if(q!=p)
complain("unfindable (lo,hi)");
addr__((size_t)(v->base[k>>logpagesize]+(k&pagemask))
-(size_t)mem+(size_t)smem)= sanitycount;
}

/*:63*/
#line 1273 "bdd14.w"
;
if(node_(p->lo)> topsink&&thevar(p)>=thevar(node_(p->lo)))
complain("bad lo rank");
if(node_(p->hi)> topsink&&thevar(p)>=thevar(node_(p->hi)))
complain("bad hi rank");
if(p->xref>=0){
q= ghost(p);
q->lo= ghost(p->lo)->xref,ghost(p->lo)->xref= addr_(&(p->lo));
q->hi= ghost(p->hi)->xref,ghost(p->hi)->xref= addr_(&(p->hi));
}
}
}
if(count!=totalnodes)
printf("! totalnodes should be %d, not %d\n",count,totalnodes);
if(extra!=totalnodes)
printf("! %d nodes have leaked\n",extra-totalnodes);

/*:60*/
#line 1223 "bdd14.w"
;
/*66:*/
#line 1342 "bdd14.w"

for(p= botsink,count= 0;p<nodeptr;p++){
q= ghost(p);
if(q->xref==-1)continue;
for(k= q->index,q= node_(q->xref);q;q= node_(addr__(ghost(q))))k++;
if(p->xref!=k)
printf("! %x->xref should be %d, not %d\n",id(p),k,p->xref);
if(k<0)count++;
}
if(count!=deadnodes)
printf("! deadnodes should be %d, not %d\n",count,deadnodes);

/*:66*/
#line 1224 "bdd14.w"
;
/*68:*/
#line 1374 "bdd14.w"

extra= topofmem-pageptr;
for(v= varhead;v<topofvars;v++)if(v->proj){
for(k= 0;k<=v->mask>>logpagesize;k++)
if(badpage(page_(v->base[k])))
printf("! bad page base %x in unique table for level %d\n",
id(v->base[k]),v-varhead);
extra-= 1+(v->mask>>logpagesize);
for(k= count= 0;k<v->mask;k+= sizeof(addr)){
p= fetchnode(v,k);
if(!p)count++;
else{
if(addr__((size_t)(v->base[k>>logpagesize]+(k&pagemask))
-(size_t)mem+(size_t)smem)!=sanitycount)
printf("! extra node %x in unique table for level %d\n",id(p),v-varhead);
if(!superlegit(p))
printf("! illegal node %x in unique table for level %d\n",id(p),v-varhead);
else if(varpart(p->index)!=v-varhead)
complain("wrong var");
}
}
if(count!=v->free)
printf("! unique table %d has %d free slots, not %d\n",
v-varhead,count,v->free);
}

/*:68*/
#line 1225 "bdd14.w"
;
/*69:*/
#line 1403 "bdd14.w"

{
register memo*m;
extra-= 1+(cachemask>>logpagesize);
for(k= 0;k<cachepages;k++){
if(badpage(page_(cachepage[k])))
printf("! bad page base %x in the cache\n",id(cachepage[k]));
for(m= memo_(cachepage[k]);m<memo_(cachepage[k])+cacheslotsperpage;m++)
if(m->r){
if(!legit(node_(m->r)))goto nogood;
if(!legit(node_(m->f)))goto nogood;
if(m->h> 0){
if(!legit(node_(m->g)))goto nogood;
if(m->h> maxbinop&&!legit(node_(m->h&-0x10)))goto nogood;
}
}
continue;
nogood:printf("! bad node in cache entry ");print_memo(m);
}
}

/*:69*/
#line 1226 "bdd14.w"
;
/*70:*/
#line 1427 "bdd14.w"

{
register page*p= pageavail;
while(p&&extra> 0){
if(badpage(p))
printf("! bad free page %x\n",id(p));
p= page_(p->dat[0]),extra--;
}
if(extra> 0)
printf("! %d pages have leaked\n",extra);
else if(p)
printf("! the free pages form a loop\n");
}

/*:70*/
#line 1227 "bdd14.w"
;
mems= savemems;
}
#endif

/*:58*//*67:*/
#line 1357 "bdd14.w"

#if includesanity
void who_points_to(node*p){
register addr q;
for(q= addr_(ghost(p)->xref);q;q= addr__(ghost(q))){
print_node(node_(q&-sizeof(node)));
printf("\n");
}
}
#endif

/*:67*//*71:*/
#line 1445 "bdd14.w"

void recursively_revive(node*p){
register node*q;
rmems++;
restart:if(verbose&4)printf("reviving %x\n",id(p));
o,p->xref= 0;
deadnodes--;
q= node_(p->lo);
if(o,q->xref<0)oooo,recursively_revive(q);
else o,q->xref++;
p= node_(p->hi);
if(o,p->xref<0)goto restart;
else o,p->xref++;
}

/*:71*//*72:*/
#line 1465 "bdd14.w"

void recursively_kill(node*p){
register node*q;
rmems++;
restart:if(verbose&4)printf("burying %x\n",id(p));
o,p->xref= -1;
deadnodes++;
q= node_(p->lo);
if(o,q->xref==0)oooo,recursively_kill(q);
else o,q->xref--;
p= node_(p->hi);
if(o,p->xref==0)goto restart;
else o,p->xref--;
}

/*:72*//*73:*/
#line 1500 "bdd14.w"

node*and_rec(node*f,node*g){
var*v,*vf,*vg;
node*r,*r0,*r1;
if(f==g)return oo,f->xref++,f;
if(f> g)r= f,f= g,g= r;
if(f<=topsink){
if(f==topsink)return oo,g->xref++,g;
return oo,botsink->xref++,botsink;
}
oo,r= cache_lookup(f,g,node_(1));
if(r)return r;
/*74:*/
#line 1521 "bdd14.w"

rmems++;
vf= thevar(f);
vg= thevar(g);
if(vf<vg)v= vf;
else v= vg;
r0= and_rec((vf==v?o,node_(f->lo):f),(vg==v?o,node_(g->lo):g));
if(!r0)return NULL;
r1= and_rec((vf==v?node_(f->hi):f),(vg==v?node_(g->hi):g));
if(!r1){
deref(r0);
return NULL;
}
r= unique_find(v,r0,r1);
if(r){
if((verbose&128)&&(v<tvar))
printf("   %x=%x&%x (level %d)\n",id(r),id(f),id(g),v-varhead);
cache_insert(f,g,node_(1),r);
}
return r;

/*:74*/
#line 1512 "bdd14.w"
;
}

/*:73*//*75:*/
#line 1546 "bdd14.w"

node*or_rec(node*f,node*g){
var*v,*vf,*vg;
node*r,*r0,*r1;
if(f==g)return oo,f->xref++,f;
if(f> g)r= f,f= g,g= r;
if(f<=topsink){
if(f==topsink)return oo,topsink->xref++,topsink;
return oo,g->xref++,g;
}
oo,r= cache_lookup(f,g,node_(7));
if(r)return r;
/*76:*/
#line 1561 "bdd14.w"

rmems++;
vf= thevar(f);
vg= thevar(g);
if(vf<vg)v= vf;
else v= vg;
r0= or_rec((vf==v?o,node_(f->lo):f),(vg==v?o,node_(g->lo):g));
if(!r0)return NULL;
r1= or_rec((vf==v?node_(f->hi):f),(vg==v?node_(g->hi):g));
if(!r1){
deref(r0);
return NULL;
}
r= unique_find(v,r0,r1);
if(r){
if((verbose&128)&&(v<tvar))
printf("   %x=%x|%x (level %d)\n",id(r),id(f),id(g),v-varhead);
cache_insert(f,g,node_(7),r);
}
return r;

/*:76*/
#line 1558 "bdd14.w"
;
}

/*:75*//*77:*/
#line 1584 "bdd14.w"

node*xor_rec(node*f,node*g){
var*v,*vf,*vg;
node*r,*r0,*r1;
if(f==g)return oo,botsink->xref++,botsink;
if(f> g)r= f,f= g,g= r;
if(f==botsink)return oo,g->xref++,g;
oo,r= cache_lookup(f,g,node_(6));
if(r)return r;
/*78:*/
#line 1600 "bdd14.w"

rmems++;
if(f==topsink)vf= 0,v= vg= thevar(g);
else{
vf= thevar(f),vg= thevar(g);
if(vf<vg)v= vf;
else v= vg;
}
r0= xor_rec((vf==v?o,node_(f->lo):f),(vg==v?o,node_(g->lo):g));
if(!r0)return NULL;
r1= xor_rec((vf==v?node_(f->hi):f),(vg==v?node_(g->hi):g));
if(!r1){
deref(r0);
return NULL;
}
r= unique_find(v,r0,r1);
if(r){
if((verbose&128)&&(v<tvar))
printf("   %x=%x^%x (level %d)\n",id(r),id(f),id(g),v-varhead);
cache_insert(f,g,node_(6),r);
}
return r;

/*:78*/
#line 1593 "bdd14.w"
;
}

/*:77*//*79:*/
#line 1633 "bdd14.w"

node*constrain_rec(node*f,node*g){
var*v,*vf,*vg;
node*r,*r0,*r1;
if(g==botsink)return oo,botsink->xref++,botsink;
if(g==topsink||f<=topsink)return oo,f->xref++,f;

if(f==g)return oo,topsink->xref++,topsink;
oo,r= cache_lookup(f,g,node_(8));
if(r)return r;
/*80:*/
#line 1646 "bdd14.w"

rmems++;
vf= thevar(f);
vg= thevar(g);
if(vf<vg)v= vf;
else{
v= vg;
if(o,node_(g->lo)==botsink){
oo,r= constrain_rec((vf==v?o,node_(f->hi):f),node_(g->hi));
goto shortcut;
}
if(o,node_(g->hi)==botsink){
oo,r= constrain_rec((vf==v?o,node_(f->lo):f),node_(g->lo));
goto shortcut;
}
}
r0= constrain_rec((vf==v?o,node_(f->lo):f),(vg==v?o,node_(g->lo):g));
if(!r0)return NULL;
r1= constrain_rec((vf==v?node_(f->hi):f),(vg==v?node_(g->hi):g));
if(!r1){
deref(r0);
return NULL;
}
r= unique_find(v,r0,r1);
shortcut:if(r){
if((verbose&128)&&(v<tvar))
printf("   %x=%x_%x (level %d)\n",id(r),id(f),id(g),v-varhead);
cache_insert(f,g,node_(8),r);
}
return r;

/*:80*/
#line 1643 "bdd14.w"
;
}

/*:79*//*82:*/
#line 1703 "bdd14.w"

node*exist_rec(node*f,node*g){
var*v,*vg;
node*r,*r0,*r1;
restart:if(g<=topsink)return oo,f->xref++,f;
if(f<=topsink)return oo,f->xref++,f;
v= thevar(f);
vg= thevar(g);
if(v> vg){
o,g= node_(g->hi);goto restart;
}
oo,r= cache_lookup(f,g,node_(15));
if(r)return r;
/*83:*/
#line 1723 "bdd14.w"

rmems++;
o,r0= exist_rec(node_(f->lo),(vg==v?o,node_(g->hi):g));
if(!r0)return NULL;
if(r0==topsink&&vg==v){
r= r0;
goto gotr;
}
r1= exist_rec(node_(f->hi),(vg==v?node_(g->hi):g));
if(!r1){
deref(r0);
return NULL;
}
if(vg> v)r= unique_find(v,r0,r1);
else{
r= or_rec(r0,r1);
deref(r0);deref(r1);
}
gotr:if(r){
if((verbose&128)&&(v<tvar))
printf("   %x=%xE%x (level %d)\n",id(r),id(f),id(g),v-varhead);
cache_insert(f,g,node_(15),r);
}
return r;

/*:83*/
#line 1716 "bdd14.w"
;
}

/*:82*//*84:*/
#line 1751 "bdd14.w"

node*all_rec(node*f,node*g){
var*v,*vg;
node*r,*r0,*r1;
restart:if(g<=topsink)return oo,f->xref++,f;
if(f<=topsink)return oo,f->xref++,f;
v= thevar(f);
vg= thevar(g);
if(v> vg){
o,g= node_(g->hi);goto restart;
}
oo,r= cache_lookup(f,g,node_(9));
if(r)return r;
/*85:*/
#line 1767 "bdd14.w"

rmems++;
o,r0= all_rec(node_(f->lo),(vg==v?o,node_(g->hi):g));
if(!r0)return NULL;
if(r0==botsink&&vg==v){
r= r0;
goto gotr;
}
r1= all_rec(node_(f->hi),(vg==v?node_(g->hi):g));
if(!r1){
deref(r0);
return NULL;
}
if(vg> v)r= unique_find(v,r0,r1);
else{
r= and_rec(r0,r1);
deref(r0);deref(r1);
}
gotr:if(r){
if((verbose&128)&&(v<tvar))
printf("   %x=%xA%x (level %d)\n",id(r),id(f),id(g),v-varhead);
cache_insert(f,g,node_(9),r);
}
return r;

/*:85*/
#line 1764 "bdd14.w"
;
}

/*:84*//*86:*/
#line 1794 "bdd14.w"

node*diff_rec(node*f,node*g){
var*v,*vg;
node*r,*r0,*r1;
if(g<=topsink)return oo,f->xref++,f;
if(f<=topsink)return oo,botsink->xref++,botsink;

v= thevar(f);
vg= thevar(g);
if(v> vg)return oo,botsink->xref++,botsink;

oo,r= cache_lookup(f,g,node_(14));
if(r)return r;
/*87:*/
#line 1810 "bdd14.w"

rmems++;
o,r0= diff_rec(node_(f->lo),(vg==v?o,node_(g->hi):g));
if(!r0)return NULL;
r1= diff_rec(node_(f->hi),(vg==v?node_(g->hi):g));
if(!r1){
deref(r0);
return NULL;
}
if(vg> v)r= unique_find(v,r0,r1);
else{
r= xor_rec(r0,r1);
deref(r0);deref(r1);
}
if(r){
if((verbose&128)&&(v<tvar))
printf("   %x=%xD%x (level %d)\n",id(r),id(f),id(g),v-varhead);
cache_insert(f,g,node_(14),r);
}
return r;

/*:87*/
#line 1807 "bdd14.w"
;
}

/*:86*//*88:*/
#line 1846 "bdd14.w"

node*yes_no_rec(int curop,node*f,node*g){
var*v,*vg;
node*r,*r0,*r1;
if(g<=topsink)return oo,f->xref++,f;
if(f<=topsink)return oo,botsink->xref++,botsink;

v= thevar(f);
vg= thevar(g);
if(v> vg)return oo,botsink->xref++,botsink;

oo,r= cache_lookup(f,g,node_(curop));
if(r)return r;
/*89:*/
#line 1862 "bdd14.w"

rmems++;
o,r0= yes_no_rec(curop,node_(f->lo),(vg==v?o,node_(g->hi):g));
if(!r0)return NULL;
if(r0<=topsink&&vg==v){
if((r0==topsink)==(curop==12)){
r= botsink;deref(r0);botsink->xref++;
goto gotr;
}
}
r1= yes_no_rec(curop,node_(f->hi),(vg==v?node_(g->hi):g));
if(!r1){
deref(r0);
return NULL;
}
if(vg> v)r= unique_find(v,r0,r1);
else{
if(curop==12)r= mux_rec(r0,botsink,r1);
else r= mux_rec(r1,botsink,r0);
deref(r0);deref(r1);
}
gotr:if(r){
if((verbose&128)&&(v<tvar))
printf("   %x=%x%s%x (level %d)\n",
id(r),id(f),binopname[curop],id(g),v-varhead);
cache_insert(f,g,node_(curop),r);
}
return r;

/*:89*/
#line 1859 "bdd14.w"
;
}

/*:88*//*91:*/
#line 1910 "bdd14.w"

node*mux_rec(node*f,node*g,node*h){
var*v,*vf,*vg,*vh;
node*r,*r0,*r1;
if(f<=topsink){
if(f==topsink)return oo,g->xref++,g;
return oo,h->xref++,h;
}
if(g==f||g==topsink)return or_rec(f,h);

if(h==f||h==botsink)return and_rec(f,g);

if(g==h)return oo,g->xref++,g;
if(g==botsink&&h==topsink)return xor_rec(topsink,f);

oo,r= cache_lookup(f,g,h);
if(r)return r;
/*92:*/
#line 1930 "bdd14.w"

rmems++;
v= vf= thevar(f);
if(g==botsink)vg= topofvars;else{
vg= thevar(g);if(v> vg)v= vg;
}
if(h==topsink)vh= topofvars;else{
o,vh= thevar(h);if(v> vh)v= vh;
}
r0= mux_rec((vf==v?o,node_(f->lo):f),(vg==v?o,node_(g->lo):g),
(vh==v?o,node_(h->lo):h));
if(!r0)return NULL;
r1= mux_rec((vf==v?o,node_(f->hi):f),(vg==v?o,node_(g->hi):g),
(vh==v?o,node_(h->hi):h));
if(!r1){
deref(r0);
return NULL;
}
r= unique_find(v,r0,r1);
if(r){
if((verbose&128)&&(v<tvar))
printf("   %x=%x?%x:%x (level %d)\n",id(r),id(f),id(g),id(h),v-varhead);
cache_insert(f,g,h,r);
}
return r;

/*:92*/
#line 1927 "bdd14.w"
;
}

/*:91*//*93:*/
#line 1959 "bdd14.w"

node*med_rec(node*f,node*g,node*h){
var*v,*vf,*vg,*vh;
node*r,*r0,*r1;
if(f> g){
if(g> h)r= f,f= h,h= r;
else if(f> h)r= f,f= g,g= h,h= r;
else r= f,f= g,g= r;
}else if(g> h){
if(f> h)r= f,f= h,h= g,g= r;
else r= g,g= h,h= r;
}
if(f<=topsink){
if(f==topsink)return or_rec(g,h);
return and_rec(g,h);
}
if(f==g)return oo,f->xref++,f;
if(g==h)return oo,g->xref++,g;
oo,r= cache_lookup(f,g,node_(addr_(h)+1));
if(r)return r;
/*94:*/
#line 1982 "bdd14.w"

rmems++;
vf= thevar(f);
vg= thevar(g);
o,vh= thevar(h);
if(vf<vg)v= vf;else v= vg;
if(v> vh)v= vh;
r0= med_rec((vf==v?o,node_(f->lo):f),(vg==v?o,node_(g->lo):g),
(vh==v?o,node_(h->lo):h));
if(!r0)return NULL;
r1= med_rec((vf==v?o,node_(f->hi):f),(vg==v?o,node_(g->hi):g),
(vh==v?o,node_(h->hi):h));
if(!r1){
deref(r0);
return NULL;
}
r= unique_find(v,r0,r1);
if(r){
if((verbose&128)&&(v<tvar))
printf("   %x=%x.%x.%x (level %d)\n",id(r),id(f),id(g),id(h),v-varhead);
cache_insert(f,g,node_(addr_(h)+1),r);
}
return r;

/*:94*/
#line 1979 "bdd14.w"
;
}

/*:93*//*95:*/
#line 2010 "bdd14.w"

node*and_and_rec(node*f,node*g,node*h){
var*v,*vf,*vg,*vh;
node*r,*r0,*r1;
if(f> g){
if(g> h)r= f,f= h,h= r;
else if(f> h)r= f,f= g,g= h,h= r;
else r= f,f= g,g= r;
}else if(g> h){
if(f> h)r= f,f= h,h= g,g= r;
else r= g,g= h,h= r;
}
if(f<=topsink){
if(f==topsink)return and_rec(g,h);
return oo,botsink->xref++,botsink;
}
if(f==g)return and_rec(g,h);
if(g==h)return and_rec(f,g);
oo,r= cache_lookup(f,g,node_(addr_(h)+2));
if(r)return r;
/*96:*/
#line 2033 "bdd14.w"

rmems++;
vf= thevar(f);
vg= thevar(g);
o,vh= thevar(h);
if(vf<vg)v= vf;else v= vg;
if(v> vh)v= vh;
r0= and_and_rec((vf==v?o,node_(f->lo):f),(vg==v?o,node_(g->lo):g),
(vh==v?o,node_(h->lo):h));
if(!r0)return NULL;
r1= and_and_rec((vf==v?o,node_(f->hi):f),(vg==v?o,node_(g->hi):g),
(vh==v?o,node_(h->hi):h));
if(!r1){
deref(r0);
return NULL;
}
r= unique_find(v,r0,r1);
if(r){
if((verbose&128)&&(v<tvar))
printf("   %x=%x&%x&%x (level %d)\n",id(r),id(f),id(g),id(h),v-varhead);
cache_insert(f,g,node_(addr_(h)+2),r);
}
return r;

/*:96*/
#line 2030 "bdd14.w"
;
}

/*:95*//*97:*/
#line 2064 "bdd14.w"

node*and_exist_rec(node*f,node*g,node*h){
var*v,*vf,*vg,*vh;
node*r,*r0,*r1;
restart:if(h<=topsink)return and_rec(f,g);
if(f==g)return exist_rec(f,h);
if(f> g)r= f,f= g,g= r;
if(f<=topsink){
if(f==topsink)return exist_rec(g,h);
return oo,botsink->xref++,botsink;
}
oo,r= cache_lookup(f,g,node_(addr_(h)+3));
if(r)return r;
vf= thevar(f);
vg= thevar(g);
o,vh= thevar(h);
if(vf<vg)v= vf;else v= vg;
if(v> vh){
o,h= node_(h->hi);goto restart;
}
/*98:*/
#line 2087 "bdd14.w"

rmems++;
r0= and_exist_rec((vf==v?o,node_(f->lo):f),(vg==v?o,node_(g->lo):g),
(vh==v?o,node_(h->hi):h));
if(!r0)return NULL;
if(r0==topsink&&vg==v){
r= r0;
goto gotr;
}
r1= and_exist_rec((vf==v?node_(f->hi):f),(vg==v?node_(g->hi):g),
(vh==v?node_(h->hi):h));
if(!r1){
deref(r0);
return NULL;
}
if(vh> v)r= unique_find(v,r0,r1);
else{
r= or_rec(r0,r1);
deref(r0);deref(r1);
}
gotr:if(r){
if((verbose&128)&&(v<tvar))
printf("   %x=%x&%xE%x (level %d)\n",id(r),id(f),id(g),id(h),v-varhead);
cache_insert(f,g,node_(addr_(h)+3),r);
}
return r;

/*:98*/
#line 2084 "bdd14.w"
;
}

/*:97*//*104:*/
#line 2195 "bdd14.w"

node*compose_rec(node*f){
var*vf;
node*r,*r0,*r1;
if(f<=topsink)return oo,f->xref++,f;
o,vf= thevar(f);
if(o,vf->timestamp==0)
return oo,f->xref++,f;
o,r= cache_lookup(f,node_(vf->timestamp),node_(0));
if(r)return r;
/*105:*/
#line 2213 "bdd14.w"

rmems++;
o,r0= compose_rec(node_(f->lo));
if(!r0)return NULL;
r1= compose_rec(node_(f->hi));
if(!r1){
deref(r0);
return NULL;
}
if(o,vf->repl)r= node_(vf->repl);else r= node_(vf->proj);
r= mux_rec(r,r1,r0);
deref(r0);deref(r1);
if(r){
if((verbose&128)&&(vf<tvar))
printf("   %x=%x[%u] (level %d)\n",id(r),id(f),vf->timestamp,vf-varhead);
cache_insert(f,node_(vf->timestamp),node_(0),r);
}
return r;

/*:105*/
#line 2205 "bdd14.w"
;
}

/*:104*//*106:*/
#line 2237 "bdd14.w"

node*binary_top(int curop,node*f,node*g){
node*r;
unsigned long long oldmems= mems,oldrmems= rmems,oldzmems= zmems;
if(verbose&2)
printf("beginning to compute %x %s %x:\n",
id(f),binopname[curop],id(g));
cacheinserts= 0;
while(1){
switch(curop){
case 1:r= and_rec(f,g);break;
case 2:r= mux_rec(g,botsink,f);break;
case 4:r= mux_rec(f,botsink,g);break;
case 6:r= xor_rec(f,g);break;
case 7:r= or_rec(f,g);break;
case 8:r= constrain_rec(f,g);break;
case 9:r= all_rec(f,g);break;
case 10:case 12:r= yes_no_rec(curop,f,g);break;
case 14:r= diff_rec(f,g);break;
case 15:r= exist_rec(f,g);break;
default:fprintf(stderr,"This can't happen!\n");exit(-69);
}
if(r)break;
attempt_repairs();
}
if(verbose&(1+2))
printf(" %x=%x%s%x (%llu mems, %llu rmems, %llu zmems)\n",
id(r),id(f),binopname[curop],id(g),
mems-oldmems,rmems-oldrmems,zmems-oldzmems);
return r;
}

/*:106*//*108:*/
#line 2274 "bdd14.w"

node*ternary_top(int curop,node*f,node*g,node*h){
node*r;
unsigned long long oldmems= mems,oldrmems= rmems,oldzmems= zmems;
if(verbose&2)
printf("beginning to compute %x %s %x %s %x:\n",
id(f),ternopname1[curop-16],id(g),ternopname2[curop-16],id(h));
cacheinserts= 0;
while(1){
switch(curop){
case 16:r= mux_rec(f,g,h);break;
case 17:r= med_rec(f,g,h);break;
case 18:r= and_and_rec(f,g,h);break;
case 19:r= and_exist_rec(f,g,h);break;
default:fprintf(stderr,"This can't happen!\n");exit(-69);
}
if(r)break;
attempt_repairs();
}
if(verbose&(1+2))
printf(" %x=%x%s%x%s%x (%llu mems, %llu rmems, %llu zmems)\n",
id(r),id(f),ternopname1[curop-16],id(g),ternopname2[curop-16],
id(h),mems-oldmems,rmems-oldrmems,zmems-oldzmems);
return r;
}

/*:108*//*109:*/
#line 2300 "bdd14.w"

node*compose_top(node*f){
node*r;
unsigned long long oldmems= mems,oldrmems= rmems,oldzmems= zmems;
if(f<=topsink)return f;
if(verbose&2)
printf("beginning to compute %x[%u]:\n",
id(f),thevar(f)->timestamp);
cacheinserts= 0;
timestamp_changed= 0;
while(1){
r= compose_rec(f);
if(r)break;
attempt_repairs();
}
if(verbose&(1+2))
printf(" %x=%x[%u] (%llu mems, %llu rmems, %llu zmems)\n",
id(r),id(f),thevar(f)->timestamp,
mems-oldmems,rmems-oldrmems,zmems-oldzmems);
return r;
}

/*:109*//*131:*/
#line 2671 "bdd14.w"

void reorder_init(void){
var*v,*vup;
collect_garbage(1);
totalvars= 0;
for(v= varhead,vup= NULL;v<topofvars;v++)if(v->proj){
v->aux= ++totalvars;
v->up= vup;
if(vup)vup->down= v;else firstvar= v;
vup= v;
}
if(vup)vup->down= NULL;else firstvar= NULL;
oldleases= leasesonlife;
leasesonlife= 1;
}

void reorder_fin(void){
cache_init();
leasesonlife= oldleases;
}

/*:131*//*137:*/
#line 2838 "bdd14.w"

void new_unique(var*v,int m){
register int f,j,k;
for(f= 6;(m<<2)> f;f<<= 1);
f= f&(-f);
o,v->free= f,v->mask= (f<<2)-1;
for(k= 0;k<=v->mask>>logpagesize;k++){
o,v->base[k]= addr_(reserve_page());
if(k){
for(j= v->base[k];j<v->base[k]+pagesize;j+= sizeof(long long))
storenulls(j);
zmems+= pagesize/sizeof(long long);
}
}
f= v->mask&pagemask;
for(j= v->base[0];j<v->base[0]+f;j+= sizeof(long long))storenulls(j);
zmems+= (f+1)/sizeof(long long);
}

/*:137*//*138:*/
#line 2863 "bdd14.w"

void insert_node(var*v,node*q){
register int j,k,mask,free;
register addr*hash;
register node*l,*h,*p,*r;
o,l= node_(q->lo),h= node_(q->hi);
restart:o,mask= v->mask,free= v->free;
for(hash= hashcode(l,h);;hash++){
k= addr_(hash)&mask;
oo,r= fetchnode(v,k);
if(!r)break;
}
if(--free<=mask>>4)
/*32:*/
#line 608 "bdd14.w"

{
register int newmask= mask+mask+1,kk= newmask>>logpagesize;
if(verbose&256)
printf("doubling the hash table for level %d(x%d) (%d slots)\n",
v-varhead,v->name,(newmask+1)/sizeof(addr));
if(kk)/*33:*/
#line 629 "bdd14.w"

{
if(newmask> maxmask){
if(verbose&(2+256+512))
printf("profile limit reached for level %d(x%d)\n",v-varhead,v->name);
goto cramped;
}
for(k= (mask>>logpagesize)+1;k<=kk;k++){
o,v->base[k]= addr_(reserve_page());
if(!v->base[k]){
for(k--;k> mask>>logpagesize;k--){
o,free_page(page_(v->base[k]));
}
goto cramped;
}
for(j= v->base[k];j<v->base[k]+pagesize;j+= sizeof(long long))
storenulls(j);
zmems+= pagesize/sizeof(long long);
}
}

/*:33*/
#line 614 "bdd14.w"

else{
for(k= v->base[0]+mask+1;k<v->base[0]+newmask;k+= sizeof(long long))
storenulls(k);
zmems+= (newmask-mask)/sizeof(long long);
}
/*34:*/
#line 663 "bdd14.w"

for(k= 0;k<newmask;k+= sizeof(addr)){
oo,r= fetchnode(v,k);
if(r){
storenode(v,k,NULL);
for(o,hash= hashedcode(r);;hash++){
j= addr_(hash)&newmask;
oo,p= fetchnode(v,j);
if(!p)break;
}
storenode(v,j,r);
}else if(k> mask)break;
}

/*:34*/
#line 620 "bdd14.w"
;
v->mask= newmask;
v->free= free+1+(newmask-mask)/sizeof(addr);
goto restart;
}

/*:32*/
#line 2876 "bdd14.w"
;
storenode(v,k,q);o,v->free= free;
return;
cramped:printf("Uh oh: insert_node hasn't enough memory to continue!\n");
show_stats();
exit(-96);
}

/*:138*//*141:*/
#line 2919 "bdd14.w"

node*swap_find(var*v,node*l,node*h){
register int j,k,mask,free;
register addr*hash;
register node*p,*r;
if(l==h){
return oo,l->xref++,l;
}
restart:o,mask= v->mask,free= v->free;
for(hash= hashcode(l,h);;hash++){
k= addr_(hash)&mask;
oo,p= fetchnode(v,k);
if(!p)goto newnode;
if(node_(p->lo)==l&&node_(p->hi)==h)break;
}
return o,p->xref++,p;
newnode:/*142:*/
#line 2938 "bdd14.w"

if(--free<=mask>>4)/*32:*/
#line 608 "bdd14.w"

{
register int newmask= mask+mask+1,kk= newmask>>logpagesize;
if(verbose&256)
printf("doubling the hash table for level %d(x%d) (%d slots)\n",
v-varhead,v->name,(newmask+1)/sizeof(addr));
if(kk)/*33:*/
#line 629 "bdd14.w"

{
if(newmask> maxmask){
if(verbose&(2+256+512))
printf("profile limit reached for level %d(x%d)\n",v-varhead,v->name);
goto cramped;
}
for(k= (mask>>logpagesize)+1;k<=kk;k++){
o,v->base[k]= addr_(reserve_page());
if(!v->base[k]){
for(k--;k> mask>>logpagesize;k--){
o,free_page(page_(v->base[k]));
}
goto cramped;
}
for(j= v->base[k];j<v->base[k]+pagesize;j+= sizeof(long long))
storenulls(j);
zmems+= pagesize/sizeof(long long);
}
}

/*:33*/
#line 614 "bdd14.w"

else{
for(k= v->base[0]+mask+1;k<v->base[0]+newmask;k+= sizeof(long long))
storenulls(k);
zmems+= (newmask-mask)/sizeof(long long);
}
/*34:*/
#line 663 "bdd14.w"

for(k= 0;k<newmask;k+= sizeof(addr)){
oo,r= fetchnode(v,k);
if(r){
storenode(v,k,NULL);
for(o,hash= hashedcode(r);;hash++){
j= addr_(hash)&newmask;
oo,p= fetchnode(v,j);
if(!p)break;
}
storenode(v,j,r);
}else if(k> mask)break;
}

/*:34*/
#line 620 "bdd14.w"
;
v->mask= newmask;
v->free= free+1+(newmask-mask)/sizeof(addr);
goto restart;
}

/*:32*/
#line 2939 "bdd14.w"
;
p= reserve_node();
storenode(v,k,p);o,v->free= free;
initnewnode(p,v-varhead,l,h);
oooo,l->xref++,h->xref++;
return p;
cramped:printf("Uh oh: swap_find hasn't enough memory to continue!\n");
show_stats();
exit(-95);

/*:142*/
#line 2935 "bdd14.w"
;
}

/*:141*//*145:*/
#line 2986 "bdd14.w"

/*133:*/
#line 2744 "bdd14.w"

void swap(var*u,var*v){
register int j,k,solptr,tangptr,umask,vmask,del;
register int hcount= 0,rcount= 0,scount= 0,tcount= 0,icount= totalnodes;
register node*f,*g,*h,*gg,*hh,*p,*pl,*ph,*q,*ql,*qh,
*firsthidden,*lasthidden;
register var*vg,*vh;
unsigned long long omems= mems,ozmems= zmems;
oo,umask= u->mask,vmask= v->mask;
del= ((u-varhead)^(v-varhead))<<(32-logvarsize);
/*134:*/
#line 2772 "bdd14.w"

solptr= j= 0;tangptr= k= umask+1;
while(1){
for(;j<k;j+= sizeof(addr)){
oo,p= fetchnode(u,j);
if(p==0)continue;
o,pl= node_(p->lo),ph= node_(p->hi);
if((pl> topsink&&(o,thevar(pl)==v))||(ph> topsink&&(o,thevar(ph)==v))){
oooo,pl->xref--,ph->xref--;
break;
}
storenode(u,solptr,p);
solptr+= sizeof(addr),scount++;
}
if(j>=k)break;
for(k-= sizeof(addr);j<k;k-= sizeof(addr)){
oo,q= fetchnode(u,k);
if(q==0)continue;
o,ql= node_(q->lo),qh= node_(q->hi);
if((ql> topsink&&(o,thevar(ql)==v))||(qh> topsink&&(o,thevar(qh)==v)))
oooo,ql->xref--,qh->xref--;
else break;
tangptr-= sizeof(addr),tcount++;
storenode(u,tangptr,q);
}
tangptr-= sizeof(addr),tcount++;
storenode(u,tangptr,p);
if(j>=k)break;
storenode(u,solptr,q);
solptr+= sizeof(addr),scount++;
j+= sizeof(addr);
}

/*:134*/
#line 2754 "bdd14.w"
;
/*135:*/
#line 2812 "bdd14.w"

for(k= 0;k<=umask>>logpagesize;k++)oo,savebase[k]= u->base[k];
new_unique(u,tcount+1);
for(k= rcount= hcount= 0;k<vmask;k+= sizeof(addr)){
oo,p= fetchnode(v,k);
if(p==0)continue;
if(o,p->xref<0){
if(hcount==0)firsthidden= lasthidden= p,hcount= 1;
else o,hcount++,p->xref= addr_(lasthidden),lasthidden= p;
oo,node_(p->lo)->xref--;
oo,node_(p->hi)->xref--;
}else{
rcount++;
oo,p->index^= del;
insert_node(u,p);
}
}

/*:135*/
#line 2755 "bdd14.w"
;
if(verbose&2048)printf(
"swapping %d(x%d)<->%d(x%d): solitary %d, tangled %d, remote %d, hidden %d\n",
u-varhead,u->name,v-varhead,v->name,scount,tcount,rcount,hcount);
/*139:*/
#line 2884 "bdd14.w"

for(k= 0;k<=vmask>>logpagesize;k++)o,free_page(page_(v->base[k]));
new_unique(v,scount);
for(k= 0;k<solptr;k+= sizeof(addr)){
o,p= node_(addr__(savebase[k>>logpagesize]+(k&pagemask)));
oo,p->index^= del;
insert_node(v,p);
}

/*:139*/
#line 2759 "bdd14.w"
;
/*140:*/
#line 2902 "bdd14.w"

for(k= tangptr;k<umask;k+= sizeof(addr)){
o,f= node_(addr__(savebase[k>>logpagesize]+(k&pagemask)));
o,g= node_(f->lo),h= node_(f->hi);
if(g<=topsink)vg= topofvars;else o,vg= thevar(g);
if(h<=topsink)vh= topofvars;else o,vh= thevar(h);

gg= swap_find(v,vg> v?g:(o,node_(g->lo)),vh> v?h:(o,node_(h->lo)));
hh= swap_find(v,vg> v?g:node_(g->hi),vh> v?h:node_(h->hi));
o,f->lo= addr_(gg),f->hi= addr_(hh);
insert_node(u,f);
}

/*:140*/
#line 2760 "bdd14.w"
;
/*143:*/
#line 2949 "bdd14.w"

for(k= 0;k<=umask>>logpagesize;k++)o,free_page(page_(savebase[k]));
if(hcount){
o,firsthidden->xref= addr_(nodeavail);
nodeavail= lasthidden;
totalnodes-= hcount;
}

/*:143*/
#line 2761 "bdd14.w"
;
if(verbose&2048)
printf(" newbies %d, change %d, mems (%llu,0,%llu)\n",
totalnodes-icount+hcount,totalnodes-icount,mems-omems,zmems-ozmems);
/*144:*/
#line 2968 "bdd14.w"

oo,j= u->name,k= v->name;
oooo,u->name= k,v->name= j,varmap[j]= v-varhead,varmap[k]= u-varhead;
oo,j= u->aux,k= v->aux;
if(j*k<0)oo,u->aux= -j,v->aux= -k;
o,j= u->proj,k= u->repl;
oo,u->proj= v->proj,u->repl= v->repl;
o,v->proj= j,v->repl= k;
if(v->repl!=0&&(o,v->timestamp==0)){
for(j= v-varhead;j> u-varhead;j--)o,varhead[j].timestamp= 1;
}else if(v->repl==0&&(o,v->timestamp!=0)&&
(v+1==topofvars||(o,(v+1)->timestamp==0))){
for(j= v-varhead;j> u-varhead;j--)o,varhead[j].timestamp= 0;
}

/*:144*/
#line 2765 "bdd14.w"
;
}

/*:133*/
#line 2987 "bdd14.w"


/*:145*//*148:*/
#line 3022 "bdd14.w"

void sift(var*v){
register int pass,bestscore,origscore,swaps;
var*u= v;
double worstratio,saferatio;
unsigned long long oldmems= mems,oldrmems= rmems,oldzmems= zmems;
bestscore= origscore= totalnodes;
worstratio= saferatio= 1.0;
swaps= pass= 0;
if(o,totalvars-v->aux<v->aux)goto siftdown;
siftup:/*149:*/
#line 3047 "bdd14.w"

while(o,u->up){
swaps++,swap(u->up,u);
u= u->up;
if(bestscore> totalnodes){
bestscore= totalnodes;
if(saferatio<worstratio)saferatio= worstratio;
worstratio= 1.0;
}else if(totalnodes> worstratio*bestscore)
worstratio= (double)totalnodes/bestscore;
}
if(pass==0){
while(u!=v){
o,swaps++,swap(u,u->down);
u= u->down;
}
pass= 1,worstratio= 1.0;
goto siftdown;
}
while(totalnodes!=bestscore){
o,swaps++,swap(u,u->down);
u= u->down;
}
goto wrapup;

/*:149*/
#line 3032 "bdd14.w"
;
siftdown:/*150:*/
#line 3072 "bdd14.w"

while(o,u->down){
swaps++,swap(u,u->down);
u= u->down;
if(bestscore> totalnodes){
bestscore= totalnodes;
if(saferatio<worstratio)saferatio= worstratio;
worstratio= 1.0;
}else if(totalnodes> worstratio*bestscore)
worstratio= (double)totalnodes/bestscore;
}
if(pass==0){
while(u!=v){
o,swaps++,swap(u->up,u);
u= u->up;
}
pass= 1,worstratio= 1.0;
goto siftup;
}
while(totalnodes!=bestscore){
o,swaps++,swap(u->up,u);
u= u->up;
}
goto wrapup;

/*:150*/
#line 3033 "bdd14.w"
;
wrapup:if(verbose&4096)printf(
"sift x%d (%d->%d), %d saved, %.3f safe, %d swaps, (%llu,0,%llu) mems\n",
u->name,v-varhead,u-varhead,origscore-bestscore,saferatio,swaps,
mems-oldmems,zmems-oldzmems);
oo,u->aux= -u->aux;
}

/*:148*//*151:*/
#line 3108 "bdd14.w"

void siftall(void){
register var*v;
reorder_init();
for(v= firstvar;v;){
if(o,v->aux<0){
o,v= v->down;
continue;
}
sift(v);
}
reorder_fin();
}

/*:151*//*156:*/
#line 3173 "bdd14.w"

void collect_garbage(int level){
register int k;
var*v;
last_ditch= 0;
if(!level)cache_purge();
else{
if(verbose&512)printf("clearing the cache\n");
for(k= 0;k<cachepages;k++)free_page(page_(cachepage[k]));
cachepages= 0;
/*103:*/
#line 2185 "bdd14.w"

if(varhead[0].timestamp==0)o,timestamp= timestamp_changed= 0;
else{
timestamp= timestamp_changed= 1;
for(v= varhead;v<topofvars&&(o,v->timestamp);v++)
o,v->timestamp= 1;
}

/*:103*/
#line 3183 "bdd14.w"
;
}
if(verbose&512)printf("collecting garbage (%d/%d)\n",deadnodes,totalnodes);
for(v= varhead;v<topofvars;v++)table_purge(v);
}

/*:156*//*159:*/
#line 3199 "bdd14.w"

void attempt_repairs(void){
register int j,k;
if(last_ditch){
printf("sorry --- there's not enough memory; we have to quit!\n");
/*6:*/
#line 138 "bdd14.w"

printf("Job stats:\n");
printf("  %llu mems plus %llu rmems plus %llu zmems\n",mems,rmems,zmems);
/*18:*/
#line 358 "bdd14.w"

j= nodeptr-(node*)mem;k= topofmem-pageptr;
printf("  %llu bytes of memory (%d nodes, %d pages)\n",
((long long)j)*sizeof(node)+((long long)k)*sizeof(page),j,k);

/*:18*/
#line 141 "bdd14.w"
;

/*:6*/
#line 3204 "bdd14.w"
;
exit(-99);
}
if(verbose&512)printf("(making a last ditch attempt for space)\n");
collect_garbage(1);
cache_init();
last_ditch= 1;
}

/*:159*//*162:*/
#line 3226 "bdd14.w"

void math_print(node*p){
var*v;
int k,s,ss,t;
node*q,*r;
if(!p)return;
outcount++;
sprintf(buf,"/tmp/bdd14-out%d.m",outcount);
outfile= fopen(buf,"w");
if(!outfile){
fprintf(stderr,"I can't open file %s for writing!\n",buf);
exit(-71);
}
fprintf(outfile,"g0=0\ng1=1\n");
if(p> topsink){
mark(p);
for(s= 0,v= topofvars-1;v>=varhead;v--)if(v->proj)
/*163:*/
#line 3250 "bdd14.w"

{
t= 0;
for(k= 0;k<v->mask;k+= sizeof(addr)){
q= fetchnode(v,k);
if(q&&(q->xref+1)<0){
t= 1;
/*164:*/
#line 3263 "bdd14.w"

fprintf(outfile,"g%x=Expand[",id(q));
r= node_(q->lo);
ss= (r<=topsink?0:thevar(r)->aux);
if(ss<s){
if(s==ss+1)fprintf(outfile,"(1+z)*");
else fprintf(outfile,"(1+z)^%d*",s-ss);
}
fprintf(outfile,"g%x+z*",id(r));
r= node_(q->hi);
ss= (r<=topsink?0:thevar(r)->aux);
if(ss<s){
if(s==ss+1)fprintf(outfile,"(1+z)*");
else fprintf(outfile,"(1+z)^%d*",s-ss);
}
fprintf(outfile,"g%x]\n",id(r));

/*:164*/
#line 3257 "bdd14.w"
;
}
}
if(t)v->aux= ++s;
}

/*:163*/
#line 3243 "bdd14.w"
;
unmark(p);
}
fprintf(outfile,"g%x\n",id(p));
fclose(outfile);
}

/*:162*/
#line 91 "bdd14.w"


main(int argc,char*argv[])
{
/*19:*/
#line 363 "bdd14.w"

register int j,k;

/*:19*//*115:*/
#line 2427 "bdd14.w"

char*c,*cc;
node*p,*q,*r;
var*v;
int lhs;
int curop;

/*:115*/
#line 95 "bdd14.w"
;
/*3:*/
#line 105 "bdd14.w"

if(argc> 2||(file_given&&!(infile= fopen(argv[1],"r")))){
fprintf(stderr,"Usage: %s [commandfile]\n",argv[0]);
exit(-1);
}

/*:3*/
#line 96 "bdd14.w"
;
/*5:*/
#line 115 "bdd14.w"

gb_init_rand(0);

/*:5*//*9:*/
#line 162 "bdd14.w"

if(sizeof(long long)!=8){
fprintf(stderr,"Sorry, I assume that sizeof(long long) is 8!\n");
exit(-2);
}

/*:9*//*12:*/
#line 259 "bdd14.w"

botsink= (node*)mem;
topsink= botsink+1;
o,botsink->lo= botsink->hi= addr_(botsink);
o,topsink->lo= topsink->hi= addr_(topsink);
oo,botsink->xref= topsink->xref= 0;
oooo,botsink->index= gb_next_rand();
oooo,topsink->index= gb_next_rand();
totalnodes= 2;
nodeptr= topsink+1;
pageptr= topofmem;

/*:12*//*23:*/
#line 443 "bdd14.w"

for(k= 0;k<varsize;k++)varmap[k]= k;

/*:23*//*44:*/
#line 924 "bdd14.w"

cache_init();

/*:44*//*62:*/
#line 1293 "bdd14.w"

if(sizeof(node)!=16){
fprintf(stderr,"Sorry, I assume that sizeof(node) is 16!\n");
exit(-3);
}

/*:62*/
#line 97 "bdd14.w"
;
while(1)/*110:*/
#line 2328 "bdd14.w"

{
/*111:*/
#line 2344 "bdd14.w"

#if debugging&includesanity
if(verbose&8192)sanity_check();
#endif
if(totalnodes>=toobig)/*153:*/
#line 3134 "bdd14.w"

{
if(verbose&(4096+8192))
printf("autosifting (totalnodes=%d, trigger=%.2f, toobig=%d)\n",
totalnodes,trigger,toobig);
siftall();
if(trigger*totalnodes>=memsize)toobig= memsize;
else toobig= trigger*totalnodes;
}

/*:153*/
#line 2348 "bdd14.w"
;
if(verbose&1024)show_stats();
while(1){
if(timestamp!=-1){
/*17:*/
#line 351 "bdd14.w"

j= (node*)(pageptr-3)-nodeptr;
if(j>=0){
for(p= nodeavail;p&&j<3;o,p= (node_(p->xref)))j++;
if(j>=3)break;
}

/*:17*/
#line 2353 "bdd14.w"
;
}
attempt_repairs();
}

/*:111*/
#line 2330 "bdd14.w"
;
/*113:*/
#line 2364 "bdd14.w"

if(infile){
if(!fgets(buf,bufsize,infile)){
if(file_given)goto alldone;

fclose(infile);
infile= NULL;
continue;
}
if(verbose&64)printf("> %s",buf);
}else while(1){
printf("> ");fflush(stdout);
if(fgets(buf,bufsize,stdin))break;
freopen("/dev/tty","r",stdin);
}

/*:113*/
#line 2331 "bdd14.w"
;
/*114:*/
#line 2397 "bdd14.w"

rescan:for(c= buf;*c==' ';c++);
switch(*c++){
case'\n':if(!infile)printf("(Type `quit' to exit the program.)\n");
case'#':continue;
case'!':printf(buf+1);continue;
case'b':/*146:*/
#line 2989 "bdd14.w"

if(totalvars){
reorder_init();
for(o,v= firstvar->down;v;){
if(ooo,v->name> v->up->name)o,v= v->down;
else{
swap(v->up,v);
if(v->up->up)v= v->up;
else o,v= v->down;
}
}
reorder_fin();
}

/*:146*/
#line 2403 "bdd14.w"
;continue;
case'C':print_cache();continue;
case'f':/*120:*/
#line 2497 "bdd14.w"

getkf;lhs= k;
passblanks;
if(*c++!='=')reporterror;
/*121:*/
#line 2511 "bdd14.w"

passblanks;
switch(*c++){
case'x':getkv;p= projection(varmap[k]);break;
case'f':getkf;p= f[k];checknull(p);break;
case'c':k= *c++-'0';if((k&-2)==0)p= botsink+k;else reporterror;break;
case'~':p= topsink;curop= 6;goto second;

case'.':/*127:*/
#line 2606 "bdd14.w"

if(o,f[lhs]){
deref(f[lhs]);
o,f[lhs]= NULL;
}

/*:127*/
#line 2519 "bdd14.w"
;continue;
default:reporterror;
}

/*:121*/
#line 2501 "bdd14.w"
;
/*122:*/
#line 2536 "bdd14.w"

passblanks;
switch(*c++){
case'&':curop= 1;break;
case'>':curop= 2;break;
case'<':curop= 4;break;
case'^':curop= 6;break;
case'|':curop= 7;break;
case'_':curop= 8;break;
case'A':curop= 9;break;
case'N':curop= 10;break;
case'Y':curop= 12;break;
case'D':curop= 14;break;
case'E':curop= 15;break;
case'?':curop= 16;break;
case'.':curop= 17;break;
case'[':curop= 0;
if(*c++!='y')reporterror;
if(*c++!=']')reporterror;
goto fourth;
case'\n':curop= 7,q= p,c--;goto fourth;
default:reporterror;
}

/*:122*/
#line 2502 "bdd14.w"
;
second:/*123:*/
#line 2560 "bdd14.w"

passblanks;
switch(*c++){
case'x':getkv;q= projection(varmap[k]);break;
case'f':getkf;q= f[k];checknull(q);break;
case'c':k= *c++-'0';if((k&-2)==0)q= botsink+k;else reporterror;break;
default:reporterror;
}

/*:123*/
#line 2503 "bdd14.w"
;
third:/*124:*/
#line 2569 "bdd14.w"


passblanks;
if(curop==1&&*c=='&')curop= 18;
if(curop==1&&*c=='E')curop= 19;
if(curop<=maxbinop)r= NULL;
else{
if(*c++!=ternopname2[curop-16][0])reporterror;
passblanks;
switch(*c++){
case'x':getkv;r= projection(varmap[k]);break;
case'f':getkf;r= f[k];checknull(r);break;
case'c':k= *c++-'0';if((k&-2)==0)r= botsink+k;else reporterror;break;
default:reporterror;
}
}

/*:124*/
#line 2504 "bdd14.w"
;
fourth:/*125:*/
#line 2588 "bdd14.w"

passblanks;
if(*c!='\n'&&*c!='#'){
reportjunk:c++;
reporterror;
}
if(curop==0)r= compose_top(p);
else if(curop<=maxbinop)r= binary_top(curop,p,q);
else r= ternary_top(curop,p,q,r);

/*:125*/
#line 2505 "bdd14.w"
;
/*126:*/
#line 2602 "bdd14.w"

if(o,f[lhs])deref(f[lhs]);
o,f[lhs]= r;

/*:126*/
#line 2506 "bdd14.w"
;

/*:120*/
#line 2405 "bdd14.w"
;continue;
case'i':/*116:*/
#line 2440 "bdd14.w"

if(infile)
printf("Sorry --- you can't include one file inside of another.\n");
else{
for(;isgraph(*c);c++);
passblanks;
for(cc= c;isgraph(*c);c++);
*c= '\0';
if(!(infile= fopen(cc,"r")))
printf("Sorry --- I couldn't open file `%s'!\n",cc);
}

/*:116*/
#line 2406 "bdd14.w"
;continue;
case'l':getk;leasesonlife= k;continue;
case'm':/*160:*/
#line 3217 "bdd14.w"

getkf;
math_print(f[k]);
fprintf(stderr,"(generating function for f%d written to %s)\n",k,buf);

/*:160*/
#line 2408 "bdd14.w"
;continue;
case'o':/*118:*/
#line 2479 "bdd14.w"

{
int unmapped= 0;
if(*c=='u')c++,unmapped= 1;
getkf;
sprintf(buf,"/tmp/f%d.bdd",k);
freopen(buf,"w",stdout);
print_function(f[k],unmapped);
freopen("/dev/tty","w",stdout);
}

/*:118*/
#line 2409 "bdd14.w"
;continue;
case'O':/*119:*/
#line 2490 "bdd14.w"

for(v= varhead;v<topofvars;v++)if(v->proj)
printf(" x%d",v->name);
printf("\n");

/*:119*/
#line 2410 "bdd14.w"
;continue;
case'p':/*117:*/
#line 2460 "bdd14.w"

if(*c=='p'){
c++;getkf;
printf("p%d:",k);
print_profile(f[k]);
}else{
getkf;
printf("f%d=",k);
print_function(f[k],0);
}

/*:117*/
#line 2411 "bdd14.w"
;continue;
case'P':print_base(0,0);continue;
case'q':goto alldone;
case'r':/*152:*/
#line 3128 "bdd14.w"

getk;
trigger= k/100.0;
if(trigger*totalnodes>=memsize)toobig= memsize;
else toobig= trigger*totalnodes;

/*:152*/
#line 2414 "bdd14.w"
;continue;
case's':/*130:*/
#line 2653 "bdd14.w"

getkv;v= &varhead[varmap[k]];
if(o,!v->proj)projection(k);
reorder_init();
if(v->up)swap(v->up,v);
reorder_fin();

/*:130*/
#line 2415 "bdd14.w"
;continue;
case'S':if(isdigit(*c))/*147:*/
#line 3007 "bdd14.w"

{
getkv;v= &varhead[varmap[k]];
if(o,v->proj){
reorder_init();
sift(v);
reorder_fin();
}
}

/*:147*/
#line 2416 "bdd14.w"

else siftall();continue;
case't':/*129:*/
#line 2635 "bdd14.w"

getkv;
tvar= &varhead[k+1];

/*:129*/
#line 2418 "bdd14.w"
;continue;
case'v':getk;verbose= k;continue;
case'V':verbose= -1;continue;
case'y':/*128:*/
#line 2612 "bdd14.w"

getkv;v= &varhead[varmap[k]];
if(o,!v->proj)projection(k);
passblanks;
if(*c++!='=')reporterror;
passblanks;
if(*c=='.')c++,q= NULL;
else{
/*123:*/
#line 2560 "bdd14.w"

passblanks;
switch(*c++){
case'x':getkv;q= projection(varmap[k]);break;
case'f':getkf;q= f[k];checknull(q);break;
case'c':k= *c++-'0';if((k&-2)==0)q= botsink+k;else reporterror;break;
default:reporterror;
}

/*:123*/
#line 2620 "bdd14.w"
;
if(o,q==node_(v->proj))q= NULL;
}
passblanks;
if(*c!='\n'&&*c!='#')goto reportjunk;
if(o,v->repl!=addr_(q)){
p= node_(v->repl);
if(p)deref(p);
/*102:*/
#line 2166 "bdd14.w"

v->repl= addr_(q);
if(q==NULL){
if(v+1<topofvars&&(o,(v+1)->timestamp!=0))goto newstamps;
while(v>=varhead&&(o,v->repl==0))
o,v->timestamp= 0,v--;
if(v>=varhead)goto newstamps;
timestamp-= timestamp_changed,timestamp_changed= 0;
}else{
oo,q->xref++;
newstamps:if(!timestamp_changed)
timestamp_changed= 1,timestamp++;
while(v>=varhead&&(o,v->timestamp!=timestamp))
o,v->timestamp= timestamp,v--;
}

/*:102*/
#line 2628 "bdd14.w"
;
}

/*:128*/
#line 2421 "bdd14.w"
;continue;
case'$':show_stats();continue;
default:reporterror;
}
nextcommand:continue;

/*:114*/
#line 2332 "bdd14.w"
;
}

/*:110*/
#line 98 "bdd14.w"
;
alldone:/*6:*/
#line 138 "bdd14.w"

printf("Job stats:\n");
printf("  %llu mems plus %llu rmems plus %llu zmems\n",mems,rmems,zmems);
/*18:*/
#line 358 "bdd14.w"

j= nodeptr-(node*)mem;k= topofmem-pageptr;
printf("  %llu bytes of memory (%d nodes, %d pages)\n",
((long long)j)*sizeof(node)+((long long)k)*sizeof(page),j,k);

/*:18*/
#line 141 "bdd14.w"
;

/*:6*/
#line 99 "bdd14.w"
;
exit(0);
}

/*:2*/
