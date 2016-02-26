#define abort(mess) {fprintf(stderr,"Parsing error: %.*s|%s, %s!\n", \
p-argv[1],argv[1],p,mess) ;exit(-1) ;} \

#define maxn 1000 \

#define vert(k) (g->vertices+(k) )  \

/*1:*/
#line 19 "./spgraph.w"

#include "gb_graph.h"
#include "gb_save.h"
/*3:*/
#line 56 "./spgraph.w"

int stack[maxn];
int llink[maxn],rlink[maxn];
char buffer[8];
int jj;
Graph*g;

/*:3*/
#line 22 "./spgraph.w"

/*7:*/
#line 94 "./spgraph.w"

void build(int stackitem,int lft,int rt)
{
register int t,j;
if(stackitem==0)gb_new_edge(vert(lft),vert(rt),0);
else{
t= stackitem>>8,j= stackitem&0xff;
if(t)t= --jj+2,build(llink[j],lft,t),build(rlink[j],t,rt);
else build(llink[j],lft,rt),build(rlink[j],lft,rt);
}
}

/*:7*/
#line 23 "./spgraph.w"

main(int argc,char*argv[])
{
register int j,k;
if(argc!=3){
fprintf(stderr,"Usage: %s SPformula foo.gb\n",argv[0]);
exit(0);
}
/*2:*/
#line 44 "./spgraph.w"

{
register char*p= argv[1];
for(j= k= 0;*p;p++)
if(*p=='-')/*4:*/
#line 63 "./spgraph.w"

stack[k++]= 0;

/*:4*/
#line 48 "./spgraph.w"

else if(*p=='s'||*p=='p')/*5:*/
#line 66 "./spgraph.w"

{
if(k<2)abort("missing operand");
rlink[++j]= stack[--k];
llink[j]= stack[k-1];
if(*p=='s')jj++;
stack[k-1]= (*p=='s'?0x100:0)+j;
}

/*:5*/
#line 49 "./spgraph.w"

else abort("bad symbol");
if(k!=1)abort("disconnected graph");
}

/*:2*/
#line 31 "./spgraph.w"
;
/*6:*/
#line 79 "./spgraph.w"

g= gb_new_graph(jj+2);
if(!g){
fprintf(stderr,"Can't create the graph!\n");
exit(-1);
}
sprintf(g->id,"SP%.152s",argv[1]);
for(k= 0;k<g->n;k++){
sprintf(buffer,"v%d",k);
vert(k)->name= gb_save_string(buffer);
}
build(stack[0],0,1);

/*:6*/
#line 32 "./spgraph.w"
;
k= save_graph(g,argv[2]);
if(k)printf("I had trouble saving in %s (anomalies %x)!\n",argv[2],k);
else printf("Graph %s saved successfully in %s.\n",g->id,argv[2]);
}

/*:1*/
