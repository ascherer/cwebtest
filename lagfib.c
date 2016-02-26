#define r 100
#define s 37
#define n 400 \

/*1:*/
#line 12 "./lagfib.w"

#include <stdio.h> 
/*2:*/
#line 32 "./lagfib.w"

int a[n+1][r];
int b[r];
int c[r],d[n+2];
int p[n];

/*:2*//*6:*/
#line 83 "./lagfib.w"

int stat[r+1];

/*:6*/
#line 14 "./lagfib.w"


main()
{
register int i,j,k,m,t;
/*3:*/
#line 38 "./lagfib.w"

for(k= 0;k<r;k++){
a[k][k]= 1;
b[k]= k;
p[k]= 1;
}
for(;k<=n;k++){
for(j= 1;j<r;j++)a[k][j]= a[k-1][j-1];
if(a[k-1][r-1]){
a[k][0]= 1;
a[k][r-s]^= 1;
}
}
m= n-1;
t= r;

/*:3*/
#line 19 "./lagfib.w"
;
while(t){
/*5:*/
#line 72 "./lagfib.w"

{
register int forbidden= 0;
for(j= 0,i= 0;j<r;j++)if(a[n][j]){
if(b[j]<n-r+t&&b[j]>=t)forbidden= 1;
else i++;
}
if(forbidden)/*7:*/
#line 86 "./lagfib.w"

{
for(i= 0;i<n;i++)d[i]= 0;
for(j= 0;j<r;j++)if(a[m][j])d[b[j]]= 1;
d[m]= 1;d[n]= 1;
printf("%d:",t);
for(i= 0;;){
while(d[i]==0)i++;
if(i==n)break;
printf(" %d",i);
while(d[i]==1)i++;
if(i> n)i= n;
printf("..%d",i-1);
}
printf("\n");
}

/*:7*/
#line 79 "./lagfib.w"

else stat[i]++;
}

/*:5*/
#line 21 "./lagfib.w"
;
t--;
/*4:*/
#line 54 "./lagfib.w"

for(k= m;a[k][t]==0;k--);
b[t]= k;
for(j= 0;j<r;j++)c[j]= a[k][j];
c[t]= 0;
p[t]= 0;p[k]= 1;
for(;k>=t;k--)
if(a[k][t])
for(j= 0;j<r;j++)a[k][j]^= c[j];
if(a[n][t])
for(j= 0;j<r;j++)a[n][j]^= c[j];
while(p[m]==1)m--;

/*:4*/
#line 23 "./lagfib.w"
;
}
/*8:*/
#line 103 "./lagfib.w"

for(j= 0;j<=r;j++)printf(" %3d: %d\n",j,stat[j]);

/*:8*/
#line 25 "./lagfib.w"
;
}

/*:1*/
