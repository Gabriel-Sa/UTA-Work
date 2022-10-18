#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edit_distance.h"

/*  Suggestions
- When you work with 2D arrays, be careful. Either manage the memory yourself, or
work with local 2D arrays. Note C99 allows parameters as array sizes as long as
they are declared before the array in the parameter list. See:
https://www.geeksforgeeks.org/pass-2d-array-parameter-c/
*/

void print_distance(int rows, int columns, int distanceArray[rows][columns], char* first_string, char* second_string)
{
	int i = 0, k = 0, j = 0;
	printf("%4c %3c", '|', '|');
	for(i = 0; i < strlen(second_string); i++)
	{
			printf("%3c|", second_string[i]);
	}
	printf("\n");
	for(j = 0; j < columns + 1; j++)
		printf("----");
	printf("\n");
	for(i = 0; i < rows; i++)
	{
		if(i == 0)
			printf("%4c", '|');
		else
			printf("%3c|", first_string[i - 1]);
		for(k = 0; k < columns; k++)
		{
			printf("%3d|", distanceArray[i][k]);
		}
		printf("\n");
		for(j = 0; j < columns + 1; j++)
			printf("----");
		printf("\n");
	}
}

int min(int insert, int remove, int replace)
{
	int min = 0;
	if(insert < remove && insert < replace)
		min = insert;
	else if(remove < insert && remove < replace)
		min = remove;
	else
		min = replace;
	return min;
}

int edit_distance(char * first_string, char * second_string, int print_table){
	int i = 0;
	int k = 0;
	int dFirst = (int)strlen(first_string) + 1;
	int dSecond = (int)strlen(second_string) + 1;

	int distanceArray[dFirst][dSecond];

	distanceArray[0][0] = 0;

	for(i = 1; i < dSecond; i++)
		distanceArray[0][i] = i;

	for(i = 1; i < dFirst; i++)
	{
		distanceArray[i][0] = i;
		for(k = 1; k < dSecond; k++)
		{
			if(first_string[i-1] == second_string[k-1])
				distanceArray[i][k] = min(distanceArray[i][k-1] + 1, distanceArray[i-1][k] + 1, distanceArray[i-1][k-1]);
			else
				distanceArray[i][k] = min(distanceArray[i][k-1] + 1, distanceArray[i-1][k] + 1, distanceArray[i-1][k-1] + 1);
		}
	}

	if(print_table == 1)
		print_distance(dFirst, dSecond, distanceArray, first_string, second_string);

	return distanceArray[dFirst-1][dSecond-1];
}

FILE* getFile(FILE* fp, char* fileName) //This function will get the file name and open it.
{
  fp = fopen(fileName, "r");
  if(fp == NULL)
  {
    printf("File did not open.\n");
    return 0;
  }
  else
  {
    return fp;
  }
}

void allocateArray(char * array[], char* temp, int i)
{
		array[i] = (char*)malloc(sizeof(char*) * (int)strlen(temp));
}

void destroyArray(char* array[], int size)
{
	int i = 0;
	for(i = 0; i < size; i++)
		free(array[i]);
}

void spellcheck(char * dictname, char * testname){
	FILE* dp = NULL;
	FILE* tp = NULL;
	char  temp[101];
	int i = 0;
	int size = 0;
	int  k = 0;

	printf("\n\n");
	dp = getFile(dp, dictname);
	if(dp == NULL)
		return;
	printf("Loading the dictionary file name: %s \n", dictname);

	tp = getFile(tp, testname);
	if(tp == NULL)
		return;
	printf("Loading the test file: %s \n\n", testname);

	fgets(temp, sizeof(temp), dp);
	size = atoi(temp);
	char* dictArray[size];
	while(fgets(temp, sizeof(temp), dp))
	{
		strtok(temp, "\n");
		allocateArray(dictArray, temp, i);
		strcpy(dictArray[i],temp);
		i++;
	}
	fclose(dp);
	int minDistance = 10000;
	int holdMin = 0;
	int match[size];

	fgets(temp, sizeof(temp), tp);

	while(fgets(temp, sizeof(temp), tp))
	{
		k = 0;
		minDistance = 10000, holdMin = 0;
		strtok(temp, "\n");
		printf("------- Current test word: %s\n", temp);
		for(i = 0; i < size; i++)
		{
			holdMin = edit_distance(temp, dictArray[i], 0);
			if(holdMin < minDistance)
			{
				k = 0;
				minDistance = holdMin;
				match[k] = i;
				k++;
			} else if(minDistance == holdMin)
			{
				match[k] = i;
				k++;
			}
		}
		printf("Minimum distance: %d \nWords that give minimum distance:\n", minDistance);

		for(i = 0; i < k; i++)
		{
			printf("%s\n", dictArray[match[i]]);
			match[i] = 0;
		}
		printf("\n");
	}


	destroyArray(dictArray, size);
	fclose(tp);
}
