/*
Gabriel de Sa
UTA ID: 1001676832
April 13 2020 Lab 4 Recursive File Space in C
Mac OS Catalina 10.15.3
*/

//#define WINDOWS /*Uncomment to use in windows*/
#ifdef WINDOWS
#include <direct.h>
#define GetDir _getcwd
#else
#include <unistd.h>
#define GetDir getcwd
#endif
#include <stdio.h>
#include <dirent.h>
#include <string.h>

long int getFileSize(char* fileName) // This function allows us to get the size of each file.
{
  long int length = 0;
  FILE* file = fopen(fileName, "r"); // open the file with its path
  if(file == NULL)
  {
    return 0;
  }
  fseek(file, 0L, SEEK_END);  //we can use fseek and ftell to get the file size
  length += ftell(file);
  fclose(file); // close the file for memory leak sake
  return length; //return the size of the file
}

long int getTotalSize(char *path) //this function does the recursive searching of directories
{
  static long int size = 0; //a static long int allows us to keep the count through calls
  char pathBuild[FILENAME_MAX]; //FILENAME_MAX is the max size that a file can be called for the system.
  struct dirent *parent = NULL; //This is a "Object" of the dirent struck, it is a like a linked list
  DIR *start = opendir(path); //this is the DIR * stream

  while((parent = readdir(start)) != NULL) //This will iterate through the files in the stream
  {
    if(strcmp(parent->d_name, ".") == 0) //dir stream always has . and .. first
    {
      parent = readdir(start);
      parent = readdir(start);
    }
    if(parent->d_type == DT_DIR) //this tests if the file being viewed is a directory or not
    {
      strcpy(pathBuild, path); // if it is a directory we make a new path so that we can the be able to search the files in the directory.
      strcat(pathBuild, "/");
      strcat(pathBuild, parent->d_name);
      getTotalSize(pathBuild);
    }
    else
    {
      char fileName[FILENAME_MAX];
      strcpy(fileName, path);
      strcat(fileName, "/");
      strcat(fileName, parent->d_name); // if we don't concatenate the file paths, the FILE cannot find them if we just call it.
      size += getFileSize(fileName); // call getFileSize for regular files only
    }
  }
  return size; // returns the final size of the array
}


int main(int argc, char const *argv[]) {
  char temp[FILENAME_MAX]; // we can use this to get the current directory
  GetDir(temp, FILENAME_MAX); //since we can only have a certain number of characters in the path we can use FILENAME_MAX

  printf("Total size is %ld Bytes.\n", getTotalSize(temp)); //prints out the answer.

  /*
    I did extensive tests on my Mac and everytime it gave the right answer. See initial comment for operating system. 
  */

  return 0;
}
