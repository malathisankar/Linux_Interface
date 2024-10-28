//game_consumer
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include "game.h"

int main()
{
  // int open(const char *pathname, int flags, mode_t mode);
  int fd = shm_open(SHM_NAME,O_RDONLY,0666);
  if (fd == -1)
  {
    perror("shm_open()");
    exit(EXIT_FAILURE);
  }
  struct Game *state = mmap(0,SHM_SIZE,PROT_READ,MAP_SHARED,fd,0);
  if (state == MAP_FAILED)
  {
    perror("mmap()");
    exit(EXIT_FAILURE);
  }
  while (1)
  {
    printf("Current Score: %d\n", state->score);
    if (state->score >= 100) 
    { 
      printf("Reached exit condition. Exiting.\n");
      break;
    }
    sleep(1);  
  }
  // int munmap(void *addr, size_t length);
  munmap(state,SHM_SIZE);
  close(fd);
}






