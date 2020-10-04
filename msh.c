/*
 Name: Gabriel de Sa
 ID: 1001676832
*/

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

#define MAX_NUM_ARGUMENTS 11 // Mav shell only supports 10 command line params

// This is a version of chdir() that is called everytime we need to go back to
// The history file. It returns nothing, and simply executes a series of
// chdir() to get back to the initial directory that the shell started on.
void _chdir() {
  static int i = 0;
  static char *user;
  static char root[MAX_COMMAND_SIZE];
  if (i == 0) {
    // if the start directory is not in a place that is writable, proceed to
    // the home directory in linux.
    FILE *history = fopen(".msh_history", "a+");
    if (history == NULL) {
      // catch if fopen fails. then use backup directory
      strcpy(root, "/home/");
      user = getenv("USER");
      strcat(root, user);
    } else {
      // if fopens works, get the current directory and create a history file.
      // this way we are always garanteed to have a history file.
      getcwd(root, sizeof(root));
      fclose(history);
    }
    i++;
  }
  // Need to go to root directory first, then go to where history is.
  chdir("/");
  chdir(root);
}

// testCommand is called everytime enter is inputted into the command line
// it allows the system to test what has been inputted. It returns a
// string with the input type, if nothing or unknown is inputted it returns
// null.
char *testCommand(char *token[MAX_NUM_ARGUMENTS]) {
  // takes the first token and calls findCommand to find what dir it is
  // located in. If found it will return the root, or else it will return
  // NULL. This allows us to simply call function to test if argument is
  // valid.
  if (!token[0]) {
    // this is to test whether or not the user entered without adding
    // anything.
    return NULL;
  } else if (strcmp(token[0], "cd") == 0) {
    // if the user inputs cd, it will indicate to main to run chdir.
    return "cd";
  } else if (strcmp(token[0], "history") == 0) {
    if (token[1] != NULL) {
      if (strcmp(token[1], "-a") == 0) {
        // if history -a is called it prints the entire history file.
        return "history -a";
      }
    }
    return "history";
  } else if (strcmp(token[0], "exit") == 0 || strcmp(token[0], "quit") == 0) {
    return "quit";
  } else if (token[0][0] == '!') {
    return "!";
  } else if (strcmp(token[0], "showpids") == 0) {
    return "showpids";
  } else {
    // return input if token[0] exists and matched no other params.
    return token[0];
  }
  return NULL;
}

// saveHistory() takes in the token array, and saves it to a file .msh_history.
void saveHistory(char *token[MAX_NUM_ARGUMENTS]) {
  char path[MAX_COMMAND_SIZE];
  // First we need to get what current directory we are in, and save it becuase
  // _chdir() will change dir to wherever .msh_history is.
  getcwd(path, sizeof(path));
  _chdir();
  // open with a+ to simply append to end of file.
  FILE *history = fopen(".msh_history", "a+");
  int i = 0;
  while (token[i] != NULL) {
    fputs(token[i], history);
    fputs(" ", history);
    i++;
  }
  fputs("\n", history);
  fclose(history);
  // return to current directory.
  chdir(path);
}

// printHistory will take an option, if history -a is called, option should be
// 1, if not proceed to how history should be displayed per assignements
// insturctions
void printHistory(int option) {
  char path[MAX_COMMAND_SIZE];
  getcwd(path, sizeof(path));
  _chdir();
  FILE *history = fopen(".msh_history", "r");
  int lineNum = 0, i = 0;
  char line[MAX_COMMAND_SIZE];
  if (option == 0) {
    // we need to count the number of lines in order to display only the last 15
    // commands.
    while (fgets(line, MAX_COMMAND_SIZE, history)) {
      lineNum++;
    }
    // now we can rewind to the top of the file and read from there.
    rewind(history);
    while (fgets(line, MAX_COMMAND_SIZE, history)) {
      // this ensures that if there are less than 15 commands itll only print
      // the first nth commands.
      if (i >= lineNum - 15 || lineNum < 15) {
        printf("%d: %s", i, line);
      }
      i++;
    }
  } else {
    // if history -a is called then print the entire history.
    while (fgets(line, MAX_COMMAND_SIZE, history)) {
      printf("%d: %s", i, line);
      i++;
    }
  }
  fclose(history);
  chdir(path);
}

// recallCommand takes in the string of the !n number is that is being recalled,
// and the token[] array so that we can add any options to token[1 - n]
// it also returns a int depending on wheter or not it was successful in finding
// the command in history.
int recallCommand(char *num, char *commands[MAX_NUM_ARGUMENTS]) {
  char commandNumber[MAX_COMMAND_SIZE];
  int i = 1, index = 0, buffI = 0, it = 0, found = 0;
  // we need to seperate the ! from the line number in order to match it in
  // history.
  while (num[i] != '\0') {
    commandNumber[i - 1] = num[i];
    i++;
  }
  // we can then make it into an int, and proceed from there to iterate through
  // the file and once it reaches that line number recall the command.
  commandNumber[i] = '\0';
  int lineNum = atoi(commandNumber);
  char path[MAX_COMMAND_SIZE];
  getcwd(path, sizeof(path));
  _chdir();
  FILE *history = fopen(".msh_history", "r");
  char line[MAX_COMMAND_SIZE];
  while (fgets(line, MAX_COMMAND_SIZE, history)) {
    if (lineNum == it) {
      found = 1;
      break;
    }
    it++;
  }
  if (found == 0) {
    // if the line number does not match one that exists in the file, return
    // with -1 to indicate failure.
    return -1;
  }
  i = 0;
  // create a buff so that we can keep each section of the command, and place it
  // into token[i](denoted commands[i])
  char buff[MAX_COMMAND_SIZE];
  while (line[i] != '\n') {
    if (line[i] == ' ') {
      buff[buffI] = '\0';
      commands[index] = strdup(buff);
      memset(buff, 0, sizeof buff);
      buffI = 0;
      index++;
    } else {
      buff[buffI] = line[i];
      buffI++;
    }
    i++;
  }
  // printout the command, and proceed in main. Return 0 to indicate success.
  printf("%s", line);
  fclose(history);
  chdir(path);
  return 0;
}

int main() {
  char *cmd_str = (char *)malloc(MAX_COMMAND_SIZE);
  // this pids array will keep the last 15 pids in memory.
  pid_t pids[15];
  int index = 0;
  for (int i = 0; i < 15; i++) {
    pids[i] = 0;
  }

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
    // we need to skip everything if there is no result ( user pressed enter)
    if (!result) {

    } else {
      if (strcmp(result, "!") == 0) {
        int out = recallCommand(token[0], token);
        if (out == -1) {
          printf("Command not in history.\n");
          free(working_root);
          continue;
        }
        result = testCommand(token);
      }
      // after we've checked that the command is a recall or not, we can save
      // token[] to history because recall will fill token with whatever command
      // was originally called. This way we do not have !n  in history.
      saveHistory(token);
      if (strcmp(result, "quit") == 0) {
        // quit indicated exit/quit. Memory needs to be cleared. so
        // free(working_root) minimizes memory leaks
        printf("Goodbye!\n");
        free(working_root);
        break;
      } else if (strcmp(result, "showpids") == 0) {
        // if the command entered is showpids, we can just print the pids array.
        int i = 0;
        while (pids[i] != 0) {
          printf("%d: %d\n", i, pids[i]);
          i++;
        }
      } else if (strcmp(result, "cd") == 0) {
        // here we take the directory inputted and change to it using chdir.
        // chdir takes a const char* so in order to have an accurate transition
        // I made sure that the pointer to token[1] was constant.
        const char *path = token[1];
        chdir(path);
      } else if (strcmp(result, "history") == 0) {
        // print last 15 history.
        printHistory(0);
      } else if (strcmp(result, "history -a") == 0) {
        // print all history
        printHistory(1);
      } else {
        // now after all these conditions we know that the command entered is
        // not part of msh, so it needs to be called externally. In order to do
        // this we fork() from the parent and enter a child process, where the
        // child process then calls execvp to proceed with the command.
        pid_t pid = fork();
        if (pid == 0) {
          // inside the child process we execvp because execvp will check the
          // current directory and most other directories for the command
          // issued.
          int execValue = execvp(token[0], token);
          if (execValue == -1) {
            // if exec fails that means that it couldnt find the command so
            // alert the user, free working_root and exit.
            printf("%s: Command not found.\n", token[0]);
            free(working_root);
            exit(0);
          }
          // if exec works, exit();
          exit(0);
        } else {
          // the parent has to wait for the child to finish so with wait(), we
          // can do that until the child process dies.
          int status;
          wait(&status);
          if (index < 15) {
            // if there are less than 14 pids, simply add the process to the
            // pids array, if not we need to move each pid down one then add the
            // newest one at the end.
            pids[index] = pid;
            index++;
          } else {
            for (int i = 0; i < 14; i++) {
              pids[i] = pids[i + 1];
            }
            pids[14] = pid;
          }
        }
      }
    }
    free(working_root);
  }
  return 0;
}
