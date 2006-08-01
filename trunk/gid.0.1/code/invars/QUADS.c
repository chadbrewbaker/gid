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
  Input: An edge list of graphs, brewformat

  Output:The number of complete graphs on 4 verticies in a graph
*/

#include <stdio.h>
#define _gcomp_MAX 100


int main()
{
  int i,j,k,l,quadCount;
  int dasGraph[_gcomp_MAX][_gcomp_MAX];
  int x,y,verts;  
  while(1){
    if(EOF==scanf("%d",&verts))
      return 0;
    if(verts>_gcomp_MAX)
      {fprintf(stderr,"\nErr: GRAPH TOO BIG\n"); return 1;}
    for(i=0;i<verts;i++){
      for(j=0;j<verts;j++)
	dasGraph[i][j]=0;
    }
    while(1){
      scanf("%d",&x);
      if(x<0)break;
      scanf("%d",&y);
      /*add edge*/
      dasGraph[x][y]=1; 
      dasGraph[y][x]=1; 
    }
    quadCount=0;
    for(i=0;i<verts;i++){
      for(j=i+1;j<verts;j++){
	for(k=j+1;k<verts;k++){
	  for(l=k+1;l<verts;l++){
	  if(dasGraph[i][j]&&dasGraph[i][k]&&dasGraph[i][l]&&
	     dasGraph[j][k]&&dasGraph[j][l]&&dasGraph[k][l])
	    quadCount++;
	  }
	}
      }
    }
    printf("%d\n",quadCount); 
  }
}
