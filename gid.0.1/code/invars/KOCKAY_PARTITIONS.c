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
Chad Brewbaker

Computes the number of Kockay partitions of a graph. The number of partitions formed by having initaly one partition, and recursively splitting partitions based on how many edges are betwen the verticies in one partition, and the other partitions

*/


#include <stdio.h>
#define _gcomp_MAX 100

int main()
{
  int i,j,k;
  short dasGraph[_gcomp_MAX][_gcomp_MAX];
  short partEdges[_gcomp_MAX][_gcomp_MAX];
  short myPart[_gcomp_MAX];
  short vertList[_gcomp_MAX];
  int partNum,newPartNum,temp;

  int x,y,verts;
  
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
      while(1)
	{
	  scanf("%d",&x);
	  if(x<0)break;
	  scanf("%d",&y);
	  /*add edge*/
	  dasGraph[x][y]=1; 
	  dasGraph[y][x]=1; 
	}
      /*Compute Kockay Partitions*/
      for(i=0;i<verts;i++)
	myPart[i]=0;
      newPartNum=1;
      for(i=0;i<verts;i++)
	vertList[i]=i;
      /*Iteratively refine based on number of edges to each partition*/
      do
	{
	  partNum=newPartNum;
	  for(i=0;i<verts;i++)
	    {
	      for(j=0;j<partNum;j++)
		partEdges[i][j]=0;
	    }
	  for(i=0;i<verts-1;i++)
	    {
	      for(j=i+1;j<verts;j++)
		{
		  if(dasGraph[i][j])
		    {
		      partEdges[i][myPart[j]]++;
		      partEdges[j][myPart[i]]++;
		    }
		}
	    }
	  for(i=0;i<verts;i++)
	    myPart[i]=-1;
	  newPartNum=0;
	  for(i=0;i<verts;i++)
	    {
	      if(myPart[i]==-1)
		{myPart[i]=newPartNum;newPartNum++;}
	      else
		continue;
	      for(j=i+1;j<verts;j++)
		{
		  if(myPart[j]==-1)
		    {
		      temp=0;
		      for(k=0;k<partNum;k++)
			{
			  if(partEdges[i][k]!=partEdges[j][k])
			    {temp=1;break;}
			}
		      if(!temp)
			myPart[j]=myPart[i];
		    }
		}
	    }
	}
      while(newPartNum>partNum);
      //      for(i=0;i<verts;i++)
      //printf("%d ",myPart[i]);
      printf("%d\n",partNum);
    }

}
