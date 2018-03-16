#include "cli_messages.h"
#include "tcp_client.h"

extern client clients_info;

bool add_friend_message_cli(void)
{
  char msg_to_send[MAX_MSG];
  char friend_username[MAXEMAIL];

  memset(friend_username, '\0', sizeof(friend_username));
  memset(msg_to_send, '\0', sizeof(msg_to_send));
  
  printf("Please enter the  email address of your friend (ex: xyz@example.com): ");  
  fgets(friend_username, sizeof(friend_username), stdin);
  strip_newline(friend_username);

  snprintf(msg_to_send, sizeof(msg_to_send), "1:%s", friend_username);
  
  tcp_client_send(msg_to_send);
  
  return true;
}

void convo_message_cli(char *msg, char *client_email, conversation *conv_tmp)
{
  char msg_to_send[RD_W_FIFO];

  memset(msg_to_send, '\0', sizeof(msg_to_send));

  snprintf(msg_to_send, sizeof(msg_to_send), "2:%s:%s:%s",
	   client_email,
	   conv_tmp->email_of_friend,
	   msg);

  printf("About to send: %s\n", msg_to_send);
  fflush(stdout);

  tcp_client_send(msg_to_send);
}

bool signin_message_cli(void)
{
  char username[MAXNAME];
  char password[MAXPASS];
  char msg_to_send[RD_W_FIFO];

  r_user_and_pass_cli(username, password);
  
  snprintf(msg_to_send, MAX_MSG, "3:%s:%s",
	  username,
	  password);

  tcp_client_send(msg_to_send);
}

bool signup_message_cli(void)
{
  char answer[MAX_YES_NO_ANS];
  char username[MAXNAME];
  char password[MAXPASS];
  char msg_to_send[RD_W_FIFO];

  r_user_and_pass_cli(username, password);
  
  snprintf(msg_to_send, MAX_MSG, "4:%s:%s",
	  username,
	  password);

  tcp_client_send(msg_to_send);
}

void r_user_and_pass_cli(char *username, char *password)
{
  printf("Please enter your email address (ex: xyz@example.com): ");
  fgets(username, sizeof(username), stdin);
  strip_newline(username);

  printf("Please enter your password: ");
  fgets(password, sizeof(password), stdin);
  strip_newline(password);

  strncpy(clients_info.email, username, sizeof(clients_info.email));
}



