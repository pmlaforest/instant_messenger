#ifndef SRV_ACTIONS_H
#define SRV_ACTIONS_H

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
#include "info_client_struct.h"
#include "str2.h"

// type of actions
void add_friend_action_srv(int sockfd, client *curr_client, char *email_of_friend);
void chat_with_friend_action_srv(int sockfd, char *dest, char *msg_cp, int msg_size);
bool signin_action_srv(int sockfd, char *username, char *password);
void signup_action_srv(int sockfd, char *username, char *password);
void send_response_to_client_srv(int sockfd, char *email, char yesno, char *action_str);
#endif

