#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
  int c,i,j;
  char argv [16][80];
  char *p [16];

  for (i = 0; i < 16; ++i)
     p[i] = NULL;

  i = j = 0;

  do {
       c = getchar();

       if (c == ' ' || c == '\n') {
           p[i] = &argv[i][0];
           argv[i][j++] = '\0';
           ++i;
           j = 0;
           }
       else
           argv[i][j++] = c;

       if (c == '\n') {
           pid_t pid = fork();
              if (!pid) {
              int rv = execvp (p[0], p);
              if (rv == -1) {
                 perror ("execvp");
                 return EXIT_FALLURE;
                 }
              }
           pid = wait(NULL);
           if (pid == -1) {
              perror("wait");
              return EXIT_FALLURE;
              }
          for (i = 0; i < 16; ++i) {
              p[i] = NULL;
              for (j = 0; j < 80; ++j)
              argv[i][j] = 0;
              }
              i = j = 0;
         }
      }
  while (c != EOF);
}
