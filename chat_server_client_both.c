// chat server---- using pipe system call 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>

#define BUFFER_SIZE 256

int main() 
{
    int pipefd1[2],pipefd2[2]; 
    pid_t pid;
    char buffer[BUFFER_SIZE];
    if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1)
    {
        perror("pipe()");
        exit(EXIT_FAILURE);
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork()");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) 
    { 
        close(pipefd1[1]); 
        close(pipefd2[0]); 
        while (1) 
        {
            
            read(pipefd1[0], buffer, BUFFER_SIZE);
            printf("Server received: %s", buffer);
            if (strncmp(buffer, "exit", 4) == 0) 
            {
                printf("Server exiting...\n");
                break;
            }         
            printf("Server: ");
            fgets(buffer, BUFFER_SIZE, stdin);
            write(pipefd2[1], buffer, strlen(buffer) + 1);
        }
        close(pipefd1[0]); 
        close(pipefd2[1]); 
        wait(NULL); 
    } 
    else 
    { 
        close(pipefd1[0]); 
        close(pipefd2[1]);
        while (1)
         {         
            printf("Client: ");
            fgets(buffer, BUFFER_SIZE, stdin);
            write(pipefd1[1], buffer, strlen(buffer) + 1);
            if (strncmp(buffer, "exit", 4) == 0) 
            {
                break;
            }
            read(pipefd2[0], buffer, BUFFER_SIZE);
            printf("Client received: %s", buffer);
        }
        close(pipefd1[1]); 
        close(pipefd2[0]);
        exit(EXIT_SUCCESS);
    }
}
