#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";

int main(int argc, char **argv)
{
    int port_num;

    printf("%s", user_agent_hdr);

    if (argc != 2) {
      printf("Usage: %s <port number>\n", argv[0]);
      exit(0);
    }

    port_num = atoi(argv[1]);

    return 0;
}