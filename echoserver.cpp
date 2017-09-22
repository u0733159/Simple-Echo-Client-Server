#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <signal.h>
#include <string>
#include <getopt.h>
#include "utility.h"


#define BUFSIZE 2000

#define USAGE                                                                 \
"usage:\n"                                                                    \
"  echoserver [options]\n"                                                    \
"options:\n"                                                                  \
"  -p                  Port (Default: 8140)\n"                                \
"  -m                  Maximum pending connections (default: 8)\n"            \
"  -h                  Show this help message\n"                              \

/* OPTIONS DESCRIPTOR ====================================================== */
static struct option gLongOptions[] = {
  {"port",          required_argument,      NULL,           'p'},
  {"maxnpending",   required_argument,      NULL,           'm'},
  {"help",          no_argument,            NULL,           'h'},
  {NULL,            0,                      NULL,             0}
};

int main(int argc, char **argv) {
  int option_char;
  int portno = 8140; /* port to listen on */
  int maxnpending = 8;
  pid_t childpid;
  void sig_child(int);
  // Parse and set command line arguments
  while ((option_char = getopt_long(argc, argv, "p:m:h", gLongOptions, NULL)) != -1) {
    switch (option_char) {
    default:
      fprintf(stderr, "%s", USAGE);
      exit(1);
    case 'p': // listen-port
      portno = atoi(optarg);
      break;
    case 'm': // server
      maxnpending = atoi(optarg);
      break;
    case 'h': // help
      fprintf(stdout, "%s", USAGE);
      exit(0);
      break;
    }
  }

  if (maxnpending < 1) {
    fprintf(stderr, "%s @ %d: invalid pending count (%d)\n", __FILE__, __LINE__, maxnpending);
    exit(1);
  }

  if ((portno < 1025) || (portno > 65535)) {
    fprintf(stderr, "%s @ %d: invalid port number (%d)\n", __FILE__, __LINE__, portno);
    exit(1);
  }

  /* Socket Code Here */
  socklen_t clilen;
  struct sockaddr_in serv_addr;
  struct sockaddr_in cli_addr;
  int sock = 0;
  sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock < 0)
  {
    perror("Error opening socket.");
    exit(1);
  }
  int x =1;
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &x, sizeof(int)) < 0) {
    perror("setsockopt(SO_REUSEADDR) failed");
  }
  bzero((char *)&serv_addr, sizeof(serv_addr));

  printf("socket retrieves success\n");
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(portno);

  if (bind(sock, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)) < 0)
  {
    perror("ERROR on binding.");
    exit(1);
  }

  if (listen(sock, maxnpending) == -1)
  {
    printf("Failed to listen.\n");
    exit(1);
  }
  clilen = sizeof(cli_addr);
  signal(SIGCHLD, sig_child);
  while (1)
  {
    char buf[BUFSIZE];

    int *newsockfd = (int*)malloc(sizeof(int));
    if (newsockfd == 0) {
      printf("ERROR: Out of Memory\n");
      exit(1);
    }
    *newsockfd = accept(sock, (struct sockaddr *)&cli_addr, &clilen);
    if (*newsockfd < 0) {
      perror("ERROR on Accept");
      exit(1);
    }

    if((childpid=fork()) == 0) {
      ssize_t rcvd_bytes;
      char recv_msg[BUFSIZE];
      close(sock);
      std::string child_pid = std::to_string(getpid());
      child_echo(*newsockfd, child_pid);
      exit(1);
    }
    close(*newsockfd);
    free(newsockfd);
  }
}
