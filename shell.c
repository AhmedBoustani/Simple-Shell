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

#define SIGKILL 9

void Print_Curr_Dir () {
  char based[100];
  getcwd(based, sizeof(based));
  printf(":%s $ ", based);
  
}

void getcomm(char *command){
  int status;
  pid_t pid;
  pid = fork();

  if (pid != 0) waitpid(-1, &status, 0);

  else {
    freopen("dir.txt", "w", stdout);
    execl("/usr/bin/which", "which", command, NULL);
    fclose(stdout);
  }
  kill(pid, SIGKILL);
}

int main (int argc, char** argv){
  char input[100], *command, *arg, *commpath, path[100], *saveptr;
  pid_t child;
  int status;

  while(1) {
    Print_Curr_Dir();
    fgets(input, 100, stdin);
    command = strtok_r(input, " \n", &saveptr);
    arg = strtok_r(NULL, " \n", &saveptr);

    if (strcmp(command, "exit") == 0) {
      printf("Exiting Shell...\n");
      break;
   }

    child = fork();

    if (child != 0)
      waitpid(-1, &status, 0);

   else {

      if (strcmp(command, "cd") == 0){
        chdir(arg);
        continue;
      }

      getcomm(command);
      FILE *fd = fopen("dir.txt", "r");
      fgets(path, 100, fd);
      fclose(fd);
      remove("dir.txt");
      commpath = strtok(path, "\n");
      execl(commpath, command, arg, NULL);
    }
  }
  kill(child, SIGKILL);
}
