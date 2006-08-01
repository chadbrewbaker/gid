
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
VERTS.c
Chad Brewbaker
*/
#include <stdio.h>
#define _gcomp_MAX 100
int main()
{
  int i,j,k,edgeCount;
  short dasGraph[_gcomp_MAX][_gcomp_MAX];
  short partEdges[_gcomp_MAX][_gcomp_MAX];
  short myPart[_gcomp_MAX];
  short vertList[_gcomp_MAX];
  int partNum,newPartNum,temp;
  int x,y,verts;
  while(1){
    if(EOF==scanf("%d",&verts))
      return 0;
    printf("%d\n",verts);
    while(1){
	  scanf("%d",&x);
	  if(x<0)break;
	  scanf("%d",&y);
    }   
  }  
}
