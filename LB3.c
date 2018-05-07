#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
int c;
int i = -1;
int j = 0; 
int k = 0;
int t = 0;
char argv [17][80];
char *p [17];
char *p_pipe [17];
char *p_write, *p_read;
char fl = 0;

for (int i = 0; i<17; ++i) {
  p[i] = NULL; p_pipe[i] = NULL;
}
p_write = p_read =  NULL;

do { 
c = getchar();
  if ( ((c == ' ' ) && ( (j == 0) || ( k != 0) )) || (c == '<') || (c == '>')  || (c == '|')) {
    if (c == '|') {
      if (k == 2) {
        argv[i][j] = '\0';
          p_read = &argv[i][0];
          k = 0;
          j = 0;
      }
      fl = 1;
    }

    if (c == '<'){
      k = 2;
    }
    if (c == '>') {
      if (k == 2) {
        argv[i][j] = '\0';
        p_read = &argv[i][0];
        j = 0;
        }
      k = 1;
    }
    continue;
  }
    
  if ((c == ' ' || c == '\n') && (j != 0) && (k == 0)) {
    if (fl == 1) {
      p_pipe[t] = &argv[i][0];
    } 
    else {
      p[i] = &argv[i][0];
    }
    argv[i][j] = '\0';
    j = 0;
  }
  else {
    if (j == 0) {
      ++i;
    }
    argv[i][j++] = c;
  }
  
  if (c == '\n') {

    if (k != 0) { 
      argv[i][j-1] = '\0';
      if (k == 1) {
        p_write = &argv[i][0];
      }
      else {
        p_read = &argv[i][0];
      }
    }

        if (fl == 1) {
      /*printf("%s\n", p_pipe[t]);*/

int pipefd[2];
      if (-1 == pipe(pipefd)) {
        perror("pipe");
        return -1;
      }
 
      pid_t pid1 = fork();
      if (pid1 == 0) {
        close(pipefd[0]);

        if (-1 == dup2(pipefd[1], STDOUT_FILENO)) {
          perror("dup2_pipe");
          return EXIT_FAILURE;
        }

int  fd_read;
        if (p_read  != NULL) { 
        fd_read  = open(p_read , O_RDONLY);
                                           
      
      if (-1 == fd_read)  {
        perror("open_less");
        return EXIT_FAILURE;
      }
      if (p_read  != NULL) {       
        if (-1 == dup2(fd_read, STDIN_FILENO)) {
          perror("dup2_less");
          return EXIT_FAILURE;
          }
      }
    }
        int rv1 = execvp(p[0], p);
        if (rv1 == -1) {
          perror("execvp_less");
          return EXIT_FAILURE;
        }
      }

      pid_t pid2 = fork();
      if (pid2 == 0) {
        close(pipefd[1]);

        if (-1 == dup2(pipefd[0], STDIN_FILENO)) {
          perror("dup2_pipe");
          return EXIT_FAILURE;
        }
int  fd_write;
        if (p_write != NULL) {
     fd_write  = open(p_write, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        /*printf("%s\n", p_write);*/

        if (-1 == fd_write)   {
          perror("open_more");
          return EXIT_FAILURE;
        }
        if (-1 == dup2(fd_write, STDOUT_FILENO)) {
          perror("dup2_more");
          /*close(fd_write);*/
          return EXIT_FAILURE;
        }
      }
    
    int rv2 = execvp(p_pipe[0], p_pipe);
    if (rv2 == -1) {
      perror("execvp_more");
      return EXIT_FAILURE;
      }
      }

      close(pipefd[0]);
      close(pipefd[1]);
      wait(NULL);
      wait(NULL);

    } 
    else {

  pid_t pid = fork();
  if (!pid) {

int fd, fd1;
        
    if (p_write != NULL || p_read != NULL ) {

      if (p_write != NULL) {
        fd  = open(p_write, O_WRONLY | O_CREAT | O_TRUNC, 0666);
      }
      if (p_read != NULL) {
        fd1  = open(p_read, O_RDONLY);
      }                                      
      
      if ((-1 == fd) || (-1 == fd1))  {
        perror("open");
        return EXIT_FAILURE;
      }
      if (p_read != NULL) {       
        if (-1 == dup2(fd1, STDIN_FILENO)) {
          perror("dup2_read");
          close(fd1);
          return EXIT_FAILURE;
          }
      }
      if (p_write != NULL) {                 
        if (-1 == dup2(fd, STDOUT_FILENO)) {
          perror("dup2_write");
          close(fd);
          return EXIT_FAILURE;
        }
      }
    }
    
    int rv = execvp(p[0], p);
    if (rv == -1) {
      perror("execvp");
      return EXIT_FAILURE;
      }
    
    if (p_write != NULL) {
      close(fd1);
    }
    if (p_read != NULL) {   
      close(fd);
    }

    pid = wait(NULL);
    if (pid == -1){
      perror("wait");
      return EXIT_FAILURE;
      }
  }  
  }  
    for (int i = 0; i<17; ++i){
      p[i] = NULL;
      p_pipe[i] = NULL;
      for (int j = 0; j<80; ++j)
        argv[i][j] = '\0';
    }
    p_write = p_read = NULL;              
    j = k = 0;    
    i = -1; 
    fl = t = 0;     
  }
}
while (c != EOF);
}
