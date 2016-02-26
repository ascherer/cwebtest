#define m 2
#define n 20
#define space (1<<(n-1) )  \

/*1:*/
#line 17 "./unavoidable.w"

#include <stdio.h> 
char avoid[m*space];
int deg[space];
int link[space];
int a[n+1];
int count;
int code;

main()
{
register int d,j,k,l,q;
register int top;
/*2:*/
#line 42 "./unavoidable.w"

for(j= 0;j<space;j++)deg[j]= m;
count= d= 0;top= -1;
for(j= n;j;j--)a[j]= 0;
a[0]= -1,j= 1;
while(1){
if(n%j==0)/*3:*/
#line 68 "./unavoidable.w"

{
d++;
if(j<n)l= j,q= n;
else for(l= 1,k= 2;;k++){


if(a[k-l]<a[k]){
q= l*(int)(n/l);
if(k> q)break;
l= k;
if(k==n)break;
}
}
for(code= 0,k= q+1;k<=n;k++)code= m*code+a[k];
for(k= 1;k<=q;k++)code= m*code+a[k];
/*4:*/
#line 87 "./unavoidable.w"

avoid[code]= 1;
q= code/m;
deg[q]--;
if(deg[q]==0)deg[q]= -1,link[q]= top,top= q,count++;

/*:4*/
#line 84 "./unavoidable.w"
;
}

/*:3*/
#line 48 "./unavoidable.w"
;
for(j= n;a[j]==m-1;j--);
if(j==0)break;
a[j]++;
for(k= j+1;k<=n;k++)a[k]= a[k-j];
}
printf("m=%d, n=%d: avoiding one arc in each of %d disjoint cycles\n",m,n,d);

/*:2*/
#line 30 "./unavoidable.w"
;
for(d= 0;count;d++){
printf("Vertices at distance %d: %d\n",d,count);
for(l= top,top= -1,count= 0;l>=0;l= link[l])
/*5:*/
#line 93 "./unavoidable.w"

for(j= m-1;j>=0;j--){
k= l+j*space;
if(!avoid[k]){
q= k/m;
deg[q]--;
if(deg[q]==0)deg[q]= l,link[q]= top,top= q,count++;
}
}

/*:5*/
#line 35 "./unavoidable.w"

}
/*6:*/
#line 107 "./unavoidable.w"

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
#line 37 "./unavoidable.w"
;
}

/*:1*/
