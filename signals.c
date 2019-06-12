#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "md5.h"
int check;
int array[5];
void sigCather(int sig);
void killAllsignals(int state, int zombi);
int main() {
        int pid;
        int zombi;
        int state;

        signal(SIGTERM, sigCather);

        for (int i = 0; i < 5; i++)
        {
                pid = fork();
                if(pid == 0) {
                        
                        printf("PID %d is ready \n", getpid());
                        check = i-1;
                        pause(); 
                        exit(0); 
                }
               
                array[i] = pid;
        }
        sleep(1);
        kill(array[4],SIGTERM);
        sleep(1);

        killAllsignals(state, zombi);

        return 0;
}
void sigCather(int sig) {
        signal(SIGTERM,sigCather);
        printf("PID %d caught one\n",getpid());
        if(check >-1) {
                kill(array[check],SIGTERM);
        }
}

void killAllsignals(int state, int zombi) {
        for (int i = 0; i < 5; i++) {
                zombi = wait(&state);
                printf("Process %d is dead\n",zombi);
                kill(zombi,SIGKILL);
        }
}

