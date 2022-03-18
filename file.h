#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <libgen.h>
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_RESET "\x1b[0m"
#define BUFF_SIZE 4096
#include "user.h"

#define db_host "localhost"
#define db_user "root"
#define db_pass ""
#define db_name "network"
void errorMsg(char *str);
void upload_file(char *UserID,int clientSocket);
void write_file(int clientSocket);

void download_file(char *UserID,int clientSocket);
void download_file_res(int clientSocket);

void remove_file(char *UserID,int sockfd);
void remove_res(int clientSocket);

void add_file_to_DB(MYSQL *con,char *UserID,char *filename);

void main_menu();
