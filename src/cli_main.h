#ifndef CLI_MAIN_H
#define CLI_MAIN_H

void display_signin_header(void);
void display_signup_header(void);
void display_chat_room_menu(void);
void poll_fd_init(void);

void main_menu_cli(void);
void signin_or_signup_menu_cli(void);

int input_from_user_ready(char *choice, int size_of_choice);

void signedin_menu_case(int choice);
void signin_or_signup_menu_case(int choice);

void messages_case(int choiceNb, char *args_from_msg[]);

void signin_response_cli(char response_from_srv);
void signup_response_cli(char response_from_srv);

bool react_to_add_friend_answer_cli(char *email_of_friend, char yesno);

void init_client(client *new_client, size_t sizeof_client);

//convos
bool cmp_convo(void* convo1, void* convo2);

bool identify_friend_with_fd(int friend_fd, conversation *conv_with_friend,
			     char mode);

#endif
