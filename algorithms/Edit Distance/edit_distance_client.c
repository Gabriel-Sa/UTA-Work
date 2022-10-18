#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "edit_distance.h"

/*
Repeatedly read 2 strings from the user. 
The strings must be separated by a space.
The program will compute the edit distance between them, 
print the cost table, the cost and the alignment.

After that, read 2 file names (one for a dictionary file and 
one for a test file) and spell-check all the words in the test file 
using the spellings in the dictionary file.
*/
	  
int main()
{
  // any word or file name can be at most 100 characters.
  char first[101],second[101], dictname[101], testname[101];	
  char c;  
  // Part1: Compute and show edit distance for pairs of words 
  printf("Repeatedly enter two words separated by a space (e.g.: cat someone).\n Stop with: -1 -1\n");  
  while (1==1) {
	scanf("%s %s%c", first, second, &c);
	printf("\n first: %s\n", first);
	printf("second: %s\n", second);
	if (strcmp(first,"-1") == 0 && strcmp(second,"-1") == 0) {
		break;
	} 
	else {
		int dist = edit_distance(first, second, 1);  // last argument, 1, indicates that the distance table should be printed
		printf("edit distance: %d\n", dist);
		printf("\n=========================================================================================\n");	
	}
  }
  
  // Part2: read filenames and call spell-check
  printf("Enter the dictionary file name: ");
  scanf("%s%c", dictname, &c);
  printf("Enter the test file name: ");
  scanf("%s%c", testname, &c);
  //printf("dictionary name: %s\ntest name: %s", dictname, testname);
  spellcheck(dictname, testname);   
  return 0;
}

