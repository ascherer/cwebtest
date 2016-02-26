#define memsize (1<<25) 
#define varsize 1000 \

/*1:*/
#line 26 "simpath-reduce.w"

#include <stdio.h> 
#include <stdlib.h> 
int lo[memsize],hi[memsize];
int firstnode[varsize];
int head;
int nodesout;
char buf[100];
int nbuf,lbuf,hbuf;
FILE*tempfile;
main(){
register int j,k,p,q,r,s,t;
/*2:*/
#line 43 "simpath-reduce.w"

tempfile= fopen("/tmp/simpath-names","w");
if(!tempfile){
fprintf(stderr,"I can't open /tmp/simpath-names for writing!\n");
exit(-1);
}
while(1){
if(!fgets(buf,100,stdin)){
fprintf(stderr,"The input line ended unexpectedly!\n");
exit(-2);
}
if(buf[0]=='#')break;
fprintf(tempfile,buf);
}
fclose(tempfile);
for(t= 1,s= 2;;t++){
if(t+1>=varsize){
fprintf(stderr,"Memory overflow (varsize=%d)!\n",varsize);
exit(-3);
}
firstnode[t]= s;
if(sscanf(buf+1,"%d",&nbuf)!=1||nbuf!=t){
fprintf(stderr,"Bad input line for arc %d: %s",t,buf);
exit(-4);
}
for(;;s++){
if(s>=memsize){
fprintf(stderr,"Memory overflow (memsize=%d)!\n",memsize);
exit(-5);
}
if(!fgets(buf,100,stdin))goto done_reading;
if(buf[0]=='#')break;
if(sscanf(buf,"%x:%x,%x",&nbuf,&lbuf,&hbuf)!=3||nbuf!=s){
fprintf(stderr,"Bad input line for node %x: %s",s,buf);
exit(-6);
}
lo[s]= lbuf,hi[s]= hbuf;
}
}
done_reading:
fprintf(stderr,"%d arcs and %d branch nodes successfully read.\n",t,s-2);
firstnode[t+1]= s;

/*:2*/
#line 38 "simpath-reduce.w"
;
/*3:*/
#line 103 "simpath-reduce.w"

lo[0]= lo[1]= -1;
for(;t;t--){
head= 0;
for(k= firstnode[t];k<firstnode[t+1];k++){
q= lo[k];
if(lo[q]>=0)lo[k]= lo[q];
q= hi[k];
if(lo[q]>=0)hi[k]= q= lo[q];
if(q)/*4:*/
#line 117 "simpath-reduce.w"

{
if(hi[q]>=0)hi[k]= -head,head= q;
else hi[k]= -hi[q];
hi[q]= -k;
}

/*:4*/
#line 112 "simpath-reduce.w"
;
}
/*5:*/
#line 127 "simpath-reduce.w"

for(p= head;p;p= -q){
for(q= -hi[p];q> 0;q= hi[q]){
r= lo[q];
if(lo[r]<=0){
printf("%x: (~%d?%x:%x)\n",q,t,r,p);
nodesout++;
lo[r]= q,lo[q]= -r-1;
}else lo[q]= lo[r];
}
for(q= -hi[p],hi[p]= 0;q> 0;r= q,q= hi[r]){
r= lo[q];
if(r<0)lo[-r-1]= -1;
}
hi[r]= 0;
}

/*:5*/
#line 114 "simpath-reduce.w"
;
}

/*:3*/
#line 39 "simpath-reduce.w"
;
fprintf(stderr,"%d branch nodes output.\n",nodesout);
}

/*:1*/
