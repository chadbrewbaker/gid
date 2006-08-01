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

#include <stdio.h>

#define _gcomp_MAX 100



void findDist(short** dasGraph, short** dist,int verts)
{
  int i,j,k;
 
}


int main()
{
  int i,j,k,l;
  short dasGraph[_gcomp_MAX][_gcomp_MAX];
  short partEdges[_gcomp_MAX][_gcomp_MAX][_gcomp_MAX];
  short myPart[_gcomp_MAX];
  short vertList[_gcomp_MAX];
  short dist[_gcomp_MAX][_gcomp_MAX];
  short edgeCount[_gcomp_MAX][_gcomp_MAX];
  


  int maxDist;
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
      /*Find distances*/
      for(i=0;i<verts;i++){
	for(j=0;j<verts;j++){
	  if(dasGraph[i][j])
	    dist[i][j]=1;
	  else
	    dist[i][j]=verts*2;
	}
      }
      for(k=0;k<verts;k++){
	/*For each vertex*/
	for(j=0;j<verts;j++){
	  /*For every possible distance level*/
	  for(i=0;i<verts;i++){
	    if(dist[i][j]>(dist[i][k]+dist[k][j]))
	      dist[j][i]=dist[i][j]=dist[i][k]+dist[k][j];
	  }
	}
      }
      maxDist=0;
      for(i=0;i<verts;i++){
	for(j=i+1;j<verts;j++){
	  if(dist[i][j]>maxDist)
	    maxDist=dist[i][j];
	}
      }
      printf("%d\n",maxDist);
    }
}
