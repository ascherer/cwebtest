#define maxm 7
#define maxn 10
#define maxmwds 20000
#define maxtriesize 1000000
#define o mems++
#define oo mems+= 2
#define ooo mems+= 3
#define bufsize maxm+maxn \

#define trunc(c) ((c) &0x1f)  \

/*1:*/
#line 23 "back-mxn-words-new.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
unsigned long long mems;
unsigned long long thresh= 10000000000;
int maxmm= maxmwds,maxnn= maxtriesize;
char mword[maxmwds][maxm+1];
int mlink[maxmwds+1][maxm];
int head[maxm][26],size[maxm][26];
int trie[maxtriesize][27];
int trieptr;
char buf[bufsize];
unsigned int count;
FILE*mfile,*nfile;
int a[maxn+1][maxn+1];
int x[maxn+1],y[maxn+1],z[maxn+1];
long long profile[maxn+2],weight[maxn+2];
main(int argc,char*argv[]){
register int i,j,k,l,m,n,p,q,mm,nn,t,xl,yl,zl;
register char*w;
/*3:*/
#line 58 "back-mxn-words-new.w"

if(argc!=3){
fprintf(stderr,"Usage: %s mwords[:mm] nwords[:nn]\n",argv[0]);
exit(-1);
}
w= strchr(argv[1],':');
if(w){
if(sscanf(w+1,"%d",&maxmm)!=1){
fprintf(stderr,"I can't parse the m-file spec `%s'!\n",argv[1]);
exit(-20);
}
*w= 0;
}
if(!(mfile= fopen(argv[1],"r"))){
fprintf(stderr,"I can't open file `%s' for reading m-words!\n",argv[1]);
exit(-2);
}
w= strchr(argv[2],':');
if(w){
if(sscanf(w+1,"%d",&maxnn)!=1){
fprintf(stderr,"I can't parse the n-file spec `%s'!\n",argv[1]);
exit(-22);
}
*w= 0;
}
if(!(nfile= fopen(argv[2],"r"))){
fprintf(stderr,"I can't open file `%s' for reading n-words!\n",argv[2]);
exit(-3);
}

/*:3*/
#line 44 "back-mxn-words-new.w"
;
/*4:*/
#line 88 "back-mxn-words-new.w"

m= mm= 0;
while(1){
if(mm==maxmm)break;
if(!fgets(buf,bufsize,mfile))break;
mm++;
for(k= 0;o,buf[k]>='a'&&buf[k]<='z';k++)o,mword[mm][k]= buf[k];
if(buf[k]!='\n'){
fprintf(stderr,"Illegal m-word: %s",buf);
exit(-10);
}
if(m==0){
m= k;
if(m> maxm){
fprintf(stderr,"Sorry, m should be at most %d!\n",maxm);
exit(-16);
}
}else if(k!=m){
fprintf(stderr,"The m-file has words of lengths %d and %d!\n",m,k);
exit(-4);
}
/*5:*/
#line 122 "back-mxn-words-new.w"

for(k= 0;k<m;k++){
j= trunc(buf[k])-1;
o,p= head[k][j];
o,head[k][j]= mm;
o,mlink[mm][k]= p;
oo,size[k][j]++;
}

/*:5*/
#line 109 "back-mxn-words-new.w"
;
}
fprintf(stderr,"OK, I've successfully read %d words of length m=%d.\n",mm,m);

/*:4*/
#line 45 "back-mxn-words-new.w"
;
/*6:*/
#line 147 "back-mxn-words-new.w"

n= nn= 0,trieptr= 1;
while(1){
if(nn==maxnn)break;
if(!fgets(buf,bufsize,nfile))break;
for(k= p= 0;o,buf[k]>='a'&&buf[k]<='z';k++,p= q){
o,q= trie[p][trunc(buf[k])];
if(q==0)break;
}
for(j= k;o,buf[j]>='a'&&buf[j]<='z';j++){
if(trieptr==maxtriesize){
fprintf(stderr,"Overflow (maxtriesize=%d)!\n",maxtriesize);
exit(-66);
}
i= trunc(buf[j]);
oo,trie[p][0]+= (1<<(i-1));
if(j<n-1||n==0){
o,trie[p][i]= trieptr;
p= trieptr++;
}
}
if(buf[j]!='\n'){
fprintf(stderr,"Illegal n-word: %s",buf);
exit(-11);
}
/*7:*/
#line 181 "back-mxn-words-new.w"

if(n==0){
n= j;
p--,trieptr--;
if(n> maxn){
fprintf(stderr,"Sorry, n should be at most %d!\n",maxn);
exit(-17);
}
}else{
if(n!=j){
fprintf(stderr,"The n-file has words of lengths %d and %d!\n",n,j);
exit(-5);
}
if(k==n){
buf[j]= 0;
fprintf(stderr,"The n-file has the duplicate word `%s'!\n",buf);
exit(-6);
}
}

/*:7*/
#line 172 "back-mxn-words-new.w"
;
o,trie[p][trunc(buf[n-1])]= nn+1;
mems-= 3;

nn++;
}
fprintf(stderr,"Plus %d words of length n=%d.\n",nn,n);
fprintf(stderr,"(The trie has %d nodes.)\n",trieptr);

/*:6*/
#line 46 "back-mxn-words-new.w"
;
fprintf(stderr,"(%llu mems to initialize the data structures)\n",mems);
/*8:*/
#line 203 "back-mxn-words-new.w"

b1:l= 1;
for(k= 1;k<=m;k++)o,a[0][k]= 0;
b2:profile[l]++;
/*11:*/
#line 254 "back-mxn-words-new.w"

if(mems>=thresh){
thresh+= 10000000000;
fprintf(stderr,"After %lld mems:",mems);
for(k= 2;k<=l;k++)fprintf(stderr," %lld",profile[k]);
fprintf(stderr,"\n");
}

/*:11*/
#line 207 "back-mxn-words-new.w"
;
if(l> n)/*10:*/
#line 244 "back-mxn-words-new.w"

{
count++;printf("%d:",count);
for(k= 1;k<=n;k++)printf(" %s",mword[x[k]]);
for(p= 0,k= 1;k<=n;k++)if(x[k]>=p)p= x[k];
for(q= 0,j= 1;j<=m;j++)if(a[n][j]> q)q= a[n][j];
printf(" (%06d,%06d; sum %07d, prod %012d)\n",p,q,p+q,p*q);
goto b5;
}

/*:10*/
#line 208 "back-mxn-words-new.w"
;
/*9:*/
#line 236 "back-mxn-words-new.w"

for(k= 1,p= maxmm+1;k<=m;k++){
for(oo,t= trie[a[l-1][k]][0],q= 0,i= 0;(1<<i)<=t;i++)
if((1<<i)&t)o,q+= size[k-1][i];
if(q<p)p= q,zl= k-1,yl= t;
}
weight[l]+= p;

/*:9*/
#line 209 "back-mxn-words-new.w"
;
i= 0;
next_i:while(((1<<i)&yl)==0)i++;
o,xl= head[zl][i];
if(xl==0)goto new_i;
b3:o,w= mword[xl];
for(k= 1;k<=m;k++){
oo,q= trie[a[l-1][k]][trunc(w[k-1])];
if(!q)goto b4;else o,a[l][k]= q;
}
ooo,x[l]= xl,y[l]= yl,z[l]= zl,l++;
goto b2;
b4:o,xl= mlink[xl][zl];
if(xl)goto b3;
new_i:if((1<<(++i))<=yl)goto next_i;
b5:l--;
if(l){
ooo,xl= x[l],yl= y[l],zl= z[l];
o,i= mword[xl][zl]-'a';
goto b4;
}

/*:8*/
#line 48 "back-mxn-words-new.w"
;
fprintf(stderr,"Altogether %u solutions (%llu mems).\n",count,mems);
/*2:*/
#line 53 "back-mxn-words-new.w"

fprintf(stderr,"Profile:          1%9.1f\n",(double)weight[1]);
for(k= 2;k<=n+1;k++)fprintf(stderr,"%19lld%9.1f\n",
profile[k],profile[k]?weight[k]/(double)profile[k]:0.0);

/*:2*/
#line 50 "back-mxn-words-new.w"
;
}

/*:1*/
