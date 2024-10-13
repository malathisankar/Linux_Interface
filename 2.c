#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

#define BUF_SIZE 1024
#define FIFO_PATH "/home/sk/Downloads/v.mp3"

int main(int argc, char **argv)
{
    int fd1, fd2;
    pid_t pid1, pid2;
    char buf[BUF_SIZE];
    ssize_t bytes_read, bytes_written;
    if (argc != 3)
    {
        printf("Usage: %s <sent-mp3-file>  <receive-mp3-file>\n", argv[0]);
        return 1;
    }
   

    if (mkfifo( FIFO_PATH, 0666) == -1)
    {
        perror("mkfifo()");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    if (pid1 == 0) 
    {
        fd1 = open(FIFO_PATH, O_RDONLY);
        if (fd1 == -1)
        {
            perror("open()");
            exit(EXIT_FAILURE);
        }
        fd2 = open(FIFO_PATH, O_WRONLY);
        if (fd2 == -1)
        {
            perror("open()");
            close(fd1);
            exit(EXIT_FAILURE);
        }
        while ((bytes_read = read(fd1, buf, BUF_SIZE)) > 0)
        {
            bytes_written = write(fd2, buf, bytes_read);
            if (bytes_written == -1)
            {
                perror("write()");
                close(fd1);
                close(fd2);
                exit(EXIT_FAILURE);
            }
        }
        close(fd1);
        close(fd2);
        printf("File sent successfully.\n");
        exit(EXIT_SUCCESS);
    }

    pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    if (pid2 == 0)
    {
        fd2 = open( FIFO_PATH, O_RDONLY);
        if (fd2 == -1)
        {
            perror("open()");
            exit(EXIT_FAILURE);
        }
        fd1 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd1 == -1)
        {
            perror("open()");
            close(fd2);
            exit(EXIT_FAILURE);
        }
        while ((bytes_read = read(fd2, buf, BUF_SIZE)) > 0)
        {
            bytes_written = write(fd1, buf, bytes_read);
            if (bytes_written == -1)
            {
                perror("write()");
                close(fd2);
                close(fd1);
                exit(EXIT_FAILURE);
            }
        }
        close(fd2);
        close(fd1);
        printf("File received successfully.\n");
        exit(EXIT_SUCCESS);
    }
    wait(NULL);
    wait(NULL);
    unlink(FIFO_PATH);
}

