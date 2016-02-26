#define maxm 10
#define maxn 100
#define buf_size BUFSIZ \

/*1:*/
#line 57 "lp.w"

#include <stdio.h> 
#include <ctype.h> 
/*2:*/
#line 84 "lp.w"

#define little_endian 1

#ifdef big_endian
#define bigend first
#else
#define bigend second
#endif
#define zap  if ((zbuf.uint.bigend&0x7fffffff)<0x3e000000) zbuf.dbl= 0.0;

union{
struct{unsigned int first,second;}uint;
double dbl;
}zbuf;

/*:2*/
#line 60 "lp.w"

typedef long intword;
char buf[buf_size];
intword a[maxm+1][maxm+maxn+1];
intword denom[maxm+maxn+1];
double aa[maxm+1][maxm+maxn+1];
double trial[maxm+maxn+1];
int verbose= 1;
int count;
int p[maxm+1],q[maxm+maxn+1];

main()
{
register intword h,i,j,k,l,m,n,s;
register double z;
/*3:*/
#line 101 "lp.w"

zbuf.dbl= .000000001;
zap;
if(zbuf.dbl){
zbuf.dbl= -.0000000001;
zap;
if(!zbuf.dbl)goto zap_OK;
}
fprintf(stderr,"Zapping doesn't work!\n");
exit(-666);
zap_OK:

/*:3*/
#line 75 "lp.w"
;
/*4:*/
#line 113 "lp.w"

for(i= n= 0;;i++){
if(!fgets(buf,buf_size,stdin))break;
if(i> maxm){
fprintf(stderr,"Sorry, I'm set up only for m<=%d!\n",maxm);
exit(-9);
}
for(k= 0,j= (i==0);buf[k];){
while(isspace(buf[k])&&buf[k]!='\n')k++;
if(buf[k]=='\n')break;
if(buf[k]=='-')l= 1,k++;else l= 0;
for(s= 0;buf[k]>='0'&&buf[k]<='9';k++)s= 10*s+buf[k]-'0';
a[i][j++]= (l?-s:s);
}
if(!buf[k]){
fprintf(stderr,"Input line too long! (%s...)\n",buf);
exit(-1);
}
if(i==0){
n= j-1;
if(n> maxn){
fprintf(stderr,"Sorry, I'm set up only for n<=%d, not n=%d!\n",
maxn,n);
exit(-2);
}
}else{
if(n!=j-1){
fprintf(stderr,"Row %d should have %d numbers!\n>%s",i,n+1,buf);
exit(-3);
}
if(a[i][j]<0){
fprintf(stderr,"Row %d's constant term shouldn't be negative!\n>%s",
i,buf);
exit(-4);
}
}
}
m= i-1;

/*:4*/
#line 76 "lp.w"
;
/*13:*/
#line 371 "lp.w"

/*14:*/
#line 396 "lp.w"

for(i= 0;i<=m;i++){
for(j= n;j> 0;j--)
if(i==0)a[0][j+m]= -a[0][j];else a[i][j+m]= a[i][j];
for(j= m;j> 0;j--)a[i][j]= (i==j);
p[i]= q[i]= i;
}
for(j= m+n+1;j>=0;j--)denom[j]= 1;
for(i= 0;i<=m;i++)for(j= m+n+1;j>=0;j--)aa[i][j]= a[i][j];

/*:14*/
#line 372 "lp.w"
;
loop:if(verbose)/*22:*/
#line 553 "lp.w"

{
printf("Step %d:\n",count);
for(i= 0;i<=m;i++){
for(j= 0;j<=m+n;j++)printf(" %d",a[i][j]);
printf("\n");
}
printf("denom");
for(j= 0;j<=m+n;j++)printf(" %d",denom[j]);
printf("\n");
for(i= 0;i<=m;i++){
for(j= 0;j<=m+n;j++)printf(" %.15g",aa[i][j]);
printf("\n");
}
}

/*:22*/
#line 373 "lp.w"
;
for(j= m+n;j> 0;j--)if(a[0][j]<0){
/*15:*/
#line 418 "lp.w"

l= 0;
for(i= 1;i<=m;i++)
if(a[i][j]> 0)/*16:*/
#line 460 "lp.w"

if(l==0)l= i,s= 0;
else{
for(h= 0;;h++){
if(h==s)trial[s++]= (double)a[l][h]/(double)a[l][j];
z= (double)a[i][h]/(double)a[i][j];
if(trial[h]!=z)break;
}
if(trial[h]> z)l= i,trial[h]= z,s= h+1;

}

/*:16*/
#line 421 "lp.w"
;
if(l==0){
printf("The maximum is infinite; the dual has no solution!\n");
/*22:*/
#line 553 "lp.w"

{
printf("Step %d:\n",count);
for(i= 0;i<=m;i++){
for(j= 0;j<=m+n;j++)printf(" %d",a[i][j]);
printf("\n");
}
printf("denom");
for(j= 0;j<=m+n;j++)printf(" %d",denom[j]);
printf("\n");
for(i= 0;i<=m;i++){
for(j= 0;j<=m+n;j++)printf(" %.15g",aa[i][j]);
printf("\n");
}
}

/*:22*/
#line 424 "lp.w"
;
exit(0);
}
/*17:*/
#line 472 "lp.w"

/*18:*/
#line 482 "lp.w"

{
register int ii,jj,kk,ll;
for(ll= 0,jj= m+n;jj> 0;jj--)if(aa[0][jj]<0){
if(jj!=j)goto mismatch;
for(ii= 1;ii<=m;ii++)if(aa[ii][j]> 0){
if(ll==0)ll= ii,s= 0;
else{
for(h= 0;;h++){
if(h==s)trial[s++]= aa[ll][h]/aa[ll][j];
z= aa[ii][h]/aa[ii][j];
zbuf.dbl= trial[h]-z;zap;
if(zbuf.dbl)break;
}
if(zbuf.dbl> 0.0)ll= ii,trial[h]= z,s= h+1;
}
}
if(ll!=l)goto mismatch;
/*19:*/
#line 514 "lp.w"

for(k= 0,z= aa[l][j];k<=m+n;k++)
if(aa[l][k])aa[l][k]= aa[l][k]/z;
for(i= 0;i<=m;i++)if(i!=l){
for(k= 0,z= aa[i][j];k<=m+n;k++)
if(k==j)aa[i][k]= 0.0;
else{
zbuf.dbl= aa[i][k]-z*aa[l][k];zap;
aa[i][k]= zbuf.dbl;
}
}

/*:19*/
#line 500 "lp.w"
;
goto fp_pivot_done;
}
mismatch:
printf("The floating-point and fixed-point implementations disagree!\n");
printf("(Floating-point pivoting at (%d,%d), not (%d,%d).)\n",ll,jj,l,j);
/*22:*/
#line 553 "lp.w"

{
printf("Step %d:\n",count);
for(i= 0;i<=m;i++){
for(j= 0;j<=m+n;j++)printf(" %d",a[i][j]);
printf("\n");
}
printf("denom");
for(j= 0;j<=m+n;j++)printf(" %d",denom[j]);
printf("\n");
for(i= 0;i<=m;i++){
for(j= 0;j<=m+n;j++)printf(" %.15g",aa[i][j]);
printf("\n");
}
}

/*:22*/
#line 506 "lp.w"
;
exit(-99);
}
fp_pivot_done:

/*:18*/
#line 473 "lp.w"
;
/*20:*/
#line 530 "lp.w"

if(verbose)printf("Pivoting on (%d,%d).\n",l,j);
for(k= 0;k<=m+n;k++)if(a[l][k]&&k!=j){
register intword t,u= a[l][k],v= a[l][j];
if(u<0)u= -u;
if(v<0)v= -v;
while(v)t= u,u= v,v= t%v;
if(u==a[l][j])a[l][k]= a[l][k]/u;
else{
v= a[l][j]/u,denom[k]*= v;
for(i= 0;i<=m;i++)a[i][k]= (i==l?a[l][k]/u:a[i][k]*v);
}
}
for(i= 0;i<=m;i++)if(i!=l){
for(k= 0,h= a[i][j];k<=m+n;k++)a[i][k]= (k==j?0:a[i][k]-h*a[l][k]);
}
a[l][j]= 1;
if(denom[j]!=1){
for(h= denom[j],denom[j]= 1,k= 0;k<=m+n;k++)if(k!=j)a[l][k]*= h;
}

/*:20*/
#line 474 "lp.w"
;
q[p[l]]= 0,p[l]= j,q[j]= l;

/*:17*/
#line 427 "lp.w"
;

/*:15*/
#line 375 "lp.w"
;
count++;
goto loop;
}
/*23:*/
#line 569 "lp.w"

printf("Optimal value %.15g=%d/%d found after %d pivots.\n",
aa[0][0],a[0][0],denom[0],count);
printf(" Optimal v's:");
for(j= m+1;j<=m+n;j++)
if(q[j])printf(" %.15g=%d/%d",aa[q[j]][0],a[q[j]][0],denom[0]);
else printf(" 0");
printf("\n Optimal u's:");
for(j= 1;j<=m;j++)printf(" %.15g=%d/%d",aa[0][j],a[0][j],denom[j]);
printf("\n");

/*:23*/
#line 379 "lp.w"
;

/*:13*/
#line 77 "lp.w"
;
}

/*:1*/
