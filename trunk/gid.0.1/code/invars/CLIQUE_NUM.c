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
 CLIQUE_NUM.c

Chad Brewbaker


This program does a depth first search for the number of cliques in a graph. Single verticies are counted as cliques.

Redundency is kept down by ensuring that the first two verticies of the cycle are lexicographicaly increasing, and no vertex has a lexicographical value less than the first vertex.




*/

#include <stdio.h>

#define _gcomp_MAX 100

int CLIQUE_NUM;
short dasGraph[_gcomp_MAX][_gcomp_MAX];

void CLIQUE_DFS(int* usedList,int verts)
{
  int i,j,k,bad;
  int lastVert,vertCount;
  int* nextList;


  vertCount=1;
  for(i=0;i<verts;i++){ 
    if(usedList[i]<0)
      break;
    vertCount=i+1;
  }
  CLIQUE_NUM++;
 
  nextList=(int*)malloc(sizeof(int)*verts);
  for(i=0;i<vertCount;i++)
    nextList[i]=usedList[i];
  for(i=vertCount;i<verts;i++)
    nextList[i]=-1;
  
  for(i=usedList[vertCount-1]+1;i<verts;i++){
    bad=0;
    for(j=0;j<vertCount;j++){ 
      if(!dasGraph[i][usedList[j]]){
	bad=1; break;
      }
    }
    if(!bad){
      nextList[vertCount]=i;
      CLIQUE_DFS(nextList,verts);
    }
  }
  free(nextList);
}





int main()
{
  int i,j,k,l;
  short partEdges[_gcomp_MAX][_gcomp_MAX][_gcomp_MAX];
  short myPart[_gcomp_MAX];
  short vertList[_gcomp_MAX];
  short dist[_gcomp_MAX][_gcomp_MAX];
  short edgeCount[_gcomp_MAX][_gcomp_MAX];
  
  int tmp,sum;
  int x,y,verts;
  int usedList[_gcomp_MAX];

 
  while(1)
    {
      CLIQUE_NUM=0;
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
      for(k=0;k<verts;k++)
	usedList[k]=-1;
      for(i=0;i<verts;i++){
	for(j=i+1;j<verts;j++){
	  if(dasGraph[i][j])
	    {
	      usedList[0]=i;
	      usedList[1]=j;
	      CLIQUE_DFS(usedList,verts);
	    }
	}
      }
      printf("%d\n",CLIQUE_NUM+verts);
    }
}
