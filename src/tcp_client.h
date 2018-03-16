#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

bool tcp_client_init(void);
bool tcp_client_send(char *msg_to_send);
bool tcp_client_receive(char *buffer, int size_of_buf);
int tcp_client_return_socket(void);

#endif
