#define maxn 100000
#define maxm 40 \

#define assign(x,y) assigned[undo_ptr].ptr= x,assigned[undo_ptr++].val= *x,*x= y \

/*1:*/
#line 20 "./strongchain.w"

#include <stdio.h> 
int L[maxn];
int ub[maxm],lb[maxm];
int choice[4*maxm];
int bound[4*maxm];
struct{int*ptr;int val;}assigned[8*maxm*maxm];
int undo_ptr;
int save[4*maxm];
int hint[4*maxm];
int verbose= 0;
int record= 0;

/*7:*/
#line 141 "./strongchain.w"

int lookup(int x)
{
register int k;
if(x<=2)return 1;
for(k= L[x];x> ub[k];k++);
return x==ub[k]&&x==lb[k];
}

/*:7*//*9:*/
#line 172 "./strongchain.w"

int impossible(int x)
{
register int k;
if(x<=2)return 0;
for(k= L[x];x> ub[k];k++);
return x<lb[k];
}

/*:9*//*12:*/
#line 205 "./strongchain.w"

place(int x,int k)
{
register int j= k,y= x;
if(ub[j]==y&&lb[j]==y)return;
while(ub[j]> y){
assign(&ub[j],y);
j--,y--;
}
j= k+1,y= x+x;
while(ub[j]> y){
assign(&ub[j],y);
j++,y+= y;
}
j= k,y= x;
while(lb[j]<y){
assign(&lb[j],y);
j--,y= (y+1)>>1;
}
j= k+1,y= x+1;
while(lb[j]<y){
assign(&lb[j],y);
j++,y++;
}
}

/*:12*//*13:*/
#line 236 "./strongchain.w"

int choice_lookup(int x)
{
register int k;
if(x<=2)return 0;
for(k= L[x];x> ub[k];k++);
return k;
}

/*:13*//*20:*/
#line 320 "./strongchain.w"

int lg(int n)
{
register int m,x;
for(x= n>>1,m= 0;x;m++)x>>= 1;
return m;
}

/*:20*/
#line 33 "./strongchain.w"


main()
{
register int a,k,l,m,n,t,work;
int special= 0,ap,app;
ub[0]= lb[0]= 1;
n= 1;m= 0;work= 0;
while(1){
L[n]= m;
printf("L(%d)=%d:",n,m);
if(m> record){
record= m;printf("*");
}
if(special)/*21:*/
#line 328 "./strongchain.w"

{
if(special==1)printf(" Binary method");
else printf(" Factor method %d x %d",special,n/special);
special= 0;
}

/*:21*/
#line 47 "./strongchain.w"

else for(k= 0;k<=m;k++)printf(" %d",ub[k]);
printf(" [%d]\n",work);
n++;work= 0;
/*2:*/
#line 80 "./strongchain.w"

/*3:*/
#line 92 "./strongchain.w"

for(k= (n-1)>>1,m= 1;k;m++)k>>= 1;

/*:3*/
#line 81 "./strongchain.w"
;
while(1){
/*18:*/
#line 293 "./strongchain.w"

{
for(k= 2,a= n/k;k<=a;k++,a= n/k)
if(n%k==0&&m==L[k]+L[a]){
special= k;goto found;
}
/*19:*/
#line 317 "./strongchain.w"

if(m==lg(n)+lg((n+n)/3))special= 1;

/*:19*/
#line 299 "./strongchain.w"
;
}

/*:18*/
#line 83 "./strongchain.w"
;
/*4:*/
#line 99 "./strongchain.w"

ub[m]= lb[m]= n;
for(k= m-1;k;k--){
lb[k]= (lb[k+1]+1)>>1;
if(lb[k]<=k)lb[k]= k+1;
}
for(k= 1;k<m;k++){
ub[k]= ub[k-1]<<1;
if(ub[k]> n-(m-k))ub[k]= n-(m-k);
}
l= 0;t= m+1;
for(k= t;k<=record;k++)lb[k]= ub[k]= maxn;
undo_ptr= 0;

/*:4*/
#line 84 "./strongchain.w"
;
/*5:*/
#line 117 "./strongchain.w"

start_level:work++;
if(verbose)/*6:*/
#line 124 "./strongchain.w"

{
printf("Entering level %d:\n",l);
for(k= 1;k<t;k++)printf(" %d..%d",lb[k],ub[k]);
printf(" |");
for(;k<=m;k++)printf(" %d..%d",lb[k],ub[k]);
printf("\n");
for(k= 0;k<l;k++)
printf("%c%d..%d",(k&3?',':' '),choice[k],bound[k]);
printf("\n");
}

/*:6*/
#line 119 "./strongchain.w"
;
if(l&3)/*14:*/
#line 251 "./strongchain.w"

{
a= hint[l];
save[l]= undo_ptr;
k= choice[l]= choice_lookup(a);
if(k==0||(a==ub[k]&&a==lb[k])){
bound[l]= 0;goto advance;
}else{
while(a>=lb[k])k++;
bound[l]= k-1;
}
goto next_place;
}

/*:14*/
#line 120 "./strongchain.w"

else/*8:*/
#line 152 "./strongchain.w"

save[l]= t;
decr_t:t--;
if(t<=2)goto found;
if(ub[t]> lb[t])goto restore_t_and_backup;
a= ub[t];
for(k= t-1;;k--)if(ub[k]==lb[k]){
ap= ub[k],app= a-ap;
if(app> ap)break;
if(lookup(app)&&lookup(ap-app))goto decr_t;
}
choice[l]= (a+1)>>1;
bound[l]= a-1;
vet_it:/*10:*/
#line 185 "./strongchain.w"

ap= choice[l];app= a-ap;
if(impossible(ap)||impossible(app)||impossible(ap-app))
goto next_choice;
hint[l+1]= ap;hint[l+2]= app;hint[l+3]= ap-app;

/*:10*/
#line 166 "./strongchain.w"
;
advance:l++;goto start_level;

/*:8*/
#line 121 "./strongchain.w"
;
/*15:*/
#line 265 "./strongchain.w"

unplace:if(!bound[l])goto backup;
while(undo_ptr> save[l]){
--undo_ptr;
*assigned[undo_ptr].ptr= assigned[undo_ptr].val;
}
choice[l]++;
a= hint[l];
next_place:if(choice[l]> bound[l])goto backup;
place(a,choice[l]);
goto advance;

/*:15*//*17:*/
#line 280 "./strongchain.w"

restore_t_and_backup:t= save[l];
backup:if(l==0)goto not_found;
--l;
if(l&3)goto unplace;
a= ub[t];
next_choice:choice[l]++;
if(choice[l]<=bound[l])goto vet_it;
goto restore_t_and_backup;

/*:17*/
#line 122 "./strongchain.w"
;

/*:5*/
#line 85 "./strongchain.w"
;
not_found:m++;
}
found:

/*:2*/
#line 51 "./strongchain.w"
;
}
}

/*:1*/
