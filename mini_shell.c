#include <stdio.h>     
#include <stdlib.h>    
#include <unistd.h>     
#include <sys/types.h>  
#include <sys/wait.h> 
#include <string.h>

int main() 
{
    char command[256]; 
    char *args[10];    
    pid_t pid;          

    while (1) 
    {
        printf("simple-shell> ");
        fgets(command, 256, stdin); 

        command[strcspn(command, "\n")] = 0;

        int i = 0;
        args[i] = strtok(command, " ");
        while (args[i] != NULL) 
        {
            i++;
            args[i] = strtok(NULL, " ");
        }

        if (strcmp(args[0], "exit") == 0) 
        {
            break;
        }

        if (strcmp(args[0], "cd") == 0) 
        {
            if (i < 2) 
            {
                fprintf(stderr, "cd: missing argument\n");
            }
            else 
            {
                if (chdir(args[1]) != 0) 
                {
                    perror("cd failed");
                }
            }
            continue; 
        }

        pid = fork();

        if (pid < 0) 
        {
            perror("Fork failed");
            exit(1);
        } 
        else if (pid == 0) 
        {
            if (execvp(args[0], args) == -1) 
            {
                perror("Execution failed");
            }
            exit(1); 
        } 
        else 
        {
            wait(NULL);
        }
    }
}

