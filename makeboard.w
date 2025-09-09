@i gb_types.w

@* Making a graph.
I'm just creating a file \.{/tmp/board,a,b,c,d,e,f,g.gb}, where $a$ thru $g$
appear on the command line.

@p
#include "gb_graph.h" /* we use the {\sc GB\_\,GRAPH} data structures */
#include "gb_basic.h" /* and the basic graph operations */
#include "gb_save.h" /* and we save our results in ASCII format */
long a,b,c,d,e,f,z;
char buf[100];
@#
int main(int argc, char* argv[])
{@+Graph *g,*gg,*ggg;
  if (argc!=8 || sscanf(argv[1],"%ld",&a)!=1 ||
                 sscanf(argv[2],"%ld",&b)!=1 ||
                 sscanf(argv[3],"%ld",&c)!=1 ||
                 sscanf(argv[4],"%ld",&d)!=1 ||
                 sscanf(argv[5],"%ld",&e)!=1 ||
                 sscanf(argv[6],"%ld",&f)!=1 ||
                 sscanf(argv[7],"%ld",&z)!=1) {
    fprintf(stderr,"Usage: %s a b c d e f g\n",argv[0]);
    exit(-1);
  }
  g=board(a,b,c,d,e,f,z);
  sprintf(buf,"/tmp/board,%ld,%ld,%ld,%ld,%ld,%ld,%ld.gb",a,b,c,d,e,f,z);
  save_graph(g,buf); /* generate an ASCII file for it */
  return 0; /* normal exit */
}

@* Index.
