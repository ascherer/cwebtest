#define m 2
#define n 20
#define space (1<<(n-1) )  \

/*1:*/
#line 20 "unavoidable2.w"

#include <stdio.h> 
#include <math.h> 
char avoid[m*space];
int deg[space];
int link[space];
int a[n+1];
double sine[n];
int count;
int code;

main()
{
register int d,j,k,l,q;
register int top;
double u= 2*3.1415926535897932385/(double)n;
register double s;
for(j= 0;j<n;j++)sine[j]= sin(j*u);
/*2:*/
#line 50 "unavoidable2.w"

for(j= 0;j<space;j++)deg[j]= m;
count= d= 0;top= -1;
for(j= n;j;j--)a[j]= 0;
a[0]= -1,j= 1;
while(1){
if(n%j==0)/*3:*/
#line 72 "unavoidable2.w"

{
d++;
if(j<n)q= n;
else{
for(q= 1;;q++){
for(l= 1,s= 0.0;l<=n;l++)s+= a[l]*sine[(l-1+n-q)%n];
if(s<.0001)break;
}
for(q++;q<n+n;q++){
for(l= 1,s= 0.0;l<=n;l++)s+= a[l]*sine[(l-1+n+n-q)%n];
if(s>=.0001)break;
}
if(q> n)q-= n;
}
for(code= 0,k= q+1;k<=n;k++)code= m*code+a[k];
for(k= 1;k<=q;k++)code= m*code+a[k];
/*4:*/
#line 92 "unavoidable2.w"

avoid[code]= 1;
q= code/m;
deg[q]--;
if(deg[q]==0)deg[q]= -1,link[q]= top,top= q,count++;

/*:4*/
#line 89 "unavoidable2.w"
;
}

/*:3*/
#line 56 "unavoidable2.w"
;
for(j= n;a[j]==m-1;j--);
if(j==0)break;
a[j]++;
for(k= j+1;k<=n;k++)a[k]= a[k-j];
}
printf("m=%d, n=%d: avoiding one arc in each of %d disjoint cycles\n",m,n,d);

/*:2*/
#line 38 "unavoidable2.w"
;
for(d= 0;count;d++){
printf("Vertices at distance %d: %d\n",d,count);
for(l= top,top= -1,count= 0;l>=0;l= link[l])
/*5:*/
#line 98 "unavoidable2.w"

for(j= m-1;j>=0;j--){
k= l+j*space;
if(!avoid[k]){
q= k/m;
deg[q]--;
if(deg[q]==0)deg[q]= l,link[q]= top,top= q,count++;
}
}

/*:5*/
#line 43 "unavoidable2.w"

}
/*6:*/
#line 112 "unavoidable2.w"

printf("Path:");
for(code= k,j= 1;j<n;j++){
code= code*m,q= code/space;
printf(" %d",q);
code-= q*space;
}
while(deg[k]>=0){
printf(" %d",deg[k]%m);
k= deg[k];
}

/*:6*/
#line 45 "unavoidable2.w"
;
}

/*:1*/
