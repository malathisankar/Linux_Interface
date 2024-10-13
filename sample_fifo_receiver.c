#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUF_SIZE 128

int main(int argc, char **argv)
{
  int fd;
  char buf[BUF_SIZE];

  if (argc != 2)
  {
     printf("Usage: %s <file-name>\n",argv[0]);
     return 1;
  }
  printf("Opening FIFO for Reading\n");
  fd = open(argv[1],O_RDONLY);
  if (fd == -1)
  {
    perror("open()");
    exit(EXIT_FAILURE);
  }
  printf("Reading message from FIFO\n");
  read(fd,buf,sizeof(buf));
  printf("Recived message: %s\n",buf);
  close(fd);
  unlink(argv[1]);
}



