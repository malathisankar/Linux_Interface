#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SHM_NAME "/shm_mem_file"
#define SHM_SIZE 4096

int main()
{
  int shm_fd;
  char *shm_ptr;
  char buf[SHM_SIZE];
  size_t bytes_read;

  FILE *file = fopen("input.txt", "r");

  if ( !file )
  {
    perror("file_open");
    exit(EXIT_FAILURE);
  }

  bytes_read = fread( buf, 1, SHM_SIZE, file );   

  fclose(file);

  shm_fd = shm_open( SHM_NAME, O_CREAT | O_RDWR, 0666);
  
  if ( shm_fd == -1 )
  {
    perror("shm_open");
    exit(EXIT_FAILURE);
  }

  ftruncate ( shm_fd, SHM_SIZE );

  shm_ptr = mmap( 0, SHM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0 );

  if ( shm_ptr == MAP_FAILED )
  {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  memcpy( shm_ptr, buf, bytes_read );

  printf("procl writer : successfully written a file content into shared memory\n");

  munmap(shm_ptr, SHM_SIZE);

  close(shm_fd);

  exit(EXIT_FAILURE);

}
