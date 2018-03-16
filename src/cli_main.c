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
#include "cli_messages.h"
#include "dynarray.h"
#include "cli_main.h"
#include "str2.h"
#include "fifo.h"
#include "tcp_client.h"

client clients_info;
int maxi_poll = 0;
dynarray *convo_wth_friends;
char cwd[MAX_CWD];
int process_nb = 0;

struct pollfd client_fd[OPEN_MAX];
static char  curr_username[MAXNAME];
static int signed_in = 'n';

void poll_fd_init(void)
{
  for (int poll_fd_nb = 0; poll_fd_nb < OPEN_MAX; poll_fd_nb++)
    {
      client_fd[poll_fd_nb].fd = -1;
    }

  client_fd[maxi_poll].fd = 0; 
  client_fd[maxi_poll].events = POLLIN;
  maxi_poll += 1;

  client_fd[maxi_poll].fd = tcp_client_return_socket();
  client_fd[maxi_poll].events = POLLIN;
  maxi_poll += 1;
}

void init_client(client *new_client, size_t sizeof_client)
{
  memset(new_client, '\0', sizeof_client);

  new_client->connected = 'y';
  new_client->sockfd = tcp_client_return_socket();
}

int main(int argc, char *argv[])
{
  conversation convo_example;
    
  if (argc == 2)
    {
      process_nb = atoi(argv[1]);
    }

  if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
       perror("getcwd() error");
       return false;
    }

  tcp_client_init();

  convo_wth_friends = dynarray_create(10, (void*) &convo_example, sizeof(conversation), cmp_convo);

  poll_fd_init();

  main_menu_cli();
}

void signedin_menu_case(int choice)
{
  char email_of_friend[MAXEMAIL];
  conversation tmp_convo;

  switch(choice)
    {
    case 1:
      {
	add_friend_message_cli();
	break;
      }
    default:
      {
	strncpy(email_of_friend,
		clients_info.friends[choice - 2].email,
		sizeof(email_of_friend));

	strncpy(tmp_convo.email_of_friend, email_of_friend, MAXEMAIL);
	
	if (dynarray_search(convo_wth_friends, (void*)&tmp_convo) == -1)
	  {
	    create_convo_with_friend(email_of_friend, sizeof(email_of_friend)); 
	  }
	else
	  {
	    printf("Conversation already in place!\n");
	  }
	break;
      }
    }
}

void signin_or_signup_menu_case(int choice)
{
  switch (choice)
    {
    case 1:
      {
	display_signin_header();
	signin_message_cli();
	break;
      }
    case 2:
      {
	display_signup_header();
	signup_message_cli();
	break;
      }
    default:
      {
	printf("Error while choosing the option\n");
	break;
      }  
    }
}

void send_msg_from_friend_to_convo(char *friend_email, char *msg)
{
  int  fd_to_write;
  conversation convo_tmp;
  conversation  *search_for_fr;
  
  strncpy(convo_tmp.email_of_friend, friend_email, MAXEMAIL);

  search_for_fr = dynarray_find(convo_wth_friends, (void*) &convo_tmp);


  if (search_for_fr == NULL || search_for_fr->fifo_w_fd == -1)
    {
      printf("could not find friend!\n");
      return;
    }
  else
    {
      printf("writing to my convo\n");
      write(search_for_fr->fifo_w_fd, msg, RD_W_FIFO);
    }
}

void signin_response_cli(char response_from_srv)
{
  if (response_from_srv == 'y')
    {
      printf("Your are connected to the server!\n\n");
      signed_in = 'y';
      display_chat_room_menu();
    }
  else
    {
      exit(0);
    }
}

void signup_response_cli(char response_from_srv)
{
  if (response_from_srv == 'y')
    {
      printf("User added\n");
    }
  else
    {
      printf("Couldn't add user\n");
    }
  
  exit(0);
}

void messages_case(int choiceNb, char *args_from_msg[])
{
  switch(choiceNb)
    {
    case 2:
      {
	conversation convo_to_look_for;

	strncpy(convo_to_look_for.email_of_friend, args_from_msg[1], strlen(args_from_msg[1]));
		
	if ((dynarray_search(convo_wth_friends, (void*)&convo_to_look_for)) == -1)
	  {
	    create_convo_with_friend(args_from_msg[1], strlen(args_from_msg[1]) + 1);
	  }

	send_msg_from_friend_to_convo(args_from_msg[1], args_from_msg[3]);
	break;
      }
    case 3:
      {
	signin_response_cli(*args_from_msg[1]);
	break;
      }
    case 4:
      {
	signup_response_cli(*args_from_msg[1]);
	break;
      }
    case 5:
      {
	printf("message transmitted (%s) to %s\n", args_from_msg[2],
	       args_from_msg[1]); 
	break;
      }
    case 13:
      {
	react_to_add_friend_answer_cli(args_from_msg[1], *args_from_msg[2]);
	display_chat_room_menu();
	break;
      }
    default:
      {
	printf("response not implemented yet\n");
	break;
      }
    }
}

void main_menu_cli(void)
{
  int option_choosen = 0;
  char choice[MAXCHOICE];
  char *args_from_msg[10];
  int nb_of_args = 0;
  char first_time = 'y';
  char response_from_srv[RD_W_FIFO];
  int choiceNb;
  int nready;
  char read_buf[RD_W_FIFO];
  int ret_from_read = 0;
  conversation convo_tmp;

  signin_or_signup_menu_cli();
  
  while(1)
    {
      nready = poll(client_fd, maxi_poll + 1, -1);

      for (int poll_i = 2; poll_i < maxi_poll; poll_i++)
	{
	  memset(read_buf, '\0', sizeof(read_buf));      

	  if (client_fd[poll_i].revents & POLLIN)
	    {
	      if (identify_friend_with_fd(client_fd[poll_i].fd, &convo_tmp, 'r') == true)
		{
		  ret_from_read = read(client_fd[poll_i].fd, read_buf, sizeof(read_buf));
		  if (ret_from_read > 0)
		    {
		      convo_message_cli(read_buf, clients_info.email, &convo_tmp);
		    }
		  else if (ret_from_read == 0) {
		    printf("no data\n");
		    fflush(stdout);
		  }
		  else
		    {
		      printf("error");
		      fflush(stdout);
		    }
		}
	    }
	}

      if (client_fd[1].revents & POLLIN)
	{
	  tcp_client_receive(response_from_srv, sizeof(response_from_srv));

	  parse_msg_received(response_from_srv, args_from_msg, &nb_of_args);

	  choiceNb = atoi(args_from_msg[0]);

	  messages_case(choiceNb, args_from_msg);

	  free_token_array(args_from_msg, nb_of_args);
	}

      else if (client_fd[0].revents & POLLIN)
	{
	  option_choosen = input_from_user_ready(choice, sizeof(choice));

	  if (signed_in == 'y')
	    {
	      signedin_menu_case(option_choosen);
	    }
	  if (signed_in == 'n' && first_time == 'y')
	    {
	      first_time = 'n';
	      signin_or_signup_menu_case(option_choosen);
	    }
	  continue;
	}
    }
}

void signin_or_signup_menu_cli(void)
{
  char choice[MAXCHOICE];
  int nb_times_loop = 0;
  int option_choosen = 0;

  printf("\n");
  printf("************************\n");
  printf("Chat 2 users - main menu\n");
  printf("************************\n\n");
  printf("1) Sign in\n");
  printf("2) Sign up\n\n");

  printf("\n");
  printf("Please choose your option : ");
  fflush(stdout);
}

void display_signin_header(void)
{
  printf("\n");
  printf("Signing in!\n");
  printf("***********\n");
  fflush(stdout);
}

void display_signup_header(void)
{
  printf("\n");
  printf("Signing up!\n");
  printf("***********\n");
  fflush(stdout);
}

void display_chat_room_menu(void)
{
  printf("************************\n");
  printf("Chat 2 users - chat room\n");
  printf("************************\n\n");
  printf("1) Add a friend\n");

  if (clients_info.friends_ind > 0) {
    printf("Chat with:\n");
    for (int friends_nb = 0; friends_nb < clients_info.friends_ind; friends_nb++) {
      printf("%d) %s\n", friends_nb + 2, clients_info.friends[friends_nb].email); 
    }
  }

  printf("\n");
  printf("Please choose your option : ");
  fflush(stdout);
}

int input_from_user_ready(char *choice, int size_of_choice)
{
  int nb_times_loop = 0;
  int option_choosen = 0;

  do {
    if (nb_times_loop > 0)
      {
	printf("Please enter an available option\n");
      }

    fgets(choice, size_of_choice, stdin);

    strip_newline(choice);
    option_choosen = atoi(choice);
    nb_times_loop++;
    
  } while(strlen(choice) > 1 || option_choosen <= 0);

  return option_choosen;
}

bool identify_friend_with_fd(int friend_fd, conversation *conv_with_friend, char mode)
{
  bool condition = false;
  conversation *convo_tmp;

  for (int conv_nb = 0; conv_nb < dynarray_count(convo_wth_friends); conv_nb++)
    {
      convo_tmp = (conversation*) dynarray_element(convo_wth_friends, conv_nb);
      
      if (mode == 'r')
	{
	  condition = (convo_tmp->fifo_rd_fd == friend_fd);

	}
      else if (mode == 'w')
	{
	  condition = (convo_tmp->fifo_w_fd == friend_fd);
	}
     
       if (condition)
	{
	  *conv_with_friend = *convo_tmp;
	  return true;
	}
    }

  return false;
}

bool react_to_add_friend_answer_cli(char *email_of_friend, char yesno)
{
  friend friend_to_add;
  int ind;

  if (yesno == 'y')
    {
      memset(&friend_to_add, '\0', sizeof(friend_to_add));
      strncpy(friend_to_add.nick_name, email_of_friend, strlen(email_of_friend));
      strncpy(friend_to_add.email, email_of_friend, strlen(email_of_friend));

      clients_info.friends[clients_info.friends_ind] = friend_to_add;
      clients_info.friends_ind++;

      return true;
    }
  else
    {
      printf("something went wrong\n");
    }

  return false;
}

bool cmp_convo(void* convo1, void* convo2)
{
  conversation *convo1_cast  = (conversation*) convo1;
  conversation *convo2_cast = (conversation*) convo2;

  if (!strncmp(convo1_cast->email_of_friend, convo2_cast->email_of_friend, MAXEMAIL))
    return true;
  
  return false;
}


 


