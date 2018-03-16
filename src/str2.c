#include <string.h>
#include <stdlib.h>

void strip_newline(char *str_to_strip)
{
  char *p_str = str_to_strip;
  
  while (*p_str != '\0')
  {
    if (*p_str == '\n')
      {
	*p_str = '\0';
      }
    p_str++;
  }
}

int ret_max_str_length(char *str1, char *str2)
{
  if (strlen(str1) > strlen(str2))
    {
      return strlen(str1);
    }
  return strlen(str2);
}

void parse_msg_received(char *msg, char *arg_array[], int *nb_of_args)
{
  char s[2] = ":";
  char *token;
  char *arg;

  *nb_of_args = 0;
  token = strtok(msg, s);

  while(token != NULL)
     {
       arg = (char *)malloc(1024*sizeof(char));
       if (arg == NULL)
	 {
	   //pthread_exit((void*)2);
	 }
       
       arg_array[*nb_of_args] = arg;
       memset(arg_array[*nb_of_args], '\0', strlen(token) + 1);
       strncpy(arg_array[*nb_of_args], token, strlen(token));
       *nb_of_args = *nb_of_args + 1;
       token = strtok(NULL, s);
   }
}

void free_token_array(char *token_arr[], int nb_of_elements)
{
  for (int nb_of_tokens = 0; nb_of_tokens < nb_of_elements; nb_of_tokens++)
      free(token_arr[nb_of_tokens]);
}

