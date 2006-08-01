
#include "stdlib.h"
#include "declarations.h"

void sort_entries(k,C,constraints)
     int k;
     struct blockmatrix C;
     struct constraintmatrix *constraints;
{
  int i,ii,jj,tempi,nume,blk;
  double tempe;
  struct sparseblock *ptr;

  for (i=1; i<=k; i++)
    {
      ptr=constraints[i].blocks;
      while (ptr != NULL)
	{
	  blk=ptr->blocknum;
	  if (C.blocks[blk].blockcategory==DIAG)
	    {
	      nume=ptr->numentries;
	      for (ii=1; ii<=nume-1; ii++)
		for (jj=ii+1; jj<=nume; jj++)
		  {
		    if (ptr->iindices[ii] > ptr->iindices[jj])
		      {
			tempi=ptr->iindices[ii];
			tempe=ptr->entries[ii];
			
			ptr->iindices[ii]=ptr->iindices[jj];
			ptr->jindices[ii]=ptr->iindices[jj];
			ptr->entries[ii]=ptr->entries[jj];
			ptr->entries[jj]=tempe;
			ptr->iindices[jj]=tempi;
			ptr->jindices[jj]=tempi;
		      };
		  };
	    };

	  ptr=ptr->next;
	};
    }; /* end i */
}
