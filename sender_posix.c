#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    struct mq_attr my_attr;

    my_attr.mq_flags = 0;
    my_attr.mq_maxmsg = 10;
    my_attr.mq_msgsize = MAX_SIZE;
    my_attr.mq_curmsgs = 0;

    mq = mq_open(MSGQ_NAME, O_WRONLY | O_CREAT, 0644, &my_attr);

    if (mq == (mqd_t) -1)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    printf("POSIX message queue sender started\n");

    while (1)
    {
        printf("Enter message: ");
        fgets(buf, MAX_SIZE, stdin);
        buf[strcspn(buf, "\n")] = '\0';  

        if (mq_send(mq, buf, strlen(buf) + 1, 0) == -1)
        {
            perror("mq_send");
            exit(EXIT_FAILURE);
        }

        if (strcmp(buf, MSG_STOP) == 0)
        {
            break;
        }
    }

    mq_close(mq);
    mq_unlink(MSGQ_NAME);
    exit(EXIT_SUCCESS);
}

