#define maxm 1000
#define maxdigs (1+(maxm/15) ) 
#define o mems++
#define oo mems+= 2 \

#define nybb(a,p) (int) ((a[p/15]>>(4*(p%15) ) ) &0xf)  \

/*1:*/
#line 31 "./back-pdi.w"

#include <stdio.h> 
#include <stdlib.h> 
int m;
typedef unsigned long long ull;
ull mems;
ull nodes;
ull thresh= 10000000000;
ull profile[maxm+3];
int count;
int vbose;
/*8:*/
#line 132 "./back-pdi.w"

int mdigs;
ull table[maxm+2][10][maxdigs];
ull z[maxdigs];

/*:8*//*14:*/
#line 238 "./back-pdi.w"

int dist[maxm+1][16],pdist[maxm+1][16];
ull a[maxm+1][maxdigs],b[maxm+1][maxdigs],sig[maxm+1][maxdigs];
int x[maxm+1],rsave[maxm+1],tsave[maxm+1],pdsave[maxm+1];

/*:14*/
#line 42 "./back-pdi.w"
;
/*4:*/
#line 79 "./back-pdi.w"

void add(ull*p,ull*q,ull*r){
register int k,c;
register ull t,w,x,y;
for(k= c= 0;k<mdigs;k++)
/*5:*/
#line 94 "./back-pdi.w"

{
o,x= *(p+k)+c;
o,y= *(q+k)+0x666666666666666;
t= x+y;
w= (t^x^y)&0x1111111111111110;
w= (w^0x1111111111111110)>>3;
t-= w+(w<<1);
o,*(r+k)= t&0xfffffffffffffff;
c= t>>60;
}

/*:5*/
#line 84 "./back-pdi.w"
;
if(c){
fprintf(stderr,"Overflow!\n");
exit(-999);
}
}

/*:4*//*6:*/
#line 109 "./back-pdi.w"

void kmult(int k,ull*a){
switch(k){
case 8:add(a,a,a);
case 4:add(a,a,a);
case 2:add(a,a,a);break;
case 6:add(a,a,a);
case 3:add(a,a,z);add(a,z,a);break;
case 5:add(a,a,z);add(z,z,z);add(a,z,a);break;
case 9:add(a,a,z);add(z,z,z);add(z,z,z);add(a,z,a);break;
case 7:add(a,a,z);add(a,z,z);add(z,z,z);add(a,z,a);break;
case 0:case 1:break;
}
}

/*:6*//*10:*/
#line 144 "./back-pdi.w"

void printnum(ull*a,int t){
register int k;
for(k= m;k>=0;k--){
if(t==k)fprintf(stderr,"|");
fprintf(stderr,"%d",nybb(a,k));
}
}

/*:10*/
#line 43 "./back-pdi.w"
;
main(int argc,char*argv[]){
register int j,k,l,p,r,t,pd,alt,blt,xl,change;
/*3:*/
#line 60 "./back-pdi.w"

if(argc<2||sscanf(argv[1],"%d",&m)!=1){
fprintf(stderr,"Usage: %s m [profile] [verbose] [extraverbose]\n",argv[0]);
exit(-1);
}
vbose= argc-2;
if(m<2||m> maxm){
fprintf(stderr,"Sorry, m should be between 2 and %d, not %d!\n",maxm,m);
exit(-2);
}
mdigs= 1+(m/15);

/*:3*/
#line 46 "./back-pdi.w"
;
/*7:*/
#line 124 "./back-pdi.w"

for(k= 1;k<10;k++){
table[1][k][0]= k;
for(j= 2;j<=m;j++)kmult(k,table[1][k]);
for(j= 2;j<=m+1;j++)
add(table[1][k],table[j-1][k],table[j][k]);
}

/*:7*/
#line 47 "./back-pdi.w"
;
/*11:*/
#line 158 "./back-pdi.w"

b1:/*15:*/
#line 243 "./back-pdi.w"

l= 1;
pd= pdsave[1]= 0;
alt= 0,blt= 9;
t= m,r= m+1;
xl= 9;
profile[1]= 1;
goto b3;

/*:15*/
#line 159 "./back-pdi.w"
;
b2:profile[l]++,nodes++;
/*12:*/
#line 189 "./back-pdi.w"

if(mems>=thresh){
thresh+= 10000000000;
fprintf(stderr,"After %lld mems:",mems);
for(k= 2;k<=l;k++)fprintf(stderr," %lld",profile[k]);
fprintf(stderr,"\n");
}

/*:12*/
#line 161 "./back-pdi.w"
;
for(k= 0;k<10;k++){
pdist[l][k]= pdist[l-1][k];
dist[l][k]= dist[l-1][k]+(k==xl?1:0);
}
oo,oo;
if(pd)/*22:*/
#line 360 "./back-pdi.w"

{
if(vbose> 1)fprintf(stderr,"Level %d, that %d was forced\n",l,xl);
for(k= 0;k<mdigs;k++)oo,sig[l][k]= sig[l-1][k];
if(--pd)goto move;
}

/*:22*/
#line 167 "./back-pdi.w"

else{
if(r==0)goto b5;
r--,add(sig[l-1],table[1][xl],sig[l]);
}
if(l> m+1)/*17:*/
#line 258 "./back-pdi.w"

{
count++;
printf("%d: ",count);
for(k= 1;k<=m+1;k++)printf("%d",x[k]);
printf("->");
for(k= m;k>=0;k--)printf("%d",nybb(sig[l],k));
printf("\n");
goto b5;
}

/*:17*/
#line 172 "./back-pdi.w"
;
b3:if(vbose> 1)fprintf(stderr,"Level %d, trying %d (%lld mems)\n",
l,xl,mems);
/*18:*/
#line 272 "./back-pdi.w"

loop:if(t>=0){
change= 0;
/*19:*/
#line 292 "./back-pdi.w"

if(blt<xl){
if(r==0)goto b5;
add(sig[l],table[1][alt],a[l]);
add(sig[l],table[1][blt],b[l]);
add(b[l],table[r-1][xl],b[l]);
}else{
for(k= 0;k<mdigs;k++)oo,a[l][k]= sig[l][k];
add(sig[l],table[r][xl],b[l]);
}
if(o,alt!=nybb(a[l],t)){
if(alt> nybb(a[l],t)){
fprintf(stderr,"Confusion (a decreased)!\n");
exit(-13);
}
alt= nybb(a[l],t);
if(blt<xl)change= 1;
}
if(o,blt!=nybb(b[l],t)){
if(blt<nybb(b[l],t)){
fprintf(stderr,"Confusion (b increased)!\n");
exit(-14);
}
blt= nybb(b[l],t);
if(blt<xl)change= 1;
}

/*:19*/
#line 275 "./back-pdi.w"
;
if(vbose> 2){
fprintf(stderr," a=");
printnum(a[l],t);
fprintf(stderr,",b=");
printnum(b[l],t);
fprintf(stderr,"\n");
}
if(change)goto loop;
while(alt==blt)/*20:*/
#line 333 "./back-pdi.w"

{
o,p= pdist[l][blt];
if(blt>=xl){
if(o,p<dist[l][blt])goto okay;
if(blt> xl)goto b5;
pd= p+1-dist[l][blt];
}
if(--r<0)goto b5;
add(sig[l],table[1][blt],sig[l]);
okay:o,pdist[l][blt]= p+1;
t--,change= 1;
if(t<0)break;
oo,alt= nybb(a[l],t),blt= nybb(b[l],t);
}

/*:20*/
#line 284 "./back-pdi.w"
;
if(change)goto loop;
}

/*:18*/
#line 175 "./back-pdi.w"
;
move:/*16:*/
#line 252 "./back-pdi.w"

oo,tsave[l]= t,rsave[l]= r;
o,pdsave[l]= pd;
o,x[l++]= xl;
goto b2;

/*:16*/
#line 176 "./back-pdi.w"
;
b4:if(xl){
xl--;
o,pd= pdist[l][xl];
goto b3;
}
b5:if(--l){
o,pd= pdsave[l];
if(pd)goto b5;
/*21:*/
#line 349 "./back-pdi.w"

oo,t= tsave[l],r= rsave[l];
if(t>=0)oo,alt= nybb(a[l],t),blt= nybb(b[l],t);
else alt= blt= 9;
o,xl= x[l];

/*:21*/
#line 185 "./back-pdi.w"
;
goto b4;
}

/*:11*/
#line 48 "./back-pdi.w"
;
fprintf(stderr,"Altogether %d solutions for m=%d (%llu nodes, %llu mems).\n",
count,m,nodes,mems);
if(vbose)/*2:*/
#line 54 "./back-pdi.w"

{
fprintf(stderr,"Profile:          1\n");
for(k= 2;k<=m+2;k++)fprintf(stderr,"%19lld\n",profile[k]);
}

/*:2*/
#line 51 "./back-pdi.w"
;
}

/*:1*/
