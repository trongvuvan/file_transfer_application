#include "folder.h"
#include "user.h"
int check_dir_exist(char *dirname)
{
    DIR *dir = opendir(dirname);
    if (dir)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
void makedir(char *UserID, int sockfd)
{
    char path[BUFF_SIZE];
    char respone[BUFF_SIZE];
    printf(COLOR_GREEN"[+]You choosed Create folder.....\n Enter folder name: \n"COLOR_RESET);
    scanf("%s", path);
    getchar();
    char *request = malloc(255*sizeof(request));
    sprintf(request, "%s*%s", UserID, path);
    printf("Token : %s\n", request);

    if (send(sockfd, request, BUFF_SIZE, 0) == -1)
    {
        exit(-1);
    }

    if (recv(sockfd, respone, BUFF_SIZE, 0) == -1)
    {
        exit(-1);
    }
    printf("%s\n", respone);
    free(request);
}
void makedir_res(int sockfd)
{
    MYSQL *con;
    con = mysql_init(NULL);
    char token[BUFF_SIZE];
    char temp[BUFF_SIZE];
    char status[BUFF_SIZE];
    if (recv(sockfd, token, BUFF_SIZE, 0) > 0)
    {
        printf("Token : %s\n", token);
        strcpy(temp, token);

        char *ID = strtok(temp, "*");
        char *foldername = strtok(NULL, "/");

        char *UserID = strtok(token, "*");
        char *path = strtok(NULL, "*");

        char *GID = get_groupid_via_foldername(con, foldername);

        printf("UserID : %s\nGroupID : %s\nFoldername : %s\nFolder create :%s\n", ID, GID, foldername, path);

        int k = check_user_access(con, UserID, foldername);
        if (k == 0)
        {
            strcpy(status, "Not allow to create ");
        }
        else
        {
            char cmd[BUFF_SIZE];
            sprintf(cmd, "mkdir %s", path);
            int check = system(cmd);
            // int check;
            // check = mkdir(path, 0777);

            // check if directory is created or not
            if (!check)
            {
                con = mysql_init(NULL);
                if (con == NULL)
                {
                    fprintf(stderr, "mysql_init() failed\n");
                    exit(1);
                }

                if (mysql_real_connect(con, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
                {
                    finish_with_error(con);
                }
                char *que = malloc(SIZE * sizeof(que));
                sprintf(que, "INSERT INTO share_folder (Foldername,UserID,GroupID) values(\"%s\",\"%s\",\"%s\")", path, UserID, GID);
                if (mysql_query(con, que))
                {
                    finish_with_error(con);
                }
                printf(COLOR_GREEN"Directory created\n"COLOR_RESET);
                strcpy(status, "Directory created\n");
            }
            else
            {
                printf(COLOR_RED"Unable to create directory\n"COLOR_RESET);
                strcpy(status, "Unable to create directory\n");
            }
        }
    }
    if (send(sockfd, status, BUFF_SIZE, 0) == -1)
    {
        printf("ERROR");
    }
}
void removedir(char *UserID, int sockfd)
{
    char foldername[BUFF_SIZE];
    char respone[BUFF_SIZE];
    printf(COLOR_GREEN"[+]You choosed remove folder..... \n Enter Folder name: \n"COLOR_RESET);
    scanf("%s", foldername);
    getchar();
    char *request = malloc(255*sizeof(request));
    sprintf(request, "%s*%s",UserID, foldername);
    printf("Token : %s \n", request);

    if (send(sockfd, request, BUFF_SIZE, 0) == -1)
    {
        printf("ERROR when send token \n");
    }
    if (recv(sockfd, respone, BUFF_SIZE, 0) > 0)
    {
        printf("%s\n", respone);
    }
    bzero(respone, BUFF_SIZE);
    free(request);
}

int rmtree(int sockfd, const char path[])
{
    size_t path_len;
    char *full_path;
    DIR *dir;
    struct stat stat_path, stat_entry;
    struct dirent *entry;
    // stat for the path
    stat(path, &stat_path);
    char status[BUFF_SIZE];
    // if path does not exists or is not dir - exit with status -1
    if (S_ISDIR(stat_path.st_mode) == 0)
    {
        fprintf(stderr, "%s: %s\n", "Is not directory", path);
        strcpy(status, "Is not directory\n");
    }
    else
    { // if not possible to read the directory for this user
        if ((dir = opendir(path)) == NULL)
        {
            fprintf(stderr, "%s: %s\n", "Can`t open directory", path);
            strcpy(status, "Can`t open directory\n");
        }
        // the length of the path
        else
        {
            path_len = strlen(path);

            // iteration through entries in the directory
            while ((entry = readdir(dir)) != NULL)
            {
                // skip entries "." and ".."
                if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
                    continue;

                // determinate a full path of an entry
                full_path = calloc(path_len + strlen(entry->d_name) + 1, sizeof(char));
                strcpy(full_path, path);
                strcat(full_path, "/");
                strcat(full_path, entry->d_name);

                // stat for the entry
                stat(full_path, &stat_entry);

                // recursively remove a nested directory
                if (S_ISDIR(stat_entry.st_mode) != 0)
                {
                    rmtree(sockfd, full_path);
                    continue;
                }

                // remove a file object
                if (unlink(full_path) == 0)
                {
                    MYSQL *con = mysql_init(NULL);
                    if (con == NULL)
                    {
                        fprintf(stderr, "mysql_init() failed\n");
                        exit(1);
                    }

                    if (mysql_real_connect(con, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
                    {
                        finish_with_error(con);
                    }
                    char *que = malloc(SIZE * sizeof(que));
                    sprintf(que, "DELETE from share_file where FileName LIKE \"%s\"", full_path);
                    if (mysql_query(con, que))
                    {
                        finish_with_error(con);
                    }
                    printf("Removed a file: %s\n", full_path);
                }
                else
                {
                    printf("Can`t remove a file: %s\n", full_path);
                }
                free(full_path);
            }
            // remove the devastated directory and close the object of it
            closedir(dir);
            
            if (rmdir(path) == 0)
            {
                MYSQL *con = mysql_init(NULL);
                if (con == NULL)
                {
                    fprintf(stderr, "mysql_init() failed\n");
                    exit(1);
                }

                if (mysql_real_connect(con, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
                {
                    finish_with_error(con);
                }
                char *que = malloc(SIZE * sizeof(que));
                sprintf(que, "DELETE from share_folder where FolderName LIKE \"%s\"", path);
                if (mysql_query(con, que))
                {
                    finish_with_error(con);
                }
                printf("Removed a directory: %s\n", path);
                return 1;
            }
            else
            {
                printf("Can`t remove a directory: %s\n", path);
                return 0;
            }
        }
    }
    return 0;
}
void removedir_res(int sockfd)
{
    MYSQL *con = (MYSQL *)malloc(255*sizeof(con));
    char token[BUFF_SIZE];
    char temp[BUFF_SIZE];
    char status[BUFF_SIZE];
    if (recv(sockfd, token, BUFF_SIZE, 0) > 0)
    {
        printf("Token : %s\n", token);
        strcpy(temp, token);

        char *ID = strtok(temp, "*");
        char *foldername = strtok(NULL, "/");
        char *GroupID = get_groupid_via_foldername(con, foldername);

        char *UserID = strtok(token, "*");
        char *path = strtok(NULL, "*");
        printf("UserID : %s\nGroupID : %s\nFolder : %s\nRemove path : %s\n",ID, GroupID, foldername, path);

        if (isGroupAdmin(con,UserID,GroupID) == 0)
        {
            strcpy(status, "Not allow to remove directory \n");
        }
        else
        {
            int check = rmtree(sockfd, path);
            if (check == 1)
            {
                strcpy(status, "remove directory successfully \n");
            }
            else
            {
                strcpy(status, "remove directory failed \n");
            }
        }
    }
    if (send(sockfd, status, strlen(status), 0) > 0)
    {
        printf("Status : %s\n",status);
    }
}