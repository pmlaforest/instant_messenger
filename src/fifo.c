#include "fifo.h"
#include "dynarray.h"

extern dynarray *convo_wth_friends;
extern int maxi_poll;
extern struct pollfd client_fd[OPEN_MAX];
//extern conversation convo_with_friends[MAX_CONVO];
extern int process_nb;
extern char cwd[MAX_CWD];

static int convo_nb = 0;

bool create_convo_with_friend(char *email_of_friend, int email_addr_len)
{
  char path_str[MAX_PATH];
  char fifo_msg[RD_W_FIFO];
  int fifo_w_fd, fifo_rd_fd;
  conversation *new_convo_with_friend;

  memset(fifo_msg, '\0', sizeof(fifo_msg));
  
  new_convo_with_friend = (conversation *) malloc(1*sizeof(conversation));
  if (new_convo_with_friend == NULL)
    {
      printf("Could not allocate memory for new conversation\n.");
      return false;
    }

  memset(new_convo_with_friend, '\0', sizeof(conversation));
  strncpy(new_convo_with_friend->email_of_friend, email_of_friend, email_addr_len);
  
  create_fifo_name_for_convo_process(new_convo_with_friend->fifo_name_rd, 2*convo_nb, process_nb);
  create_fifo_name_for_convo_process(new_convo_with_friend->fifo_name_w, 2*convo_nb + 1, process_nb);

  create_convo_process_path(path_str, sizeof(path_str),
			    cwd,
			    new_convo_with_friend->fifo_name_rd,
			    new_convo_with_friend->fifo_name_w,
			    email_of_friend);

  printf("Trying to create process with %s\n", path_str);
  
  system(path_str);

  create_fifo_streams(new_convo_with_friend);

  dynarray_add(convo_wth_friends, (void*)new_convo_with_friend);
  
  client_fd[maxi_poll].fd = new_convo_with_friend->fifo_rd_fd;
  client_fd[maxi_poll].events  = POLLIN;
  maxi_poll += 1;
  convo_nb += 1;

  free(new_convo_with_friend);
  return true;
}

bool create_fifo_streams(conversation *convo)
{
  if (access(DIRECTORY_FIFO_FILES, F_OK) == -1) {
    mkdir(DIRECTORY_FIFO_FILES, 0777);
  }
  if (access(convo->fifo_name_rd, F_OK) == -1) {
    mkfifo(convo->fifo_name_rd, 0777);
  }
  if (access(convo->fifo_name_w, F_OK) == -1) {
    mkfifo(convo->fifo_name_w, 0777);
  }

  convo->fifo_w_fd = open(convo->fifo_name_w, O_WRONLY);
  convo->fifo_rd_fd = open(convo->fifo_name_rd, O_RDONLY);

  return true;
}


bool create_fifo_name_for_convo_process(char *fifo_name, int nb_of_opened_convo, int process_nb)
{
  if (nb_of_opened_convo > MAX_CONVO)
    {
      return false;
    }
  
  snprintf(fifo_name, MAX_FIFO_NAME, "%s%d%d",
	   FIFO_NAME,
	   process_nb,
	   nb_of_opened_convo);

  return true;
}

bool create_convo_process_path(char *path_to_prog, size_t path_size, char *work_dir, char *arg1, char *arg2, char *arg3)
{
  memset(path_to_prog, '\0', path_size);

  snprintf(path_to_prog, path_size, "%s%s\"%s/%s %s %s %s\"",
	   "x-terminal-emulator",
	   " -e ",
	   work_dir,
	   CONVO_PROG_NAME,
	   arg1,
	   arg2,
	   arg3
	   );
  
  return true;
}
