#define maxn 25 \

/*1:*/
#line 8 "./sat-commafree.w"

#include <stdio.h> 
#include <stdlib.h> 
int n;
char a[maxn+1];
main(int argc,char*argv[]){
register int i,j,k;
register unsigned int x,y,z;
register unsigned long long m,acc,xy;
/*2:*/
#line 22 "./sat-commafree.w"

if(argc!=2||sscanf(argv[1],"%d",&n)!=1){
fprintf(stderr,"Usage: %s n\n",argv[0]);
exit(-1);
}
if(n<2||n> maxn){
fprintf(stderr,"n should be between 2 and %d, not %d!\n",maxn,n);
exit(-2);
}
printf("~ sat-commafree %d\n",n);

/*:2*/
#line 17 "./sat-commafree.w"
;
/*3:*/
#line 35 "./sat-commafree.w"

f1:a[0]= -1,j= 1;
f2:if(j==n)/*4:*/
#line 45 "./sat-commafree.w"

{
for(i= 0;i<n;i++){
for(x= 0,k= 0;k<n;k++)
x= (x<<1)+a[1+((i+k)%n)];
printf(" %x",x);
}
printf("\n");
}

/*:4*/
#line 37 "./sat-commafree.w"
;
f3:for(j= n;a[j]==1;j--);
f4:if(j){
a[j]= 1;
f5:for(k= j+1;k<=n;k++)a[k]= a[k-j];
goto f2;
}

/*:3*/
#line 18 "./sat-commafree.w"
;
/*5:*/
#line 55 "./sat-commafree.w"

m= (1LL<<n)-1;
for(x= 0;x<(1<<n);x++){
/*6:*/
#line 65 "./sat-commafree.w"

acc= (((unsigned long long)x)<<n)+x;
for(k= 1;k<n;k++)
if(((acc>>k)&m)==x)break;
if(k<n)continue;

/*:6*/
#line 58 "./sat-commafree.w"
;
for(y= 0;y<(1<<n);y++){
/*7:*/
#line 71 "./sat-commafree.w"

acc= (((unsigned long long)y)<<n)+y;
for(k= 1;k<n;k++)
if(((acc>>k)&m)==y)break;
if(k<n)continue;

/*:7*/
#line 60 "./sat-commafree.w"
;
/*9:*/
#line 83 "./sat-commafree.w"

xy= (((unsigned long long)x)<<n)+y;
for(j= 1;j<n;j++){
z= (xy>>j)&m;
/*8:*/
#line 77 "./sat-commafree.w"

acc= (((unsigned long long)z)<<n)+z;
for(k= 1;k<n;k++)
if(((acc>>k)&m)==z)break;
if(k<n)continue;

/*:8*/
#line 87 "./sat-commafree.w"
;
printf("~%x ~%x ~%x\n",
x,y,z);
}

/*:9*/
#line 61 "./sat-commafree.w"
;
}
}

/*:5*/
#line 19 "./sat-commafree.w"
;
}

/*:1*/
