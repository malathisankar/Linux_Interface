#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define MESSAGE "Hello from Sender!"

int main(int argc, char **argv)
{
  int fd;
  if (argc != 2)
  {
     printf("Usage: %s <file-name>\n",argv[0]);
     return 1;
  }

  if (mkfifo(argv[1],0666) == -1)
  {
    perror("mkfifo()");
    exit(EXIT_FAILURE);
  }
  printf("Opening FIFO for Writing\n");
  fd = open(argv[1],O_WRONLY);
  if (fd == -1)
  {
    perror("open()");
    exit(EXIT_FAILURE);
  }
  printf("Writing message to FIFO: %s\n",MESSAGE);
  write(fd,MESSAGE,strlen(MESSAGE) + 1);
  close(fd);
}
