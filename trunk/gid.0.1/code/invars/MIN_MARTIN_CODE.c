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
 MIN_MARTIN_CODE.c
Chad Brewbaker
*/
#include <stdio.h>
#define _gcomp_MAX 100


short dasGraph[_gcomp_MAX][_gcomp_MAX];
int verts;


int codeSize(int arr[])
{
  int i,count;
  count=0;
  for(i=0;i<verts;i++){
    if(arr[i])
      count++;
  }
  return count;
}

void intersect(int arr1[], int arr2[],int arr3[])
{
  int i;
  for(i=0;i<verts;i++)
    arr3[i]=arr1[i]*arr2[i];
}

void BALL(int arr[],int v)
{
  int i;
  for(i=0;i<verts;i++)
    arr[i]=0;
  for(i=0;i<verts;i++){
    if(v!=i){
      if(dasGraph[v][i])
	arr[i]=1;
    }
  }
  arr[v]=1;
  return;
}

int isMartinCode(int code[])
{
  int* arr1;
  int* arr2;
  int* arr3;
  int i,j,ok;
  if(codeSize(code)<1)
    return 0;

  arr1=(int*)malloc(verts*sizeof(int));
  arr2=(int*)malloc(verts*sizeof(int));
  arr3=(int*)malloc(verts*sizeof(int));
  
  /*The intersecion of BALL[i] \cup C != EMPTYSET*/
  for(i=0;i<verts;i++){
    BALL(arr1,i);
    intersect(code,arr1,arr2);
    if(codeSize(arr2)==0)
      return 0;
  }
  /*For i,j  i!=j     BALL[i]\cup C !=BALL[j]\cup C      */
  for(i=0;i<verts;i++){
    for(j=i+1;j<verts;j++){
      BALL(arr1,i);
      intersect(arr1,code,arr2);
      BALL(arr1,j);
      intersect(arr1,code,arr3);
      ok=0;
      for(i=0;i<verts;i++){
	if(arr2[i]!=arr3[i])
	  ok=1;
      }
      if(!ok)
	return 0;
    }
  }
  free(arr1);
  free(arr2);
  free(arr3);
  return 1;
}


int getMinCode()
{
  long i,j;
  int* arr;
  int minCodeSz;

  minCodeSz=verts+1;
  arr = (int*)malloc( (verts+1) * sizeof(int));
  for(i=0;i<=verts;i++)
    arr[i]=0;
  while(1){

    if(arr[verts])
      break;
    // for(j=0;j<verts;j++)
    //  printf("%d ",arr[j]);
    //printf("%d\n",codeSize(arr));
    if(isMartinCode(arr)&&(codeSize(arr)< minCodeSz))
      minCodeSz=codeSize(arr);
    for(i=0;i<=verts;i++){
      if(!arr[i])
	break;
    }
    arr[i]=1;
    for(j=0;j<i;j++)
      arr[j]=0;
  }
  free(arr);
  return minCodeSz;
}


int main()
{
  int i,j;
  int x,y,minCode;
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
    /*Graph is read in, lets do a dfs search for martin codes*/
    /*Its probably NP hard, so just do brute force*/ 
    
    minCode=getMinCode();
    if(minCode>verts)
      minCode=0;
    printf("%d\n",minCode);
  }  
}
