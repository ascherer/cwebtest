@x
#include "gb_flip.h" /* and its random number generator */
@y
#include "gb_flip.h" /* and its random number generator */
@#
#include <stdlib.h> /* |exit| */
@z

@x
void print_state(FILE *stream)
{
  register int i,j,l;
@y
void print_state(FILE *stream)
{
  register int j,l;
@z

@x
void print_progress(void) {
  register int l,k,d,p;
@y
void print_progress(void) {
  register int l,k,d;
@z
