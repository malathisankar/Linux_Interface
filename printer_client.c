// printer_client.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>

#define QUEUE_NAME  "/printer_queue"
#define MAX_SIZE    1024
#define MSG_STOP    "exit"

int main(int argc, char **argv) 
{
    mqd_t mq;
    char buffer[MAX_SIZE];
    // mqd_t mq_open(const char *name, int oflag);
    mq = mq_open(QUEUE_NAME, O_WRONLY);
    if (mq == (mqd_t) -1)
    {
        perror("mq_open");
        exit(1);
    }

    printf("Enter print jobs (type 'exit' to quit):\n");

    while (1)
     {
        //  void *memset(void *s, int c, size_t n);
        memset(buffer, 0, sizeof(buffer));
        printf("> ");
        // char *fgets(char *s, int size, FILE *stream);
        fgets(buffer, MAX_SIZE, stdin);
        buffer[strlen(buffer) - 1] = '\0'; 
        // int mq_send(mqd_t mqdes, const char *msg_ptr,size_t msg_len, unsigned int msg_prio);
        if (mq_send(mq, buffer, MAX_SIZE, 0) == -1) 
        {
            perror("mq_send");
            continue;
        }
        // int strncmp(const char *s1, const char *s2, size_t n);.
        if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0)
         {
            printf("Exiting client...\n");
            break;
        }
    }
    // int mq_close(mqd_t mqdes);
    mq_close(mq);
}

