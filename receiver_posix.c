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
#define LOG_FILE "logfile.txt"

int main()
{
    int log_fd = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (log_fd == -1) 
    {
        perror("open()");
        exit(EXIT_FAILURE);
    }

    mqd_t mq;
    char buf[MAX_SIZE];
    ssize_t bytes_read; 

    mq = mq_open(MSGQ_NAME, O_RDONLY);
    if (mq == (mqd_t) -1) 
    {
        perror("mq_open()");
        exit(EXIT_FAILURE);
    }
    printf("Message queue receiver started, writing to: %s\n", LOG_FILE);
    while (1)
    {
        bytes_read = mq_receive(mq, buf, MAX_SIZE, NULL);
        if (bytes_read >= 0) 
	{
            buf[bytes_read] = '\0';  
            printf("Receiver: %s\n", buf);

            if (write(log_fd, buf, strlen(buf)) == -1) 
	    {
                perror("write()");
		exit(EXIT_FAILURE);
            }
            if (write(log_fd, "\n", 1) == -1) 
	    {
                perror("write()");
	       	exit(EXIT_FAILURE);
            }
        } else {
            perror("mq_receive()");
            exit(EXIT_FAILURE);
        }

        if (strcmp(buf, MSG_STOP) == 0) 
	{
            printf("Stopping log server...\n");
            break;
        }
    }

    mq_close(mq);
    close(log_fd);
    printf("Message queue closed\n");
    exit(EXIT_SUCCESS);
}

