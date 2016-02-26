#define nmax 16
#define lmax 25742
/*1:*/
#line 21 "./erection.w"

#include <stdio.h> 
int n;
int mask;
int S[lmax+1],L[lmax+1];
int r;
int h;
int nh;
int avail;
int unused;
int x;
int rank[1<<nmax];
/*8:*/
#line 93 "./erection.w"

void insert(void);
void generate(void){
register int t,v,y,j,k;
for(j= L[h];j!=h;j= L[j]){
y= S[j];
t= mask-y;
/*9:*/
#line 107 "./erection.w"

for(k= L[nh];k!=nh;k= L[k])if((S[k]&y)==y)t&= ~S[k];

/*:9*/
#line 101 "./erection.w"
;
/*10:*/
#line 110 "./erection.w"

while(t){
x= y|(t&-t);
insert();
t&= ~x;
}

/*:10*/
#line 102 "./erection.w"
;
}
}

/*:8*//*11:*/
#line 123 "./erection.w"

void insert(void){
register int j,k;
j= nh;
store:S[nh]= x;
loop:k= j;
continu:j= L[k];
if(rank[S[j]&x]<=r)goto loop;
if(j!=nh){
if(x==(x|S[j])){
L[k]= L[j],L[j]= avail,avail= j;
goto continu;
}else{
x|= S[j],nh= j;goto store;
}
}
j= avail;
if(j)avail= L[j];
else j= unused++;
L[j]= L[nh];L[nh]= j;S[j]= x;
}

/*:11*//*12:*/
#line 149 "./erection.w"

void enlarge(void){
while(1){
x= 0;
scanf("%x",&x);
if(!x)return;
if(rank[x]> r)insert();
}
}

/*:12*//*13:*/
#line 164 "./erection.w"

void print_set(int t){
register int j,k;
printf(" ");
for(j= 1,k= 0;j<=t;j<<= 1,k++)if(t&j)printf("%x",k);
}

/*:13*//*14:*/
#line 171 "./erection.w"

void print_list(int h){
register int j;
printf("Closed sets for rank %d:",r);
for(j= L[h];j!=h;j= L[j])print_set(S[j]);
printf("\n");
}

/*:14*//*15:*/
#line 183 "./erection.w"

void mark(int m){
register int t,v;
if(rank[m]> r){
if(rank[m]==100+r)print_set(m);
rank[m]= r;
for(t= m;t;t= v){
v= t&(t-1);
mark(m-t+v);
}
}
}

/*:15*//*16:*/
#line 199 "./erection.w"

void sort(){
register int i,j,k;
int hd[101+nmax],tl[101+nmax];
for(i= 100;i<=100+n;i++)hd[i]= -1;
j= L[h];L[h]= h;
while(j!=h){
i= rank[S[j]];
k= L[j];
L[j]= hd[i];
if(L[j]<0)tl[i]= j;
hd[i]= j;
j= k;
}
for(i= 100;i<=100+n;i++)
if(hd[i]>=0)L[tl[i]]= L[h],L[h]= hd[i];
}


/*:16*//*17:*/
#line 221 "./erection.w"

void unmark(int m,int card){
register t,v;
if(rank[m]<100){
rank[m]= card;
for(t= mask-m;t;t= v){
v= t&(t-1);
unmark(m+t-v,card+1);
}
}
}

/*:17*//*18:*/
#line 233 "./erection.w"

void print_circuits(void){
register int i,k;
printf("The circuits are:");
for(k= 1;k<=mask;k+= k)for(i= 0;i<k;i++)if(rank[k+i]==rank[i]){
print_set(k+i);
unmark(k+i,rank[i]+101);
}
printf("\n");
}

/*:18*/
#line 33 "./erection.w"

main(){
register int i,j,k;
if(scanf("%d",&n)!=1||n> 16||n<0){
fprintf(stderr,"Sorry, I can't deal with a universe of size %d.\n",n);
exit(-1);
}
mask= (1<<n)-1;
/*2:*/
#line 48 "./erection.w"

k= 1;rank[0]= 100;
while(k<=mask){
for(i= 0;i<k;i++)rank[k+i]= rank[i]+1;
k= k+k;
}

/*:2*/
#line 41 "./erection.w"
;
/*3:*/
#line 58 "./erection.w"

L[1]= 2;L[2]= 1;S[2]= 0;h= 1;
unused= 3;

/*:3*/
#line 42 "./erection.w"
;
rank[0]= 0,r= 0;
while(rank[mask]> r)/*4:*/
#line 62 "./erection.w"

{
/*5:*/
#line 74 "./erection.w"

nh= avail;
if(nh)avail= L[nh];
else nh= unused++;
L[nh]= nh;

/*:5*/
#line 64 "./erection.w"
;
generate();
if(r)enlarge();
/*6:*/
#line 80 "./erection.w"

for(j= h;L[j]!=h;j= L[j]);
L[j]= avail;avail= h;

/*:6*/
#line 67 "./erection.w"
;
r++;h= nh;
sort();
print_list(h);
/*7:*/
#line 84 "./erection.w"

printf("Independent sets for rank %d:",r);
for(j= L[h];j!=h;j= L[j])mark(S[j]);
printf("\n");

/*:7*/
#line 71 "./erection.w"
;
}

/*:4*/
#line 44 "./erection.w"
;
print_circuits();
}

/*:1*/
