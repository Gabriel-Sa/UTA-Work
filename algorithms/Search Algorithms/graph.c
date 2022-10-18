/*
Gabriel de Sa
UTA ID: 1001676832
Homework 8  
*/

#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"

enum colors
{
  WHITE = -1,
  GREY = 0,
  BLACK = 1,
  EMPTY = 2,
  INF = 100000000
};

void getUserInput(int* weighted, int* startingVertex, int* numVertices)
{
  printf("Is the graph a weighted graph (enter 1 for yes): ");
  scanf("%d", weighted );
  printf("Enter starting vertex for MST: " );
  scanf("%d", startingVertex);
  printf("Enter Number of Vertices, N: ");
  scanf("%d", numVertices);
}

void allocateArray(char* verticesArray[], int i, char nameTemp[101])
{
  verticesArray[i] = (char*)malloc(sizeof(char*) * (int)strlen(nameTemp));
  strcpy(verticesArray[i], nameTemp);
}

void freeAllocated(char* verticesArray[], int numVertices)
{
  int i = 0;
  for(i = 0; i < numVertices; i++)
    free(verticesArray[i]);
}

int isIn(char name[101], int N, char* verticesArray[])
{
  int i = 0;
  for(i = 0; i < N; i++)
  {
    if(strcmp(name, verticesArray[i]) == 0)
      return i;
  }
  return -1;
}

void getConnections(int N, int weighted, int w2D[][N], int e2D[][N], char* verticesArray[])
{
  int i = 0, k = 0, tempHold = 0;
  char name1[101] = " ", name2[101] = " ", weight[101] = " ";
  // Get names for verticesArray
  for(i = 0; i < N; i++)
  {
    printf("Enter Name: ");
    scanf("%s", name1);
    allocateArray(verticesArray, i, name1);
  }

  // Fill in zeros for both arrays
  for(i = 0; i < N; i++)
  {
    for(k = 0; k < N; k++)
    {
      e2D[i][k] = 0;
      w2D[i][k] = 0;
    }
  }
  // get input for edges
  if(weighted == 1)
  {
    while(strcmp(name1, "-1") != 0 && strcmp(name2, "-1") != 0 && strcmp(weight, "-1") != 0)
    {

      if(strcmp(name1, "-1") == 0 && strcmp(name2, "-1") == 0 && strcmp(weight, "-1") == 0)
        break;

      printf("Enter name1 name2 weight: ");
      scanf("%s %s %s", name1, name2, weight);

      tempHold = isIn(name1, N, verticesArray);
      if(tempHold != -1)
      {
        for(k = 0; k < N; k++)
        {
          if(strcmp(name2, verticesArray[k]) == 0)
          {
            e2D[tempHold][k] = 1;
            e2D[k][tempHold] = 1;
            w2D[tempHold][k] = atoi(weight);
            w2D[k][tempHold] = atoi(weight);
          }
        }
      }
    }
  }
  else
  {
    while(strcmp(name1, "-1") != 0 && strcmp(name2, "-1") != 0)
    {
      printf("Enter name1 name2: ");
      scanf("%s %s", name1, name2);

      if(strcmp(name1, "-1") == 0 && strcmp(name2, "-1") == 0)
        break;

      tempHold = isIn(name1, N, verticesArray);
      if(tempHold != -1)
      {
        for(k = 0; k < N; k++)
        {
          if(strcmp(name2, verticesArray[k]) == 0)
          {
            e2D[tempHold][k] = 1;
            e2D[k][tempHold] = 1;
          }
        }
      }
    }
  }
}

void printArray(int N, int array[][N])
{
  int i = 0, k = 0;
  printf("%4s|", " ");
  for(i = 0; i < N; i++)
    printf("%4d", i);

  printf("\n");
  for(i = 0; i < N; i++)
  {
    printf("\n%4d|", i);
    for(k = 0; k < N; k++)
    {
      printf("%4d", array[i][k]);
    }
  }
  printf("\n");
}

void dfs_visit(int N, int i, int hold[], int visited[], int e2D[N][N])
{
  visited[i] = GREY;
  static int count = 0;
  hold[count] = i;
  count++;
  int k = 0;
  for(k = 0; k < N; k++)
  {
    if(visited[k] == WHITE && e2D[i][k] == 1)
    {
      dfs_visit(N, k, hold, visited, e2D);
    }
  }

  visited[i] = BLACK;
}

void dfs(int N, int e2D[N][N], char* verticeArray[N])
{
  int groupCount = 0, hold[N], groupNum = 1, i = 0, k = 0;
  int visited[N], groupBreakIndex[N];
  printf("\nRunning DFS...\nFinished DFS\n");
  for(i = 0; i < N; i++)
  {
    visited[i] = WHITE;
  }

  for(i = 0; i < N; i++)
  {
    if(visited[i] == WHITE)
    {
      dfs_visit(N, i, hold, visited, e2D);
      groupBreakIndex[groupCount] = i;
      groupCount++;
    }
  }
  printf("\nNumber of groups: %d\n", groupCount);
  for(i = 1; i < groupCount; i++)
  {
    printf("\nGroup: %d\n", groupNum);
    while(hold[k] != groupBreakIndex[i])
    {
      printf("%s\n", verticeArray[hold[k]]);
      k++;
    }
    groupNum++;
  }
  if(groupNum == groupCount)
  {
    printf("\nGroup: %d\n", groupNum);
    while(k < N)
    {
      printf("%s\n", verticeArray[hold[k]]);
      k++;
    }
  }
}

void MST_Prim(int N, int w2D[N][N], int start, int e2D[N][N])
{
  int d[N], p[N], mst[N], i = 0, k = 0, minVal = INF, minVertex = -1, totalCost = 0;
  for(i = 0; i < N; i++)
  {
    d[i] = INF;
    p[i] = -1;
    mst[i] = WHITE;
  }
  d[start] = 0;
  mst[start] = BLACK;
  for(i = 0; i < N; i++)
  {
    minVal = INF;
    minVertex = start;
    for(k = 0; k < N; k++)
    {
      if(mst[k] == WHITE && d[k]<minVal)
      {
        minVal = d[k];
        minVertex = k;
      }
    }
    if(minVertex == -1)
    {
      break;
    }
    else
    {
      mst[minVertex] = BLACK;
    }
    mst[minVertex] = BLACK;
    for(k = 0; k < N; k++)
    {
      if(e2D[minVertex][k] == 1 && d[k] > w2D[minVertex][k] && mst[k] == WHITE)
      {
        d[k] = w2D[minVertex][k];
        p[k] = minVertex;
      }
    }
    printf("(%3d,%3d,%3d)\n", p[minVertex], minVertex, d[minVertex]);
    totalCost += d[minVertex];
  }
  printf("Total MST cost: %d\n", totalCost);
}

int main(int argc, char* argv[])
{
  int weighted = 0, startingVertex = 0, numVertices = 0, i = 0; // k = 0;
  getUserInput(&weighted, &startingVertex, &numVertices);
  int e2D[numVertices][numVertices], w2D[numVertices][numVertices];
  char* verticesArray[numVertices];

  getConnections(numVertices, weighted, w2D, e2D, verticesArray);

  printf("\nGraph:\nWeighted = %d\nN = %d\n", weighted, numVertices);

  for(i = 0; i < numVertices; i++)
    printf("%d-%s\n", i, verticesArray[i]);
  if(weighted == 1)
  {
    printf("\nEdges 2D Matrix:\n");
    printArray(numVertices, e2D);
    printf("\nWeights 2D Matrix:\n");
    printArray(numVertices, w2D);
  }
  else
  {
    printf("\nEdges 2D Matrix:\n");
    printArray(numVertices, e2D);
  }

  dfs(numVertices, e2D, verticesArray);
  if(weighted != 1)
  {
    printf("\nThis is not a weighted graph. We will NOT run MST_Prim for it.\n");
  }
  else
  {
    printf("\nRunning MST_Prim...\nMST tree edges: \n");
    MST_Prim(numVertices, w2D, startingVertex, e2D);
  }

  freeAllocated(verticesArray, numVertices);
  return 0;
}
