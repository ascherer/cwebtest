#define maxdigs 22
#define maxdim 8
#define bufsize maxdim+5
#define maxm 8
#define o mems++
#define oo mems+= 2 \

#define slack 0 \

#define thresh 25 \

#define show_unresolved 0 \

/*2:*/
#line 61 "back-skeleton-shortest.w"

#include <stdio.h> 
#include <stdlib.h> 
/*6:*/
#line 153 "back-skeleton-shortest.w"

typedef char bignum[maxdigs];

/*:6*/
#line 64 "back-skeleton-shortest.w"
;
int m;
int z;
int vbose;
char buf[bufsize];
char rawpat[maxdim][maxdim];
char last[maxdim];
int count;
unsigned long long nodes;
int unresolved;
unsigned long long mems;
/*11:*/
#line 213 "back-skeleton-shortest.w"

bignum cnst[82];

/*:11*//*17:*/
#line 289 "back-skeleton-shortest.w"

char off[maxm];
bignum constr[maxm];
char id[maxm];
char ids;

/*:17*//*21:*/
#line 432 "back-skeleton-shortest.w"

bignum ja[maxdigs][10];
unsigned long long stamp[maxdigs][10];
char choice[maxm][10],where[maxm][10];

char csize[maxdigs][maxm];
char stack[maxm];
char stackptr;
char a[maxdigs];
bignum total;

/*:21*//*40:*/
#line 739 "back-skeleton-shortest.w"

char acc[maxm][maxdigs];
char g[maxm];
int shadow[maxm];

/*:40*/
#line 75 "back-skeleton-shortest.w"
;
/*7:*/
#line 159 "back-skeleton-shortest.w"

int isequal(bignum a,bignum b){
register int la= a[0],i;
if(oo,la!=b[0])return 0;
for(i= 1;i<=la;i++)if(oo,a[i]!=b[i])return 0;
return 1;
}

void copy(bignum a,bignum b){
register int lb= b[0],i;
for(o,i= 0;i<=lb;i++)oo,a[i]= b[i];
}

/*:7*//*8:*/
#line 175 "back-skeleton-shortest.w"

void add(bignum a,bignum b,bignum c,int p){
register int lb= b[0],lc= c[0],i,k,d;
if(oo,lc==0){
copy(a,b);
return;
}
for(i= 1;i<=p&&i<=lb;i++)oo,a[i]= b[i];
for(k= 0;i<=lb||i<=lc+p||k;i++){
d= k+(i<=lb?o,b[i]:0)+(i<=lc+p&&i> p?o,c[i-p]:0);
if(d>=10)k= 1,d-= 10;else k= 0;
o,a[i]= d;
}
o,a[0]= i-1;
if(i>=maxdigs){
fprintf(stderr,"Integer overflow, more than %d digits!\n",
maxdigs-1);
exit(-666);
}
if(a[a[0]]==0)
fprintf(stderr,"why?\n");
}

/*:8*//*9:*/
#line 198 "back-skeleton-shortest.w"

void print_bignum(bignum a){
register int i,la= a[0];
if(!la)fprintf(stderr,"0");
else for(i= la;i;i--)fprintf(stderr,"%d",
a[i]);
}

/*:9*/
#line 76 "back-skeleton-shortest.w"
;
main(int argc,char*argv[]){
register int d,i,ii,imax,j,jj,k,kk,l,lc,lj,n,t,tt,x,pos,maxl,printed;
/*3:*/
#line 98 "back-skeleton-shortest.w"

if(argc<3||sscanf(argv[1],"%d",
&m)!=1||sscanf(argv[2],"%d",
&z)!=1){
fprintf(stderr,"Usage: %s m z [verbose] [extraverbose] < foo.dots\n",
argv[0]);
exit(-1);
}
if(m<2||m>=maxm){
fprintf(stderr,"m should be between 2 and %d, not %d!\n",
maxm-1,m);
exit(-2);
}
if(m+z> maxdigs-2){
fprintf(stderr,"m+z should be at most %d, not %d!\n",
maxdigs-2,m+z);
exit(-3);
}
vbose= argc-3;

/*:3*/
#line 79 "back-skeleton-shortest.w"
;
/*4:*/
#line 118 "back-skeleton-shortest.w"

for(n= k= 0;;n++){
if(!fgets(buf,bufsize,stdin))break;
if(n>=maxdim){
fprintf(stderr,"Recompile me: I allow at most %d lines of input!\n",
maxdim);
exit(-3);
}
/*5:*/
#line 136 "back-skeleton-shortest.w"

for(j= 0;buf[j]&&buf[j]!='\n';j++){
if(buf[j]=='*'){
if(j>=maxdim){
fprintf(stderr,"Recompile me: I allow at most %d columns per row!\n",
maxdim);
exit(-5);
}
oo,rawpat[n][j]= 1,k++,last[n]= j+1;
}
}

/*:5*/
#line 126 "back-skeleton-shortest.w"
;
}
fprintf(stderr,"OK, I've got a pattern with %d rows and %d asterisks.\n",
n,k);
if(m<n-1){
fprintf(stderr,"So there must be at least %d multiplier digits, not %d!\n",
n-1,m);
exit(-2);
}

/*:4*/
#line 80 "back-skeleton-shortest.w"
;
/*10:*/
#line 208 "back-skeleton-shortest.w"

o,cnst[0][0]= 0;
for(k= 1;k<10;k++)oo,cnst[k][0]= 1,cnst[k][1]= k;
for(;k<=81;k++)oo,o,cnst[k][0]= 2,cnst[k][2]= k/10,cnst[k][1]= k%10;

/*:10*/
#line 81 "back-skeleton-shortest.w"
;
/*13:*/
#line 239 "back-skeleton-shortest.w"

for(i= 0;i<m;i++)o,off[i]= i;

/*:13*/
#line 82 "back-skeleton-shortest.w"
;
while(1){
/*18:*/
#line 295 "back-skeleton-shortest.w"

{
/*15:*/
#line 265 "back-skeleton-shortest.w"

for(i= pos= 0;i<=m;i++)
if(oo,off[m+1-n+i]+last[i]> pos)pos= off[m+1-n+i]+last[i];
pos+= slack-1;

/*:15*/
#line 297 "back-skeleton-shortest.w"
;
/*16:*/
#line 274 "back-skeleton-shortest.w"

for(k= ids= 0;k<=m;k++){
o,i= k-(m+1-n),constr[k][0]= 0;
if(i>=0){
for(oo,j= pos-off[k]-last[i]+1;j>=0;j--)o,constr[k][j]= 0;
for(o,j= last[i]-1;j>=0;j--){
if(o,rawpat[i][j])
oo,o,constr[k][pos-off[k]-j+1]= 1,constr[k][0]= pos-off[k]-j+1;
else oo,constr[k][pos-off[k]-j+1]= 0;
}
}
for(j= k-1;j>=0;j--)if(oo,isequal(constr[j],constr[k]))break;
if(j>=0)oo,id[k]= id[j];else o,id[k]= ids++;
}

/*:16*/
#line 298 "back-skeleton-shortest.w"
;
if(vbose){
fprintf(stderr,"Constraints for offsets");
for(k= 0;k<=m;k++)fprintf(stderr," %d",
off[k]);
fprintf(stderr,":");
for(k= 0;k<=m;k++){
fprintf(stderr," ");
print_bignum(constr[k]);
}
fprintf(stderr,".\n");
}
}

/*:18*/
#line 84 "back-skeleton-shortest.w"
;
for(d= 0;d<10;d++){
if(vbose)fprintf(stderr," *=%d:\n",
d);
/*20:*/
#line 381 "back-skeleton-shortest.w"

b1:o,maxl= maxdigs-2-off[m-1];
l= 0;
/*22:*/
#line 443 "back-skeleton-shortest.w"

if(d==0&&off[m-1]>=m)goto b5;
for(i= 0,j= 1;j<10;j++)if(j!=d){
for(k= 0;k<m;k++)oo,choice[k][i]= j,where[k][j]= i;
i++;
}
for(k= 0;k<m;k++)oo,oo,
csize[0][k]= i,choice[k][i]= d,where[k][d]= i,where[k][0]= 9;


/*:22*/
#line 384 "back-skeleton-shortest.w"
;
b2:nodes+= 10;
if(vbose> 1){
fprintf(stderr,"Level %d,",
l);
/*23:*/
#line 453 "back-skeleton-shortest.w"

for(k= 0;k<m;k++)fprintf(stderr,"%d",
csize[l][k]);
fprintf(stderr,"\n");

/*:23*/
#line 389 "back-skeleton-shortest.w"
;
}
if(l>=maxl)/*34:*/
#line 618 "back-skeleton-shortest.w"

{
for(k= 0;k<m;k++)if(o,csize[l][k]> 1)break;
if(k<m){
unresolved++;
if(o,a[l-1]==0||show_unresolved){
fprintf(stderr,"Unresolved case with d=%d and offsets",
d);
for(k= 0;k<m;k++)fprintf(stderr," %d",
off[k]);
fprintf(stderr,":\n a=...");
for(k= l-1;k>=0;k--)fprintf(stderr,"%d",
a[k]);
fprintf(stderr,", status ");
for(k= 0;k<m;k++)fprintf(stderr,"%d",
csize[l][k]);
fprintf(stderr,"!\n");
}
}
goto b5;
}

/*:34*/
#line 391 "back-skeleton-shortest.w"
;
/*30:*/
#line 544 "back-skeleton-shortest.w"

if(printed)goto nope;
for(k= 0;k<m;k++)if(o,csize[l][k]> 1)goto nope;
for(k= m-1;k>=0;k--)
/*31:*/
#line 555 "back-skeleton-shortest.w"

{
oo,o,j= choice[k][0],lj= ja[l-1][j][0],lc= constr[k][0];
if(lc> lj)goto nope;
for(i= 1;i<=lj;i++){
o,t= ja[l-1][j][i],tt= (i<=lc?o,constr[k][i]:0);
if((t==d&&tt==0)||(t!=d&&tt!=0))goto nope;
}
}

/*:31*/
#line 548 "back-skeleton-shortest.w"
;
/*32:*/
#line 565 "back-skeleton-shortest.w"

oo,oo,add(total,ja[l-1][choice[0][0]],ja[l-1][choice[1][0]],off[1]);
for(k= 2;k<m;k++)oo,o,add(total,total,ja[l-1][choice[k][0]],off[k]);
o,lj= total[0],lc= constr[m][0];
if(lc> lj)goto nope;
for(i= 1;i<=lj;i++){
o,t= total[i],tt= (i<=lc?o,constr[m][i]:0);
if((t==d&&tt==0)||(t!=d&&tt!=0))goto nope;
}

/*:32*/
#line 549 "back-skeleton-shortest.w"
;
/*33:*/
#line 580 "back-skeleton-shortest.w"

count++;
for(i= l-1;a[i]==0;i--);
printf("%d,%d;",
i+1,off[m-1]+1);
for(k= 0;k<m;k++)printf("%d|%d,",
ja[l-1][choice[k][0]][0],off[k]);
printf("%d, ",
total[0]);
for(;i>=0;i--)printf("%d",
a[i]);
printf(" x ");
for(k= m-1,i= off[k];k>=0;k--,i--){
while(i> off[k])printf("0"),i--;
printf("%d",
choice[k][0]);
}
printf(",d=%d (#%d)\n",
d,count);
printed= 1;

/*:33*/
#line 550 "back-skeleton-shortest.w"
;
while(o,a[l-1]==0)l--;
goto b5;
nope:

/*:30*/
#line 392 "back-skeleton-shortest.w"
;
x= 0;
b3:if(slack&&l==0&&x==0)goto b4;
if(x==d)goto b4;
if(vbose> 2)fprintf(stderr," testing %d\n",
x);
/*24:*/
#line 460 "back-skeleton-shortest.w"

for(stackptr= 0,k= m-1;k>=0;k--)
/*25:*/
#line 484 "back-skeleton-shortest.w"

{
o,imax= csize[l][k];
for(i= 0;i<imax;i++){
o,j= choice[k][i];
/*26:*/
#line 507 "back-skeleton-shortest.w"

if(o,stamp[l][j]!=nodes+x){
o,stamp[l][j]= nodes+x;
if(l==0)oo,copy(ja[0][j],cnst[x*j]);
else oo,add(ja[l][j],ja[l-1][j],cnst[x*j],l);
}
oo,t= (ja[l][j][0]<=l?0:ja[l][j][l+1]);
o,tt= (constr[k][0]<=l?0:o,constr[k][l+1]);
if((tt==1&&t==d)||(tt!=1&&t!=d))goto jok;

/*:26*/
#line 489 "back-skeleton-shortest.w"
;
if(vbose> 2)fprintf(stderr," c%d loses option %d\n",
k,j);
if(--imax==0)goto b4;
if(i!=imax)
oo,oo,oo,choice[k][i]= choice[k][imax],where[k][choice[k][imax]]= i--,
choice[k][imax]= j,where[k][j]= imax;

jok:continue;
}
o,csize[l+1][k]= imax;
if(imax==1&&(o,csize[l][k]!=1))o,stack[stackptr++]= k;
}

/*:25*/
#line 462 "back-skeleton-shortest.w"
;
while(stackptr){
o,k= stack[--stackptr];
if(vbose> 2)fprintf(stderr," b%d must be %d\n",
off[k],choice[k][0]);
/*27:*/
#line 517 "back-skeleton-shortest.w"

for(o,kk= 0,j= choice[k][0];kk<m;kk++)if(oo,id[kk]!=id[k]){
oo,i= csize[l+1][kk]-1,ii= where[kk][j];
if(ii<=i){
if(i==0)goto b4;
o,csize[l+1][kk]= i;
if(i==1)o,stack[stackptr++]= kk;
if(ii!=i)
oo,oo,oo,choice[kk][ii]= choice[kk][i],where[kk][choice[kk][i]]= ii,
choice[kk][i]= j,where[kk][j]= i;
}
}

/*:27*/
#line 467 "back-skeleton-shortest.w"
;
}
for(o,t= csize[l+1][0],k= 1;k<m&&t<=thresh;k++)o,t*= csize[l+1][k];
if(t<=thresh){
/*35:*/
#line 675 "back-skeleton-shortest.w"

{
for(k= 0;k<m;k++)o,shadow[k]= 0;
/*36:*/
#line 685 "back-skeleton-shortest.w"

bb1:k= 0;
bb2:if(k==m)/*38:*/
#line 706 "back-skeleton-shortest.w"

{
for(o,i= 0,lc= constr[m][0];i<=l;i++){
o,t= acc[m-1][i];
if(i<lc)o,tt= constr[m][i+1];else tt= 0;
if((t==d&&tt==0)||(t!=d&&tt!=0))goto noncomp;
}
if(vbose> 2){
fprintf(stderr," ok ");
for(k= m-1;k>=0;k--)fprintf(stderr,"%d",
choice[k][g[k]]);
fprintf(stderr,"\n");
}
for(k= 0;k<m;k++)oo,shadow[k]|= 1<<g[k];
noncomp:goto bb5;
}

/*:38*/
#line 687 "back-skeleton-shortest.w"
;
g[k]= 0;
bb3:/*37:*/
#line 697 "back-skeleton-shortest.w"

oo,o,j= choice[k][g[k]],lj= ja[l][j][0];
for(i= 0;o,i<off[k];i++)oo,acc[k][i]= acc[k-1][i];
for(ii= 1,kk= 0;i<=l;i++,ii++){
t= (k> 0?o,acc[k-1][i]+kk:kk);
if(ii<=lj)o,t+= ja[l][j][ii];
if(t>=10)o,acc[k][i]= t-10,kk= 1;else o,acc[k][i]= t,kk= 0;
}

/*:37*/
#line 689 "back-skeleton-shortest.w"
;
k++;
goto bb2;
bb4:oo,g[k]++;
if(o,g[k]<csize[l+1][k])goto bb3;
bb5:k--;
if(k>=0)goto bb4;

/*:36*/
#line 678 "back-skeleton-shortest.w"
;
if(o,shadow[0]==0)goto b4;
for(k= 0;k<m;k++){
if(oo,shadow[k]+1!=1<<csize[l+1][k])/*39:*/
#line 723 "back-skeleton-shortest.w"

{
o,imax= csize[l+1][k];
for(i= imax-1;i>=0;i--)if(o,(shadow[k]&(1<<i))==0){
o,j= choice[k][i];
if(vbose> 2)fprintf(stderr," b%d ain't %d\n",
k,j);
imax--;
if(i!=imax)
oo,oo,oo,choice[k][i]= choice[k][imax],where[k][choice[k][imax]]= i,
choice[k][imax]= j,where[k][j]= imax;
}
o,csize[l+1][k]= imax;
if(imax==1)o,stack[stackptr++]= k;
}

/*:39*/
#line 681 "back-skeleton-shortest.w"
;
}
}

/*:35*/
#line 471 "back-skeleton-shortest.w"
;
while(stackptr){
o,k= stack[--stackptr];
if(vbose> 2)fprintf(stderr," b%d has to be %d\n",
off[k],choice[k][0]);
/*27:*/
#line 517 "back-skeleton-shortest.w"

for(o,kk= 0,j= choice[k][0];kk<m;kk++)if(oo,id[kk]!=id[k]){
oo,i= csize[l+1][kk]-1,ii= where[kk][j];
if(ii<=i){
if(i==0)goto b4;
o,csize[l+1][kk]= i;
if(i==1)o,stack[stackptr++]= kk;
if(ii!=i)
oo,oo,oo,choice[kk][ii]= choice[kk][i],where[kk][choice[kk][i]]= ii,
choice[kk][i]= j,where[kk][j]= i;
}
}

/*:27*/
#line 476 "back-skeleton-shortest.w"
;
}
}

/*:24*/
#line 398 "back-skeleton-shortest.w"
;
o,a[l]= x;
if(vbose> 1)fprintf(stderr,"Trying a[%d]=%d\n",
l,x);
/*28:*/
#line 535 "back-skeleton-shortest.w"

if(x)printed= 0;

/*:28*/
#line 402 "back-skeleton-shortest.w"
;
l= l+1;goto b2;
b4:if(x==9)goto b5;
x= x+1;goto b3;
b5:l= l-1;
if(l>=0){
if(vbose> 1)fprintf(stderr,"Back to level %d\n",
l);
o,x= a[l];
/*29:*/
#line 542 "back-skeleton-shortest.w"


/*:29*/
#line 411 "back-skeleton-shortest.w"
;
goto b4;
}

/*:20*/
#line 88 "back-skeleton-shortest.w"
;
}
/*14:*/
#line 245 "back-skeleton-shortest.w"

for(i= m-1;i> 0;i--)
if(o,off[i]<i+z)break;
if(i==0)break;
o,off[i]++;
for(i++;i<m;i++)oo,off[i]= off[i-1]+1;

/*:14*/
#line 90 "back-skeleton-shortest.w"
;
}
fprintf(stderr,"Altogether %d solutions, %lld nodes, %lld mems.\n",
count,nodes/10,mems);
if(unresolved)fprintf(stderr,"... %d cases were unresolved!\n",
unresolved);
}

/*:2*/
