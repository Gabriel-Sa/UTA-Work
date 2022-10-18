// The MIT License (MIT)
//
// Copyright (c) 2020 Trevor Bakker & Gabriel de Sa
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>

#define MAX_NUM_ARGUMENTS 4

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

struct BPB_Block
{
  int16_t BPB_BytsPerSec;
  int8_t BPB_SecPerClus;
  int16_t BPB_RsvdSecCnt;
  int8_t BPB_NumFATS;
  int16_t BPB_RootEntCnt;
  int32_t BPB_FATSz32;
  int32_t BPB_RootClus;
} bpb_Block;

struct __attribute__((__packed__)) DirectoryEntry
{
  char DIR_Name[11];
  uint8_t DIR_Attr;
  uint8_t Unused1[8];
  uint16_t DIR_FirstClusterHigh;
  uint8_t Unused2[4];
  uint16_t DIR_FirstClusterLow;
  uint32_t DIR_FileSize;
} dir[16];

// Fills the BPB block with information from the file system bios param block.
int fillStructs(FILE* image)
{
  fseek(image, 11, SEEK_SET);
  fread(&bpb_Block.BPB_BytsPerSec, 2, 1, image);
  fseek(image, 13, SEEK_SET);
  fread(&bpb_Block.BPB_SecPerClus, 1, 1, image);
  fseek(image, 14, SEEK_SET);
  fread(&bpb_Block.BPB_RsvdSecCnt, 2, 1, image);
  fseek(image, 16, SEEK_SET);
  fread(&bpb_Block.BPB_NumFATS, 1, 1, image);
  fseek(image, 17, SEEK_SET);
  fread(&bpb_Block.BPB_RootEntCnt, 2, 1, image);
  fseek(image, 36, SEEK_SET);
  fread(&bpb_Block.BPB_FATSz32, 4, 1, image);
  fseek(image, 44, SEEK_SET);
  fread(&bpb_Block.BPB_RootClus, 4, 1, image);
  int rootAddress = (bpb_Block.BPB_NumFATS * bpb_Block.BPB_FATSz32 * bpb_Block.BPB_BytsPerSec)
  +(bpb_Block.BPB_RsvdSecCnt * bpb_Block.BPB_BytsPerSec);
  fseek(image, rootAddress, SEEK_SET);
  fread(dir, sizeof(struct DirectoryEntry), 16, image);
}

// Taken from compare.c provided with the project GitHub.
int compare(char* userIn, char* directory)
{
  char * prevDir = "..";
  if(strncmp(prevDir, userIn, 2) == 0)
  {
    if(strncmp(prevDir, directory, 2) == 0)
    {
      return 1;
    }
    return 0;
  }
  char normalized[12];
  strncpy(normalized, directory, 11);
  normalized[11] = '\0';

  char input[12];
  memset(input, 0, 11);
  strncpy(input, userIn, strlen(userIn));


  char expanded_name[12];
  memset( expanded_name, ' ', 12 );

  char *token = strtok( input, "." );

  strncpy( expanded_name, token, strlen( token ) );

  token = strtok( NULL, "." );

  if( token )
  {
    strncpy( (char*)(expanded_name+8), token, strlen(token ) );
  }

  expanded_name[11] = '\0';

  int i;
  for( i = 0; i < 11; i++ )
  {
    expanded_name[i] = toupper( expanded_name[i] );
  }

  if( strncmp( expanded_name, normalized, 11 ) == 0 )
  {
    return 1;
  }

  return 0;
}

// Given a logical block address, loop up into the first FAT and return the logical block address
// of the block in the file. If there is no futhur blocks then return -1.
int16_t nextLB(uint32_t sector, FILE* image)
{
    uint32_t FATAddress = (bpb_Block.BPB_BytsPerSec * bpb_Block.BPB_RsvdSecCnt) + (sector * 4);
    int16_t val;
    fseek(image, FATAddress, SEEK_SET);
    fread(&val, 2, 1, image);
    return val;
}

// Finds the starting address of a block of data given the sector number.
int LBAToOffset(int32_t sector)
{
  return (bpb_Block.BPB_BytsPerSec * (sector - 2))
  + (bpb_Block.BPB_BytsPerSec * bpb_Block.BPB_RsvdSecCnt)
  + (bpb_Block.BPB_BytsPerSec * bpb_Block.BPB_NumFATS * bpb_Block.BPB_FATSz32);
}

// Reads from the given file at the position, in bytes, specified by the
// position parameter and output the number of bytes specified. Output the num
// of bytes in hexadecimal.
int fatRead(char * fileName, int position, int numBytes, FILE* image)
{
  if(position < 0)
  {
    printf("Position cannot be less than 0\n");
    return -1;
  }
  int i = 0, found = 0, cluster = 0, size = 0, offSet = 0, byteOffSet = 0;
  int bytesLeft = numBytes;
  for(i = 0; i < 16 ; i++)
  {
    if(compare(fileName, dir[i].DIR_Name) == 1)
    {
      cluster = dir[i].DIR_FirstClusterLow;
      size = position;
      while(size >= bpb_Block.BPB_BytsPerSec)
      {
        cluster = nextLB(cluster, image);
        size = size - bpb_Block.BPB_BytsPerSec;
      }
      offSet = LBAToOffset(cluster);
      byteOffSet = (position % bpb_Block.BPB_BytsPerSec);
      fseek(image, offSet + byteOffSet, SEEK_SET);

      unsigned char buff[bpb_Block.BPB_BytsPerSec];
      int firstBlock = bpb_Block.BPB_BytsPerSec - position;
      fread(buff, 1, firstBlock, image);
      for( i = 0; i < firstBlock; i++)
      {
        printf("%x ", buff[i]);
      }

      bytesLeft -= bpb_Block.BPB_BytsPerSec;

      while(bytesLeft >= 512)
      {
        cluster = nextLB(cluster, image);
        offSet = LBAToOffset(cluster);
        fseek(image, offSet, SEEK_SET);
        fread(buff, 1,bpb_Block.BPB_BytsPerSec, image);
        for( i = 0; i < bpb_Block.BPB_BytsPerSec; i++)
        {
          printf("%x ", buff[i]);
        }
        bytesLeft -= bpb_Block.BPB_BytsPerSec;
      }
      if(bytesLeft)
      {
        cluster = nextLB(cluster, image);
        offSet = LBAToOffset(cluster);
        fseek(image, offSet, SEEK_SET);
        fread(buff, 1,bpb_Block.BPB_BytsPerSec, image);
        for(i = 0; i < bytesLeft; i++)
        {
          printf("%x\n", buff[i]);
        }
      }
      printf("\n");
    }
  }

}

// Lists the contents in the current directory.
int ls()
{
  int i;
  for(i = 0; i < 16; i++)
  {
    char name[12];
    strncpy(name, dir[i].DIR_Name, 11);
    name[11] = '\0';
    if((dir[i].DIR_Attr == 0x01 || dir[i].DIR_Attr == 0x10 || dir[i].DIR_Attr == 0x20) &&
        name[0] != 0xffffffe5)
    {
      printf("%s\n", name);
    }
  }
  return 0;
}


// change the current working directory to a given directory.
// if the directory doesn't exists then print
// "cd: directory: no such file or directory."
// Supports relative and absolute paths.
int cd(char * directory, FILE* image)
{
  int i = 0, offset=0, cluster, found = 0;
  for(i = 0; i < 16; i++)
  {
    if(compare(directory, dir[i].DIR_Name) == 1 && dir[i].DIR_Attr == 0x10)
    {
      cluster = dir[i].DIR_FirstClusterLow;
      if(cluster == 0)
      {
        cluster = 2;
      }
      offset = LBAToOffset(cluster);
      fseek(image, offset, SEEK_SET);
      fread(dir, sizeof(struct DirectoryEntry), 16, image);
      found = 1;
      break;
    }
  }
  if(found == 0)
  {
    printf("ERROR: %s is not a directory. \n", directory);
  }
}

// Take the file from the fat32 image and place it in the current directory.
// If the file doesn't exist print error file not found.
// If the user passes in a non null for newFIleName then take that file and
// rename it to the newFileName.
int get(char* fileName, char* newFileName)
{
  FILE* copy;
  if(newFileName == NULL)
  {
    copy = fopen(fileName, "w");
  }
  else
  {
    copy = fopen(newFileName, "w");
  }
  return 0;
}

// Stat will print the attributes and starting cluster number of the file or
// directory name. If the parameter is a directory then the size is 0.
// If the file or directory doesn't exist it'll print "Error: FIle not found."
int stat(char* name)
{
  int i = 0;
  for(i = 0 ; i < 16; i++)
  {
    if(compare(name, dir[i].DIR_Name) == 1)
    {
      printf("%-10s%10s%35s\n", "File Attribute", "Size", "Starting Cluster Number");
      printf("%-10d%14d%16d\n", dir[i].DIR_Attr, dir[i].DIR_FileSize, dir[i].DIR_FirstClusterLow);
      break;
    }
  }
  return 0;
}

// Prints out information about the file system in both hexadecimal and base 10
int bpb()
{
  // Prints in both Hex and Decimal
  //Bytes per sector
  printf("BPB_BytsPerSec:\t%d\n", bpb_Block.BPB_BytsPerSec);
  printf("BPB_BytsPerSec:\t0x%x\n\n", bpb_Block.BPB_BytsPerSec);

  //Sectors Per Cluster
  printf("BPB_SecPerClus:\t%d\n", bpb_Block.BPB_SecPerClus);
  printf("BPB_SecPerClus:\t0x%x\n\n", bpb_Block.BPB_SecPerClus);

  // Reserved Sector Count
  printf("BPB_RsvdSecCnt:\t%d\n", bpb_Block.BPB_RsvdSecCnt);
  printf("BPB_RsvdSecCnt:\t0x%x\n\n", bpb_Block.BPB_RsvdSecCnt);

  // Number of Fats
  printf("BPB_NumFATS:\t%d\n", bpb_Block.BPB_NumFATS);
  printf("BPB_NumFATS:\t0x%x\n\n", bpb_Block.BPB_NumFATS);

  // The fat32 32-bit count of secotrs occupied by one FAT.
  printf("BPB_FATSz32:\t%d\n", bpb_Block.BPB_FATSz32);
  printf("BPB_FATSz32:\t0x%x\n\n", bpb_Block.BPB_FATSz32);

  return 0;
}

// Opens the fat32 image.
// File names of fat32 images shall not contian spaces and shall be limited to
// 100 characters.
FILE* open(char* imageName)
{
  FILE *fOpen = fopen(imageName, "r+");
  if(fOpen == NULL)
  {
    printf("Failed to open file %s\n", imageName);
    return NULL;
  }
  else
  {
    return fOpen;
  }
}

int main()
{
  FILE *image;
  int fileOpen = 0;
  char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );

  while( 1 )
  {
    // Print out the mfs prompt
    printf ("mfs> ");

    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );

    /* Parse input */
    char *token[MAX_NUM_ARGUMENTS];

    int   token_count = 0;

    // Pointer to point to the token
    // parsed by strsep
    char *arg_ptr;
    char *working_str  = strdup( cmd_str );

    // we are going to move the working_str pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *working_root = working_str;

    // Tokenize the input stringswith whitespace used as the delimiter
    while ( ( (arg_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) &&
              (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( arg_ptr, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 )
      {
        token[token_count] = NULL;
      }
        token_count++;
    }

    // Implementing checks to see what inputs has been entered.
    int token_index  = 0;
    if(!token[0]) continue;
    if(strcmp(token[0], "exit") == 0)
    {
      if(fileOpen==1)
      {
        fclose(image);
      }
      free(working_root);
      break;
    }
    if(strcmp(token[0], "open") == 0)
    {
      if(token_count > 2 && fileOpen != 1)
      {
        image = open(token[1]);
        if(image)
        {
          fileOpen = 1;
          fillStructs(image);
          free(working_root);
          continue;
        }
      }
      else
      {
        if(fileOpen == 1)
        {
          printf("Error: File System image already open.\n");
          free(working_root);
          continue;
        }
        else
        {
          printf("Specify an image to mount. \n");
          free(working_root);
          continue;
        }
      }
    }
    if(fileOpen == 1)
    {
      if(strcmp(token[0], "close") == 0)
      {
        int testClosed = fclose(image);
        if(testClosed == 0)
        {
          fileOpen = 0;
          printf("unmounted\n");
        }
        else
        {
          printf("Error closing file!\n");
        }
      }
      else if(strcmp(token[0], "bpb") == 0)
      {
        bpb();
      }
      else if(strcmp(token[0], "stat") == 0)
      {
        if(token_count > 2 && token[1])
        {
          stat(token[1]);
        }
        else
        {
          printf("Error: Enter the file/directory name. \n");
        }
      }
      else if(strcmp(token[0], "get") == 0)
      {
        if(token_count > 2 && token[1])
        {
          get(token[1], NULL);
        }
        if(token_count > 2 && token[2])
        {
          get(token[1], token[2]);
        }
        else
        {
          printf("Error: Enter the file/directory name. \n");
        }
      }
      else if(strcmp(token[0], "cd") == 0)
      {
        if(token_count > 2)
        {
          cd(token[1], image);
        }
        else
        {
          printf("Error: Enter the directory name. \n");
        }
      }
      else if(strcmp(token[0], "ls") == 0)
      {
        ls();
      }
      else if(strcmp(token[0], "read") == 0)
      {
        if(token_count > 2 && token[1] && token[2])
        {
          fatRead(token[1], atoi(token[2]), atoi(token[3]), image);
        }
        else
        {
          printf("Error: Enter the file/directory name. \n");
        }
      }
      else
      {
        printf("Error: Command Not Found\n");
      }
    }
    else
    {
      if(strcmp(token[0], "close") == 0)
      {
        printf("Error: File system image must be mounted first.\n" );
      }
      else
      {
        printf("Error: File System not mounted.\n" );
      }
    }

    free( working_root );

  }
  return 0;
}
