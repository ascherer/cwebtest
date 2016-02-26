#define maps (6*6*6*6) 
#define isos (24*maps)  \

#define pack(a,b,c,d) ((((a) *3+(b) ) *3+(c) ) *3+(d) )  \

#define pmap(d) trit[perm[p][d]]
#define ppack(p,a,b,c,d) (((((p) *6+(a) ) *6+(b) ) *6+(c) ) *6+(d) )  \

#define head current[0]
#define curval(i) current[i].v \

#define infty 81 \

#define succ(p) (element*) ((char*) p+sizeof(node) )  \

/*1:*/
#line 38 "setset.w"

#include <stdio.h> 
/*3:*/
#line 88 "setset.w"

typedef char SETcard;


/*:3*//*9:*/
#line 182 "setset.w"

typedef struct elt_struct{
SETcard val;
char tlevel;
char level;
struct elt_struct*link;
struct elt_struct*next;
struct elt_struct*fixer;
}element;

typedef struct{
SETcard v;
element image[isos];
}node;

/*:9*/
#line 40 "setset.w"

/*4:*/
#line 94 "setset.w"

int hexform[81]= {
0x1111,0x1112,0x1113,0x1121,0x1122,0x1123,0x1131,0x1132,0x1133,
0x1211,0x1212,0x1213,0x1221,0x1222,0x1223,0x1231,0x1232,0x1233,
0x1311,0x1312,0x1313,0x1321,0x1322,0x1323,0x1331,0x1332,0x1333,
0x2111,0x2112,0x2113,0x2121,0x2122,0x2123,0x2131,0x2132,0x2133,
0x2211,0x2212,0x2213,0x2221,0x2222,0x2223,0x2231,0x2232,0x2233,
0x2311,0x2312,0x2313,0x2321,0x2322,0x2323,0x2331,0x2332,0x2333,
0x3111,0x3112,0x3113,0x3121,0x3122,0x3123,0x3131,0x3132,0x3133,
0x3211,0x3212,0x3213,0x3221,0x3222,0x3223,0x3231,0x3232,0x3233,
0x3311,0x3312,0x3313,0x3321,0x3322,0x3323,0x3331,0x3332,0x3333};

/*:4*//*5:*/
#line 110 "setset.w"

char z[3][3]= {{0,2,1},{2,1,0},{1,0,2}};
char third[81][81];

/*:5*//*8:*/
#line 146 "setset.w"

char trit[4];
char perm[24][4]= {
{0,1,2,3},{0,2,1,3},{1,0,2,3},{1,2,0,3},{2,0,1,3},{2,1,0,3},
{0,1,3,2},{0,3,1,2},{1,0,3,2},{1,3,0,2},{3,0,1,2},{3,1,0,2},
{0,2,3,1},{0,3,2,1},{2,0,3,1},{2,3,0,1},{3,0,2,1},{3,2,0,1},
{1,2,3,0},{1,3,2,0},{2,1,3,0},{2,3,1,0},{3,1,2,0},{3,2,1,0}};
char aut[31104][81],tua[31104][81];

/*:8*//*10:*/
#line 203 "setset.w"

node current[22];

/*:10*//*12:*/
#line 222 "setset.w"

unsigned int tab[82];
char here[81];

/*:12*//*13:*/
#line 237 "setset.w"

element*top[82];
element*oldtop[22][81];
element*back[22],*aback[22];

/*:13*//*17:*/
#line 288 "setset.w"

int auts;
element*newauts;

/*:17*//*35:*/
#line 499 "setset.w"

int non_iso_count[30];
double total_count[30];
int verbose= 0;

/*:35*/
#line 41 "setset.w"

/*15:*/
#line 253 "setset.w"

int count(element*p)
{
register int c;
register element*q;
for(q= p,c= 0;q;q= q->next)c++;
return c;
}

/*:15*/
#line 42 "setset.w"


main()
{
/*18:*/
#line 292 "setset.w"

int l;
register int j,k;

/*:18*//*22:*/
#line 316 "setset.w"

element*p,*pp;
int ll;
int alf;
register element*q,*r;
int jj;

/*:22*/
#line 46 "setset.w"

/*6:*/
#line 116 "setset.w"

{
int a,b,c,d,e,f,g,h;
for(a= 0;a<3;a++)for(b= 0;b<3;b++)for(c= 0;c<3;c++)for(d= 0;d<3;d++)
for(e= 0;e<3;e++)for(f= 0;f<3;f++)for(g= 0;g<3;g++)for(h= 0;h<3;h++)
third[pack(a,b,c,d)][pack(e,f,g,h)]= pack(z[a][e],z[b][f],z[c][g],z[d][h]);
}

/*:6*//*7:*/
#line 133 "setset.w"

{
int a,b,c,d,e,f,g,h,p,s,t;
for(p= 0;p<24;p++)
for(a= 0;a<6;a++)for(b= 0;b<6;b++)for(c= 0;c<6;c++)for(d= 0;d<6;d++)
for(e= 0;e<3;e++)for(f= 0;f<3;f++)for(g= 0;g<3;g++)for(h= 0;h<3;h++)
trit[0]= perm[a][e],trit[1]= perm[b][f],
trit[2]= perm[c][g],trit[3]= perm[d][h],
alf= ppack(p,a,b,c,d),
s= pack(e,f,g,h),t= pack(pmap(0),pmap(1),pmap(2),pmap(3)),
aut[alf][s]= t,tua[alf][t]= s;
}

/*:7*//*11:*/
#line 208 "setset.w"

for(j= 0;j<isos;j++)
head.image[j].val= infty,head.image[j].tlevel= 1,
head.image[j].link= head.image[j].fixer= &head.image[j];

/*:11*//*14:*/
#line 244 "setset.w"

head.v= -1;
for(k= 1;k<isos-1;k++)
head.image[k].next= &head.image[k+1];
top[infty]= &head.image[1];

/*:14*/
#line 47 "setset.w"
;
/*16:*/
#line 265 "setset.w"

l= 0;j= 0;
moveup:while(tab[j])j++;
if(j==infty)goto big_backup;
l++,curval(l)= j,here[j]= 1;
for(k= 0;k<infty;k++)oldtop[l][k]= top[k];
auts= 1,newauts= NULL;
/*21:*/
#line 306 "setset.w"

for(pp= NULL,p= top[j];p;r= p->next,p->next= pp,pp= p,p= r){
ll= p->level;
alf= p-&current[ll].image[0];
/*23:*/
#line 339 "setset.w"

jj= tua[alf][curval(p->tlevel+1)];
if(tab[jj]||(jj<j&&!here[jj])){
for(jj= curval(p->tlevel)+1;jj<curval(p->tlevel+1);jj++){
k= tua[alf][jj];
if(k> j)tab[k]++;
else if(here[k])
/*33:*/
#line 470 "setset.w"

{
for(jj--;jj> curval(p->tlevel);jj--){
k= tua[alf][jj];
if(k> j)tab[k]--;
}
r= p,p= pp,pp= r;
goto backup_a;
}

/*:33*/
#line 346 "setset.w"
;
}
continue;
}

/*:23*/
#line 310 "setset.w"
;
/*24:*/
#line 353 "setset.w"

for(ll++,q= succ(p);q<&current[l].image[0];ll++,q= succ(q)){
q->val= aut[alf][curval(ll)];
for(r= p->fixer;r->link->val<q->val;r= r->link);
q->link= r->link;
r->link= q;
}
q->val= curval(p->tlevel),q->link= p->fixer->link,p->fixer->link= q;

/*:24*/
#line 311 "setset.w"
;
/*25:*/
#line 362 "setset.w"

for(r= q,ll= p->tlevel+1;r->link->val==curval(ll);r= r->link,ll++);
if(r->link->val<curval(ll))
/*32:*/
#line 464 "setset.w"

{
r= p->next,p->next= pp,pp= r;
goto backup_b;
}

/*:32*/
#line 365 "setset.w"
;
q->tlevel= ll,q->fixer= r;
/*26:*/
#line 378 "setset.w"

for(jj= (ll> l?j:curval(ll))-1;jj> curval(p->tlevel);jj--){
k= tua[alf][jj];
if(k> j)tab[k]++;
}

/*:26*/
#line 367 "setset.w"
;
if(ll> l)auts++,q->next= newauts,newauts= q;
else jj= tua[alf][curval(ll)],q->level= l,q->next= top[jj],top[jj]= q;

/*:25*/
#line 312 "setset.w"
;
}
top[j]= NULL,back[l]= pp;

/*:21*/
#line 273 "setset.w"
;
/*29:*/
#line 417 "setset.w"

for(pp= NULL,p= top[infty];p;r= p->next,p->next= pp,pp= p,p= r){
alf= p-&current[l-1].image[0];
jj= aut[alf][j];
if(jj<j)/*31:*/
#line 455 "setset.w"

{
r= p,p= pp,pp= r;
goto backup_c;
}

/*:31*/
#line 421 "setset.w"
;
q= succ(p);
q->link= p->fixer->link,p->fixer->link= q;
if(jj> j){
q->val= jj,q->level= l,q->tlevel= l,q->fixer= p->fixer;
jj= tua[alf][j],q->next= top[jj],top[jj]= q;
}else{
q->val= jj,q->tlevel= l+1,q->fixer= q;
auts++,q->next= newauts,newauts= q;
}
for(jj= curval(l-1)+1;jj<j;jj++){
k= tua[alf][jj];
if(k> j)tab[k]++;
}
}
top[infty]= newauts,aback[l]= pp;

/*:29*/
#line 275 "setset.w"
;
/*34:*/
#line 484 "setset.w"

if(verbose||l<=8){
for(j= 1;j<l;j++)printf(".");
printf("%04x (%d)\n",hexform[curval(l)],auts);
}else if(l>=20){
for(j= 1;j<=l;j++)printf(" %x",hexform[curval(j)]);
printf(" (%d)\n",auts);
}
non_iso_count[l]++;
total_count[l]+= 31104.0/(double)auts;

/*:34*/
#line 276 "setset.w"
;
/*19:*/
#line 296 "setset.w"

for(j= 1;j<l;j++)tab[third[curval(j)][curval(l)]]++;

/*:19*/
#line 277 "setset.w"
;
j= curval(l)+1;goto moveup;
big_backup:/*20:*/
#line 299 "setset.w"

for(j= 1;j<l;j++)tab[third[curval(j)][curval(l)]]--;

/*:20*/
#line 279 "setset.w"
;
j= curval(l);
/*30:*/
#line 438 "setset.w"

pp= NULL,p= aback[l];
backup_c:while(p){
alf= p-&current[l-1].image[0];
q= succ(p);
p->fixer->link= q->link;
for(jj= curval(l-1)+1;jj<j;jj++){
k= tua[alf][jj];
if(k> j)tab[k]--;
}
r= p->next,p->next= pp,pp= p,p= r;
}
top[infty]= pp;

/*:30*/
#line 281 "setset.w"
;
/*28:*/
#line 398 "setset.w"

pp= NULL,p= back[l];
backup_a:while(p){
alf= p-&current[p->level].image[0];
if(p->fixer->link<&current[l].image[0]){
for(jj= curval(p->tlevel)+1;jj<curval(p->tlevel+1);jj++){
k= tua[alf][jj];
if(k> j)tab[k]--;
}
}else{
ll= current[l].image[alf].tlevel;
/*27:*/
#line 386 "setset.w"

for(jj= (ll> l?j:curval(ll))-1;jj> curval(p->tlevel);jj--){
k= tua[alf][jj];
if(k> j)tab[k]--;
}

/*:27*/
#line 409 "setset.w"
;
backup_b:ll= p->level;
for(r= p->fixer,jj= l-ll;jj;r= r->link)
if(r->link> p)jj--,r->link= r->link->link;
}
r= p->next,p->next= pp,pp= p,p= r;
}

/*:28*/
#line 282 "setset.w"
;
for(k= 0;k<infty;k++)top[k]= oldtop[l][k];
here[j]= 0;
j++,l--;
if(l)goto moveup;

/*:16*/
#line 48 "setset.w"
;
/*36:*/
#line 504 "setset.w"

for(j= 1;j<=21;j++)
printf("%20.20g SETless %d-sets (%d cases)\n",
total_count[j],j,non_iso_count[j]);

/*:36*/
#line 49 "setset.w"
;
}

/*:1*/
