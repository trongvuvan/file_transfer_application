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

int access_group(char *groupID, char *userID);
int access_share_group(char *groupID, char *userID);
char *my_group(char *userID, char *temp);
char *shareGroup(char *userID, char *temp);
int create_group(char *userID, char *groupName, char *groupDescrp);
int delete_group(char *groupID);
int out_group(char *userID, char *groupID);