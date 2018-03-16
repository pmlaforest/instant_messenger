#ifndef FIFO_H
#define FIFO_H

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


//convo process (fifo, etc)
bool create_convo_with_friend(char *email_of_friend, int email_addr_len);
bool create_convo_process_path(char *path_to_prog, size_t path_size, char *work_dir,
			       char* arg1, char *arg2, char *arg3);
bool create_fifo_name_for_convo_process(char *fifo_name, int nb_of_opened_convo,
					int process_nb);
bool create_fifo_streams(conversation *convo);

#endif





