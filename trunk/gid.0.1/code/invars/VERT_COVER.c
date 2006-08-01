
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
 VERT_COVER.c

Chad Brewbaker


This program does a depth first search for the vertex chromatic number, the number of verticies needed so that every vertex in the graph touches one of the verticies in our set.

Redundency is kept down by ensuring that the elements in each partition are added lexicographicaly.


*/

#include <stdio.h>

#define _gcomp_MAX 100

int minCover;
short dasGraph[_gcomp_MAX][_gcomp_MAX];
int WRK_ARR[_gcomp_MAX];


int TEST_COVER(int* usedList,int verts, int size){
  int i,j,count;
  
  for(i=0;i<verts;i++){
    WRK_ARR[i]=0;
  }
  for(i=0;i<size;i++){
    WRK_ARR[usedList[i]]=1;
    for(j=0;j<verts;j++){
      if(dasGraph[usedList[i]][j])
	WRK_ARR[j]=1;
    }
  }
  count=0;
  for(i=0;i<verts;i++)
    count+=WRK_ARR[i];
  if(count==verts)
    return 1;
  return 0;
}

void COVER_DFS(int* usedList,int verts,int size)
{
  int i,j,k,bad;
  int lastVert,vertCount;
  int count;
  
  if(size==verts){
    minCover=verts;
    return;
  }
  if(size<minCover){
    if(TEST_COVER(usedList,verts,size)){
      minCover=size;
      return;
    }  
  }
  for(i=usedList[size-1]+1;i<verts;i++){
    usedList[size]=i;
    COVER_DFS(usedList,verts,size+1);
  }
}





int main()
{
  int i,j;
  int tmp,sum;
  int x,y,verts;
  int usedList[_gcomp_MAX];

  
  while(1){
    if(EOF==scanf("%d",&verts))
      return 0;
    if(verts>_gcomp_MAX){
      fprintf(stderr,"\nErr: GRAPH TOO BIG\n"); return 1;
    }
    
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
    
    minCover=verts;
    usedList[0]=0;
    for(i=0;i<verts;i++){
      usedList[0]=i;
      COVER_DFS(usedList,verts,1);
    }
    printf("%d\n",minCover);
  }
}
