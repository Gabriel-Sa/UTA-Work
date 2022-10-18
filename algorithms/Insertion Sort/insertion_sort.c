/*
 Gabriel de Sa
 1001676832
 CSE 2320 - 002
 */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"


void insertion_Sort(char *pointerArray[], int n, int givenFunction(const char* leftStr, const char* rightStr))
{
  for(int i = 1; i < n; i++)
  {
    char* min_index = pointerArray[i];
    int k = i-1;
    while(k >= 0 && (givenFunction(min_index, pointerArray[k]) < 0))
    {
      pointerArray[k+1] = pointerArray[k];
      k = k - 1;
    }
    pointerArray[k+1] = min_index;
  }
}

int cmpLength(const char* left, const char* right)
{
  int res;
  if(strlen(left) == strlen(right)) // 0 for same length.
  {
    res = 0;
  }
  if(strlen(left) < strlen(right)) //negative for shorter than
  {
    res = -1;
  }
  if(strlen(left) > strlen(right)) //positive for greater than
  {
    res = 1;
  }
  return res;
}

int cmpGreater(const char* left, const char* right)
{
  return -strcmp(left, right);
}

int cmpLL(const char* left, const char* right)
{
  int res;
  if(strlen(left) == strlen(right)) // 0 for same length.
  {
    res = strcmp(left, right);
  }
  if(strlen(left) < strlen(right)) //negative for shorter than
  {
    res = -1;
  }
  if(strlen(left) > strlen(right)) //positive for greater than
  {
    res = 1;
  }
  return res;
}

FILE* getFile(FILE* fp) //This function will get the file name and open it.
{
  char fileName[101]; //Max number of characters a file name can be.
  printf("Enter File Name: ");
  scanf("%s", fileName);
  fp = fopen(fileName, "r");
  if(fp == NULL)
  {
    printf("File did not open.\n");
    return 0;
  }
  else
  {
    printf("\tData is Loading. \n");
    return fp;
  }
}

int getSize(FILE* fp, int n)
{
  char nameHold[101];
  n = 0;

  while(fgets(nameHold, sizeof(nameHold), fp))
  {
    n++;
  }
  rewind(fp);
  return n;
}

void allocateMemory(FILE* fp, char* stringArray[], char* pointerArray[], int n)
{
  char nameTemp[101];
  int charCount = 0;
  int i = 0;

  while(fgets(nameTemp, sizeof(nameTemp), fp))
  {
    /*
    This next step had to be done because the file that was given had \r\n after each word.
    While the last word did not have anything after it. After Grace there was no \r\n.
    If there is a new line character after grace the program will print exactly what the program
    example printed. That 201 characters were used and 30 pointers.
    */

    if((nameTemp[(int)strlen(nameTemp) - 1] == '\r' || nameTemp[(int)strlen(nameTemp) - 1] == '\n'))
    {
      nameTemp[(int)strlen(nameTemp) - 1] = '\0';
      if(nameTemp[(int)strlen(nameTemp) - 1] == '\r' || nameTemp[(int)strlen(nameTemp) - 1] == '\n')
      {
        nameTemp[(int)strlen(nameTemp) - 1] = '\n';
      }
      else if (nameTemp[(int)strlen(nameTemp)] == '\0')
      {
        nameTemp[(int)strlen(nameTemp)] = '\n';
      }
      charCount = charCount + (int)strlen(nameTemp);
      stringArray[i] = malloc(((int)strlen(nameTemp)+1)*sizeof(char));
      strcpy(stringArray[i], nameTemp);
    }
    else
    {
      if(nameTemp[(int)strlen(nameTemp)] != '\n' || nameTemp[(int)strlen(nameTemp)] != '\r')
      {
        nameTemp[(int)strlen(nameTemp)] = '\n';
      }
      charCount = charCount + strlen(nameTemp);
      stringArray[i] = malloc(((int)strlen(nameTemp)+1)*sizeof(char));
      strcpy(stringArray[i], nameTemp);
    }
    i++;
  }
  printf("\t100%% Data Loaded \n");
  printf("\tAllocated space for storing data: %d B (%d chars and %d pointers)\n", (charCount + n*(8)),charCount, n);
  printf("\tPointer Size: %lu Bytes\n", sizeof(int*));
  for(int i = 0; i < n; i++)
  {
    pointerArray[i] = stringArray[i];
  }
  fclose(fp);
}

void printData(char* pointerArray[], int n)
{
  for(int i = 0; i < n; i++)
  {
    printf("%s", pointerArray[i]);
  }
}

void release(char* array[], int n)
{
  for(int i = 0; i < n; i++)
  {
    free(array[i]);
    array[i] = NULL;
  }
}

int main(int argc, char const *argv[])
{
  FILE* fp = NULL;
  int n = 0;
  fp = getFile(fp);
  if(fp == NULL)
    return 0;
  else
  {
    n = getSize(fp, n);
    char* stringArray[n];
    char* pointerArray[n];
    allocateMemory(fp, stringArray, pointerArray, n);
    printf("\nOriginal Data:\n");
    printData(pointerArray, n);
    printf("\n-------------- compare by LENGTH only ---------------\n");
    insertion_Sort(pointerArray, n, cmpLength);
    printData(pointerArray, n);

    printf("\n-------------- compare by STRCMP only ---------------\n");
    insertion_Sort(pointerArray, n, strcmp);
    printData(pointerArray, n);

    printf("\n------------- compare by GREATER only ---------------\n");
    insertion_Sort(pointerArray, n, cmpGreater);
    printData(pointerArray, n);

    printf("\n-------- compare by LENGTH and LEXICOGRAPHIC ---------\n");
    insertion_Sort(pointerArray, n, cmpLL);
    printData(pointerArray, n);

    release(stringArray, n);
  }
  return 0;
}
