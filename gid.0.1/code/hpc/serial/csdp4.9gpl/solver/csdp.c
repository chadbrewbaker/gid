#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "declarations.h"

int main(argc,argv)
     int argc;
     char *argv[];
{
  int ret;
  int n;
  int k;
  struct blockmatrix C;
  double *a;
  struct constraintmatrix *constraints;
  struct blockmatrix X,Z;
  double *y;
  double pobj,dobj;

  /*
   * Check that we have enough arguments.
   */

  if ((argc < 2) || (argc > 4)) 
    {
      printf("CSDP 4.9\n");
      printf(" \n");
      printf("Usage: \n");
      printf("\n");
      printf("csdp <input problem> [<final solution>] [<initial solution>]\n");
      exit(1);
    };


  /*
   * First, read the problem in. 
   */
  ret=read_prob(argv[1],&n,&k,&C,&a,&constraints,1);

  if (argc == 4)
    {
      read_sol(argv[3],n,k,C,&X,&y,&Z);
    }
  else
    {
      initsoln(n,k,C,a,constraints,&X,&y,&Z);
    };

  /*
   * Call the solver.
   */

  ret=easy_sdp(n,k,C,a,constraints,0.0,&X,&y,&Z,&pobj,&dobj);

  /*
   * Write out the solution if necessary.
   */
  if (argc >= 3)
    write_sol(argv[2],n,k,X,y,Z);

  /*
   * Free up memory.
   */

  free_prob(n,k,C,a,constraints,X,y,Z);

  return(ret);
}

