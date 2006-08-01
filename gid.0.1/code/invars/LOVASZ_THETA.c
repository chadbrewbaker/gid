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
LOVASZ_THETA.c
Chad Brewbaker

WARNING: This program needs the Brian Borcher's "theta" utility from CSDP to compute the lovasz theta function. MAX_CLIQUE<= theta <= VERTEX_CHROMATIC_NUMBER

*/

#include <stdio.h>
#include <stdlib.h>

#define _gcomp_MAX 100



int main()
{
  char c;
  int edgeCount,verts;
  int i,j,x,y;
  char BUFF[256];
  FILE* fptr;
  short dasGraph[_gcomp_MAX][_gcomp_MAX];

  fptr=fopen("lovasz.wrk","w");
  fclose(fptr);
  
  while(1){
 /*Write the graph complement to a temp file in the format:
    verts
   edges
  1 2
  3 45
  ...
  */
    if(EOF==scanf("%d",&verts))
      break;
    if(verts>_gcomp_MAX){
      fprintf(stderr,"\nErr: GRAPH TOO BIG\n"); 
      return 1;
    }
    
    for(i=0;i<verts;i++){
      for(j=0;j<verts;j++)
	dasGraph[i][j]=1;
    }
    while(1){
      scanf("%d",&x);
      if(x<0)break;
      scanf("%d",&y);
      /*add edge*/
      dasGraph[x][y]=0; 
      dasGraph[y][x]=0; 
    }
    edgeCount=0;
    for(i=0;i<verts;i++){
      for(j=i+1;j<verts;j++){
	if(dasGraph[i][j]==1)
	  edgeCount++;
      }
    }
 
    fptr=fopen("lovasz.wrk2","w");
    fprintf(fptr,"%d\n%d\n",verts,edgeCount);
    for(i=0;i<verts;i++){
      for(j=i+1;j<verts;j++){
	if(dasGraph[i][j]==1)
	  fprintf(fptr,"%d %d\n",i+1,j+1);
      }
    }
    fclose(fptr);
    /*Call CSDP theta unix utility*/
    system("theta lovasz.wrk2 |tail -c16 >>lovasz.wrk");
  }
  fptr=fopen("lovasz.wrk","r");
  while(1){
      c = fgetc (fptr);
      if(c==EOF)
	break;
      printf("%c",c);
    }
  fclose (fptr);
  system("rm lovasz.wrk");
  system("rm lovasz.wrk2");
  return 0;
}
