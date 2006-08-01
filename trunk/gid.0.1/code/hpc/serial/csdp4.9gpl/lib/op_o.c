/*
  Compute a matrix representation of the operator
 
        O(.)=A(inv(Z)*A'(.)*X)

  The ith colum of the result is O(e_i), where e_i is the vector with
  a 1 in position i and 0's elsewhere.  

  */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "declarations.h"


void op_o(k,constraints,byblocks,Zi,X,O,work1,work2)
     int k;
     struct constraintmatrix *constraints;
     struct sparseblock **byblocks;
     struct blockmatrix Zi;
     struct blockmatrix X;
     double *O;
     struct blockmatrix work1;
     struct blockmatrix work2;
{
  int i,j;
  int ii,jj;
  int blk;
  int ldam;
  int blocksize;
  int p,q,r,s;

  struct sparseblock *ptri;
  struct sparseblock *ptrj;
  struct sparseblock *ptr;
  struct sparseblock *ptr2;

  int blocknum;
  double contrib;
  double *Ziblk;
  double *Xblk;
  double *workblk;
  double *work2blk;
  double ent,enti,entj,scale1,scale2;

  int numdenseblocks;
  /*
   *  Work out the leading dimension for the array.  Note that we may not
   *  want to use k itself, for cache issues.
   */
  if ((k % 2) == 0)
    ldam=k+1;
  else
    ldam=k;

  /*
   * First, zero out the O matrix.
   */

  for (j=1; j<=k; j++)
    for (i=1; i<=j; i++)
      O[ijtok(i,j,ldam)]=0.0;

  /*
   * Loop through all of the sparse-sparse pairs.
   */

  numdenseblocks=0;
  for (i=1; i<=k; i++)
    {
      ptri=constraints[i].blocks;

      while (ptri != NULL)
	{

	  if (ptri->issparse==1)
	    {
	      blocknum=ptri->blocknum;
	      blocksize=ptri->blocksize;

	      switch(work1.blocks[blocknum].blockcategory)
		{
		  /*
		   *  A diagonal block.
		   */

		case DIAG:
		  Ziblk=Zi.blocks[blocknum].data.vec;
		  Xblk=X.blocks[blocknum].data.vec;

		  ptrj=ptri;

		  while (ptrj != NULL)
		    {
		      if (ptrj->issparse==1)
			{
			  j=ptrj->constraintnum;

			  /*
			   * Do the contribution from constraints i and j of 
                           * this block.
			   */
			  
			  contrib=0.0;

			  /* bad old code.
			  for (ii=1; ii<=ptri->numentries; ii++)
			    {
			      enti=ptri->entries[ii];
			      p=ptri->iindices[ii];
			      q=p;

			      for (jj=1; jj<=ptrj->numentries; jj++)
				{
				  entj=ptrj->entries[jj];
				  r=ptrj->iindices[jj];
				  s=r;

				  if ((q==r) && (p==s))
				    contrib += enti*Ziblk[q]*
				      entj*Xblk[p];
				  
				}; 
			    }; 
		      
			  */

			  p=1;
			  q=1;
			  while ((p<=ptri->numentries) &&
				 (q<=ptrj->numentries))
			    {
			      if (ptri->iindices[p] < ptrj->iindices[q])
				{
				  p=p+1;
				}
			      else
				{
				  if (ptri->iindices[p] > ptrj->iindices[q])
				    {
				      q=q+1;
				    }
				  else
				    {
				      /*
				       * Match!
				       */
				      contrib += ptri->entries[p]*ptrj->entries[q]*
					Ziblk[ptri->iindices[p]]*
					Xblk[ptri->iindices[p]];
				      p=p+1;
				      q=q+1;
				    };
				};
			    };

			  O[ijtok(i,j,ldam)] += contrib;
			}; /* end if issparse */
		      ptrj=ptrj->nextbyblock;
		    }; /* end while */
		  break;

		  /*
		   * A matrix block.
		   */

		case MATRIX:
		  Ziblk=Zi.blocks[blocknum].data.mat;
		  Xblk=X.blocks[blocknum].data.mat;

		  ptrj=ptri;
		  
		  while (ptrj != NULL)
		    {
		      if (ptrj->issparse==1)
			{
			  /*
			   * The following prefetch seems to give about a
			   * 5% performance improvement on certain problems.
			   * e.g. truss8 on a 1200 Mhz Athlon.  
			   *
			   * It won't be compiled unless we're using gcc.
			   */

#ifdef __GNUC__
#if (((__GNUC__ == 3) && (__GNUC_MINOR__ > 1)) || (__GNUC__ > 3)) 
			  __builtin_prefetch(ptrj->nextbyblock,0,3);
#endif
#endif

			  j=ptrj->constraintnum;
			  
			  /*
			   * Do the contribution from constraints i and j of this block.
			   */
			  
			  contrib=0.0;
			  for (ii=1; ii<=ptri->numentries; ii++)
			    {

			      enti=ptri->entries[ii];
			      p=ptri->iindices[ii];
			      q=ptri->jindices[ii];

			      /*
			       * We'll keep the p==q test outside of the
			       * inner loop.
			       *
			       * In what follows, we've made use of the
			       * symmetry of Ziblk and Xblk, by permuting
			       * all array indices so that p and q are last.
			       * This means that we stay in columns p and q
			       * of Ziblk and Xblk as much as possible, 
			       * improving locality.
			       *
			       */

			      if (p==q)
				{
				  for (jj=1; jj<=ptrj->numentries; jj++)
				    {
				      entj=ptrj->entries[jj];
				      r=ptrj->iindices[jj];
				      s=ptrj->jindices[jj];

				      if (r==s)
					{
					  /* here p==q, r==s */
					  contrib += enti*entj*
					    Ziblk[ijtok(r,q,blocksize)]*
					    Xblk[ijtok(s,p,blocksize)];
					}
				      else
					{
					  /* here p=q, r!=s */

					  contrib += enti*entj*
					    (Ziblk[ijtok(r,q,blocksize)]*
					     Xblk[ijtok(s,p,blocksize)]
					     +
					     Ziblk[ijtok(s,q,blocksize)]*
					     Xblk[ijtok(r,p,blocksize)]);
					};
				    }; /* end for */
				}
			      else /* p!= q */
				{
				  for (jj=1; jj<=ptrj->numentries; jj++)
				    {
				      entj=ptrj->entries[jj];
				      r=ptrj->iindices[jj];
				      s=ptrj->jindices[jj];
				  
				      if (r==s)
					{
					  /* p!=q, but r=s */
					  contrib += enti*entj*
					    (Ziblk[ijtok(r,q,blocksize)]*
					     Xblk[ijtok(s,p,blocksize)]
					     +
					     Ziblk[ijtok(r,p,blocksize)]*
					     Xblk[ijtok(s,q,blocksize)]);
					}
				      else
					{
					  /* here, p!=q and r!=s */
					  contrib += enti*entj*
					    (Ziblk[ijtok(r,q,blocksize)]*
					     Xblk[ijtok(s,p,blocksize)]+
					     Ziblk[ijtok(r,p,blocksize)]*
					     Xblk[ijtok(s,q,blocksize)]+
					     Ziblk[ijtok(s,q,blocksize)]*
					     Xblk[ijtok(r,p,blocksize)]+
					     Ziblk[ijtok(s,p,blocksize)]*
					     Xblk[ijtok(r,q,blocksize)]);
					}; /* end if then else */
				    }; /* end for jj */
				}; /* end if (p==q) */
			    }; /* end for ii */
		      
			  O[ijtok(i,j,ldam)] += contrib;
			  /*		  printf("i,j,block %d,%d,%d \n",i,j,ptri->blocknum); */

			}; /* end if */
		      ptrj=ptrj->nextbyblock;
		    }; /* end while */
		  break; /* end of MATRIX case. */
		case PACKEDMATRIX:
		default:
		  printf("op_o illegal block type \n");
		  exit(12);
		}; /* end switch */

	    }
	  else
	    {
	      numdenseblocks++;
	    }; /* end if */

	  /*
	   * Go to the next block in this constraint. 
	   */
	  ptri=ptri->next;

	}; /* end while */
	  

    }; /* end for */

  /*
   * Now, handle dense blocks.
   */

  if (numdenseblocks > 0)
    {
      for (blk=1; blk<=work1.nblocks; blk++)
	{
	  
	  ptr=byblocks[blk];
	  
	  while (ptr != NULL)
	    {
	      /*
	       * Is this block dense?
	       */
	      
	      if (ptr->issparse==0)
		{
		  
		  /*
		   * put this block into a work matrix.
		   */
		  
		  blocksize=work1.blocks[blk].blocksize;
		  workblk=work1.blocks[blk].data.mat;
		  work2blk=work2.blocks[blk].data.mat;
		  Xblk=X.blocks[blk].data.mat;
		  Ziblk=Zi.blocks[blk].data.mat;
	
		  for (i=0; i<=blocksize*blocksize-1; i++)
		    {
		      workblk[i]=0.0;
		    };

		  for (i=1; i<=ptr->numentries; i++)
		    {
		      ent=ptr->entries[i];
		      p=ptr->iindices[i];
		      q=ptr->jindices[i];
		      workblk[ijtok(p,q,blocksize)]=ent;
		      if (p != q)
			workblk[ijtok(q,p,blocksize)]=ent;
		    };
		  
		  /*
		   * Now, multiply out Zi*work*X.
		   */
		  scale1=1.0;
		  scale2=0.0;
		  mat_mult_raw(blocksize,scale1,scale2,Ziblk,workblk,work2blk);
		  mat_mult_raw(blocksize,scale1,scale2,work2blk,Xblk,workblk);
		  
		  /*
		   * Now loop through all of the constraints with this block.
		   */
		  
		  ptr2=byblocks[blk];
		  
		  while (ptr2 != NULL)
		    {
		      /*
		       * Skip if we've already done this.
		       */
		      if ((ptr2->issparse==0) && 
			  ((ptr->numentries < ptr2->numentries) || 
			   ((ptr->numentries==ptr2->numentries) && 
			    (ptr->constraintnum<ptr2->constraintnum))))
			{
			  ptr2=ptr2->nextbyblock;
			  continue;
			};
		      
		      contrib=0.0;
                      blocksize=ptr2->blocksize;

			  /*
			   * Another GCC prefetch for improved performance.
			   * It won't be compiled unless we're using gcc.
			   */

#ifdef __GNUC__
#if (((__GNUC__ == 3) && (__GNUC_MINOR__ > 1)) || (__GNUC__ > 3)) 
		      __builtin_prefetch(ptr2->nextbyblock,0,3);
#endif		      
#endif		      

		      for (i=1; i<=ptr2->numentries; i++)
			{
			  ent=ptr2->entries[i];
			  p=ijtok(ptr2->iindices[i],ptr2->jindices[i],blocksize);
			  q=ijtok(ptr2->jindices[i],ptr2->iindices[i],blocksize);
			  contrib += ent*workblk[p];
			  if (p != q)
			    {
			      contrib += ent*workblk[q];
			    };
			};

		      if (ptr2->constraintnum > ptr->constraintnum)
			{
			  O[ijtok(ptr->constraintnum,ptr2->constraintnum,ldam)] += contrib;
			}
		      else
			{
			  O[ijtok(ptr2->constraintnum,ptr->constraintnum,ldam)] += contrib;
			};
		      ptr2=ptr2->nextbyblock;
		    }; /* end while ptr2 != NULL */
		  
		}; /* end if ptr->issparse==0 */
	      
	      /*
	       * Go on to the next constraint in which this block appears.
	       */
	      
	      ptr=ptr->nextbyblock;
	    }; /* end while ptr != NULL */
	}; /* end for blk loop */
      
      
    }; /* end if numdenseblocks > 0 */

}




