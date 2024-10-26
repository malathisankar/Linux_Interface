#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>

int main()
{
  key_t key;
  int shm_id;
  char *ptr;
  char buf[40] = "Linus Torvalds\n";
  key = ftok("shmd",72);
  //  int shmget(key_t key, size_t size, int shmflg);
  shm_id = shmget(key,1024,0644 | IPC_CREAT);
  perror("shmget()");
  printf("shm_id: %d",shm_id);
  // void *shmat(int shmid, const void *shmaddr, int shmflg);
  ptr = (char *)shmat(shm_id,NULL,SHM_RND);
  perror("shmat()");
  strcpy(ptr,buf);
  // int shmdt(const void *shmaddr);
  shmdt(ptr);
  perror("shmdt()");
}
