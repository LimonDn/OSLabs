#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
  int c;
  int i = -1;
  int j = 0;
  char argv [16][80];
  char *p [16];

  for (int  i = 0; i < 16; ++i)
     p[i] = NULL;

  do {
       c = getchar();
       if (c == ' ' && j == 0){
          continue;
       }
       if ((c == ' ' || c == '\n') && (j != 0)) {
           p[i] = &argv[i][0];
           argv[i][j++] = '\0';
           j = 0;
           }
       else{
        if (j == 0) {
          ++i;
        }
           argv[i][j++] = c;
           }
       if (c == '\n') {
           pid_t pid = fork();
              if (!pid) {
              int rv = execvp (p[0], p);
              if (rv == -1) {
                 perror ("execvp");
                 return EXIT_FAILURE;
                 }
              }
           pid = wait(NULL);
           if (pid == -1) {
              perror("wait");
              return EXIT_FAILURE;
              }
          for (int i = 0; i < 16; ++i) {
              p[i] = NULL;
              for (int j = 0; j < 80; ++j)
              argv[i][j] = '\0';
              }
              i = -1;
         }
      }
  while (c != EOF);
}
