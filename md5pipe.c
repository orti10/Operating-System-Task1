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
#define BUFFER_SIZE 20
#define BUFFER_SIZE32 10
#define READ_END 0
#define WRITE_END 1


int main()
{

  char write_msg[BUFFER_SIZE];
  char write_msg2[BUFFER_SIZE32];
  int fd[2];
  int fd2[2];
  int  pid;
  if (pipe(fd) == -1) {
    fprintf(stderr, "Pipe failed");
    return 1;
  }
  if (pipe(fd2) == -1) {
    fprintf(stderr, "Pipe failed");
    return 1;
  }
  pid = fork();
 
  if (pid < 0) {
    fprintf(stderr, "Fork failed");
    return 1;
  }
 
  else if (pid>0){
    close(fd[READ_END]);
    printf("plain text: ");
    scanf("%20s",write_msg);
    write(fd[WRITE_END],write_msg,20);    
    close(fd[WRITE_END]);
    wait(NULL);
    close(fd2[WRITE_END]);
    read(fd2[READ_END],write_msg2,32);
    close(fd2[READ_END]);
    if(strlen(write_msg2)==32){
            printf("encrypted by process %d:%s\n",getpid(),write_msg2);
            kill(pid,SIGKILL);
    }
    else{
            exit(0);
    }
    }
 
  else {
    close(fd[WRITE_END]);
    read(fd[READ_END],write_msg,20);
    std::string s=md5(write_msg);
    close(fd[READ_END]);
    close(fd2[READ_END]);
    write(fd2[WRITE_END],s.c_str(),32);
    close(fd2[WRITE_END]);
    exit(0);
  }

  return 0;
}