#ifndef SRV_BD_H
#define SRV_BD_H

int open_bd(void);
int create_schema_in_bd(void);
int add_user_to_bd(client client_to_add);
int close_bd(void);

#endif
