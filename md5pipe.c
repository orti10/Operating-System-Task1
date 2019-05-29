#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
        int     fd[2], nbytes;
        pid_t   childpid;
        char    string[] = "Hello, world! I'm the son and this my message!\n";
        char    readbuffer[80];

        pipe(fd);   // piping fd[0] & fd[1]

        if((childpid = fork()) == -1)   // here we create a SON process
        {
                perror("fork");
                exit(1);
        }

         if(childpid == 0)    // child process
    {
            write(fd[1], string, (strlen(string)+1)); #C1
            printf("Verification : Message was sent successfully by the SON!\n"); #C2
            exit(0);
    }
    else    // father process
    {
            /* Read in a string from the pipe */
            nbytes = read(fd[0], readbuffer, sizeof(readbuffer)); #F1
            printf("I'm the father and I received that string: %s", readbuffer); F2
    }

        return(0);
}
