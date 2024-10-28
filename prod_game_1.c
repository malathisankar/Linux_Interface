//game_producer.c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "game.h"

int main()
{
  // int open(const char *pathname, int flags, mode_t mode);
  int fd = shm_open(SHM_NAME,O_CREAT | O_RDWR, 0666);
  //  void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
  if (fd == -1)
  {
    perror("shm_open()");
    exit(EXIT_FAILURE);
  }
  if (ftruncate(fd,SHM_SIZE) == -1)
  {
    perror("ftruncate()");
    close(fd);
    exit(EXIT_FAILURE);
  }
  struct Game *state = mmap(0,SHM_SIZE,PROT_READ | PROT_WRITE, MAP_SHARED,fd,0);
  if (state == MAP_FAILED)
  {
    perror("mmap()");
    close(fd);
    exit(EXIT_FAILURE);
  }

state->score = 0; //initialize
for (int i = 0;i < 10; i++)
{
  state->score = state->score + 10;
  printf("Updated Score: %d\n",state->score);
  sleep(1);
}
// int munmap(void *addr, size_t length);
munmap(state,SHM_SIZE);
close(fd);
shm_unlink(SHM_NAME);
}
