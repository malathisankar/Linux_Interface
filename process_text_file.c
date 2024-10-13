#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>

#define MAX_SIZE 256

int main(int argc, char *argv[]) {
    pid_t pid;               
    int fd;                  
    char buffer[MAX_SIZE];  
    off_t file_size;       
    int file_fd;            
    if (argc != 2) 
    {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    } 
    char fifo_name[256];
    snprintf(fifo_name, sizeof(fifo_name), "%s_fifo", argv[1]);  
    if (mkfifo(fifo_name, 0666) == -1) 
    {
        perror("mkfifo()");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if (pid < 0) 
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    if (pid == 0) 
    {    
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
        printf("Received data: %s\n", buffer);
        close(fd); 
        if (num_bytes > MAX_SIZE) 
        {
            printf("File exceeds 256 bytes.\n");   
            fd = open(fifo_name, O_WRONLY);
            if (fd == -1) 
            {
                perror("open()");
                exit(EXIT_FAILURE);
            }
            write(fd, "File too large", 15);
            close(fd);
            exit(EXIT_FAILURE);
        } 
        fd = open(fifo_name, O_WRONLY);
        if (fd == -1)
         {
            perror("open()");
            exit(EXIT_FAILURE);
        }
        write(fd, "File received successfully", 27);
        close(fd);

    } 
    else 
    {  
        file_fd = open(argv[1], O_RDONLY);
        if (file_fd == -1) 
        {
            perror("open()");
            exit(EXIT_FAILURE);
        }   
        file_size = lseek(file_fd, 0, SEEK_END);
        lseek(file_fd, 0, SEEK_SET);  

        if (file_size > MAX_SIZE) 
        {
            printf("File size exceeds 256 bytes. Exiting.\n");
            close(file_fd);
            exit(EXIT_FAILURE);
        }
        read(file_fd, buffer, file_size);
        buffer[file_size] = '\0';
        close(file_fd);  
        fd = open(fifo_name, O_WRONLY);
        if (fd == -1) 
        {
            perror("open()");
            exit(EXIT_FAILURE);
        }
        write(fd, buffer, file_size);
        close(fd);  
        fd = open(fifo_name, O_RDONLY);
        if (fd == -1) 
        {
            perror("open()");
            exit(EXIT_FAILURE);
        } 
        read(fd, buffer, MAX_SIZE);
        printf("Received acknowledgment: %s\n", buffer);
        close(fd);  
    }
}

