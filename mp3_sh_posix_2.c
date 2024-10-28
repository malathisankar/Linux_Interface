//mp3_read
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define SHM_NAME "/my_mp3_share"
#define SHM_SIZE (10 * 1024 * 1024)

int main()
{
  int fd;
  char *shmem;
  // int open(const char *pathname, int flags, mode_t mode);
  fd = shm_open(SHM_NAME,O_RDONLY,0666);
  if (fd == -1)
  {
    perror("shm_open()");
    exit(EXIT_FAILURE);
  }
  //void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
  shmem = mmap(0,SHM_SIZE,PROT_READ,MAP_SHARED,fd,0);
  if (shmem == MAP_FAILED)
  {
    perror("mmap()");
    exit(EXIT_FAILURE);
  }
  // FILE *fopen(const char *pathname, const char *mode);
  FILE *file = fopen("/home/sk/api/memorylayout/28oct24/output.mp3","wb");
  if (file == NULL)
  {
    perror("Failed to create output file");
    exit(EXIT_FAILURE);
  }
  // size_t fwrite(const void *ptr, size_t size, size_t nmemb,FILE *stream);
  size_t bytesWritten = fwrite(shmem,1,SHM_SIZE,file);
  printf("Written %zu bytes to output.mp3 from shared memory.\n",bytesWritten);
  //  int fclose(FILE *stream);
  fclose(file);
  //  int munmap(void *addr, size_t length);
  munmap(shmem,SHM_SIZE);
  close(fd);
  // int unlink(const char *pathname);
  shm_unlink(SHM_NAME);
}

