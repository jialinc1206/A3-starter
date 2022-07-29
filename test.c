#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <float.h>


int main()
{
    printf("\nTEST\n\n");

    FILE* tmpf = tmpfile();
    fputs("2010,1,1,0,NA,-11\n", tmpf);
    fputs("2011,2,1,1,NA,-12\n", tmpf);
    fputs("2012,3,1,2,NA,-11\n", tmpf);
    rewind(tmpf);

    char *buffer = NULL;		// pointer to stirng
	        // char* table[100];
    int line[6];
  size_t bufsize = 99;


	// read input 1st line
  buffer = (char *)malloc(bufsize * sizeof(char));
  if(buffer == NULL)
  {
    perror("load_table malloc");
    exit(1);
  }
  
  if(fgets(buffer, 100, tmpf) == NULL) {
    perror("load_table filename open");
    free(buffer);
    exit(1);
  }
  char *token = strtok(buffer, ",");
                // printf("before while loop\n");
  int c = 0;


    while(token != NULL) {
        if(strcmp(token, "NA") == 0)
        token = "0";
        line[c] = atoi(token);
        token = strtok(NULL, ",");
        c++;
    }
    printf("load_table duplicate entry: %d-%d-%d %d\n", line[0], line[1], line[2], line[3]);
    c = 0;
  while(fgets(buffer, 100, tmpf) != NULL) {
      token = strtok(buffer, ",");
          while(token != NULL) {
                     // printf("%s", token);
        if(strcmp(token, "NA") == 0) {
            token = "0";
        }

        line[c] = atoi(token);
        token = strtok(NULL, ",");
        c++;
    }
    printf("load_table duplicate entry: %d-%d-%d %d\n", line[0], line[1], line[2], line[3]);
    c = 0;
  }
                // printf("after while loop\n");

    return 0;
}