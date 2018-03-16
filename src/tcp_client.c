#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <limits.h>

#include "structs_clients.h"

#define SERV_PORT 2000
#define MAX_QUESTION 2000
#define SERV_ADDR "127.0.0.1"
#define SA struct sockaddr

static int sockfd;

bool tcp_client_init(void)
{
  struct sockaddr_in servaddr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
    {
      printf("socket() error\n");
      exit(0);
    }

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);
  inet_pton(AF_INET, SERV_ADDR, &servaddr.sin_addr);

  if (connect(sockfd, (SA*) &servaddr, sizeof(servaddr)) == -1)
    {
      printf("Couldn't connect to the server... Please try again later\n");
      exit(0);
    }

  return true;
}

bool tcp_client_send(char *msg_to_send)
{
  char tmp_msg[RD_W_FIFO];

  memset(tmp_msg, '\0', sizeof(tmp_msg));
  strncpy(tmp_msg, msg_to_send, strlen(msg_to_send));
  
  write(sockfd, tmp_msg, RD_W_FIFO);

  return true;
}

bool tcp_client_receive(char *buffer, int size_of_buf)
{
  memset(buffer, '\0', size_of_buf);

  read(sockfd, buffer, RD_W_FIFO);

  return true;
}

int tcp_client_return_socket(void)
{
  return sockfd;
}

