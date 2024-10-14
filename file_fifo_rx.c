#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_SIZE 256
#define ACK_MSG "File received successfully!"
#define ERR_MSG "File exceeds size limit of 256 bytes!"

int main(int argc, char *argv[])
{
    int fifo_fd;
    char buf[MAX_SIZE];
    ssize_t bytes_read;
    char fifo_name[256];

    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    snprintf(fifo_name, sizeof(fifo_name), "%s_fifo", argv[1]);

    printf("Opening FIFO for reading...\n");
    fifo_fd = open(fifo_name, O_RDONLY);
    if (fifo_fd == -1) 
    {
        perror("open()");
        exit(EXIT_FAILURE);
    }

    bytes_read = read(fifo_fd, buf, sizeof(buf));
    if (bytes_read == -1) 
    {
        perror("read()");
        close(fifo_fd);
        exit(EXIT_FAILURE);
    }

    buf[bytes_read] = '\0';  

    if (bytes_read > MAX_SIZE) 
    {
        printf("File size exceeds 256 bytes. Notifying process 1.\n");
        close(fifo_fd);  
        fifo_fd = open(fifo_name, O_WRONLY);
        if (fifo_fd == -1)
        {
            perror("open()");
            exit(EXIT_FAILURE);
        }
        write(fifo_fd, ERR_MSG, strlen(ERR_MSG) + 1);
        close(fifo_fd);  
    } 
    else
    {
        printf("File received: %s\n", buf);
        close(fifo_fd);  
        fifo_fd = open(fifo_name, O_WRONLY);
        if (fifo_fd == -1)
        {
            perror("open()");
            exit(EXIT_FAILURE);
        }

        write(fifo_fd, ACK_MSG, strlen(ACK_MSG) + 1);
        close(fifo_fd); 
    }
    unlink(fifo_name);
}
