// Name: Gabriel de Sa
// ID: 1001676832

#define _GNU_SOURCE

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define WHITESPACE " \t\n"
// We want to split our command line up into tokens
// so we need to define what delimits our tokens.
// In this case  white space
// will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255 // The maximum command-line size

#define MAX_NUM_ARGUMENTS 11 // Mav shell only supports five arguments

char *findCommand(char *command) {
  // A function that finds what directory a command belongs too.

  return "/bin/";
}

char *testCommand(char *token[MAX_NUM_ARGUMENTS]) {
  // takes the first token and calls findCommand to find what dir it is
  // located in. If found it will return the root, or else it will return NULL.
  // This allows us to simply call function to test if argument is valid.
  if (!token[0]) {
    // this is to test whether or not the user entered without adding anything.
    return NULL;
  } else if (strcmp(token[0], "cd") == 0) {
    // if the user inputs cd, it will indicate to main to run chdir.
    return "cd";
  } else {
    return findCommand(token[0]);
  }
  return NULL;
}

int main() {

  char *cmd_str = (char *)malloc(MAX_COMMAND_SIZE);

  while (1) {
    // Print out the msh prompt
    printf("msh> ");

    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while (!fgets(cmd_str, MAX_COMMAND_SIZE, stdin)) {
    }

    /* Parse input */
    char *token[MAX_NUM_ARGUMENTS];

    int token_count = 0;

    // Pointer to point to the token
    // parsed by strsep
    char *argument_ptr;

    char *working_str = strdup(cmd_str);

    // we are going to move the working_str pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *working_root = working_str;

    // Tokenize the input strings with whitespace used as the delimiter
    while (((argument_ptr = strsep(&working_str, WHITESPACE)) != NULL) &&
           (token_count < MAX_NUM_ARGUMENTS)) {
      token[token_count] = strndup(argument_ptr, MAX_COMMAND_SIZE);
      if (strlen(token[token_count]) == 0) {
        token[token_count] = NULL;
      }
      token_count++;
    }

    // Function that checks whether or not the command inputted is supported.
    char *result = testCommand(token);
    if (!result) {
      continue;
    } else if (strcmp(result, "cd") == 0) {
      chdir(token[1]);
    } else {
      char *dir = malloc(strlen(result) + strlen(token[0]) + 1);
      strcpy(dir, result);
      strcat(dir, token[0]);
      pid_t pid = fork();
      if (pid == 0) { // use execVP
        int execValue = execl(dir, token[0], NULL, NULL, NULL, NULL, NULL, NULL,
                              NULL, NULL);
        if (execValue == -1) {
          perror("Exec Failed\n");
        }
      } else {
        int status;
        wait(&status);
      }
      free(dir);
    }
    free(working_root);
  }
  return 0;
}
