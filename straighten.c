#define maxn 100
#define maxf 300 \

/*2:*/
#line 69 "straighten.w"

#include <stdio.h> 
/*3:*/
#line 98 "straighten.w"

int a[maxn+2];
int b[maxn+1];
int n;
int kk;
int f;
int p[maxn+1];
int q[maxn+1];
int t[maxn][maxn],tt[maxn][maxn];
int aa[maxn+2];
int stand[maxf][maxn];
int basis[maxf][maxf];
int rep[maxf];

/*:3*//*9:*/
#line 194 "straighten.w"

int prec[maxn+1][maxn+1];
int w[maxn+1];
int trie[maxf*maxn][maxn+1];
int trienodes;

/*:9*/
#line 71 "straighten.w"

/*11:*/
#line 233 "straighten.w"

void findrep(void)
{
register int i,j,k,l,inv,sign;
int row[maxn+1],col[maxn+1];
/*12:*/
#line 246 "straighten.w"

inv= 0;
for(k= 0;k<a[1];k++){
for(j= 1;j<b[k+1];j++)if(t[j][k]<t[j-1][k]){
for(i= j-1,l= t[j][k];;i--){
t[i+1][k]= t[i][k];
inv++;
if(i==0||l> t[i-1][k])break;
}
t[i][k]= l;
}
}

/*:12*/
#line 238 "straighten.w"
;
for(j= 0;j<f;j++)rep[j]= 0;
/*13:*/
#line 259 "straighten.w"

for(j= 0;j<kk;j++)for(k= 0;k<a[j+1];k++){
l= t[j][k];
row[l]= j,col[l]= k;
tt[j][k]= 0;
}

/*:13*/
#line 240 "straighten.w"
;
/*14:*/
#line 272 "straighten.w"

for(j= 1;j<=kk;j++)aa[j]= 0;
aa[0]= maxn+1;
l= 1;
newlev:j= 1;
tryit:if(tt[j-1][col[l]]==0&&aa[j-1]> aa[j]){
w[l]= j,tt[j-1][col[l]]= l,aa[j]++;
if(l==n)/*15:*/
#line 289 "straighten.w"

{
sign= inv;
for(j= 1;j<kk;j++)for(k= 0;k<a[j+1];k++)
for(l= 0;l<j;l++)if(tt[l][k]> tt[j][k])sign++;
for(k= 0,j= 1;j<n;j++)k= trie[k][w[j]];
rep[trie[k][w[n]]]= (sign&1?-1:1);
l= n;
j= w[l],tt[j-1][col[l]]= 0,aa[j]--;
goto levdone;
}

/*:15*/
#line 279 "straighten.w"
;
l++;goto newlev;
}
tryagain:if(++j<=b[col[l]+1])goto tryit;
levdone:l--;
if(l){
j= w[l],tt[j-1][col[l]]= 0,aa[j]--;
goto tryagain;
}

/*:14*/
#line 241 "straighten.w"
;
}

/*:11*/
#line 72 "straighten.w"

main()
{
register int j,jj,k,l;
/*4:*/
#line 112 "straighten.w"

for(j= 0;;j++){
if(j> maxn){
fprintf(stderr,"Partition too long (maxn=%d)!\n",maxn);
exit(-1);
}
if(scanf("%d",&a[j+1])!=1){
fprintf(stderr,"Partition should end with zero!\n");
exit(-2);
}
if(a[j+1]==0)break;
if(a[j+1]<0){
fprintf(stderr,"Partition contains a negative element (%d)!\n",a[j+1]);
exit(-3);
}
if(a[j+1]> maxn){
fprintf(stderr,"Partition element %d is too big (maxn=%d)!\n",a[j+1],maxn);
exit(-4);
}
}
kk= j;
for(j= 2,n= a[1];j<=kk;j++)n+= a[j];
if(n> maxn){
fprintf(stderr,"Shape is too big (n=%d, maxn=%d)!\n",n,maxn);
exit(-5);
}

/*:4*/
#line 76 "straighten.w"
;
/*5:*/
#line 141 "straighten.w"

for(k= a[1],j= 1;k;j++)while(k> a[j+1])b[k--]= j;

/*:5*/
#line 77 "straighten.w"
;
/*6:*/
#line 150 "straighten.w"

/*7:*/
#line 165 "straighten.w"

for(j= jj= 0;j<kk;j++)for(k= 0;k<a[j+1];k++){
t[j][k]= ++jj;
if(k> 0)prec[jj-1][jj]= 1;
if(j> 0)prec[t[j-1][k]][jj]= 1;
}

/*:7*/
#line 151 "straighten.w"
;
v1:for(j= 0;j<=n;j++)p[j]= q[j]= j,prec[0][j]= 1;
v2:/*8:*/
#line 181 "straighten.w"

if(f==maxf){
fprintf(stderr,"Too many standard tableaux exist (maxf=%d)!\n",maxf);
exit(-6);
}
for(j= 0;j<kk;j++)for(k= 0;k<a[j+1];k++)w[q[t[j][k]]]= j+1;
for(j= 1,k= 0;j<n;j++,k= l){
l= trie[k][w[j]];
if(l==0)l= trie[k][w[j]]= ++trienodes;
}
trie[k][w[n]]= 1;
f++;

/*:8*/
#line 153 "straighten.w"
;
k= n;
v3:j= q[k],l= p[j-1];
if(prec[l][k])goto v5;
v4:p[j-1]= k,p[j]= l,q[k]= j-1,q[l]= j;
goto v2;
v5:while(j<k)l= p[j+1],p[j]= l,q[l]= j,j++;
p[k]= q[k]= k;
k--;
if(k)goto v3;
/*10:*/
#line 204 "straighten.w"

l= 1,k= 0,j= 0;
newlev:w[l]= 1;
tryit:if(trie[k][w[l]]){
if(l==n){
for(;l;l--)stand[j][l]= w[l];
l= n;trie[k][w[l]]= j++;goto levdone;
}
q[l]= k,k= trie[k][w[l]],l++;goto newlev;
}
tryagain:if(w[l]==kk)goto levdone;
w[l]++;goto tryit;
levdone:l--;
if(l){
k= q[l];
goto tryagain;
}
if(j!=f){
fprintf(stderr,"Oops, I goofed!\n");
exit(-7);
}

/*:10*/
#line 163 "straighten.w"
;

/*:6*/
#line 78 "straighten.w"
;
printf("There are %d standard tableaux of shape",f);
for(j= 1;j<=kk;j++)printf(" %d",a[j]);
printf(".\n");
/*16:*/
#line 311 "straighten.w"

for(k= 0;k<f;k++){
for(j= 1;j<=kk;j++)aa[j]= 0;
for(j= 1;j<=n;j++)l= stand[k][j],t[l-1][aa[l]]= j,aa[l]++;
findrep();
for(j= 0;j<f;j++)basis[k][j]= rep[j];
}

/*:16*/
#line 82 "straighten.w"
;
while(1){
/*17:*/
#line 319 "straighten.w"

for(j= 1;j<=n;j++)if(scanf("%d",&p[j])!=1)break;
if(j<=n)break;
for(j= 1;j<=n;j++)q[j]= 0;
for(j= 1;j<=n;j++)if(p[j]<=0||p[j]> n||q[p[j]]){
fprintf(stderr,"Not a permutation of {1,...,%d}:",n);
for(j= 1;j<=n;j++)fprintf(stderr," %d",p[j]);
fprintf(stderr,"!\n");
exit(-8);
}else q[p[j]]= j;

/*:17*/
#line 84 "straighten.w"
;
printf("Representation of");
for(j= 1;j<=n;j++)printf(" %d",p[j]);
printf(":\n");
for(jj= 0;jj<f;jj++){
/*18:*/
#line 330 "straighten.w"

for(j= 1;j<=kk;j++)aa[j]= 0;
for(j= 1;j<=n;j++)l= stand[jj][j],t[l-1][aa[l]]= p[j],aa[l]++;
findrep();

/*:18*/
#line 90 "straighten.w"
;
/*19:*/
#line 335 "straighten.w"

for(j= 0;j<f;j++){
l= rep[j];
if(l)for(k= j+1;k<f;k++)rep[k]-= l*basis[j][k];
}

/*:19*/
#line 91 "straighten.w"
;
for(j= 0;j<f;j++)printf("% 3d",rep[j]);
printf("\n");
}
}
}

/*:2*/
