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

void send_log(const char *message,int log_type)
{
  int msgid;
  // int msgget(key_t key, int msgflg);
  msgid = msgget(KEY,0666 | IPC_CREAT);
  if (msgid == -1)
  {
    perror("msgget()");
    exit(EXIT_FAILURE);
  }
  l_message.msg_type = log_type;
  // char *strncpy(char *dest, const char *src, size_t n);
  strncpy(l_message.msg_text,message,sizeof(l_message.msg_text)-1);
  //  int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
    if (msgsnd(msgid,&l_message,sizeof(l_message.msg_text),0) == -1)
    {
      perror("msgsnd()");
      exit(EXIT_FAILURE);
    }
    printf("Log sent: %s\n",message);
}

int main()
{
  send_log("This is an info message", 1);
  send_log("This is an warning message",2);
  send_log("This is an error message",3);
}





