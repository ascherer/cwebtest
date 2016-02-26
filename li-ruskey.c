#define maxn 100
#define abort(f,d,n) {fprintf(stderr,f,d) ;exit(-n) ;} \

#define head (vertex+1+n)  \

#define principal(l) (jj[ab[l]->id]==k)  \

/*1:*/
#line 23 "li-ruskey.w"

#include <stdio.h> 
/*6:*/
#line 166 "li-ruskey.w"

typedef struct info_struct{
char id;
/*12:*/
#line 271 "li-ruskey.w"

char alfprime;
char del;

/*:12*/
#line 169 "li-ruskey.w"
;
struct info_struct*sib;
struct info_struct*ref;
}info;

/*:6*//*29:*/
#line 673 "li-ruskey.w"

typedef struct fnode_struct{
char bit;

struct fnode_struct*left,*right;
struct fnode_struct**fixup;
struct fnode_struct*focus;
struct fnode_struct**tau[2];
}fnode;

/*:29*//*55:*/
#line 1038 "li-ruskey.w"

typedef struct{
fnode*vert;
fnode**ref;
}fstack_node;

/*:55*/
#line 25 "li-ruskey.w"

/*4:*/
#line 115 "li-ruskey.w"

char jj[maxn];
char js[maxn];
char stack[maxn];

/*:4*//*8:*/
#line 183 "li-ruskey.w"

info infonode[((maxn*maxn)>>2)+(maxn>>1)];
info*list[maxn][2];

/*:8*//*14:*/
#line 301 "li-ruskey.w"

int nn[maxn];

/*:14*//*24:*/
#line 545 "li-ruskey.w"

char fj[maxn][2];

/*:24*//*30:*/
#line 690 "li-ruskey.w"

fnode vertex[maxn+1];

/*:30*//*32:*/
#line 711 "li-ruskey.w"

fnode*tau_table[((maxn*maxn)>>1)+maxn+maxn];
fnode**tau_ptr;
char ct[maxn];
char verbose= 1;
info*ab[maxn];

/*:32*//*37:*/
#line 816 "li-ruskey.w"

char rbuf[8];

/*:37*//*47:*/
#line 959 "li-ruskey.w"

char been_there_and_done_that;

/*:47*//*57:*/
#line 1054 "li-ruskey.w"

fstack_node fstack[maxn];

/*:57*//*60:*/
#line 1095 "li-ruskey.w"

fnode*cur_vert;

/*:60*/
#line 26 "li-ruskey.w"

/*15:*/
#line 306 "li-ruskey.w"

void print_info(int k)
{
register info*i;
printf("Info for vertex %d: A =",k);
for(i= list[k][0];i;i= i->sib)printf(" %d",i->id);
if(list[k][0])printf(", B =");else printf(" (), B =");
for(i= list[k][1];i;i= i->sib)printf(" %d",i->id);
if(list[k][1])printf("\n");else printf(" ()\n");
for(i= list[k][0];i;i= i->sib)
printf(" alf%d=%d, alf%d'=%d\n",
i->id,i->alfprime^i->del,i->id,i->alfprime);
for(i= list[k][1];i;i= i->sib)
printf(" bet%d=%d, bet%d'=%d\n",
i->id,i->alfprime^i->del,i->id,i->alfprime);
}

void print_all_info(int n)
{
register int k;
for(k= 0;k<=n;k++)print_info(k);
}

/*:15*//*58:*/
#line 1067 "li-ruskey.w"

void setbits(char k)
{
register info*i;
vertex[k].bit= 1;
for(i= list[k][0];i;i= i->sib)
if(jj[i->id]==k)setbits(i->id);
}

/*:58*//*59:*/
#line 1084 "li-ruskey.w"

void entourage(char k,char t)
{
register fnode*p= vertex+k;
register info*i;
if(t)setbits(k);
for(i= list[k][t];i;i= i->sib)
entourage(i->id,i->alfprime^i->del);
cur_vert->left= p,p->right= cur_vert,cur_vert= p;
}

/*:59*/
#line 27 "li-ruskey.w"

int main(int argc,char*argv[])
{
/*5:*/
#line 120 "li-ruskey.w"

register int j,k,l= 0;
int n;

/*:5*//*9:*/
#line 187 "li-ruskey.w"

info*i,*ii;

/*:9*//*45:*/
#line 936 "li-ruskey.w"

fnode*p,*q,*r;

/*:45*/
#line 30 "li-ruskey.w"
;
/*2:*/
#line 83 "li-ruskey.w"

{
register char*c= argv[1];
if(argc!=2)abort("Usage: %s graphspec\n",argv[0],1);
for(k= 1;*c;k++){
if(k>=maxn)abort("Sorry, I can only handle %d vertices!\n",maxn,2);
if(*c=='+')js[k]= 0;
else if(*c=='-')js[k]= 1;
else abort("Parsing error: `%s' should start with + or -!\n",c,3);
for(j= 0,c++;*c>='0'&&*c<='9';c++)j= 10*j+*c-'0';
/*3:*/
#line 102 "li-ruskey.w"

if(j>=k){
fprintf(stderr,"Parsing error: `%d%s' should start ",j,c);
abort("with a number less than %d!\n",k,4);
}
while(j<stack[l])l--;
if(j!=stack[l]){
fprintf(stderr,"Parsing error: `%d%s' shouldn't start ",j,c);
fprintf(stderr,"with a number between %d ",stack[l]);
abort("and %d!\n",stack[l+1],5);
}
stack[++l]= k;

/*:3*/
#line 93 "li-ruskey.w"
;
jj[k]= j;
}
n= k-1;
}

/*:2*/
#line 31 "li-ruskey.w"
;
/*7:*/
#line 174 "li-ruskey.w"

list[0][0]= list[0][1]= NULL;
for(k= 1,i= &infonode[0];k<=n;k++,i++)
for(j= jj[k],ii= NULL;;ii= i++,j= jj[j]){
i->id= k,i->ref= ii;
i->sib= list[j][js[k]],list[j][js[k]]= i;
if(j==0||js[j]==js[k])break;
}

/*:7*//*13:*/
#line 279 "li-ruskey.w"

for(k= n;k>=0;k--){
register int s,t;
for(s= 1,i= list[k][0],ii= NULL;i;s*= nn[i->id],i= i->sib){
i->del= 1;
if((nn[i->id]&1)==0)ii= i;
if(i->ref)i->alfprime= i->ref->alfprime^i->ref->del;
else i->alfprime= 1;
}
for(i= list[k][0];ii;i= i->sib)
if(i==ii)ii= NULL;else i->del= 0;
for(t= 1,i= list[k][1],ii= NULL;i;t*= nn[i->id],i= i->sib){
i->del= 1;
if((nn[i->id]&1)==0)ii= i;
if(i->ref)i->alfprime= i->ref->alfprime^i->ref->del;
else i->alfprime= 0;
}
for(i= list[k][1];ii;i= i->sib)
if(i==ii)ii= NULL;else i->del= 0;
nn[k]= s+t;
}

/*:13*//*16:*/
#line 329 "li-ruskey.w"

if(verbose){
print_all_info(n);
printf("(Altogether %d solutions.)\n",nn[0]);
}

/*:16*//*23:*/
#line 538 "li-ruskey.w"

for(k= n;k>=0;k--)for(j= 0;j<=1;j++){
i= list[k][j];
if(i)fj[k][j]= fj[i->id][i->alfprime^i->del];
else fj[k][j]= k;
}

/*:23*//*31:*/
#line 704 "li-ruskey.w"

tau_ptr= tau_table;
for(k= 0;k<=n;k++){
/*33:*/
#line 738 "li-ruskey.w"

for(i= list[k][0],ii= list[k][1],l= 0;i||ii;l++){
if(!ii||(i&&i->id> ii->id))ab[l]= i,ct[l]= 0,i= i->sib;
else ab[l]= ii,ct[l]= 1,ii= ii->sib;
}
if(l&&verbose){
printf("Union%d: (",k);
for(j= l-1;;){
printf("%c%d",'a'+ct[j],ab[j]->id);
if(j==0)break;
j--;
if(principal(j))printf(")(");
}
printf(")\n");
}

/*:33*/
#line 707 "li-ruskey.w"
;
/*34:*/
#line 754 "li-ruskey.w"

if(!l)vertex[k].tau[0]= vertex[k].tau[1]= NULL;
else{
ab[l]= NULL;
vertex[k].tau[1]= tau_ptr;
/*35:*/
#line 794 "li-ruskey.w"

/*36:*/
#line 800 "li-ruskey.w"

for(l= 0;!principal(l);l++);
if(ct[l]){
for(j= l+1;ab[j]&&ct[j];j++);
if(ab[j]){
if(verbose)sprintf(rbuf,"%dr+",ab[j]->id);
*tau_ptr++= vertex+fj[ab[j]->id][ab[j]->alfprime];
}else{
if(verbose)sprintf(rbuf,"%d+",k);
*tau_ptr++= vertex+k;
}
}else{
if(verbose)sprintf(rbuf,"%d+",ab[l]->id);
*tau_ptr++= vertex+ab[l]->id;
}

/*:36*/
#line 795 "li-ruskey.w"
;
/*38:*/
#line 821 "li-ruskey.w"

while(ab[l]&&!ct[l]&&principal(l))l++;
if(!ab[l]){
if(verbose)printf(" %d:%s\n",k,rbuf);
*tau_ptr++= vertex+k;
}else if(principal(l)){
if(verbose)printf(" %d:%s",ab[l]->id,rbuf);
*tau_ptr++= vertex+ab[l]->id;
}else{
if(verbose)printf(" %dr:%s",ab[l]->id,rbuf);
*tau_ptr++= vertex+fj[ab[l]->id][ab[l]->alfprime];
}

/*:38*/
#line 796 "li-ruskey.w"
;
while(ab[l])/*39:*/
#line 839 "li-ruskey.w"

{
if(principal(l))
for(l++;ab[l]&&ct[l]&&principal(l);l++);
else for(l++;!principal(l);l++);
*tau_ptr++= vertex+ab[l-1]->id;
if(ab[l]&&!ct[l]&&principal(l))
for(l++;ab[l]&&!ct[l]&&principal(l);l++);
if(!ab[l]){
if(verbose)printf(" %d:%d\n",k,*(tau_ptr-1)-vertex);
*tau_ptr++= vertex+k;
}else if(principal(l)){
if(verbose)printf(" %d:%d",ab[l]->id,*(tau_ptr-1)-vertex);
*tau_ptr++= vertex+ab[l]->id;
}else{
if(verbose)printf(" %dr:%d",ab[l]->id,*(tau_ptr-1)-vertex);
*tau_ptr++= vertex+fj[ab[l]->id][ab[l]->alfprime];
}
}

/*:39*/
#line 797 "li-ruskey.w"
;
tau_ptr++;

/*:35*/
#line 759 "li-ruskey.w"
;
vertex[k].tau[0]= tau_ptr;
/*40:*/
#line 865 "li-ruskey.w"

/*41:*/
#line 871 "li-ruskey.w"

for(l= 0;!principal(l);l++);
if(!ct[l]){
for(j= l+1;ab[j]&&!ct[j];j++);
if(ab[j]){
if(verbose)sprintf(rbuf,"%dr+",ab[j]->id);
*tau_ptr++= vertex+fj[ab[j]->id][ab[j]->alfprime];
}else{
if(verbose)sprintf(rbuf,"%d+",k);
*tau_ptr++= vertex+k;
}
}else{
if(verbose)sprintf(rbuf,"%d+",ab[l]->id);
*tau_ptr++= vertex+ab[l]->id;
}

/*:41*/
#line 866 "li-ruskey.w"
;
/*42:*/
#line 887 "li-ruskey.w"

while(ab[l]&&ct[l]&&principal(l))l++;
if(!ab[l]){
if(verbose)printf(" %d:%s\n",k,rbuf);
*tau_ptr++= vertex+k;
}else if(principal(l)){
if(verbose)printf(" %d:%s",ab[l]->id,rbuf);
*tau_ptr++= vertex+ab[l]->id;
}else{
if(verbose)printf(" %dr:%s",ab[l]->id,rbuf);
*tau_ptr++= vertex+fj[ab[l]->id][ab[l]->alfprime];
}

/*:42*/
#line 867 "li-ruskey.w"
;
while(ab[l])/*43:*/
#line 905 "li-ruskey.w"

{
if(principal(l))
for(l++;ab[l]&&!ct[l]&&principal(l);l++);
else for(l++;!principal(l);l++);
*tau_ptr++= vertex+ab[l-1]->id;
if(ab[l]&&ct[l]&&principal(l))
for(l++;ab[l]&&ct[l]&&principal(l);l++);
if(!ab[l]){
if(verbose)printf(" %d:%d\n",k,*(tau_ptr-1)-vertex);
*tau_ptr++= vertex+k;
}else if(principal(l)){
if(verbose)printf(" %d:%d",ab[l]->id,*(tau_ptr-1)-vertex);
*tau_ptr++= vertex+ab[l]->id;
}else{
if(verbose)printf(" %dr:%d",ab[l]->id,*(tau_ptr-1)-vertex);
*tau_ptr++= vertex+fj[ab[l]->id][ab[l]->alfprime];
}
}

/*:43*/
#line 868 "li-ruskey.w"
;
tau_ptr++;

/*:40*/
#line 761 "li-ruskey.w"
;
/*44:*/
#line 929 "li-ruskey.w"

for(l= 0,p= NULL;ab[l];l++)if(principal(l)){
q= vertex+ab[l]->id;
if(p)q->right= p,p->left= q;
p= q;
}

/*:44*/
#line 762 "li-ruskey.w"
;
}

/*:34*/
#line 708 "li-ruskey.w"
;
}

/*:31*//*61:*/
#line 1100 "li-ruskey.w"

for(k= 0;k<=n+1;k++)vertex[k].focus= &vertex[k];
cur_vert= head;
entourage(0,0);
cur_vert->left= head,head->right= cur_vert;

/*:61*/
#line 32 "li-ruskey.w"
;
/*46:*/
#line 942 "li-ruskey.w"

while(1){
/*53:*/
#line 1011 "li-ruskey.w"

for(k= 0;k<=n;k++)putchar('0'+vertex[k].bit);
if(verbose)/*54:*/
#line 1029 "li-ruskey.w"

for(l= 0,q= head->left,p= q->focus;q!=head;q= r){
printf(" %s%d_%d%s",q!=p?"(":"",q-vertex,q->bit,q!=p?")":"");
/*56:*/
#line 1044 "li-ruskey.w"

if(q->fixup)fstack[++l].vert= q,fstack[l].ref= q->fixup;
if(q==fstack[l].vert){
putchar('!');
r= *(fstack[l].ref);
if(*(fstack[l].ref+1))
fstack[l].vert= *(fstack[l].ref+1),fstack[l].ref+= 2;
else l--;
}else r= q->left;

/*:56*/
#line 1032 "li-ruskey.w"
;
if(q==p)p= r->focus;
}

/*:54*/
#line 1013 "li-ruskey.w"
;
putchar('\n');

/*:53*/
#line 944 "li-ruskey.w"
;
/*48:*/
#line 965 "li-ruskey.w"

q= head->left;
p= q->focus;
q->focus= q;

/*:48*/
#line 946 "li-ruskey.w"
;
if(p!=head){
if(p->fixup)/*50:*/
#line 979 "li-ruskey.w"

{
q= *(p->fixup),r= *(p->fixup+1);
p->left= q,q->right= p;
if(r)r->fixup= p->fixup+2;
p->fixup= NULL;
}

/*:50*/
#line 948 "li-ruskey.w"
;
if(p->bit==0)/*51:*/
#line 987 "li-ruskey.w"

{
p->bit= 1;
if(p->tau[1]){
q= (*(p->tau[1]))->right;
r= *(p->tau[1]+1);
q->left= r,r->right= q;
r= *(p->tau[1]+2);
if(r)r->fixup= p->tau[1]+3;
}
}

/*:51*/
#line 949 "li-ruskey.w"

else/*52:*/
#line 999 "li-ruskey.w"

{
p->bit= 0;
if(p->tau[0]){
q= (*(p->tau[0]))->right;
r= *(p->tau[0]+1);
q->left= r,r->right= q;
r= *(p->tau[0]+2);
if(r)r->fixup= p->tau[0]+3;
}
}

/*:52*/
#line 950 "li-ruskey.w"

}else if(been_there_and_done_that)break;
else{
printf("... and now we generate in reverse:\n");
been_there_and_done_that= 1;continue;
}
/*49:*/
#line 974 "li-ruskey.w"

q= p->left;
p->focus= q->focus;
q->focus= q;

/*:49*/
#line 956 "li-ruskey.w"
;
}

/*:46*/
#line 33 "li-ruskey.w"
;
return 0;
}

/*:1*/
