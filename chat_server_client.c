#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUF_SIZE 100

int main()
{
  int pipefd[2];
  pid_t pid;
  char buf[BUF_SIZE];
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
    close(pipefd[0]);
    printf("Client: Enter Message: ");
    fgets(buf,BUF_SIZE,stdin);
    write(pipefd[1],buf,strlen(buf) + 1);
    close(pipefd[1]);
  }
  else
  {
    close(pipefd[1]);
    read(pipefd[0],buf,BUF_SIZE);
    printf("Server Recived:  %s",buf);
    close(pipefd[0]);
  }
}
