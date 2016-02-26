#define big 25
#define small 5
#define total_req (big+small) 
#define eps (argc> 2) 
#define debug (argc> 3) 
#define verbose (argc> 4)  \

#define init_pts 10 \

#define pack(a,b,c,d) (a<<24) +(b<<16) +(c<<8) +d \

#define cos36 80.9017
#define cos72 30.9017
#define sin36 58.7785
#define sin72 95.1057 \

/*1:*/
#line 57 "decagon.w"

#include <stdio.h> 

/*2:*/
#line 90 "decagon.w"

typedef struct node_struct{
struct node_struct*next,*prev;
int s;
int t;
int dir;
}node;

/*:2*/
#line 60 "decagon.w"

/*4:*/
#line 121 "decagon.w"

node*avail;
node*next_node;
node*bad_node;

/*:4*//*5:*/
#line 128 "decagon.w"

int triang[6][9]= {
{2,1,1,1,1,1,2,1,0},
{1,1,1,2,1,0,2,1,1},
{2,1,0,2,1,1,1,1,1},
{1,0,1,3,0,1,1,1,0},
{3,0,1,1,1,0,1,0,1},
{1,1,0,1,0,1,3,0,1}};

/*:5*//*6:*/
#line 142 "decagon.w"

node*poly[total_req];
int top;

/*:6*//*8:*/
#line 197 "decagon.w"

unsigned int x[init_pts+2*total_req];
unsigned int delta_s[10]= {
pack(0,0,0,1),pack(0,0,1,0),pack(0,1,0,0),pack(1,0,0,0),pack(1,-1,1,-1),
pack(0,0,0,-1),pack(0,0,-1,0),pack(0,-1,0,0),pack(-1,0,0,0),pack(-1,1,-1,1)};
unsigned int delta_t[10]= {
pack(-1,1,0,0),pack(0,1,-1,1),pack(1,-1,1,0),pack(0,0,1,-1),pack(0,1,-1,0),
pack(1,-1,0,0),pack(0,-1,1,-1),pack(-1,1,-1,0),pack(0,0,-1,1),pack(0,-1,1,0)};

/*:8*//*11:*/
#line 226 "decagon.w"

int thresh1[6];
int thresh3[6];

/*:11*//*13:*/
#line 274 "decagon.w"

node*bhoice[total_req];
int way[total_req];
node*choice[total_req];
node*save[total_req];
int lb[total_req],ub[total_req];
int ht[total_req];

/*:13*//*33:*/
#line 612 "decagon.w"

char buffer[100];
FILE*eps_file;
int bbxlo,bbylo,bbxhi,bbyhi;

/*:33*//*36:*/
#line 643 "decagon.w"

float cos[]= {100.0,cos36,cos72,-cos72};
float sin[]= {0.0,sin36,sin72,sin72};

/*:36*/
#line 61 "decagon.w"

/*3:*/
#line 100 "decagon.w"

node*get_avail()
{
register node*p;
if(avail){
p= avail;
avail= p->next;
}
else if(next_node==bad_node){
printf("ALLOCATING...\n");
p= (node*)calloc(1000,sizeof(node));
if(p==NULL){
printf("Out of memory!\n");exit(-1);
}
next_node= p+1;
bad_node= p+1000;
}
else p= next_node++;
return p;
}

/*:3*//*35:*/
#line 625 "decagon.w"

print_coord(j)
int j;
{
register float xx,yy;
register int k;
register unsigned b;
for(xx= yy= 0.0,k= 0,b= x[j];k<4;k++,b>>= 8){
xx+= ((int)(b&0xff)-128)*cos[k];yy+= ((int)(b&0xff)-128)*sin[k];
}
fprintf(eps_file," %d %d",(int)xx,(int)yy);
}

/*:35*//*38:*/
#line 664 "decagon.w"

temp1()
{
printf("");
}
temp2()
{
printf("");
}/*:38*/
#line 62 "decagon.w"


main(argc,argv)
int argc;char*argv[];
{
int i,j,k;
register int l;
int vert= 0;
int count= 0,interval= 1,eps_interval= 1,big_need= big,small_need= small;
register node*p,*q,*pp,*qq,*r,*rr;
if(argc> 1){
sscanf(argv[1],"%d",&interval);
if(eps)sscanf(argv[2],"%d",&eps_interval);
}
/*7:*/
#line 153 "decagon.w"

p= get_avail();
poly[0]= p;
for(j= 0;j<init_pts;j++){
q= get_avail();
p->s= 4;p->t= vert++;p->dir= j;p->next= q;q->prev= p;
p= (j<init_pts-1?get_avail():poly[0]);
q->s= 1;q->t= 1;q->next= p;p->prev= q;
}

/*:7*//*9:*/
#line 206 "decagon.w"

x[0]= pack(128,128,128,128);
for(j= 1,p= poly[0];j<init_pts;j++,p= p->next->next)
x[j]= x[j-1]+p->next->s*delta_s[p->dir]+p->next->t*delta_t[p->dir];

/*:9*//*10:*/
#line 220 "decagon.w"

for(j= 0;j<6;j++){
thresh1[j]= 13*triang[j][1]+8*triang[j][2];
thresh3[j]= 13*triang[j][7]+8*triang[j][8];
}

/*:10*//*37:*/
#line 647 "decagon.w"

{
float xx,yy;
unsigned b;
bbxlo= bbylo= 100000;bbxhi= bbyhi= -100000;
for(j= 0;j<init_pts;j++){
for(xx= yy= 0.0,k= 0,b= x[j];k<4;k++,b>>= 8){
xx+= ((int)(b&0xff)-128)*cos[k];yy+= ((int)(b&0xff)-128)*sin[k];
}
if((int)xx<bbxlo)bbxlo= (int)xx;
if((int)yy<bbylo)bbylo= (int)yy;
if((int)xx> bbxhi)bbxhi= (int)xx;
if((int)yy> bbyhi)bbyhi= (int)yy;
}
}

/*:37*/
#line 76 "decagon.w"
;
/*12:*/
#line 237 "decagon.w"

l= 0;
newlev:if(l==total_req){
if(top<0)/*30:*/
#line 579 "decagon.w"

{
count++;
if(count%interval==0){
printf("%d:",count);
for(j= 0;j<l;j++)printf(" %d-%d",choice[j]->t,way[j]);
printf("\n");
}
if(eps&&count%eps_interval==0)/*31:*/
#line 592 "decagon.w"

{
/*32:*/
#line 602 "decagon.w"

sprintf(buffer,"%s.%d",argv[0],count);
eps_file= fopen(buffer,"w");
if(!eps_file){
printf("Can't open file %s!\n",buffer);exit(-4);
}
fprintf(eps_file,"%%!\n%%%%BoundingBox: %d %d %d %d\n",
bbxlo-1,bbylo-1,bbxhi+1,bbyhi+1);
fprintf(eps_file,"/t { moveto lineto lineto closepath stroke } bind def\n");

/*:32*/
#line 594 "decagon.w"
;
for(j= 0;j<l;j++)/*34:*/
#line 617 "decagon.w"

{
print_coord(choice[j]->t);
print_coord(init_pts+j+j);
print_coord(init_pts+1+j+j);
fprintf(eps_file," t\n");
}

/*:34*/
#line 595 "decagon.w"
;
fclose(eps_file);
}

/*:31*/
#line 587 "decagon.w"
;
}

/*:30*/
#line 240 "decagon.w"
;
goto backup;
}
ht[l]= top;
lb[l]= (big_need==0?3:0);
ub[l]= (small_need==0?3:6);
/*14:*/
#line 289 "decagon.w"

for(p= poly[top],rr= p->prev->prev,i= 10000000;;rr= p,p= r){
q= p->next;r= q->next;
if(p->s<5){
for(j= lb[l],k= 0;j<ub[l];j++)
if(p->s>=triang[j][0]&&
(r->s> 5||(13*(q->s)+8*(q->t))>=thresh3[j])&&
(p->s> triang[j][0]||rr->s> 5
||13*p->prev->s+8*p->prev->t>=thresh1[j]))k++;
if(k<i)i= k,pp= p;
}
if(r==poly[top])break;
}
choice[l]= pp;

/*:14*/
#line 246 "decagon.w"
;
way[l]= lb[l];
tryit:j= way[l];
p= choice[l];
if(p->s<triang[j][0])goto nogood;
q= p->next;r= q->next;
if(r->s<5){
if(13*q->s+8*q->t<thresh3[j])goto nogood;
if(13*q->s+8*q->t==thresh3[j]&&r->s<triang[j][6])goto nogood;
}
if(p->s==triang[j][0]&&p->prev->prev->s<5){
if(13*p->prev->s+8*p->prev->t<thresh1[j])
goto nogood;
if(13*p->prev->s+8*p->prev->t==thresh1[j]
&&p->prev->prev->s<triang[j][3])goto nogood;
}
/*15:*/
#line 304 "decagon.w"

/*16:*/
#line 315 "decagon.w"

save[l]= poly[top];rr= get_avail();
for(pp= rr,p= choice[l];;p= p->next){
pp->s= p->s;pp->t= p->t;pp->dir= p->dir;
qq= get_avail();pp->next= qq;qq->prev= pp;p= p->next;
qq->s= p->s;qq->t= p->t;
if(p->next==choice[l])break;
pp= get_avail();qq->next= pp;pp->prev= qq;
}
qq->next= rr;rr->prev= qq;

/*:16*/
#line 305 "decagon.w"
;
/*17:*/
#line 326 "decagon.w"

pp= get_avail();pp->t= vert;
qq= get_avail();qq->t= vert+1;
r= get_avail();r->s= triang[j][4];r->t= triang[j][5];
pp->next= r;r->prev= pp;r->next= qq;qq->prev= r;
k= (rr->dir+triang[j][0]+100)%10;
x[vert]= x[rr->t]+triang[j][1]*delta_s[k]+triang[j][2]*delta_t[k];
k= (k+triang[j][3]+5)%10;pp->dir= k;pp->s= 10-triang[j][3];
x[vert+1]= x[vert]+triang[j][4]*delta_s[k]+triang[j][5]*delta_t[k];

/*:17*/
#line 306 "decagon.w"
;
/*18:*/
#line 356 "decagon.w"

q= rr->next;p= q->next;
k= 13*q->s+8*q->t;
if(k==thresh3[j])/*19:*/
#line 383 "decagon.w"

{
r->next= p;p->prev= r;
q->next= qq;qq->next= avail;avail= q;
p->s-= triang[j][6];
}

/*:19*/
#line 359 "decagon.w"

else{
if(k> thresh3[j]){
q->s-= triang[j][7];q->t-= triang[j][8];
qq->s= 5-triang[j][6];
}else{
p->s-= 5;
q->s= triang[j][7]-q->s;q->t= triang[j][8]-q->t;
qq->s= 10-triang[j][6];
}
qq->next= q;q->prev= qq;qq->dir= pp->dir+5-qq->s;
for(p= p->next->next;p!=rr;p= p->next->next)
if(x[p->t]==x[vert+1]){
/*20:*/
#line 403 "decagon.w"

qq->prev= p->prev;p->prev->next= qq;
k= qq->s+p->s-10;
qq->s= (p->prev->prev->dir+105-qq->dir)%10;
p->prev= r;r->next= p;
p->s= k-qq->s;
k= qq->s;
if(k==0||k==5){
qq= qq->prev;
if(k==5)qq->s+= q->s,qq->t+= q->t;
else if(13*(qq->s-q->s)+8*(qq->t-q->t)<0)
qq->s= q->s-qq->s,qq->t= q->t-qq->t,qq->prev->s-= 5,qq->prev->dir+= 5;
else qq->s-= q->s,qq->t-= q->t,q->next->s-= 5;
qq->next= q->next;q->next->prev= qq;
qq= q->next;q->next= avail;avail= q->prev;
}
poly[top++]= qq;

/*:20*/
#line 372 "decagon.w"
;
break;
}
}
/*21:*/
#line 425 "decagon.w"

if(p->s==0||p->s==5){
q= p->next;
if(p->s==5)r->s+= q->s,r->t+= q->t;
else if(13*(r->s-q->s)+8*(r->t-q->t)<=0)
r->s= q->s-r->s,r->t= q->t-r->t,pp->s-= 5,pp->dir+= 5;
else r->s-= q->s,r->t-= q->t,q->next->s-= 5;
r->next= q->next;q->next->prev= r;
q->next= avail;avail= p;
}

/*:21*/
#line 376 "decagon.w"
;

/*:18*/
#line 307 "decagon.w"
;
/*22:*/
#line 446 "decagon.w"

if(rr->s==triang[j][0]){
q= rr->prev;p= q->prev;
k= 13*q->s+8*q->t;
if(k==thresh1[j]){
if(p==pp->next->next){
rr->next= avail;avail= pp;top--;goto insert_done;
}
/*23:*/
#line 476 "decagon.w"

p->next= pp->next;
pp->next->prev= p;
p->s-= 10-pp->s;p->dir= pp->dir;
pp->next= avail;rr->next= pp;avail= q;

/*:23*/
#line 454 "decagon.w"
;
goto insert_almost_done;
}else/*24:*/
#line 482 "decagon.w"

{
if(k> thresh1[j]){
q->s-= triang[j][1];q->t-= triang[j][2];pp->s-= 5;
}else{
p->s-= 5;
p->dir+= 5;
q->s= triang[j][1]-q->s;q->t= triang[j][2]-q->t;
}
q->next= pp;pp->prev= q;
rr->next= avail;avail= rr;
}

/*:24*/
#line 456 "decagon.w"
;
}else{
rr->s-= triang[j][0];rr->dir+= triang[j][0];
q= get_avail();q->prev= rr;rr->next= q;
q->s= triang[j][1];q->t= triang[j][2];
q->next= pp;pp->prev= q;
p= rr;
}
for(p= p->prev->prev;p!=pp;p= p->prev->prev)
if(x[p->t]==x[vert]){
/*25:*/
#line 495 "decagon.w"

qq= pp->next;
if(qq->next==p){
p->s+= pp->s-5;
p->prev= pp->prev;pp->prev->next= p;
qq->next= avail;avail= pp;
}else{
q= p->next;
p->next= qq;qq->prev= p;
pp->next= q;q->prev= pp;
k= pp->dir;pp->dir= p->dir;p->dir= k;
k= p->s+pp->s-10;
pp->s= (pp->prev->prev->dir+105-pp->dir)%10;
p->s= k-pp->s;
k= pp->s;
if(k==0||k==5){
pp= pp->prev;
if(k==5)pp->s+= q->s,pp->t+= q->t;
else if(13*(pp->s-q->s)+8*(pp->t-q->t)<0)
pp->s= q->s-pp->s,pp->t= q->t-pp->t,pp->prev->s-= 5,pp->prev->dir+= 5;
else pp->s-= q->s,pp->t-= q->t,q->next->s-= 5;
pp->next= q->next;q->next->prev= pp;
pp= q->next;q->next= avail;avail= q->prev;
}
poly[top++]= pp;
}

/*:25*/
#line 466 "decagon.w"
;
break;
}
insert_almost_done:
/*26:*/
#line 528 "decagon.w"

if(p->s==0||p->s==5){
q= p->next;r= p->prev;
if(p->s==5)r->s+= q->s,r->t+= q->t;
else if(13*(r->s-q->s)+8*(r->t-q->t)<=0)
r->s= q->s-r->s,r->t= q->t-r->t,r->prev->s-= 5,r->prev->dir+= 5;
else r->s-= q->s,r->t-= q->t,q->next->s-= 5;
r->next= q->next;q->next->prev= r;
q->next= avail;avail= p;
p= r->next;
}

/*:26*/
#line 470 "decagon.w"
;
poly[top]= p;
insert_done:

/*:22*/
#line 308 "decagon.w"
;

/*:15*/
#line 262 "decagon.w"
;
if(debug)/*27:*/
#line 540 "decagon.w"

{
int badsums= 0,negangle= 0;
if(verbose)
printf("Level %d: vertex %d with triangle %d\n",l,choice[l]->t,way[l]);
for(j= ht[l];j<=top;j++){
for(p= poly[j],k= p->prev->prev->dir,i= 0;;){
q= p->next;
if(q->prev!=p)
printf(" badlink!");
if(verbose)printf(" %d(%d)",p->t,p->s);
if(p->s==0||p->s==5)
printf(" badangle!");
if(p->s<0&&j==top)negangle= 1;
if((k+105-p->s-p->dir)%10!=0)
printf("baddir!");
i+= 5-p->s;k= p->dir;
p= q->next;
if(p->prev!=q)
printf(" badlink!");
if(verbose)printf(" %d,%d",q->s,q->t);
if(p==poly[j])break;
}
if(i!=10)badsums++;
if(verbose)printf("\n");
}
if(badsums&&!negangle)
printf(" badsum!\n");
}

/*:27*/
#line 263 "decagon.w"
;
if(way[l]<3)big_need--;else small_need--;
l++;vert+= 2;goto newlev;
nogood:if(++way[l]<ub[l])goto tryit;
backup:if(l==0)goto done;
l--;vert-= 2;
if(way[l]<3)big_need++;else small_need++;
/*28:*/
#line 570 "decagon.w"

for(j= top;j>=ht[l];j--){
poly[j]->prev->next= avail;avail= poly[j];
}
top= ht[l];poly[top]= save[l];

/*:28*/
#line 270 "decagon.w"
;
goto nogood;
done:

/*:12*/
#line 77 "decagon.w"
;
printf("Altogether %d solutions.\n",count);
}

/*:1*/
