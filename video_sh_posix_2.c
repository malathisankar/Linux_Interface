//video_mp4_read
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define SHM_NAME "/my_mp4_share"
//#define SHM_SIZE (10 * 1024 * 1024)

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
  //  int fstat(int fd, struct stat *statbuf);
  struct stat sb;
  if (fstat(fd,&sb) == -1)
  {
    perror("fstat()");
    exit(EXIT_FAILURE);
  }
  size_t fileSize = sb.st_size;
  //void *mmap(void *addr, size_t length, int prot, int flags,int fd, off_t offset);
  shmem = mmap(0,fileSize,PROT_READ,MAP_SHARED,fd,0);
  if (shmem == MAP_FAILED)
  {
    perror("mmap()");
    exit(EXIT_FAILURE);
  }
  // FILE *fopen(const char *pathname, const char *mode);
  FILE *file = fopen("/home/sk/api/memorylayout/28oct24/output.mp4","wb");
  if (file == NULL)
  {
    perror("Failed to create output file");
    exit(EXIT_FAILURE);
  }
  // size_t fwrite(const void *ptr, size_t size, size_t nmemb,FILE *stream);
  size_t bytesWritten = fwrite(shmem,1,fileSize,file);
   if (bytesWritten != fileSize) 
   {
        fprintf(stderr, "Warning: Only wrote %zu bytes out of %zu\n", bytesWritten, fileSize);
    }
  printf("Written %zu bytes to output.mp4 from shared memory.\n",bytesWritten);
  //  int fclose(FILE *stream);
  fclose(file);
  //  int munmap(void *addr, size_t length);
  munmap(shmem,fileSize);
  close(fd);
  // int unlink(const char *pathname);
  shm_unlink(SHM_NAME);
}


