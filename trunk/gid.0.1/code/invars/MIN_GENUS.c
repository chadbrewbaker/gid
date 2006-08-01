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
 MIN_GENUS.c

Chad Brewbaker


This program does a depth first search for the minimal surface embedding of a graph.

Redundency is kept down by ensuring that the first two verticies of the cycle are lexicographicaly increasing, and no vertex has a lexicographical value less than the first vertex.


Genus = (\pi,\lambda) where: 

\pi_{v} is a rotation system for vertex v. 
  i.e. a cyclic permutation of the edges incident to v

\lambda is a signature mapping
  Every edge gets a sign (-1,+1)  

 \lamda(i,j) is positive IFF \pi{i} and \pi{j} are both clockwise, or both are anti-clockwise  (i.e. if you put directions on the edge from the permutation you should have opposite directions)


The embedding is orientable IFF every cycle in G has an even number of edges with negative signature. 





A facial walk is a closed walk in G determined by face traversal procedure


Pick and edge ij and store i->j in a list

look at the rotations of j and pick the next one after i.
Traverse this next edge and repeat until i->j is found again.





genus= 1- 1/2 \Chi(embedding)
where \Chi(embedding)= verts-edges+facial walks





*/

#include <stdio.h>

#define _gcomp_MAX 100

int MIN_GENUS;
short dasGraph[_gcomp_MAX][_gcomp_MAX];
int EDGES;

/*do a face traversal to get the genus*/
/*We keep track of which edges we have visited by*/
/*turning them negative.(remember to clean this)*/

/*Travel from i to j. Get the vertex next in the rotation after i*/

void printGraph(int verts)
{
  int i,j;
  
  for(i=0;i<verts;i++){
    for(j=0;j<verts;j++){
      printf("%d ",dasGraph[i][j]);
    }
    printf("\n");
  } 
  printf("\n");
}

int myabs(int i)
{
  if(i<0)
    i=i*-1;
  return i;
}
void traverse(int i,int j,int verts)
{
  int k,deg;
  while(dasGraph[i][j]>0){

    //printf("(%d,%d) ",i,j);
    /*Mark this edge as used*/
    dasGraph[i][j]*=-1;
    /*Take the next edge in the permutation for vertex j after vertex i*/
    deg=0;
    for(k=0;k<verts;k++){
      if(dasGraph[j][k])
	deg++;
    }
    for(k=0;k<verts;k++){
      if(dasGraph[j][k]>0){
	if((myabs(dasGraph[j][i])-1+deg+deg)%deg == (myabs(dasGraph[j][k])-2+deg+deg)%deg){
	  i=j;
	  j=k;
	  break;
	}
      }
    }
    //printGraph(verts);
    //printf("Now going for %d %d\n",i,j);
  }
  //printf("\n");
}

int FIND_GENUS(int verts)
{
  int i,j,faces;
  faces=0;
  //printGraph(verts);
  //printf("\n Counting faces:\n");
  for(i=0;i<verts;i++){
    for(j=0;j<verts;j++){
      if(i!=j  &&(dasGraph[i][j]>0)){
	traverse(i,j,verts);
	faces++;
      }
    }
  }
  /*Put the signs back to normal*/
  for(i=0;i<verts;i++){
    for(j=0;j<verts;j++)
      if(dasGraph[i][j]<0)
	dasGraph[i][j]*=-1;
  }
  //printf("Faces found:%d\n",faces);
  return (EDGES-verts+2-faces)/2;
}

/*Fix all possile permutations for this level*/
void GENUS_DFS(int verts,int level)
{
  int i,j,r,s,temp,degree;
  int* arr;
  int* arr2;/*An list of indexes for adjacent verticies*/

  //  printf("GENUS_DFS %d %d\n",verts,level);
  //printGraph(verts);

  if(MIN_GENUS==0)
    return;

  if(level==verts){
    temp=FIND_GENUS(verts);
    if(MIN_GENUS>temp)
      MIN_GENUS=temp;
    return;
  }

  
  arr=(int*)malloc(sizeof(int)*(verts+1));
  arr2=(int*)malloc(sizeof(int)*(verts+1));
  degree=0;
  for(i=0;i<verts;i++){
    if(dasGraph[level][i]){
      arr2[degree]=i;
      degree++;
    }
  }
  for(i=0;i<=degree;i++)
    arr[i]=i;
  i=1;
  while(i){
    for(i=1;i<=degree;i++){
      dasGraph[level][arr2[i-1]]=arr[i];
    }
   /*  printf("["); */
/*     for(i=0;i<verts;i++){ */
/*       printf("%d ",dasGraph[level][i]); */
/*     } */
/*     printf("]\n"); */

    /*Skip non-orientable embeddings?  */
    /*For vert above us make sure we dont have the same?*/
    /*             */
    GENUS_DFS(verts,level+1);
    for(i=degree-1;arr[i]>arr[i+1];i--){};
    for(j=degree;arr[i]>arr[j];j--){};
    temp=arr[i];
    arr[i]=arr[j];
    arr[j]=temp;
    r=degree;
    s=i+1;
    while(r>s){
      temp=arr[r];
      arr[r]=arr[s];
      arr[s]=temp;
      r--;s++;
    }
  }
  free(arr);
  free(arr2);
}




int main()
{
  int i,j;
  int x,y,verts;
  
  while(1){
    if(EOF==scanf("%d",&verts))
      return 0;
    if(verts>_gcomp_MAX){
      fprintf(stderr,"\nErr: GRAPH TOO BIG\n"); 
      return 1;
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
    EDGES=0;
    for(i=0;i<verts;i++){
      for(j=i+1;j<verts;j++)
	if(dasGraph[i][j])
	  EDGES++;
    }
    MIN_GENUS=EDGES;
    GENUS_DFS(verts,0);    
    printf("%d\n",MIN_GENUS);
  }
}
