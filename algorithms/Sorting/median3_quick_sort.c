#include "stdio.h"
#include "stdlib.h"

void printInit(int* array, int sizeofArray)
{
  int i = 0;
  printf("Loaded New Array:\n");
  printf("Indexes:\n");
  for(i = 0; i < sizeofArray; i++)
  {
    printf("  %d,", i);
  }
  printf("\nOriginal Array: \n");
  for(i = 0; i < sizeofArray; i++)
  {
    printf("  %d,", array[i]);
  }
  printf("\n");
}

void printPartitions(int *intArray, int start, int end, int pivot, int med,int part)
{
  int i = 0;
  switch (part) {
    case 0:
      printf("Start of partition(A, N, %d, %d)\n", start, end);
      for(i = 0; i <= end; i++)
        printf("  %d,", intArray[i]);
      printf("  A[%d]=%d,  A[%d]=%d,  A[%d]=%d\n", start, intArray[start], med, intArray[med], end, intArray[end]);
      break;
    case 1:
      for(i = 0; i <= end; i++)
        printf("  %d,", intArray[i]);
      printf("  <- updated pivot: %d\n", pivot);
      break;
    case 2:
    for(i = 0; i <= end; i++)
      printf("  %d,", intArray[i]);
    printf("  <- partitioned around pivot %d, at final index %d,\n  end of partition(A, N, %d, %d)\n", pivot, start, start, end);
      break;
  }

}

int partition(int* intArray, int start, int end)
{
  int median = (start + end)/2;
  printPartitions(intArray, start, end, 0, median, 0);
  int hold = intArray[median];
  intArray[median] = intArray[end];
  intArray[end] = hold;
  int pivot = intArray[end];
  printPartitions(intArray, start, end, pivot, median, 1);
  int t = start, j = start, temp = 0;
  for(j = start; j<=end-1; j++)
  {
    if(intArray[j] <= pivot)
    {
      temp = intArray[j];
      intArray[j] = intArray[t];
      intArray[t] = temp;
      t++;
    }
  }
  temp = intArray[t];
  intArray[t] = intArray[end];
  intArray[end] = temp;
  printPartitions(intArray, start, end, pivot, median, 2);
  return t;
}


void qs(int* intArray, int start, int end)
{
  if(start >= end)
    return;
  int pIndex = partition(intArray, start, end);
  qs(intArray, start, pIndex-1);
  qs(intArray, pIndex+1, end);
}

int main(int argc, char const *argv[])
{
  int sizeofArray = 0, count = 0, hold, i = 0;
  int* intArray = NULL;
  while(sizeofArray != -1)
  {
    count = 0;
    scanf("%d", &sizeofArray);
    if(sizeofArray != -1)
    {
      if(count == 0)
        intArray = (int*) malloc(sizeofArray*sizeof(int));

      while(count < sizeofArray)
      {
        scanf("%d", &hold);
        intArray[count] = hold;
        count++;
      }
      printInit(intArray, sizeofArray);
      qs(intArray, 0, sizeofArray-1);
      printf("Sorted Array: \n");
      for(i = 0; i < sizeofArray; i++)
      {
        printf("  %d,", intArray[i]);
      }
      printf("\n");
      free(intArray);
      intArray = NULL;
    }
    else
    {
      break;
    }
  }
  return 0;
}
