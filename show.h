#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/wait.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <dirent.h> 
#include <errno.h>
#include "user.h"
#define db_host "localhost"
#define db_user "root"
#define db_pass ""
#define db_name "network"
char *show_all_user(char *temp);
char *show_profile(char *userID, char *temp);
char *list_member(char *groupID, char *temp);
char *list_folder(char *groupID, char *temp);
char *list_file(char *groupID, char *temp);