//mp3_write
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>

#define SHM_NAME "/my_mp3_share"
#define SHM_SIZE (10 * 1024 * 1024)

int main()
{
  int fd;
  char *shmem;
  // int open(const char *pathname, int flags, mode_t mode);
  fd = shm_open(SHM_NAME,O_CREAT | O_RDWR, 0666);
  //int ftruncate(int fd, off_t length);
  ftruncate(fd,SHM_SIZE);
  //  void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
  shmem = mmap(0,SHM_SIZE,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
  if (shmem == MAP_FAILED)
  {
    perror("mmap()");
    exit(EXIT_FAILURE);
  }
  // FILE *fopen(const char *pathname, const char *mode); //POSIX source
  FILE *file = fopen("/home/sk/Downloads/v.mp3","rb");
  if (file == NULL)
  {
    perror("Failed to open file");
    exit(EXIT_FAILURE);
  }
  // size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
  size_t bytesRead = fread(shmem,1,SHM_SIZE,file);
  printf("Read %zu bytes from MP3 file into shared memory.\n",bytesRead);
  //int fclose(FILE *stream);
  fclose(file);
  // int munmap(void *addr, size_t length);
  munmap(shmem,SHM_SIZE);
  close(fd);
}

