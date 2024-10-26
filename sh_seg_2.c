#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main()
{
  key_t key;
  int shm_id,size;
  char *ptr;
  char buf[20];
  key = ftok("shmd",72);
  perror("ftok()");
  printf("Enter the size of the memory segment in bytes.\n");
  scanf("%d",&size);
  // int shmget(key_t key, size_t size, int shmflg);
  shm_id = shmget(key,size,0);
  perror("shmget()");
  // void *shmat(int shmid, const void *shmaddr, int shmflg);
  ptr = (char *)shmat(shm_id,NULL,0);
  perror("shmat()");
  strncpy(buf,ptr,size);
  printf("data: %s",buf);
  // int shmdt(const void *shmaddr);
  shmdt(ptr);
}
