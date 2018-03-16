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
#include <sys/prctl.h>
#include <sys/signal.h>
#include <fcntl.h>
#include <poll.h>
#include <limits.h>

#include "structs_clients.h"
#include "str2.h"

/* Convention for arguments received at startup: 
   first one : name of the fifo to write info in
   second one : name of the fifo to read info from
*/

// 0 is write, 1 is read
static struct pollfd read_or_write_fd[2];



int main(int argc, char *argv[])
{
  int fd_fifo_w, fd_fifo_rd;
  char fifo_msg_w[RD_W_FIFO];
  char fifo_msg_rd[RD_W_FIFO];
  char nick_name[MAXEMAIL];

  if (argc != 4){
    printf("Wrong number of arguments!\n");
    exit(-1);
  }

  fd_fifo_rd = open(argv[2], O_RDONLY);
  fd_fifo_w = open(argv[1], O_WRONLY);
  strncpy(nick_name, argv[3], sizeof(nick_name));
  

  prctl(PR_SET_PDEATHSIG, SIGHUP);

  read_or_write_fd[0].fd = 0;
  read_or_write_fd[0].events = POLLIN;

  read_or_write_fd[1].fd = fd_fifo_rd;
  read_or_write_fd[1].events = POLLIN;

  while (1)
    {
      int nready = poll(read_or_write_fd, 2 + 1, -1);
      
      if (read_or_write_fd[0].revents & POLLIN)
	{
	  memset(fifo_msg_w, '\0', sizeof(fifo_msg_w));
	  fgets(fifo_msg_w, sizeof(fifo_msg_w), stdin);
	  //strip_newline(fifo_msg_w);
	  write(fd_fifo_w, fifo_msg_w, sizeof(fifo_msg_w));
	}

      else if (read_or_write_fd[1].revents & POLLIN)
	{
	  read(fd_fifo_rd, fifo_msg_rd, sizeof(fifo_msg_rd));
	  printf("%s> %s\n", nick_name, fifo_msg_rd);
	}

    }

  return 0;
}
