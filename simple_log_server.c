#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#define KEY 1234

struct log_msg
{
  long msg_type;
  char msg_text[100];
}l_message;

int main()
{
  int msgid;
  // int msgget(key_t key, int msgflg);
  msgid = msgget(KEY,0666 | IPC_CREAT);
  if (msgid == -1)
  {
    perror("msgget()");
    exit(EXIT_FAILURE);
  }
  printf("Log Server started, Waiting for messages....\n");
  while (1)
  {
    // ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,int msgflg);

    if (msgrcv(msgid,&l_message,sizeof(l_message.msg_text),0,0) == -1)
    {
      perror("msgrcv()");
      exit(EXIT_FAILURE);
    }
    if (l_message.msg_type == 1)
    {
      printf("INFO: %s\n",l_message.msg_text);
    }
    else if (l_message.msg_type == 2)
    {
      printf("WARNING: %s\n",l_message.msg_text);
    }
    else if (l_message.msg_type == 3)
    {
      printf("ERROR: %s\n",l_message.msg_text);
    }
    else
    {
      printf("Invalid log type: %s\n",l_message.msg_text);
    }
  }
}




