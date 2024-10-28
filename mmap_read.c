#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHM_NAME "/shm_mem_file"
#define SHM_SIZE 4096

int main()
{
  int shm_fd;
  char *shm_ptr;
  
  shm_fd = shm_open( SHM_NAME, O_RDONLY, 0666 );

  if ( shm_fd == -1 )
  {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }

  shm_ptr = mmap( 0, SHM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0 );

  if ( shm_ptr == MAP_FAILED )
  {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  printf("reader : content from shared memory\n");
  printf("%s\n", shm_ptr);

  munmap( shm_ptr, SHM_SIZE );
  close(shm_fd);

  shm_unlink(SHM_NAME);
  exit(EXIT_SUCCESS);

}
