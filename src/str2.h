#ifndef STR2_H
#define STR2_H

void strip_newline(char *str_to_strip);
int ret_max_str_length(char *str1, char *str2);
void parse_msg_received(char *msg, char *arg_array[], int *nb_of_args);
void free_token_array(char *token_arr[], int nb_of_elements);

#endif
