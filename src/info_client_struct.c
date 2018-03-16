#include "info_client_struct.h"

client info_client[MAXCLI];
int info_client_ind = 0;

void init_info_client(void)
{
  for (int client_no = 0; client_no < MAXCLI; client_no++)
    {
      info_client[client_no].sockfd = -1;
    }
}

bool get_user_with_email(client *user_to_get, char *email)
{
  for (int nb_users = 0; nb_users < info_client_ind; nb_users++)
    {
      if ((strncmp(info_client[nb_users].email, email, sizeof(info_client[nb_users].email)) == 0))
	{
	  *user_to_get = info_client[nb_users];
	  return true;
	}
    }
  return false;
}

bool get_user_with_fd(client *user_to_get, int fd)
{
  for (int nb_users = 0; nb_users < info_client_ind; nb_users++)
    {
      if (info_client[nb_users].sockfd == fd)
	{
	  *user_to_get = info_client[nb_users];
	  return true;
	}
    }
  return false;
}

bool validate_email_password_pair(char *email, char *pass)
{
  int nb_users;
  
  for (nb_users = 0; nb_users < info_client_ind; nb_users++) {

    if ((strncmp(info_client[nb_users].email, email, sizeof(info_client[nb_users].email)) == 0) &&
	(strncmp(info_client[nb_users].password, pass, sizeof(info_client[nb_users].password)) == 0))
      {
	return true;
      }
  }
  return false;
}

bool verify_user_exists(char *email)
{
  for (int nb_users = 0; nb_users < info_client_ind; nb_users++)
    {
      if (strncmp(info_client[nb_users].email, email, sizeof(info_client[nb_users].email)) == 0)
	{
	  return true;
	}
    }
  return false;
}

bool update_user_to_db(client *user_to_update)
{
    for (int nb_users = 0; nb_users < info_client_ind; nb_users++)
    {
      if (strncmp(info_client[nb_users].email, user_to_update->email, sizeof(info_client[nb_users].email)) == 0)
	{
	  info_client[nb_users] = *user_to_update;
	  return true;
	}
    }
  return false;
}

void add_user_to_db(char *nick_name, char* email, char* pass)
{
  client new_user;

  memset(&new_user, '\0', sizeof(client));
  
  strncpy(new_user.nick_name, nick_name, strlen(nick_name));
  strncpy(new_user.email, email, strlen(email));
  strncpy(new_user.password, pass, strlen(pass));

  strncpy(info_client[info_client_ind].nick_name, new_user.nick_name,
	  sizeof(info_client[info_client_ind].nick_name));
  strncpy(info_client[info_client_ind].email, new_user.email,
	  sizeof(info_client[info_client_ind].email));
  strncpy(info_client[info_client_ind].password, new_user.password,
	  sizeof(info_client[info_client_ind].password));
  info_client_ind++;
}
