//#include <love.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIGKILL 9  //Signal to kil a process
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Print the current directory
void Print_Curr_Dir () {
  char dirpath[100]; // variable to store the path to the working directory
  // get the path of the current working directory and store it in based
  getcwd(dirpath, sizeof(dirpath));
  // Print the result
  printf(":" ANSI_COLOR_GREEN "%s" ANSI_COLOR_RESET "$ ", dirpath);
}

void Get_Command_Path(char *command){
  int status;
  pid_t pid;

  pid = fork();

  if (pid != 0) waitpid(-1, &status, 0);

  else {
    // redirect the default output from stdout to the specified file: dir.txt
    freopen("dir.txt", "w", stdout);
    execl("/usr/bin/which", "which", command, NULL);
    fclose(stdout);
  }
  // kill the process if it's not over
  kill(pid, SIGKILL);
}

int main (int argc, char** argv){
  char input[100], *command, *arg, *commpath, path[100], *saveptr;
  pid_t child;
  int status;

  while(1) {
    // Print the path to the working directory
    Print_Curr_Dir();
    // get the input from stdin and store it in variable input
    fgets(input, 100, stdin);
    // tokenize the string in input into command string and argument string
    command = strtok_r(input, " \n", &saveptr);
    arg = strtok_r(NULL, " \n", &saveptr);

    // stop the program if exit
    if (strcmp(command, "exit") == 0) {
      printf(ANSI_COLOR_BLUE "Exiting Shell...\n");
      return 0;
    }

    child = fork();

    if (child != 0)
      waitpid(-1, &status, 0);

   else { 
      /* There's no binary file for cd
       * however there's a function that allows the change of the directory
       * nevertheless, when exit, it goes back to the previous directory
       * rather than stopping the program */

      if (strcmp(command, "cd") == 0){
        chdir(arg);
        continue;
      }

      Get_Command_Path(command);
      FILE *fd = fopen("dir.txt", "r");
      fgets(path, 100, fd);
      fclose(fd);
      remove("dir.txt");
      commpath = strtok(path, "\n"); // remove \n
      // Execute the command
      execl(commpath, command, arg, NULL);
    }
  }
  // Kill the process if not stopped
  kill(child, SIGKILL);
}
