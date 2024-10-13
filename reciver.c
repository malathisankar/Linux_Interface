// receiver.c
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_SIZE 256

int main(int argc, char *argv[]) 
{
    int fd;
    char buffer[MAX_SIZE];
    char fifo_name[256];
    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    snprintf(fifo_name, sizeof(fifo_name), "%s_fifo", argv[1]);
    fd = open(fifo_name, O_RDONLY);
    if (fd == -1) 
    {
        perror("open()");
        exit(EXIT_FAILURE);
    }
    ssize_t num_bytes = read(fd, buffer, MAX_SIZE);
    if (num_bytes == -1) 
    {
        perror("read()");
        close(fd);
        exit(EXIT_FAILURE);
    }
    buffer[num_bytes] = '\0';
    close(fd);
    printf("Received data: %s\n", buffer);
    fd = open(fifo_name, O_WRONLY);
    if (fd == -1) 
    {
        perror("open()");
        exit(EXIT_FAILURE);
    }
    write(fd, "File received successfully", 27);
    close(fd);
}
