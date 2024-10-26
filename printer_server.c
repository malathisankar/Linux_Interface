// printer_server.c
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

int main() 
{
    mqd_t mq;
    char buffer[MAX_SIZE + 1]; //Buffer to store received messages, with an extra byte for the null-terminator (+1)
    struct mq_attr attr;
    ssize_t bytes_read;
    
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_SIZE;
    attr.mq_curmsgs = 0;
    
    // mqd_t mq_open(const char *name, int oflag, mode_t mode,struct mq_attr *attr);
    mq = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
    if (mq == (mqd_t) -1)
    {
        perror("mq_open");
        exit(1);
    }

    printf("Printer Server: Waiting for print jobs...\n");

    while (1) 
    {
        // void *memset(void *s, int c, size_t n);
        memset(buffer, 0, sizeof(buffer));
        // ssize_t mq_receive(mqd_t mqdes, char *msg_ptr,size_t msg_len, unsigned int *msg_prio);
        bytes_read = mq_receive(mq, buffer, MAX_SIZE, NULL);
        if (bytes_read >= 0)
         {
            buffer[bytes_read] = '\0';
            printf("Received print job: %s\n", buffer);
            // int strncmp(const char *s1, const char *s2, size_t n);
            if (strncmp(buffer, MSG_STOP, strlen(MSG_STOP)) == 0) 
            {
                printf("Printer Server: Exiting...\n");
                break;
            }

            printf("Processing job...\n");
            sleep(2); 
            printf("Job processed.\n");
        } 
        else 
        {
            perror("mq_receive");
        }
    }
    // int mq_close(mqd_t mqdes);
    mq_close(mq);
    mq_unlink(QUEUE_NAME);
}

