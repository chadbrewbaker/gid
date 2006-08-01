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

  Output:Maximum number of edge crossings when the graph is drawn as points on a line.
*/

#include <stdio.h>
#define _gcomp_MAX 100
 int dasGraph[_gcomp_MAX][_gcomp_MAX];
int maxCross;
int myABS(int x, int y)
{
  int retval;
  retval=x-y;
  if(retval<0)
    retval=retval*-1;
  return retval;
}

int CROSSING(int i,int j, int k, int l){
  int temp;
  if(j<i){
    temp=i;
    i=j;
    j=temp;
  }
  if(l<k){
    temp=k;
    k=l;
    l=temp;
  }
  if(i==k  && j==l)
    return 0;
  if(j<=k || l<=i)
    return 0;
  return 1;
}

/*this double counts, so dont forget to divide by 2*/
void CROSS_COUNT(int* arr, int verts){
  int i,j,k,l;
  int crossTemp;
  crossTemp=0;
  for(i=0;i<verts;i++){
    for(j=i+1;j<verts;j++){
      if(dasGraph[i][j]){
	for(k=0;k<verts;k++){
	  for(l=k+1;l<verts;l++){
	    if(dasGraph[k][l]){
	      if(CROSSING(arr[i],arr[j],arr[k],arr[l]))
		  crossTemp++;
	    }
	  }
	}
      }
    }
  }
  if(crossTemp>maxCross)
    maxCross=crossTemp;
}


void MAX_CROSS(int verts)
{
  int i,j,k,size;
  int temp,r,s;
  int* arr;
  arr=(int*)malloc(sizeof(int)*(verts+1));

  size=verts;
  
  for(i=0;i<=size;i++)
    arr[i]=i;
  i=1;
  while(i){
    CROSS_COUNT(arr+1,verts);
    for(i=size-1;arr[i]>arr[i+1];i--){};
    for(j=size;arr[i]>arr[j];j--){};
    temp=arr[i];
    arr[i]=arr[j];
    arr[j]=temp;
    r=size;
    s=i+1;
    while(r>s){
      temp=arr[r];
      arr[r]=arr[s];
      arr[s]=temp;
      r--;s++;
    }
  }
  free(arr);
}




int main()
{
  int i,j,k;
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
    maxCross=0;
    MAX_CROSS(verts);
    printf("%d\n",maxCross/2); 
  }
}
