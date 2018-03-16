#include "srv_actions.h"
#include "srv_main.h"

void add_friend_action_srv(int sockfd, client *curr_client, char *email_of_friend)
{
  bool friend_added;
  
  friend_added = add_friend_srv(curr_client, email_of_friend);
  
  if (friend_added == false)
    {
      printf("Friend couldn't be added!\n");
      send_response_to_client_srv(sockfd, email_of_friend, 'n', "13");
    }
  else
    {
      printf("Friend accepted\n");
      send_response_to_client_srv(sockfd, email_of_friend, 'y', "13");
    }
}

void chat_with_friend_action_srv(int sockfd, char *dest, char *msg_cp, int msg_size)
{
  client new_client;
  int msg_transmitted;
  
  msg_transmitted = get_user_with_email(&new_client, dest);

  if (msg_transmitted == false)
    {
      printf("Couldn't find user!\n");
      send_response_to_client_srv(sockfd, dest, 'n', "5");
      return;
    }

  if (new_client.sockfd == -1)
    {
      printf("Client not connected!\n");
      send_response_to_client_srv(sockfd, dest, 'n', "5");
      return;
    }
  else
    {
      write(new_client.sockfd, msg_cp, msg_size);
    }
	  
  send_response_to_client_srv(sockfd, dest, 'y', "5");
}

bool signin_action_srv(int sockfd, char *username, char *password)
{
  char msg3[4];
  client newly_connected_client;
  
  memset(msg3, '\0', sizeof(msg3));

  if (validate_email_password_pair(username, password) == false)
    {
      strncpy(msg3, "3:n", strlen("3:n"));
      write(sockfd, msg3, sizeof(msg3)); 
      return false;
    }

  get_user_with_email(&newly_connected_client, username);
  newly_connected_client.sockfd = sockfd;
  newly_connected_client.connected = 'y';
  update_user_to_db(&newly_connected_client);
  
  strncpy(msg3, "3:y", strlen("3:y"));
  write(sockfd, msg3, sizeof(msg3));
  return true;
}

void signup_action_srv(int sockfd, char *username, char *password)
{
  char msg4[4];

  memset(msg4, '\0', sizeof(msg4));
  strncpy(msg4, "4:n", strlen("4:n"));

  if (verify_user_exists(username) == false)
    {
      add_user_to_db(username, username, password);
      strncpy(msg4, "4:y", strlen("4:y"));
    }

  write(sockfd, msg4, sizeof(msg4));      

  pthread_exit((void *)2);
}

void send_response_to_client_srv(int sockfd, char *email, char yesno, char *action_str)
{
  char msg_to_send[RD_W_FIFO];
  
  memset(msg_to_send, '\0', sizeof(msg_to_send));

  snprintf(msg_to_send, sizeof(msg_to_send), "%s:%s:%c",
	   action_str,
	   email,
	   yesno);
  
  write(sockfd, msg_to_send, sizeof(msg_to_send));
}
