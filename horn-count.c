#define n 5
#define nn (1<<n) 
#define nfactorial 120
#define final_level (nn-1) 
#define verbose (n<5)  \

/*1:*/
#line 16 "horn-count.w"

#include <stdio.h> 
char f[nn];
unsigned char perm[nfactorial][nn],iperm[nfactorial][nn];
int link[nfactorial];
int wait[nn];
int disc[nn];
int log0[nn],logl[nn];
int log[nfactorial*nn*2];
int logptr;
int forced[nn];
int forcings[nfactorial];
unsigned int sols,tsols;
/*16:*/
#line 303 "horn-count.w"

int timestamp;
int stamp[nfactorial];
void sanity(int l)
{
register c,j,jj,k,p;
if(l==0)return;
timestamp++;
/*17:*/
#line 322 "horn-count.w"

for(k= l;k<nn;k++)for(p= wait[k];p;p= link[p]){
stamp[p]= timestamp;
jj= iperm[p][k];
if(f[jj]!=1){
printf("error: wait[%x] contains noncritical perm %d!\n",k,p);
goto error_exit;
}
for(j= c= 0;;j++){
if(perm[p][j]> jj){
if(f[j]==0)c++;
else if(perm[p][j]==k)break;
}else if(f[j]!=f[perm[p][j]]){
printf("error: perm %d on wait[%x] contains early mismatch f[%x]!=f[%x]!\n",
p,k,j,perm[p][j]);
goto error_exit;
}
}
if(c!=forcings[p]){
printf("error: forcings[%d] is %d, not %d, in wait[%x]!\n",
p,forcings[p],c,k);
goto error_exit;
}
}

/*:17*/
#line 311 "horn-count.w"
;
/*18:*/
#line 353 "horn-count.w"

for(k= 1;k<l;k++){
for(p= disc[k];p;p= link[p]){
stamp[p]= timestamp;
for(jj= 0;jj<l;jj++)if(f[jj]!=f[perm[p][jj]])break;
if(jj==l){
printf("error: disc[%x] contains the nondiscardable perm %d!\n",k,p);
goto error_exit;
}
if(f[jj]==0){
printf("error: disc[%x] contains the counterexample perm %d!\n",k,p);
goto error_exit;
}
}
for(p= wait[k];p;p= link[p]){
stamp[p]= timestamp;
for(jj= 0;jj<l;jj++)if(f[jj]!=f[perm[p][jj]])break;
if(jj==l){
printf("error: wait[%x] contains the nondiscardable perm %d!\n",k,p);
goto error_exit;
}
if(f[jj]==0){
printf("error: wait[%x] contains the counterexample perm %d!\n",k,p);
goto error_exit;
}
for(j= c= 0;j<jj;j++)
if(perm[p][j]> jj&&f[j]==0)c++;
if(c!=forcings[p]){
printf("error: forcings[%d] is %d, not %d, in wait[%x]!\n",
p,forcings[p],c,k);
goto error_exit;
}
}
}

/*:18*/
#line 312 "horn-count.w"
;
/*19:*/
#line 388 "horn-count.w"

for(p= wait[0];p;p= link[p]){
stamp[p]= timestamp;
for(c= j= 0;j<l;j++){
if(f[j]!=f[perm[p][j]]){
if(f[j]==0){
printf("error: wait[0] contains the counterexample perm %d!\n",k,p);
goto error_exit;
}
printf("error: wait[0] contains the discardable perm %d!\n",k,p);
}
if(perm[p][j]>=l)c++;
}
if(c!=forcings[p]){
printf("error: forcings[%d] is %d, not %d, in wait[0]!\n",
p,forcings[p],c);
goto error_exit;
}
}

/*:19*/
#line 313 "horn-count.w"
;
for(p= 1;p<nfactorial;p++)if(stamp[p]!=timestamp){
printf("error: perm %d has disappeared!\n",p);
goto error_exit;
}
return;
error_exit:printf("(Detected at level %x)\n",l);return;
}

/*:16*/
#line 29 "horn-count.w"


main()
{
register int d,j,k,l,m,p,q,t,auts;
/*3:*/
#line 81 "horn-count.w"

d= nfactorial>>1,perm[d][0]= 1;
for(m= 2;m<n;){
m++,d= d/m;
for(k= 0;k<nfactorial;){
for(k+= d,j= m-1;j> 0;k+= d,j--)perm[k][0]= j;
perm[k][0]++,k+= d;
for(j++;j<m;k+= d,j++)perm[k][0]= j;
}
}
for(j= 0;j<nn;j++)perm[0][j]= j;
for(k= 1;k<nfactorial;k++){
m= 1<<(perm[k][0]-1);
for(j= 0;j<nn;j++){
d= perm[k-1][j];
d^= d>>1;
d&= m;
d|= d<<1;
perm[k][j]= perm[k-1][j]^d;
}
}
for(p= 0;p<nfactorial;p++)
for(k= 0;k<nn;k++)iperm[p][perm[p][k]]= k;

/*:3*/
#line 34 "horn-count.w"
;
/*4:*/
#line 105 "horn-count.w"

for(p= 1;p<nfactorial;p++)link[p]= wait[0],wait[0]= p;

/*:4*/
#line 35 "horn-count.w"
;
/*2:*/
#line 40 "horn-count.w"

l= logptr= 0;auts= nfactorial;
newlevel:if(l==final_level)goto backtrack;
logl[l]= logptr;
if(verbose){
printf("Entering level %x (%d auts so far)\n",l,auts);
}
if(forced[l]){
if(verbose)printf(" forced rejection of %x\n",l);
goto reject;
}
/*5:*/
#line 108 "horn-count.w"

for(j= 0;j<l;j++)if(f[j]&&!(f[j&l])){
if(verbose)printf(" rejecting %x for closure\n",l);
goto reject;
}

/*:5*/
#line 51 "horn-count.w"
;
/*9:*/
#line 189 "horn-count.w"

for(p= wait[l],wait[l]= 0;p;p= q){
q= link[p];
for(k= iperm[p][l]+1;k<l;k++){
if(f[k]==0&&iperm[p][k]<iperm[p][l])forcings[p]--;
j= perm[p][k];
if(j<l){
if(f[j]==f[k])continue;
if(f[k]==0)/*12:*/
#line 262 "horn-count.w"

{
t= p;
goto kludge;
}

/*:12*/
#line 197 "horn-count.w"
;
log[logptr++]= -j,link[p]= disc[l],disc[l]= p;
goto nextp;
}else if(f[k]==1){
log[logptr++]= j,link[p]= wait[j],wait[j]= p;
for(j= k-1;j> iperm[p][l];j--)
if(f[j]==0&&perm[p][j]> k&&perm[p][j]<l)forcings[p]++;
goto nextp;
}else{
if(verbose)printf(" f[%x]=1 will force f[%x]=0\n",j);
forcings[p]++,forced[j]++;
}
}
log[logptr++]= 0,link[p]= wait[0],wait[0]= p;
nextp:continue;
}

/*:9*/
#line 53 "horn-count.w"
;
f[l]= 1;
if(verbose)printf(" accepting %x\n",l);
/*10:*/
#line 218 "horn-count.w"

log0[l]= logptr;
for(auts= 1,p= wait[0],wait[0]= 0;p;p= q){
q= link[p];
j= perm[p][l];
if(j==l)goto retain_it;
else if(j> l)log[logptr++]= j,link[p]= wait[j],wait[j]= p;
else if(f[j]==0)log[logptr++]= -1,link[p]= disc[l],disc[l]= p;
else goto retain_it;
continue;
retain_it:log[logptr++]= 0,link[p]= wait[0],wait[0]= p;
auts++;
}

/*:10*/
#line 56 "horn-count.w"

/*6:*/
#line 114 "horn-count.w"

{
sols++;
tsols+= nfactorial/auts;
if(n<6){
printf("%d:",sols);
for(j= 0;j<nn;j++)if(f[j])
printf(" %x",j);
printf(" (%d aut%s)\n",auts,auts> 1?"s":"");
}
}

/*:6*/
#line 57 "horn-count.w"
;
l++;
goto newlevel;
undo:/*13:*/
#line 268 "horn-count.w"

t= 0;
while(logptr> log0[l]){
j= log[--logptr];
if(j<0)p= disc[l],disc[l]= link[p];
else p= wait[j],wait[j]= link[p];
link[p]= t,t= p;
}
wait[0]= t;

/*:13*/
#line 60 "horn-count.w"
;
/*11:*/
#line 240 "horn-count.w"

t= 0;
while(logptr> logl[l]){
j= log[--logptr];
if(j<0){
p= disc[l],disc[l]= link[p],k= iperm[p][-j];
if(f[k]==0&&iperm[p][k]<iperm[p][l])forcings[p]++;
}else if(j> 0){
p= wait[j],wait[j]= link[p],k= iperm[p][j];
for(j= k-1;j> iperm[p][l];j--)
if(f[j]==0&&perm[p][j]> k&&perm[p][j]<l)forcings[p]--;
}else p= wait[0],wait[0]= link[p],k= l;
link[p]= t,t= p,k--;
while(k> iperm[p][l]){
j= perm[p][k];
if(j> l&&f[k]==0)forcings[p]--,forced[j]--;
kludge:if(f[k]==0&&iperm[p][k]<iperm[p][l])forcings[p]++;
k--;
}
}
wait[l]= t;

/*:11*/
#line 61 "horn-count.w"
;
reject:f[l]= 0;
/*14:*/
#line 278 "horn-count.w"

for(auts= 1,p= wait[0];p;p= link[p]){
j= perm[p][l];
if(j> l){
if(verbose)printf(" forcing f[%x]=0\n",j);
forcings[p]++,forced[j]++;
}
if(iperm[p][l]<l)forcings[p]--;
if(verbose)auts++;
}

/*:14*/
#line 63 "horn-count.w"
;
l++;
goto newlevel;
backtrack:while(l> 0){
l--;
if(f[l]==1){
if(verbose)printf(" now rejecting %x\n",l);
goto undo;
}
else/*15:*/
#line 289 "horn-count.w"

for(p= wait[0];p;p= link[p]){
j= perm[p][l];
if(j> l){
forcings[p]--,forced[j]--;
}
if(iperm[p][l]<l)forcings[p]++;
}

/*:15*/
#line 72 "horn-count.w"
;
}
for(p= 1;p<nfactorial;p++)if(forcings[p])
printf("error: forcings[%d] not restored to zero!\n",p);
for(k= 1;k<nn;k++)if(forced[k])
printf("error: forced[%x] not restored to zero!\n",k);

/*:2*/
#line 36 "horn-count.w"
;
printf("Altogether %d solutions (reduced from %d).\n",sols+1,tsols+1);
}

/*:1*/
