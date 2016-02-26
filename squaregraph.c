#define maxp 10
#define verbose (argc> 1)  \

#define rdel(x) pp-del[pp+pp-x] \

#define mate(k) ((k) ^1)  \

/*2:*/
#line 115 "squaregraph.w"

#include <stdio.h> 
#include <stdlib.h> 
/*3:*/
#line 149 "squaregraph.w"

int del[4*maxp];
int occ[3*maxp+1];
int back[2*maxp];

/*:3*//*12:*/
#line 280 "squaregraph.w"

struct{
int succ;
int from,to;
int reg;
}e[2*maxp*maxp];
int eptr;

/*:12*/
#line 118 "squaregraph.w"
;
int pp;
int ptot,pltot;
int vtot[maxp*maxp],vltot[maxp*maxp],stot[maxp*maxp],sltot[maxp*maxp];

/*6:*/
#line 190 "squaregraph.w"

int abcabc(int k,int l){
register int i,j;
for(j= back[k];j;j= back[j]){
if(l<j+del[j])continue;
for(i= back[j];;i= back[i]){
if(j+del[j]<i+del[i])goto OK;
if(i+del[i]<k)goto OK;
return 1;
OK:if(i==0)break;
}
}
return 0;
}

/*:6*//*13:*/
#line 293 "squaregraph.w"

int newedge(int s,int t){
e[eptr].from= e[eptr+1].to= s;
e[eptr].to= e[eptr+1].from= t;
e[eptr].reg= e[eptr+1].reg= 0;
eptr+= 2;
if(eptr>=2*maxp*maxp){
fprint(stderr,"Memory overflow!\n");
exit(-2);
}
return eptr-2;
}

/*:13*//*15:*/
#line 315 "squaregraph.w"

int pred(int k){
register int j;
for(j= k;e[j].succ!=k;j= e[j].succ);
return j;
}

/*:15*//*18:*/
#line 352 "squaregraph.w"

void finishchord(int q,int s,int t){
register int m= newedge(s,t);
e[m].succ= e[t+1].succ,e[m+1].succ= e[q].succ;
e[t+1].succ= m+1,e[q].succ= m;
}

/*:18*//*19:*/
#line 371 "squaregraph.w"

int internalchord(int p,int q,int s,int t){
register int m= newedge(s,t),mm= newedge(e[p].from,e[p].to);
register int pp= mate(p),ppp= pred(pp);
e[m].succ= mm,e[m+1].succ= e[q].succ;
e[mm].succ= e[p].succ,e[mm+1].succ= pp;
e[p].succ= m+1,e[q].succ= m,e[ppp].succ= mm+1;
return mm+1;
}

/*:19*//*20:*/
#line 386 "squaregraph.w"

void newchord(int s,int t){
register int p,q;
for(q= s+1;;){
p= e[q].succ;
for(p= e[p].succ;;p= e[p].succ){
if(p==q){
fprintf(stderr,"This can't happen (newchord loop)!\n");
exit(-1);
}
if(p<=pp){
if(p==t+1)break;
}else{
if(((t-e[p].from+pp)%pp)<((e[p].to-e[p].from+pp)%pp))break;
}
}
if(p<=pp)break;
q= internalchord(p,q,s,t);
}
finishchord(q,s,t);
}

/*:20*/
#line 123 "squaregraph.w"
;

main(int argc){
register int j,k,l,p,q,s;
int rfl,rot;
for(p= 3;p<=maxp;p++){
pp= p+p;
ptot= pltot= 0;
for(j= 2;j<p;j++){
/*4:*/
#line 154 "squaregraph.w"

del[0]= j,del[j]= pp-j;
occ[0]= occ[j]= 1;
k= 1,s= 0;

/*:4*/
#line 132 "squaregraph.w"
;
/*5:*/
#line 166 "squaregraph.w"

advance:while(occ[k])k++;
if(k==pp)
/*8:*/
#line 225 "squaregraph.w"

{
/*9:*/
#line 235 "squaregraph.w"

for(k= 0;k<pp;k++)del[pp+k]= del[k];
for(k= 1;k<pp;k++){
for(q= k;q<k+pp;q++)if(del[q]!=del[q-k])break;
if(del[q]<del[q-k])goto next;
if(q==k+pp)break;
}
rot= pp/k;
/*10:*/
#line 250 "squaregraph.w"

rfl= 0;
for(k= 1;k<=pp;k++){
for(q= k;q<k+pp;q++)if(rdel(q)!=del[q-k])break;
if(q==k+pp){
rfl= 1;break;
}
if(rdel(q)<del[q-k])goto next;
}

/*:10*/
#line 243 "squaregraph.w"
;

/*:9*/
#line 227 "squaregraph.w"
;
/*16:*/
#line 325 "squaregraph.w"

/*17:*/
#line 332 "squaregraph.w"

for(k= 1;k<pp;k++)e[k].succ= k+1,e[k].reg= 0;
e[pp].succ= 1,e[pp].reg= 0;
eptr= pp+2;
e[pp+1].reg= -1;

/*:17*/
#line 326 "squaregraph.w"
;
for(k= s;;k= back[k]){
newchord(k,(k+del[k])%pp);
if(k==0)break;
}

/*:16*/
#line 228 "squaregraph.w"
;
/*22:*/
#line 452 "squaregraph.w"

for(l= 0,k= 1;k<eptr;k++)if(e[k].reg==0){
e[k].reg= ++l;
for(q= e[k].succ;e[q].reg==0;q= e[q].succ){
if(q<=pp)goto next;
e[q].reg= l;
}
}

/*:22*/
#line 229 "squaregraph.w"
;
/*23:*/
#line 470 "squaregraph.w"

ptot++;
printf("%d:",ptot);
for(k= 0;k<pp;k++)printf(" %d",del[k]);
printf(" (%d%s),",rot,rfl?"R":"");
printf(" %d v, %d e, %d iv, %d sq\n",l,(eptr>>1)-1-p,l-pp,(eptr>>2)-p);

if(verbose)/*24:*/
#line 479 "squaregraph.w"

for(k= q= 1;k<=l;k++){
register int r;
printf(" %d --",k);
while(e[q].reg!=k)q++;
for(r= e[q].succ;;r= e[r].succ){
if(r<=pp)break;
printf(" %d",e[mate(r)].reg);
if(r==q)break;
}
printf("\n");
}

/*:24*/
#line 477 "squaregraph.w"
;

/*:23*/
#line 230 "squaregraph.w"
;
/*25:*/
#line 492 "squaregraph.w"

q= pp/rot;
if(rfl==0)q<<= 1;
pltot+= q;
vtot[l]++;
vltot[l]+= q;
stot[(eptr>>2)-p]++;
sltot[(eptr>>2)-p]+= q;

/*:25*/
#line 231 "squaregraph.w"
;
goto next;
}

/*:8*/
#line 169 "squaregraph.w"
;
occ[k]= 1,back[k]= s;
l= k+j;
nextslot:while(occ[l])l++;
if(l>=pp)goto backtrack;
if(abcabc(k,l)){
l++;goto nextslot;
}
del[k]= l-k,del[l]= pp-l+k,occ[l]= 1;
s= k++;
goto advance;
next:occ[s+del[s]]= 0;
k= s;
backtrack:occ[k]= 0;
k= back[k];
if(k){
l= k+del[k],occ[l]= 0,l++;
goto nextslot;
}
occ[j]= 0;

/*:5*/
#line 133 "squaregraph.w"
;
}
printf("%d (%d labeled) with perimeter %d; ",ptot,pltot,pp);
printf("%d (%d labeled) with %d vertices; ",vtot[pp],vltot[pp],pp);
printf("%d (%d labeled) with %d vertices; ",vtot[pp+1],vltot[pp+1],pp+1);
printf("%d (%d labeled) with %d squares.\n",stot[p-1],sltot[p-1],p-1);
}
}

/*:2*/
