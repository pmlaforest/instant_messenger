#ifndef INFO_CLIENT_STRUCT_H
#define INFO_CLIENT_STRUCT_H

#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/prctl.h>
#include <sys/signal.h>
#include <pthread.h>
#include <stdint.h>

#include "structs_clients.h"

bool validate_email_password_pair(char *email, char *pass);
bool get_user_with_email(client *user_to_get, char *email);
void init_info_client(void);
bool get_user_with_fd(client *user_to_get, int fd);
bool verify_user_exists(char *email);
bool update_user_to_db(client *user_to_update);
void add_user_to_db(char *nick_name, char* email, char* pass);

#endif
