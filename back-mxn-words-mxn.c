#define maxm 7
#define maxn 10
#define maxmwds 20000
#define maxtriesize 100000
#define o mems++
#define oo mems+= 2
#define bufsize maxm+maxn \

#define trunc(c) ((c) &0x1f)  \

/*1:*/
#line 23 "back-mxn-words-mxn.w"

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
unsigned long long mems;
unsigned long long thresh= 10000000000;
typedef struct{
int c;
int link;
}trielt;
int maxmm= maxmwds,maxnn= maxtriesize;
char mword[maxmwds][maxm+1];
int trie[maxtriesize][27];
int trieptr;
trielt tri[maxtriesize];
int triptr;
char buf[bufsize];
unsigned int count;
FILE*mfile,*nfile;
int a[maxn+1][maxn+1];
int x[maxm*maxn+1];
long long profile[maxm*maxn+2];
/*8:*/
#line 217 "back-mxn-words-mxn.w"

int compress(int p,int l){
register int a,c,k;
oo,oo;
if(l==0)return p;
for(c= 0,k= 1;k<27;k++)if(o,trie[p][k])c++;
a= triptr;
triptr+= c+1;
if(triptr>=maxtriesize){
fprintf(stderr,"Tri overflows (maxtriesize=%d)!\n",maxtriesize);
exit(-67);
}
for(c= 0,k= 1;k<27;k++)if(o,trie[p][k]){
o,tri[a+c].link= compress(trie[p][k],l-1);
tri[a+c++].c= k;
o,trie[p][k]= 0;
}
return a;
}

/*:8*/
#line 45 "back-mxn-words-mxn.w"
;
main(int argc,char*argv[]){
register int i,j,k,l,m,n,p,q,mm,nn,xl,li,lj,lnk,chr;
register char*w;
/*3:*/
#line 62 "back-mxn-words-mxn.w"

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
#line 49 "back-mxn-words-mxn.w"
;
/*4:*/
#line 92 "back-mxn-words-mxn.w"

m= mm= 0;
while(1){
if(mm==maxmm)break;
if(!fgets(buf,bufsize,mfile))break;
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
mm++;
}
/*7:*/
#line 192 "back-mxn-words-mxn.w"

for(i= 0,trieptr= 1;i<mm;i++){
for(o,k= p= 0;k<m;k++,p= q){
o,q= trie[p][trunc(mword[i][k])];
if(q==0)break;
}
for(j= k;j<m;j++){
if(j<m-1){
if(trieptr==maxtriesize){
fprintf(stderr,"Overflow (maxtriesize=%d)!\n",maxtriesize);
exit(-67);
}
o,trie[p][trunc(mword[i][j])]= trieptr;
p= trieptr++;
}
}
o,trie[p][trunc(mword[i][m-1])]= i+1;
}
compress(0,m);

/*:7*/
#line 114 "back-mxn-words-mxn.w"
;
fprintf(stderr,"OK, I've successfully read %d words of length m=%d.\n",mm,m);
fprintf(stderr,"(The tri has %d elements.)\n",triptr);

/*:4*/
#line 50 "back-mxn-words-mxn.w"
;
/*5:*/
#line 129 "back-mxn-words-mxn.w"

n= nn= 0,trieptr= 1;
while(1){
if(nn==maxnn)break;
if(!fgets(buf,bufsize,nfile))break;
for(k= p= 0;o,buf[k]>='a'&&buf[k]<='z';k++,p= q){
o,q= trie[p][trunc(buf[k])];
if(q==0)break;
}
for(j= k;o,buf[j]>='a'&&buf[j]<='z';j++){
if(j<n-1||n==0){
if(trieptr==maxtriesize){
fprintf(stderr,"Overflow (maxtriesize=%d)!\n",maxtriesize);
exit(-66);
}
o,trie[p][trunc(buf[j])]= trieptr;
p= trieptr++;
}
}
if(buf[j]!='\n'){
fprintf(stderr,"Illegal n-word: %s",buf);
exit(-11);
}
/*6:*/
#line 161 "back-mxn-words-mxn.w"

if(n==0){
n= j;
if(n> maxn){
fprintf(stderr,"Sorry, n should be at most %d!\n",maxn);
exit(-17);
}
p--,trieptr--;
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

/*:6*/
#line 152 "back-mxn-words-mxn.w"
;
o,trie[p][trunc(buf[n-1])]= nn+1;
mems-= 3;

nn++;
}
fprintf(stderr,"Plus %d words of length n=%d.\n",nn,n);
fprintf(stderr,"(The trie has %d nodes.)\n",trieptr);

/*:5*/
#line 51 "back-mxn-words-mxn.w"
;
fprintf(stderr,"(%llu mems to initialize the data structures)\n",mems);
/*9:*/
#line 239 "back-mxn-words-mxn.w"

b1:l= 1;
for(k= 0;k<m;k++)o,a[k][0]= 0;
b2:profile[l]++;
/*11:*/
#line 278 "back-mxn-words-mxn.w"

if(mems>=thresh){
thresh+= 10000000000;
fprintf(stderr,"After %lld mems:",mems);
for(k= 2;k<=l;k++)fprintf(stderr," %lld",profile[k]);
fprintf(stderr,"\n");
}

/*:11*/
#line 243 "back-mxn-words-mxn.w"
;
if(l> m*n)/*10:*/
#line 267 "back-mxn-words-mxn.w"

{
count++;printf("%d:",count);
for(k= 1;k<=n;k++)
printf(" %s",mword[tri[x[m*k]].link-1]);
for(p= 0,k= 1;k<=n;k++)if(tri[x[m*k]].link> p)p= tri[x[m*k]].link;
for(q= 0,j= 1;j<=m;j++)if(a[j-1][n]> q)q= a[j-1][n];
printf(" (%06d,%06d; sum %07d, prod %012d)\n",p,q,p+q,p*q);
goto b5;
}

/*:10*/
#line 244 "back-mxn-words-mxn.w"
;
li= (l-1)%m;
lj= ((l-1)/m)+1;
if(li)xl= lnk;
else xl= 0;
o,lnk= tri[xl].link;
b3:chr= tri[xl].c;
oo,q= trie[a[li][lj-1]][chr];
if(!q)goto b4;
o,x[l]= xl;
o,a[li][lj]= q;
l++;
goto b2;
b4:o,lnk= tri[++xl].link;
if(lnk)goto b3;
b5:l--;
if(l){
o,xl= x[l];
li= (l-1)%m;
lj= ((l-1)/m)+1;
goto b4;
}

/*:9*/
#line 53 "back-mxn-words-mxn.w"
;
fprintf(stderr,"Altogether %u solutions (%llu mems).\n",count,mems);
/*2:*/
#line 58 "back-mxn-words-mxn.w"

fprintf(stderr,"Profile:          1\n");
for(k= 2;k<=m*n+1;k++)fprintf(stderr,"%19lld\n",profile[k]);

/*:2*/
#line 55 "back-mxn-words-mxn.w"
;
}

/*:1*/
