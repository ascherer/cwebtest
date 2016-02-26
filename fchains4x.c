#define footsize 100 \

#define fun(p) ((p) -func)  \

/*1:*/
#line 18 "./fchains4x.w"

#include <stdio.h> 
#include <stdlib.h> 
typedef struct node_struct{
unsigned int footprint[footsize];
int parent;
int cost;
struct node_struct*prev,*next;
}node;
node func[1<<15];
node head[9];
int x[100];
char buf[100];
char name[32*footsize][16];
unsigned int ttt;
main(int argc,char*argv[])
{
register int c,j,k,r,t,m,mm,s;
register unsigned int u;
register node*p,*q,*pp;
/*2:*/
#line 46 "./fchains4x.w"

m= argc+3;
for(k= 1;k<=m;k++){
if(k<=4)x[k]= 0xffff/((1<<(1<<(4-k)))+1);
else if(sscanf(argv[k-4],"%x",&x[k])!=1){
fprintf(stderr,"Parameter %s should have been hexadecimal!\n",argv[k-4]);
exit(-1);
}
if(x[k]> 0xffff){
fprintf(stderr,"Parameter %s is too big!\n",argv[k-4]);
exit(-1);
}
if(x[k]>=0x8000)x[k]^= 0xffff;
}

/*:2*/
#line 38 "./fchains4x.w"
;
/*8:*/
#line 133 "./fchains4x.w"

for(p= &func[2];p<&func[0x8000];p++)
(p-1)->next= p,p->prev= p-1,p->cost= 8;
func[1].cost= 8;
for(k= 0;k<=8;k++)
head[k].parent= -1,head[k].next= head[k].prev= &head[k];
head[0].next= head[0].prev= &func[0];
func[0].next= func[0].prev= &head[0];
head[8].next= &func[1],func[1].prev= &head[8];
head[8].prev= &func[0x7fff],func[0x7fff].next= &head[8];
/*9:*/
#line 146 "./fchains4x.w"

for(k= 1;k<=m;k++){
p= &func[x[k]];
if(p->cost==0)continue;
p->next->prev= p->prev,p->prev->next= p->next;
p->cost= 0;
p->next= head[0].next,p->prev= &head[0];
p->next->prev= p,p->prev->next= p;
}
c= (1<<15)-1-m;

/*:9*/
#line 143 "./fchains4x.w"
;
/*10:*/
#line 157 "./fchains4x.w"

s= 0;
for(r= 2;r<=m;r++)for(k= 1;k<r;k++){
t= x[k]&x[r],sprintf(name[s],"%d&%d(%04x)",k,r,t);/*11:*/
#line 168 "./fchains4x.w"

p= &func[t];
if(p->cost> 1){
if(s>=32*footsize){
fprintf(stderr,"Too many special functions (footsize=%d)!\n",footsize);
exit(-3);
}
p->next->prev= p->prev,p->prev->next= p->next;
p->cost= 1,p->parent= (x[k]<<16)+x[r];
p->footprint[s>>5]= 1<<(s&0x1f);
p->next= head[1].next,p->prev= &head[1];
p->next->prev= p,p->prev->next= p;
s++;
c--;
}

/*:11*/
#line 160 "./fchains4x.w"
;
t= x[k]&(~x[r]),sprintf(name[s],"%d>%d(%04x)",k,r,t);/*11:*/
#line 168 "./fchains4x.w"

p= &func[t];
if(p->cost> 1){
if(s>=32*footsize){
fprintf(stderr,"Too many special functions (footsize=%d)!\n",footsize);
exit(-3);
}
p->next->prev= p->prev,p->prev->next= p->next;
p->cost= 1,p->parent= (x[k]<<16)+x[r];
p->footprint[s>>5]= 1<<(s&0x1f);
p->next= head[1].next,p->prev= &head[1];
p->next->prev= p,p->prev->next= p;
s++;
c--;
}

/*:11*/
#line 161 "./fchains4x.w"
;
t= (~x[k])&x[r],sprintf(name[s],"%d<%d(%04x)",k,r,t);/*11:*/
#line 168 "./fchains4x.w"

p= &func[t];
if(p->cost> 1){
if(s>=32*footsize){
fprintf(stderr,"Too many special functions (footsize=%d)!\n",footsize);
exit(-3);
}
p->next->prev= p->prev,p->prev->next= p->next;
p->cost= 1,p->parent= (x[k]<<16)+x[r];
p->footprint[s>>5]= 1<<(s&0x1f);
p->next= head[1].next,p->prev= &head[1];
p->next->prev= p,p->prev->next= p;
s++;
c--;
}

/*:11*/
#line 162 "./fchains4x.w"
;
t= x[k]|x[r],sprintf(name[s],"%d|%d(%04x)",k,r,t);/*11:*/
#line 168 "./fchains4x.w"

p= &func[t];
if(p->cost> 1){
if(s>=32*footsize){
fprintf(stderr,"Too many special functions (footsize=%d)!\n",footsize);
exit(-3);
}
p->next->prev= p->prev,p->prev->next= p->next;
p->cost= 1,p->parent= (x[k]<<16)+x[r];
p->footprint[s>>5]= 1<<(s&0x1f);
p->next= head[1].next,p->prev= &head[1];
p->next->prev= p,p->prev->next= p;
s++;
c--;
}

/*:11*/
#line 163 "./fchains4x.w"
;
t= x[k]^x[r],sprintf(name[s],"%d^%d(%04x)",k,r,t);/*11:*/
#line 168 "./fchains4x.w"

p= &func[t];
if(p->cost> 1){
if(s>=32*footsize){
fprintf(stderr,"Too many special functions (footsize=%d)!\n",footsize);
exit(-3);
}
p->next->prev= p->prev,p->prev->next= p->next;
p->cost= 1,p->parent= (x[k]<<16)+x[r];
p->footprint[s>>5]= 1<<(s&0x1f);
p->next= head[1].next,p->prev= &head[1];
p->next->prev= p,p->prev->next= p;
s++;
c--;
}

/*:11*/
#line 164 "./fchains4x.w"
;
}
mm= (s+31)/32;

/*:10*/
#line 144 "./fchains4x.w"
;

/*:8*/
#line 39 "./fchains4x.w"
;
for(r= 2;c;r++)
for(k= (r-1)>>1;k>=0;k--)
/*3:*/
#line 61 "./fchains4x.w"

for(p= head[k].next;p->parent>=0;p= p->next)
for(q= head[r-1-k].next;q->parent>=0;q= q->next){
for(j= 0;j<mm;j++)
if(p->footprint[j]&q->footprint[j])
/*6:*/
#line 101 "./fchains4x.w"

{
t= fun(p)&fun(q);
if(func[t].cost>=r-1)/*7:*/
#line 119 "./fchains4x.w"

{
pp= &func[t];
if(pp->cost> r-1){
if(pp->cost==8)c--;
pp->next->prev= pp->prev,pp->prev->next= pp->next;
pp->cost= r-1,pp->parent= (fun(p)<<16)+fun(q);
for(j= 0;j<mm;j++)pp->footprint[j]= 0;
pp->next= head[r-1].next,pp->prev= &head[r-1];
pp->next->prev= pp,pp->prev->next= pp;
}
for(j= 0;j<mm;j++)pp->footprint[j]|= p->footprint[j]&q->footprint[j];
}

/*:7*/
#line 104 "./fchains4x.w"
;
t= fun(p)&(~fun(q));
if(func[t].cost>=r-1)/*7:*/
#line 119 "./fchains4x.w"

{
pp= &func[t];
if(pp->cost> r-1){
if(pp->cost==8)c--;
pp->next->prev= pp->prev,pp->prev->next= pp->next;
pp->cost= r-1,pp->parent= (fun(p)<<16)+fun(q);
for(j= 0;j<mm;j++)pp->footprint[j]= 0;
pp->next= head[r-1].next,pp->prev= &head[r-1];
pp->next->prev= pp,pp->prev->next= pp;
}
for(j= 0;j<mm;j++)pp->footprint[j]|= p->footprint[j]&q->footprint[j];
}

/*:7*/
#line 106 "./fchains4x.w"
;
t= (~fun(p))&fun(q);
if(func[t].cost>=r-1)/*7:*/
#line 119 "./fchains4x.w"

{
pp= &func[t];
if(pp->cost> r-1){
if(pp->cost==8)c--;
pp->next->prev= pp->prev,pp->prev->next= pp->next;
pp->cost= r-1,pp->parent= (fun(p)<<16)+fun(q);
for(j= 0;j<mm;j++)pp->footprint[j]= 0;
pp->next= head[r-1].next,pp->prev= &head[r-1];
pp->next->prev= pp,pp->prev->next= pp;
}
for(j= 0;j<mm;j++)pp->footprint[j]|= p->footprint[j]&q->footprint[j];
}

/*:7*/
#line 108 "./fchains4x.w"
;
t= fun(p)|fun(q);
if(func[t].cost>=r-1)/*7:*/
#line 119 "./fchains4x.w"

{
pp= &func[t];
if(pp->cost> r-1){
if(pp->cost==8)c--;
pp->next->prev= pp->prev,pp->prev->next= pp->next;
pp->cost= r-1,pp->parent= (fun(p)<<16)+fun(q);
for(j= 0;j<mm;j++)pp->footprint[j]= 0;
pp->next= head[r-1].next,pp->prev= &head[r-1];
pp->next->prev= pp,pp->prev->next= pp;
}
for(j= 0;j<mm;j++)pp->footprint[j]|= p->footprint[j]&q->footprint[j];
}

/*:7*/
#line 110 "./fchains4x.w"
;
t= fun(p)^fun(q);
if(func[t].cost>=r-1)/*7:*/
#line 119 "./fchains4x.w"

{
pp= &func[t];
if(pp->cost> r-1){
if(pp->cost==8)c--;
pp->next->prev= pp->prev,pp->prev->next= pp->next;
pp->cost= r-1,pp->parent= (fun(p)<<16)+fun(q);
for(j= 0;j<mm;j++)pp->footprint[j]= 0;
pp->next= head[r-1].next,pp->prev= &head[r-1];
pp->next->prev= pp,pp->prev->next= pp;
}
for(j= 0;j<mm;j++)pp->footprint[j]|= p->footprint[j]&q->footprint[j];
}

/*:7*/
#line 112 "./fchains4x.w"
;
goto pqdone;
}

/*:6*/
#line 66 "./fchains4x.w"

/*4:*/
#line 73 "./fchains4x.w"

{
t= fun(p)&fun(q);
if(func[t].cost>=r)/*5:*/
#line 87 "./fchains4x.w"

{
pp= &func[t];
if(pp->cost> r){
if(pp->cost==8)c--;
pp->next->prev= pp->prev,pp->prev->next= pp->next;
pp->cost= r,pp->parent= (fun(p)<<16)+fun(q);
for(j= 0;j<mm;j++)pp->footprint[j]= 0;
pp->next= head[r].next,pp->prev= &head[r];
pp->next->prev= pp,pp->prev->next= pp;
}
for(j= 0;j<mm;j++)pp->footprint[j]|= p->footprint[j]|q->footprint[j];
}

/*:5*/
#line 76 "./fchains4x.w"
;
t= fun(p)&(~fun(q));
if(func[t].cost>=r)/*5:*/
#line 87 "./fchains4x.w"

{
pp= &func[t];
if(pp->cost> r){
if(pp->cost==8)c--;
pp->next->prev= pp->prev,pp->prev->next= pp->next;
pp->cost= r,pp->parent= (fun(p)<<16)+fun(q);
for(j= 0;j<mm;j++)pp->footprint[j]= 0;
pp->next= head[r].next,pp->prev= &head[r];
pp->next->prev= pp,pp->prev->next= pp;
}
for(j= 0;j<mm;j++)pp->footprint[j]|= p->footprint[j]|q->footprint[j];
}

/*:5*/
#line 78 "./fchains4x.w"
;
t= (~fun(p))&fun(q);
if(func[t].cost>=r)/*5:*/
#line 87 "./fchains4x.w"

{
pp= &func[t];
if(pp->cost> r){
if(pp->cost==8)c--;
pp->next->prev= pp->prev,pp->prev->next= pp->next;
pp->cost= r,pp->parent= (fun(p)<<16)+fun(q);
for(j= 0;j<mm;j++)pp->footprint[j]= 0;
pp->next= head[r].next,pp->prev= &head[r];
pp->next->prev= pp,pp->prev->next= pp;
}
for(j= 0;j<mm;j++)pp->footprint[j]|= p->footprint[j]|q->footprint[j];
}

/*:5*/
#line 80 "./fchains4x.w"
;
t= fun(p)|fun(q);
if(func[t].cost>=r)/*5:*/
#line 87 "./fchains4x.w"

{
pp= &func[t];
if(pp->cost> r){
if(pp->cost==8)c--;
pp->next->prev= pp->prev,pp->prev->next= pp->next;
pp->cost= r,pp->parent= (fun(p)<<16)+fun(q);
for(j= 0;j<mm;j++)pp->footprint[j]= 0;
pp->next= head[r].next,pp->prev= &head[r];
pp->next->prev= pp,pp->prev->next= pp;
}
for(j= 0;j<mm;j++)pp->footprint[j]|= p->footprint[j]|q->footprint[j];
}

/*:5*/
#line 82 "./fchains4x.w"
;
t= fun(p)^fun(q);
if(func[t].cost>=r)/*5:*/
#line 87 "./fchains4x.w"

{
pp= &func[t];
if(pp->cost> r){
if(pp->cost==8)c--;
pp->next->prev= pp->prev,pp->prev->next= pp->next;
pp->cost= r,pp->parent= (fun(p)<<16)+fun(q);
for(j= 0;j<mm;j++)pp->footprint[j]= 0;
pp->next= head[r].next,pp->prev= &head[r];
pp->next->prev= pp,pp->prev->next= pp;
}
for(j= 0;j<mm;j++)pp->footprint[j]|= p->footprint[j]|q->footprint[j];
}

/*:5*/
#line 84 "./fchains4x.w"
;
}

/*:4*/
#line 67 "./fchains4x.w"
;
pqdone:continue;
}

/*:3*/
#line 42 "./fchains4x.w"
;
/*12:*/
#line 184 "./fchains4x.w"

while(1){
printf("Truth table (hex): ");
fflush(stdout);
if(!fgets(buf,100,stdin))break;
if(sscanf(buf,"%x",&ttt)!=1)break;
printf("%04x has cost ",ttt);
if(ttt&0x8000)ttt^= 0xffff;
printf("%d, parents (%04x,%04x), and footprint",
func[ttt].cost,func[ttt].parent>>16,func[ttt].parent&0xffff);
for(j= 0;j<mm;j++)if(func[ttt].footprint[j]){
s= 32*j;
for(u= func[ttt].footprint[j];u;u>>= 1,s++)
if(u&1)printf(" %s",name[s]);
}
printf("\n");
}

/*:12*/
#line 43 "./fchains4x.w"
;
}

/*:1*/
