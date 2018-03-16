#ifndef MESSAGES_CLI_H
#define MESSAGES_CLI_H

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
#include "cli_main.h"
#include "str2.h"

bool signin_message_cli(void);
bool signup_message_cli(void);
bool add_friend_message_cli(void);
void r_user_and_pass_cli(char *username, char *password);
void convo_message_cli(char *msg, char *client_email, conversation *conv_tmp);

#endif
