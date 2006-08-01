/*
Copyright (C)2004 Chad Brewbaker

See the file gpl.txt in the root directory.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License version 2.0
as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

/*
  VERT_ANTI_CHROM_COUNT.c

Chad Brewbaker


This program does a depth first search for the number of partitons of the graph into cliques.

Redundency is kept down by ensuring that the elements in each partition are added lexicographicaly.


*/

#include <stdio.h>

#define _gcomp_MAX 100

int CHROM_COUNT;
short dasGraph[_gcomp_MAX][_gcomp_MAX];

void CHROM_DFS(int* usedList,int verts,int size)
{
  int i,j,k,bad;
  int lastVert,vertCount;
  
  if(verts==size){
    CHROM_COUNT++;
    return;
  }

 /*
	Color List
	Verts are colored according to their min lexicographic element
	
	DFS()
	for(i=0;i<verts;i++)
	Include the vert in every list with lower color and no conflict;
	Include the vert in a new list;

 */  
  for(i=0;i<size;i++){
    /*Find the first element of every partition*/
    if(usedList[i]==i){
      bad=0;
      for(j=i;j<size;j++){
	if(usedList[j]==i && !dasGraph[j][size])
	  {bad=1; break;}
      }
      if(!bad){
	usedList[size]=i;
	/*Include the vert in the partition because it has no conflicts*/
	CHROM_DFS(usedList,verts,size+1);
      }
    }
  }
  usedList[size]=size;
  /*Include the vert in its own partition*/
  CHROM_DFS(usedList,verts,size+1);
}





int main()
{
  int i,j;
  int tmp,sum;
  int x,y,verts;
  int usedList[_gcomp_MAX];

 
  while(1)
    {
      if(EOF==scanf("%d",&verts))
	return 0;
      if(verts>_gcomp_MAX)
	{fprintf(stderr,"\nErr: GRAPH TOO BIG\n"); return 1;}

      for(i=0;i<verts;i++)
	{
	  for(j=0;j<verts;j++)
	    dasGraph[i][j]=0;
	}
      CHROM_COUNT=0;
      while(1)
	{
	  scanf("%d",&x);
	  if(x<0)break;
	  scanf("%d",&y);
	  /*add edge*/
	  dasGraph[x][y]=1; 
	  dasGraph[y][x]=1; 
	}
      usedList[0]=0;
      CHROM_DFS(usedList,verts,1);
      printf("%d\n",CHROM_COUNT);
    }
}
