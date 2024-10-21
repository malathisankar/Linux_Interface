#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <mqueue.h>

#define MSGQ_NAME "/p_msgq"
#define MAX_SIZE 1024
#define MSG_STOP "exit"

int main()
{
  mqd_t mq;
  char buf[MAX_SIZE];
  ssize_t bytes_read; 
  mq = mq_open(MSGQ_NAME, O_RDONLY);
  if (mq == (mqd_t) -1) 
  {
    perror("mq_open ()");
    exit (EXIT_FAILURE);
  }
  printf("messge queues reciver started\n");

  while (1)
  {
    bytes_read = mq_receive (mq, buf, MAX_SIZE, NULL);

    if (bytes_read >= 0)
    {
      buf[bytes_read]= '\0';
      printf("Reciver: %s\n",buf);
    }
    else
    {
      perror("mq_recvier()");
      exit(EXIT_FAILURE);
    }
    if (strcmp(buf, MSG_STOP)==0)
    {
      printf("buf:%s\n",buf);
      break;
    }
  }
  mq_close(mq);
  printf("message queues closed \n");
  exit (EXIT_FAILURE);
}
