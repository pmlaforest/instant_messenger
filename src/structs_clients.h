#ifndef STRUCT_CLIENTS_H
#define STRUCT_CLIENTS_H

#define MAXLINE  1024
#define MAXNAME  100
#define MAXPASS  50
#define MAXEMAIL 100
#define MAXCLI   10
#define MAX_FRIENDS 10
#define MAXANS 100
#define MAXCHOICE 5
#define MAX_YES_NO_ANS 5
#define MAX_PATH 1024
#define FD_BUF_SIZE 10
#define MAX_FIFO_MSG 1024
#define MAX_CONVO 10
#define MAX_FIFO_NAME 255
#define MAX_CWD 1024
#define RD_W_FIFO 1024
#define MAX_MSG 1024
#define MAX_PIPE_BUF 1024
#define OPEN_MAX 20

#define DIRECTORY_FIFO_FILES "/tmp/users_chat_room"
#define FIFO_NAME "/tmp/users_chat_room/my_fifo"
#define CONVO_PROG_NAME "conv_window"

typedef struct info_friend {
  char nick_name[MAXNAME];
  char email[MAXEMAIL];
} friend;

typedef struct info_user {
  char nick_name[MAXNAME];
  char email[MAXEMAIL];
  char password[MAXPASS];
  friend friends[MAX_FRIENDS];
  int  friends_ind;
  int sockfd;
  char connected;
} client;

typedef struct info_convo {
  char fifo_name_rd[MAX_FIFO_NAME];
  int  fifo_rd_fd;
  char fifo_name_w[MAX_FIFO_NAME];
  int  fifo_w_fd;
  char email_of_friend[MAXEMAIL];
  char fifo_rd_msg[MAX_FIFO_MSG];
  char fifo_w_msg[MAX_FIFO_MSG];
}conversation;

#endif
