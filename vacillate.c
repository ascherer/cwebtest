#define maxn 1000 \

#define infty 1000 \

/*1:*/
#line 13 "vacillate.w"

#include <stdio.h> 
char buf[maxn];
int last[maxn];
int a[maxn],b[maxn];
int p[maxn][maxn],q[maxn][maxn];
int r[maxn];
int dualins[maxn],dualdel[maxn];
int verbose= 1;

main(int argc,char*argv[])
{
register int i,j,k,m,n,xi,xip;
while(fgets(buf,maxn,stdin)){
/*2:*/
#line 35 "vacillate.w"

printf("Given: %s",buf);
for(k= 0,m= -1;
(buf[k]>='0'&&buf[k]<='9')||(buf[k]>='a'&&buf[k]<='z');k++){
j= (buf[k]>='a'?buf[k]-'a'+10:buf[k]-'0');
if(j> m){
if(j!=m+1){
buf[k]= 0;
fprintf(stderr,"Bad form: %s%d should be %s%d!\n",
buf,j,buf,m+1);
continue;
}
m= j,last[m]= 0;
}
a[k+1]= last[j],last[j]= k+1;
}
n= k;

/*:2*/
#line 27 "vacillate.w"
;
/*3:*/
#line 53 "vacillate.w"

for(k= 1;k<=n;k++)b[k]= 0;
for(k= 1;k<=n;k++)if(a[k])b[a[k]]= k;

/*:3*/
#line 28 "vacillate.w"
;
/*4:*/
#line 59 "vacillate.w"

/*5:*/
#line 66 "vacillate.w"

for(k= 1;k<=n;k++){
r[k]= q[0][k]= q[k][0]= 0,p[0][k]= p[k][0]= infty;
for(j= 1;j<=n;j++)q[k][j]= infty,p[k][j]= 0;
}

/*:5*/
#line 60 "vacillate.w"
;
for(k= 1;k<=n;k++){
/*7:*/
#line 99 "vacillate.w"

if(a[k]){
for(i= 1,j= 0;r[i];i++)if(p[i][r[i]]==k){
j= r[i],r[i]= j-1,p[i][j]= 0;
dualdel[k]= j;
break;
}
if(!j){
fprintf(stderr,"I couldn't find %d in p!\n",k);
exit(-1);
}
d1:xip= infty;
while(1){
d2:while(q[i][j+1]<xip)j++;
xi= q[i][j];
d3:q[i][j]= xip;
d4:if(i> 1)i--,xip= xi;
else break;
}
if(xi!=a[k]){
fprintf(stderr,"I removed %d, not %d, from q!\n",xi,a[k]);
exit(-2);
}
}else dualdel[k]= 0;
/*8:*/
#line 127 "vacillate.w"

for(i= 1;r[i];i++)printf(" %d",r[i]);
if(verbose&&i> 1){
printf(" (");
for(i= 1;r[i];i++){
if(i> 1)printf(";");
for(j= 1;j<=r[i];j++)
printf("%s%d",j> 1?",":"",p[i][j]);
}
printf("),(");
for(i= 1;r[i];i++){
if(i> 1)printf(";");
for(j= 1;j<=r[i];j++)
printf("%s%d",j> 1?",":"",q[i][j]);
}
printf(")");
}
if(i==1)printf(" 0\n");else printf("\n");

/*:8*/
#line 123 "vacillate.w"
;

/*:7*/
#line 62 "vacillate.w"
;
/*6:*/
#line 79 "vacillate.w"

if(b[k]){
i1:i= 1,xi= b[k],j= r[1]+1;
while(1){
i2:while(xi> p[i][j-1])j--;
xip= p[i][j];
i3:p[i][j]= xi;
i4:if(xip)i++,xi= xip;
else break;
}
q[i][j]= k;
r[i]= j;
dualins[k]= j;
}else dualins[k]= 0;
/*8:*/
#line 127 "vacillate.w"

for(i= 1;r[i];i++)printf(" %d",r[i]);
if(verbose&&i> 1){
printf(" (");
for(i= 1;r[i];i++){
if(i> 1)printf(";");
for(j= 1;j<=r[i];j++)
printf("%s%d",j> 1?",":"",p[i][j]);
}
printf("),(");
for(i= 1;r[i];i++){
if(i> 1)printf(";");
for(j= 1;j<=r[i];j++)
printf("%s%d",j> 1?",":"",q[i][j]);
}
printf(")");
}
if(i==1)printf(" 0\n");else printf("\n");

/*:8*/
#line 93 "vacillate.w"
;

/*:6*/
#line 63 "vacillate.w"
;
}

/*:4*/
#line 29 "vacillate.w"
;
/*9:*/
#line 148 "vacillate.w"

for(k= 1;k<=n;k++){
if(dualdel[k])/*11:*/
#line 157 "vacillate.w"

{
i= dualdel[k],j= r[i],r[i]= j-1,xip= infty;
while(1){
while(q[i][j+1]<xip)j++;
xi= q[i][j];
q[i][j]= xip;
if(i> 1)i--,xip= xi;
else break;
}
a[k]= xi;
}

/*:11*/
#line 150 "vacillate.w"
;
if(dualins[k])/*10:*/
#line 154 "vacillate.w"

i= dualins[k],j= r[i]+1,r[i]= j,q[i][j]= k;

/*:10*/
#line 151 "vacillate.w"
;
}

/*:9*/
#line 30 "vacillate.w"
;
/*12:*/
#line 170 "vacillate.w"

for(k= 1,m= -1;k<=n;k++)
if(a[k])buf[k-1]= buf[a[k]-1];
else m++,buf[k-1]= (m> 9?'a'+m-10:'0'+m);
printf("Dual: %s",buf);

/*:12*/
#line 31 "vacillate.w"
;
}
}

/*:1*/
