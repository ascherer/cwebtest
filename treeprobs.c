#define maxn 100
#define bufsize 50 \

#define panic(mess) {fprintf(stderr,"%s!\n",mess) ;exit(-1) ;} \

/*5:*/
#line 154 "treeprobs.w"

#include <stdio.h> 
#include <stdlib.h> 
char buf[bufsize];
int edgej[maxn],edgek[maxn];
int S[maxn][maxn];
int kids[maxn];
int where[maxn];
int x[maxn];

main(int argc,char*argv[]){
register int j,k,n,p,q,r,s;
int m;
/*6:*/
#line 173 "treeprobs.w"

if(argc!=2||sscanf(argv[1],"%d",&m)!=1){
fprintf(stderr,"Usage: %s m\n",argv[0]);
exit(-99);
}

/*:6*/
#line 167 "treeprobs.w"
;
/*7:*/
#line 181 "treeprobs.w"

for(n= 1;;n++){
if(!fgets(buf,bufsize,stdin))break;
if(n==maxn)
panic("too many edges");
if(sscanf(buf,"%d %d",&edgej[n],&edgek[n])!=2)
panic("bad input line");
kids[edgej[n]]++;
where[edgej[n]]= n;
}
if(edgek[n-1]!=n)
panic("inconsistent input");

/*:7*/
#line 168 "treeprobs.w"
;
/*8:*/
#line 194 "treeprobs.w"

for(j= 1;j<n;j++){
switch(kids[j]){
case 2:S[j+1][0]= edgek[where[j]];
for(k= 0;S[j][k];k++)S[j+1][k+1]= S[j][k];
if(edgek[where[j]-1]!=j+1)
panic("bad edge for two-kid node");
break;
case 1:for(k= 0;S[j][k];k++)S[j+1][k]= S[j][k];
if(edgek[where[j]]!=j+1)
panic("bad edge for one-kid node");
break;
case 0:if(S[j][0]!=j+1)
panic("bad preorder for no-kid node");
for(k= 1;S[j][k];k++)S[j+1][k-1]= S[j][k];
break;
default:panic("too many kids");
}
}
if(S[n][0])
panic("S[n] not empty");

/*:8*/
#line 169 "treeprobs.w"
;
/*9:*/
#line 216 "treeprobs.w"

for(s= 0;s<2;s++)for(k= 0;k<2;k++)
printf("T[%d,%d,%d]=%d\n",n,s,k,s==k);
for(j= n-1;j;j--){
for(s= 0;s<=m;s++){
if(s<m+1-j)s= m+1-j;
if(s> n+1-j)break;
for(k= 0;S[j][k];k++);
r= k;
while(1){
/*10:*/
#line 237 "treeprobs.w"

printf("T[%d,%d",j,s);
for(k= 0;k<=r;k++)printf(",%d",x[k]);
printf("]=");
if(s-x[0]<0||s-x[0]> n-j)printf("0");
else switch(kids[j]){
case 0:/*11:*/
#line 249 "treeprobs.w"

printf("T[%d,%d",j+1,s-x[0]);
for(k= 1;k<=r;k++)printf(",%d",x[k]);
printf("]");

/*:11*/
#line 243 "treeprobs.w"
;break;
case 1:/*12:*/
#line 254 "treeprobs.w"

for(p= 0;p<2;p++){
if(p)printf("+");
printf("w[%d,%d,%d]",j+1,x[0],p);
printf("T[%d,%d,%d",j+1,s-x[0],p);
for(k= 1;k<=r;k++)printf(",%d",x[k]);
printf("]");
}

/*:12*/
#line 244 "treeprobs.w"
;break;
case 2:/*13:*/
#line 263 "treeprobs.w"

for(p= 0;p<2;p++){
if(p)printf("+");
printf("w[%d,%d,%d](",j+1,x[0],p);
for(q= 0;q<2;q++){
if(q)printf("+");
printf("w[%d,%d,%d]",edgek[where[j]],x[0],q);
printf("T[%d,%d,%d,%d",j+1,s-x[0],p,q);
for(k= 1;k<=r;k++)printf(",%d",x[k]);
printf("]");
}
printf(")");
}

/*:13*/
#line 245 "treeprobs.w"
;break;
}
printf("\n");

/*:10*/
#line 226 "treeprobs.w"
;
for(k= 0;x[k];k++){
x[k]= 0;
}
if(k> r)break;
x[k]= 1;
}
}
}
printf("ans=T[1,%d,0]+T[1,%d,1]\n",m,m);

/*:9*/
#line 170 "treeprobs.w"
;
}

/*:5*/
