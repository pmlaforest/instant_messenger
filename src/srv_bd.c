#include <stdio.h>
#include "structs_clients.h"

sqlite3 *db;

int open_bd(void)
{
  rc = sqlite3_open("clients.db", &db);
  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return(1);
  }
  return 0;
}

int create_schema_in_bd(void)
{
  char *zErrMsg = 0;
  char create_user_table_str[] = "CREATE TABLE IF NOT EXISTS user ( \
                                  user_id   int, \
                                  nick_name varchar(100), \
                                  email     varchar(100), \
                                  password  varchar(50), \ 
                                  nb_of_friends int);";

  char create_friend_table_str[] = "CREATE TABLE IF NOT EXISTS friend ( \
                                    user1_id int, \
                                    user2_id int);";

  if (opend_db() != 0){
    return -1;
  }

  int rc1 = sqlite3_exec(db, create_user_table_str, NULL, 0, &zErrMsg);
  int rc2 = sqlite3_exec(db, create_friend_table_str, NULL, 0, &zErrMsg);
  
  if( rc1!=SQLITE_OK || rc2 !=SQLITE_OK){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return -1;
  }

  return 0;
}

/*
int add_user_to_bd(client client_to_add)
{
  rc = sqlite3_exec(db, argv[2], callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
}
*/

int close_bd(void)
{
  sqlite3_close(db);
}
