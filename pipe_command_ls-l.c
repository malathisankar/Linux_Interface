//ls -l | wc -l
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
  int pipefd[2];
  pid_t pid;
  if (pipe(pipefd) == -1)
  {
    perror("pipe()");
    exit(EXIT_FAILURE);
  }
  pid = fork();
  if (pid == -1)
  {
    perror("fork()");
    exit(EXIT_FAILURE);
  }
  if (pid == 0)
  {
    //close(pipefd[0]);
    dup2(pipefd[1],STDOUT_FILENO);
   // close(pipefd[1]);
    // int execlp(const char *file, const char *arg, .../* (char  *) NULL */);
    execlp("ls","ls","-l",NULL);
    perror("execlp()");
    exit(EXIT_FAILURE);
  }
  else
  {
    close(pipefd[1]);
    dup2(pipefd[0],STDIN_FILENO);
    //close(pipefd[0]);
    execlp("wc","wc","-l",NULL);
    perror("execlp()");
    exit(EXIT_FAILURE);
  }
  wait(NULL);
}
