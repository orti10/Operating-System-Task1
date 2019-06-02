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
using std::cout; using std::endl;
#define MSG_SIZE 20
#define MSG_SIZE32 10
#define READ_END 0
#define WRITE_END 1


int main() {

  char msg1[MSG_SIZE];
  char msg2[MSG_SIZE32];
  int pipe_fd1[2];
  int pipe_fd2[2];
  int  pid;
  
  if (pipe(pipe_fd1) == -1 || pipe(pipe_fd2) == -1) {
    fprintf(stderr, "Pipe failed");
    return 1;
  }
  
  pid = fork();

  if (pid < 0) {
    fprintf(stderr, "Fork failed");
    return 1;
  }

  else if (pid > 0) {
    close(pipe_fd1[READ_END]);
    printf("plain text: ");
    scanf("%20s",msg1);
    write(pipe_fd1[WRITE_END],msg1,20);    
    close(pipe_fd1[WRITE_END]);
    wait(NULL);
    close(pipe_fd2[WRITE_END]);
    read(pipe_fd2[READ_END],msg2,32);
    close(pipe_fd2[READ_END]);
    if(strlen(msg2)==32){
      printf("encrypted by process %d:%s\n",getpid(),msg2);
      kill(pid, SIGKILL);
    }
    else{
      exit(0);
    }
  }

  else {
    close(pipe_fd1[WRITE_END]);
    read(pipe_fd1[READ_END],msg1,20);
    std::string s=md5(msg1);
    close(pipe_fd1[READ_END]);
    close(pipe_fd2[READ_END]);
    write(pipe_fd2[WRITE_END],s.c_str(),32);
    close(pipe_fd2[WRITE_END]);
    exit(0);
  }

  return 0;
}