#define maxn 63
#define maxp 1000 \

/*1:*/
#line 38 "./domination.w"

#include <stdio.h> 
#include <string.h> 
char perm[maxp][maxn+1];
char work[maxn+1];
char rel[maxp][maxp];
char red[maxp][maxp];

main(int argc,char*argv[])
{
register int i,j,k,l,ll,m,n,s,dom;
/*2:*/
#line 56 "./domination.w"

if(argc!=2){
fprintf(stderr,"Usage: %s digits_to_permute\n",argv[0]);
exit(-1);
}
for(j= 0;argv[1][j];j++){
if(j> maxn){
fprintf(stderr,"String too long (maxn=%d)!\n",maxn);
exit(-2);
}
if(argv[1][j]<'0'||argv[1][j]> '9'){
fprintf(stderr,"The string %s should contain digits only!\n",argv[1]);
exit(-3);
}
if(j> 0&&argv[1][j-1]> argv[1][j]){
fprintf(stderr,"The string %s should be nondecreasing!\n",argv[1]);
exit(-4);
}
work[j+1]= argv[1][j];
}
n= j;

/*:2*/
#line 49 "./domination.w"
;
/*3:*/
#line 80 "./domination.w"

m= 0;
l1:if(m==maxp){
fprintf(stderr,"Too many permutations (maxp=%d)!\n",maxp);
exit(-5);
}
for(j= 0;j<n;j++)perm[m][j]= work[j+1];
m++;
l2:for(j= n-1;work[j]>=work[j+1];j--);
if(j==0)goto done;
l3:for(l= n;work[j]>=work[l];l--);
s= work[j],work[j]= work[l],work[l]= s;
l4:for(k= j+1,l= n;k<l;k++,l--)s= work[k],work[k]= work[l],work[l]= s;
goto l1;
done:

/*:3*/
#line 50 "./domination.w"
;
/*4:*/
#line 100 "./domination.w"

for(l= 0;l<m;l++)for(ll= l+1;ll<m;ll++){
dom= 0;
for(k= work[n]+1;k<=work[1];k++)for(j= 0;j<n;j++){
for(i= s= 0;i<=j;i++)
s+= (perm[l][i]>=k?1:0)-(perm[ll][i]>=k?1:0);
if(s> 0)goto fin;
if(s<0)dom= 1;
}
if(dom)rel[l][ll]= 1;
fin:
}

/*:4*/
#line 51 "./domination.w"
;
/*5:*/
#line 115 "./domination.w"

for(l= 0;l<m;l++)for(ll= l+1;ll<m;ll++){
if(rel[l][ll]){
for(j= l+1;j<ll;j++)if(rel[l][j]&&rel[j][ll])goto nope;
red[l][ll]= 1;
}
nope:
}

/*:5*/
#line 52 "./domination.w"
;
/*6:*/
#line 124 "./domination.w"

for(l= 0;l<m;l++){
printf("%s <",perm[l]);
for(ll= l+1;ll<m;ll++)if(red[l][ll])printf(" %s",perm[ll]);
printf("\n");
}

/*:6*/
#line 53 "./domination.w"
;
}

/*:1*/
