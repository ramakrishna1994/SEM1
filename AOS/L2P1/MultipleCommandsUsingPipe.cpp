#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main (void) {
    int fd[2];

    pipe(fd);

    if (fork()) {
    	printf("process 1\n");
        // Child 1 process
        dup2(fd[0], 0); // wc reads from the pipe
        close(fd[1]); // Closing the both ends of pipe to avoid misuse of pipe
        /*close(fd[1]);*/
        printf("send : 10\n");
        if(execlp("wc", "wc", "-l", NULL)<0)
        {

        	perror("wc command failed");
        }
        printf("send : 5\n");
    } else if(fork()){
        // child 2  process
    	printf("process 2\n");
        dup2(fd[1], 1); // ls writes to the pipe
        close(fd[0]); // Closing the both ends of pipe to avoid misuse of pipe
        //close(fd[1]);*/
        printf("send : 11\n");
        if(execlp("ls", "ls", NULL)<0)
        {
        	perror("ls command failed");
        }
        printf("send : 6\n");
    }
    else
    {
    	close(fd[0]); // Closing the both ends of pipe to release resources
    	close(fd[1]);
    }
    exit(EXIT_SUCCESS);
}
