#define maxn 100 \

/*1:*/
#line 20 "./yplay.w"

#include <stdio.h> 
char a[maxn+1][maxn+1][maxn+maxn+1];
main(){
register int i,j,k,n,s;
/*2:*/
#line 30 "./yplay.w"

fgets(a[0][0],maxn+2,stdin);
for(n= 0;a[0][0][n]==' ';n++);
a[0][0][n+n+1]= '\0';
for(k= 1;k<=n;k++){
fgets(a[0][k],maxn+2,stdin);
a[0][k][n+n+1]= 0;
}

/*:2*/
#line 25 "./yplay.w"
;
for(k= 1;k<=n;k++)/*3:*/
#line 39 "./yplay.w"

for(j= 0;j<=n-k;j++){
for(i= 0;i<=n+n-k-k;i++)a[k][j][i]= ' ';
for(i= n-k-j;i<=n-k+j;i+= 2){
s= 0;
if(a[k-1][j][i+1]=='o')s++;
if(a[k-1][j+1][i]=='o')s++;
if(a[k-1][j+1][i+2]=='o')s++;
a[k][j][i]= (s> 1?'o':'x');
}
}

/*:3*/
#line 26 "./yplay.w"
;
/*4:*/
#line 51 "./yplay.w"

for(k= 0;k<=n;k++){
printf(a[0][k]);
for(j= 1;j<=k;j++)printf("  %s",a[j][k-j]);
printf("\n");
}

/*:4*/
#line 27 "./yplay.w"
;
}

/*:1*/
