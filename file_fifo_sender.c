#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_SIZE 256

int main(int argc, char *argv[])
{
    int fifo_fd, file_fd;
    char buf[MAX_SIZE];
    char fifo_name[256];
    ssize_t file_size;

    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    snprintf(fifo_name, sizeof(fifo_name), "%s_fifo", argv[1]);
    
    if (mkfifo(fifo_name, 0666) == -1)
    {
        perror("mkfifo()");
        exit(EXIT_FAILURE);
    }

    file_fd = open(argv[1], O_RDONLY);
    if (file_fd == -1)
    {
        perror("open() for file");
        exit(EXIT_FAILURE);
    }

  
    file_size = lseek(file_fd, 0, SEEK_END);  
    lseek(file_fd, 0, SEEK_SET);            

    if (file_size > MAX_SIZE)
    {
        printf("File size exceeds 256 bytes. Exiting.\n");
        close(file_fd);
        unlink(fifo_name);
        exit(EXIT_FAILURE);
    }

    read(file_fd, buf, file_size);
    buf[file_size] = '\0'; 
    close(file_fd);
    fifo_fd = open(fifo_name, O_WRONLY);
    if (fifo_fd == -1)
    {
        perror("open() for FIFO");
        unlink(fifo_name);
        exit(EXIT_FAILURE);
    }
    write(fifo_fd, buf, file_size);
    close(fifo_fd);  
    fifo_fd = open(fifo_name, O_RDONLY);
    if (fifo_fd == -1)
    {
        perror("open() for reading acknowledgment");
        unlink(fifo_name);
        exit(EXIT_FAILURE);
    }

    read(fifo_fd, buf, sizeof(buf));
    printf("Received response: %s\n", buf);
    close(fifo_fd);
    unlink(fifo_name);
}
