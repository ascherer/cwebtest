@*Intro. What's the lexicographically smallest solution to the
$\infty$-queens problem? I mean, consider the sequence $q_0$, $q_1$, \dots,
where $q_n$ is the least nonnegative integer not in the sets
$\{q_k\mid 0\le k<n\}$,
$\{q_k+k-n\mid 0\le k<n\}$,
$\{q_k-k+n\mid 0\le k<n\}$.

Call those sets $A_n$, $B_n$, $C_n$ (as in exercise 7.2.2--10).
This program calculates them, as bit vectors, until running out of memory.

I divide the sequence into two parts: $r_n=0$, 2, 4, 8, 10, \dots,
the ``record high'' values; and $s_n=1$, 3, 5, 7, 6, 9, \dots,
the other values. It appears that the differences $r_{n+1}-r_n$
and $s_{n+1}-s_n$ are confined to a small set of values.

Neil Sloane conjectured (in August 2016, see OEIS A275884) that,
when $q_n$ is a new record high, we have $q_n\approx n\phi$;
otherwise $q_n\approx n/\phi$. I test this conjecture.

I also noticed a strange fact: The sum of $q_n$ for $n$ between
$64k$ and $64k+63$ seems to be very close to a multiple of 64!
So I've calculated those statistics too.


@d maxmem 1000000 /* this many words, for each of $A_n$, $B_n$, $C_n$ */
@d lgbitsperwd 6 /* usually 6, but smaller when debugging */
@d bitsperwd (1<<lgbitsperwd)
@d shortmask (bitsperwd-1)
@d mask ((1LL<<bitsperwd)-1)
@d deBruijn 0x03f79d71b4ca8b09 /* the least de Bruijn cycle of length 64 */
@d maxdelta 100

@c
#include <stdio.h>
#include <stdlib.h>
unsigned long long a[maxmem],b[maxmem],c[maxmem+1];
int acount,bcount,ccount; /* how much of |a|, |b|, |c| we've touched */
int afull; /* this many initial octabytes of |a| are all 1s */
int queue[bitsperwd];
int lastupper,lastlower;
int strangemod[bitsperwd];
int upperdelta[maxdelta],lowerdelta[maxdelta],lowerdeltaneg[maxdelta];
int upperdeltahi,lowerdeltahi,lowerdeltaneghi;
int uppern;
char decode[64]; /* table for calculating the ruler function */
main() {
  register j,k,n,q,del;
  register unsigned long long t,u,v;
  @<Set up the |decode| table@>;
  for (n=0;;n++) {
    @<Determine $q=q_n$@>;
    @<Record statistics about $q$@>;
  }
done:@+@<Print the final stats@>;
}

@ @<Set up the |decode| table@>=
for (j=0,v=1;v;j++,v<<=1) {
  u=v*deBruijn;
  decode[u>>58]=j;
}

@ The basic recurrence is
|A=A+t|, |B=(B+t)>>1|, |C=(C+t)<<1|,
where |t| is the smallest bit not in $A\OR B\OR C$.
But we must do it in multiprecision.

@<Determine $q=q_n$@>=
while (a[afull]==mask) afull++;
for (k=afull;;k++) {
  u=~(a[k]|b[k]|c[k]);
  if (u&mask) break;
}
t=u&-u;
q=(k<<lgbitsperwd)+decode[(t*deBruijn)>>58];
a[k]+=t;
b[k]+=t;
c[k]+=t; /* possibly |k=ccount| here */
@<Shift |b| right one@>;
@<Shift |c| left one@>;

@ We needn't bother with the least significant |afull| words
of~|b|, because they will never be examined.

@<Shift |b| right one@>=
if (k>=bcount) bcount++;
for (j=afull;j<bcount;j++)
  b[j]=(b[j]>>1)+((b[j+1]&1)<<shortmask);

@ @<Shift |c| left one@>=
if (k>=ccount || (c[ccount-1]&(1LL<<shortmask))) ccount++;
if (ccount==maxmem) goto done;
for (j=ccount-1;j;j--) 
  c[j]=((c[j]<<1)&mask)+(c[j-1]>>shortmask);
c[0]=(c[0]<<1)&mask;

@ @<Record statistics about $q$@>=
queue[n&shortmask]=q;
if (q>lastupper) {
  uppern++;
  del=q-lastupper;
  lastupper=q;
  if (del>upperdeltahi) {
    if (del>=maxdelta) {
      fprintf(stderr,"Overflow of the upperdelta table!\n");
      exit(-1);
    }
    upperdeltahi=del;
    printf("new upperdelta %d at %d\n",
                      del,n);
  }
  upperdelta[del]++;
}@+else {
  del=q-lastlower;
  lastlower=q;
  if (del<0) {
    del=-del;
    if (del>lowerdeltaneghi) {
      if (del>=maxdelta) {
        fprintf(stderr,"Overflow of the lowerdeltaneg table!\n");
        exit(-2);
      }
      lowerdeltaneghi=del;
      printf("new lowerdeltaneg %d at %d\n",
                        del,n);
    }
    lowerdeltaneg[del]++;
  }@+else {
    if (del>lowerdeltahi) {
      if (del>=maxdelta) {
        fprintf(stderr,"Overflow of the lowerdelta table!\n");
        exit(-2);
      }
      lowerdeltahi=del;
      printf("new lowerdelta %d at %d\n",
                        del,n);
    }
    lowerdelta[del]++;
  }
}
if ((n&shortmask)==shortmask) {
  for (q=k=0;k<bitsperwd;k++) q+=queue[k];
  strangemod[q&shortmask]++;
}
if (n && ((n&0xffff)==0))
  printf("ratios after %d: %16.8f %16.8f %16.8f\n",
                   n,(double)uppern/(double)n,
                     (double)lastupper/(double)n,
                     (double)lastlower/(double)n);
@ @<Print the final stats@>=
printf("OK, I computed %d elements of the sequence, with %d records.\n",
                               n,uppern);
printf("Upper deltas:");
for (k=0;k<=upperdeltahi;k++) printf(" %d",
                                     upperdelta[k]);
printf("\n");
printf("Lower deltas:");
for (k=0;k<=lowerdeltahi;k++) printf(" %d",
                                     lowerdelta[k]);
printf("\n");
printf("Lower negative deltas:");
for (k=0;k<=lowerdeltaneghi;k++) printf(" %d",
                                     lowerdeltaneg[k]);
printf("\n");
printf("Strange mods:");
for (k=0;k<bitsperwd;k++) printf(" %d",
                                       strangemod[k]);
printf("\n");

@*Index.
