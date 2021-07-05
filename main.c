#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int childPid[2];
// the signal handler
void handler(int SIGNUM)
{
    int code;
    int pid = wait(&code);

    if (WIFEXITED(code))
    {
        int child = pid == childPid[0] ? (1) : (2);
        code = WEXITSTATUS(code);
        printf("\nHey there! I'm child %d, I've found the target at position %d\n", child, code);
    }
    killpg(getpgrp(), SIGKILL);
    signal(SIGUSR1, handler);
}

void main(int argc, char **argv)
{
    // define the handler for the signal
    signal(SIGUSR1, handler);
    //******************************
    // get the input from the terminal
    int targetValue = atoi(argv[1]);          // the target value to search for
    int arrSize = argc - 2;                   // array size equals: size of all arguments - the first two arguments
    int *arr = malloc(sizeof(int) * arrSize); // list fo numbers to search in
    for (int i = 0; i < arrSize; i++)         // get the list of numbers
        arr[i] = atoi(argv[i + 2]);
    //******************************
    // Forking
    int pid, parentPID = getpid();
    printf("\nI am the parent, my pid is: %d \n", parentPID);
    for (int child = 1; child <= 2; child++) // creating only two children to divide the list on them
    {
        childPid[child - 1] = pid = fork();
        if (pid == -1) // error in forking
            perror("error in fork");

        else if (pid == 0) // child process
        {
            printf("\nI'm child %d with pid %d, my parent's pid is: %d\n", child, getpid(), parentPID);

            int start = (child - 1) * arrSize / 2;
            int boundary = child * arrSize / 2; // for the first child it's arrSize/2 and for the second child it's arrSize
            for (int i = start; i < boundary; i++)
            {
                if (arr[i] == targetValue)
                {
                    kill(parentPID, SIGUSR1); // if found, send a signal to the parent
                    exit(i);                  // the exit code is the position of the target
                }
            }
            // if target is not found
            sleep(3);
            printf("\nchild %d terminates...\n", child);
            exit(0); 
        }
        else // parent process
        {
            // do nothing
        }
    }
    //******************************
    // cheking for the output of the children
    if (getpid() == parentPID) // only the parent can acces this scope
    {
        sleep(5);
        int code = 0;
        for (int i = 1; i <= 2; i++)
        {
            pid = wait(&code); // wait for the code send by this process

            if (WIFEXITED(code)) // if exited normally
            {
                code = WEXITSTATUS(code);
                printf("\nI'm the child with pid: %d, I've been terminated with exit code: %d", pid, code);
            }
        }
        // if target is not found
        printf("\n\nOoops, target value is not found\n");
    }
}
