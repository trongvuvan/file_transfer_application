#include "file.h"
#include "user.h"
void errorMsg(char *str)
{
    printf(COLOR_RED "ERROR: %s\n" COLOR_RESET, str);
}

void upload_file(char *UserID, int new_socket)
{
    ssize_t total = 0;
    printf(COLOR_BLUE "\n[+] Uploading ....." COLOR_RESET);
    char *filename = malloc(BUFF_SIZE * sizeof(char));
    printf(COLOR_GREEN "\n[+] Enter filename : " COLOR_RESET);
    scanf("%[^\n]", filename);
    getchar();
    // printf("file name: %s\n", filename);

    char *filepath = malloc(BUFF_SIZE * sizeof(char));
    printf(COLOR_GREEN "\n[+] Enter destination : " COLOR_RESET);
    scanf("%[^\n]", filepath);
    getchar();
    // printf("Destination path : %s\n", filepath);

    char *name = basename(filename);
    char request[BUFF_SIZE] = "";

    sprintf(request, "%s*%s/%s", UserID, filepath, name);
    // printf("token send: %s\n", request);

    FILE *fd = fopen(filename, "r");
    // checking if the file exist or not
    if (fd == NULL)
    {
        errorMsg("\n[-] File not found !!");
    }
    else
    {
        printf("\n[+] File found !!\n");

        if (send(new_socket, request, BUFF_SIZE, 0) == -1)
        {
            errorMsg("\n[-] Error sending file path ");
        }
        char conf[25];
        if (read(new_socket, conf, BUFF_SIZE) == -1)
        {
            errorMsg("Couldn't receive confirmation message");
        }
        else
        {
            // check have or not
            if (strcmp(conf, "notallow") == 0)
            {
                errorMsg("Not allow to upload \n");
            }
            else
            {

                if (strcmp(conf, "exist") == 0)
                {
                    errorMsg("File exist \n");
                }
                else
                {

                    fseek(fd, 0L, SEEK_END);
                    long int sz = ftell(fd);
                    fclose(fd);

                    // opening file
                    int fp = open(filename, O_RDONLY);
                    if (fp == -1)
                    {
                        perror("Couldn't open file");
                    }

                    ssize_t n;

                    printf("\n[+] File size: %ld", sz);
                    // sending file size to client
                    if (write(new_socket, &sz, sizeof(sz)) == -1)
                    {
                        perror("write");
                    }

                    char buffer[BUFF_SIZE] = "";

                    // reading from file and sending to client
                    while (1)
                    {
                        n = read(fp, buffer, BUFF_SIZE);
                        if (n == 0)
                            break;
                        if (n == -1)
                        {
                            perror("read");
                            break;
                        }
                        if (write(new_socket, buffer, n) == -1)
                        {
                            perror("write");
                            break;
                        }
                        total += n;
                        int percen = (total * 100) / sz;

                        printf(COLOR_GREEN "\r%d %% upload. total %ld bytes" COLOR_RESET, percen, total);
                    }
                    close(fp);
                    char respone[BUFF_SIZE] = {0};
                    if (recv(new_socket, respone, BUFF_SIZE, 0) > 0)
                    {
                        printf("\n%s", respone);
                    }
                }
            }
        }
    }
    free(filename);
    free(filepath);
}
void write_file(int sock)
{
    MYSQL *con;
    con = mysql_init(NULL);

    char status[BUFF_SIZE] = "";
    char buffer[BUFF_SIZE] = "";
    char token[BUFF_SIZE] = "";
    if (recv(sock, token, BUFF_SIZE, 0) == -1)
    {
        errorMsg("error\n");
    }
    else
    {

        printf("token received : %s \n", token);
        if (strlen(token) < 10)
        {
        }
        else
        {
            ssize_t total = 0;
            ssize_t n = 0;
            char temp[BUFF_SIZE];
            strcpy(temp, token);

            char *ID = strtok(token, "*");
            char *foldername = strtok(NULL, "/");
            char *GID = get_groupid_via_foldername(con, foldername);
            char *FolderID = get_folderid_via_foldername(con, foldername);
            char *UserID = strtok(temp, "*");
            char *filepath = strtok(NULL, "*");
            printf("UserID : %s \nGroupID : %s\nfoldername: %s \nFolerID : %s\nfilepath: %s \n", ID, GID, foldername, FolderID, filepath);
            char buff[BUFF_SIZE] = "";

            strncpy(buff, filepath, strlen(filepath));

            // checking if the file exist or not
            int k = check_user_access(con, UserID, foldername);
            if (k == 0)
            {
                char conf2[9] = "notallow";
                char buf[BUFF_SIZE] = {0};
                strncpy(buf, conf2, strlen(conf2));
                if (send(sock, buf, BUFF_SIZE, 0) == -1)
                {
                    perror("Couldn't send file confirmation message.\n");
                }
            }
            else
            {
                FILE *fd = fopen(filepath, "r");
                if (fd != NULL)
                {
                    send(sock, "exist", strlen("exist"), 0);
                }
                else
                {
                    send(sock, "allow", strlen("allow"), 0);
                    // opening a file with needed filename
                    int fp = open(filepath, O_WRONLY | O_CREAT, 0777);
                    if (fp == -1)
                    {
                        strcpy(status, "Error, file exists\n");
                        perror("Can't open file");
                    }
                    else
                    {
                        long int sz;

                        // reading file size
                        if (read(sock, &sz, sizeof(sz)) == -1)
                        {
                            perror("read");
                        }

                        printf("size: %ld", sz);

                        // reading from socket and writing to file
                        // block by block till all bytes are read
                        do
                        {
                            n = read(sock, buffer, BUFF_SIZE);

                            if (n == -1)
                            {
                                perror("read");
                                break;
                            }
                            if (write(fp, buffer, n) == -1)
                            {
                                perror("write");
                                break;
                            }
                            // percentage of file downloaded
                            total += n;
                            int percen = (total * 100) / sz;

                            printf(COLOR_BLUE "\r%d %% received. total %ld bytes" COLOR_RESET, percen, total);

                        } while (n > 0 && total < sz);
                        close(fp);
                        printf("\n");
                        con = mysql_init(NULL);
                        if (con == NULL)
                        {
                            fprintf(stderr, "mysql_init() failed\n");
                        }
                        if (mysql_real_connect(con, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
                        {
                            finish_with_error(con);
                        }
                        char *check = malloc(SIZE * sizeof(check));
                        sprintf(check, "SELECT *from share_file where FileName LIKE \"%s\"", filepath);
                        if (mysql_query(con, check))
                        {
                            finish_with_error(con);
                        }
                        MYSQL_RES *result = mysql_store_result(con);
                        if (result == NULL)
                        {
                            finish_with_error(con);
                        }
                        MYSQL_ROW row;
                        if (!(row = mysql_fetch_row(result)))
                        {
                            char *que = malloc(SIZE * sizeof(que));
                            sprintf(que, "INSERT INTO share_file(FileName,UserID,FolderID) values(\"%s\",\"%s\",\"%s\")", filepath, UserID, FolderID);
                            if (mysql_query(con, que))
                            {
                                finish_with_error(con);
                            }
                            free(que);
                        }
                        strcpy(status, "successfully \n");
                    }
                    if (send(sock, status, BUFF_SIZE, 0) > 0)
                    {
                        printf("status : %s", status);
                    }
                }
            }
        }
    }
    // getting next argument
}

// DOWNLOAD
// DOWNLOAD
// DOWNLOAD
// DOWNLOAD

// DOWNLOAD
// DOWNLOAD
// DOWNLOAD
// DOWNLOAD

void download_file(char *UserID, int sock)
{
    // char respone[BUFF_SIZE] = {0};
    char buffer[BUFF_SIZE] = {0};
    char *filepath = malloc(100 * sizeof(char));
    printf(COLOR_GREEN "Enter file path to download : " COLOR_RESET);
    scanf("%[^\n]", filepath);
    getchar();

    long int sz;
    
    ssize_t n = 0;
    char request[BUFF_SIZE] = {0};
    sprintf(request, "%s*%s", UserID, filepath);
    char token[BUFF_SIZE] = {0};
    strncpy(token, request, strlen(request));
    // printf("UserID : %s \nfilepath : %s\n", UserID, filepath);
    // printf("Token : %s\n", token);

    // check file exist
    char *filename = basename(filepath);
    FILE *ff = fopen(filename, "r");
    if (ff != NULL)
    {
        errorMsg("[+] File exist \n");
    }
    else
    {
        if (send(sock, token, BUFF_SIZE, 0) == -1)
        {
            perror("Couldn't send filename");
        }
        else
        {
            char conf[9] = {0};
            if (read(sock, conf, BUFF_SIZE) == -1)
            {
                perror("Couldn't receive confirmation message");
            }
            else
            {
                // check have or not
                printf(COLOR_BLUE "Confirm: %s in server\n" COLOR_RESET, conf);
                if (strcmp(conf, "notfound") == 0)
                {
                    printf(COLOR_RED "No such file in server: %s\n" COLOR_RESET, filepath);
                }
                else
                {
                    char conf2[25] = "";
                    if (read(sock, conf2, BUFF_SIZE) == -1)
                    {
                        perror("Couldn't receive confirmation message");
                    }
                    // check permission
                    if (strcmp(conf2, "notallow") == 0)
                    {
                        errorMsg("Not allow to download \n");
                    }
                    else
                    {
                        ssize_t total = 0;
                        // getting filename
                        char *filename = basename(filepath);
                        // opening a file with needed filename
                        int fp = open(filename, O_WRONLY | O_CREAT, 0777);
                        if (fp == -1)
                        {
                            perror("Can't open file");
                        }

                        // reading file size
                        if (read(sock, &sz, sizeof(sz)) == -1)
                        {
                            perror("read");
                        }

                        printf("size: %ld", sz);

                        // reading from socket and writing to file
                        // block by block till all bytes are read
                        do
                        {
                            n = read(sock, buffer, BUFF_SIZE);

                            if (n == -1)
                            {
                                perror("read");
                                break;
                            }
                            if (write(fp, buffer, n) == -1)
                            {
                                perror("write");
                                break;
                            }
                            // percentage of file downloaded
                            
                            total += n;
                            int percen = (total * 100) / sz;

                            printf(COLOR_GREEN "\r%d %% downloaded , total %ld bytes" COLOR_RESET, percen, total);
                        } while (n > 0 && total < sz);
                        printf("\n");
                        close(fp);
                        if (total == sz)
                        {
                            printf(COLOR_GREEN "Download successfully \n" COLOR_RESET);
                        }
                        else
                            printf(COLOR_RED "Download failed \n" COLOR_RESET);
                    }
                }
                // get status
            }
        }
    }
    free(filepath);
    // getting next argument
}
void download_file_res(int new_socket)
{
    MYSQL *con;
    con = mysql_init(NULL);

    // char status[BUFF_SIZE] = {0};
    char token[BUFF_SIZE] = {0};
    char temp[BUFF_SIZE] = {0};
    if (read(new_socket, token, BUFF_SIZE) == -1)
    {
        perror("Couldn't receive command");
    }
    else
    {
        printf("\nToken :%s \n", token);
        strcpy(temp, token);

        char *ID = strtok(temp, "*");
        char *foldername = strtok(NULL, "/");

        char *UserID = strtok(token, "*");
        char *file = strtok(NULL, "*");
        char filename[BUFF_SIZE] ={0};
        char *GID = get_groupid_via_foldername(con, foldername);
        strcpy(filename, file);
        printf("UserID : %s\nFile belong to Group : %s\nfoldername :%s \nFilename : %s\n", ID, GID, foldername, filename);

        printf(COLOR_BLUE "[+] Sending ....." COLOR_RESET);
        printf("%s", filename);
        FILE *fd = fopen(filename, "r");

        // checking if the file exist or not
        if (fd == NULL)
        {
            char conf[9] = "notfound";
            char buf[BUFF_SIZE] = {0};
            strncpy(buf, conf, strlen(conf));
            if (send(new_socket, buf, BUFF_SIZE, 0) == -1)
            {
                perror("Couldn't send file confirmation message.\n");
            }
        }
        else
        {
            char conf[6] = "found";
            char buf[BUFF_SIZE] = {0};
            strncpy(buf, conf, strlen(conf));
            // send confirm
            if (send(new_socket, buf, BUFF_SIZE, 0) == -1)
            {
                perror("Couldn't send file confirmation message.\n");
            }
            // check allow
            if (check_user_access(con, UserID, foldername) == 0)
            {
                send(new_socket, "notallow", BUFF_SIZE, 0);
                // strcpy(status, "download failed");
            }
            else
            {
                send(new_socket, "allow", BUFF_SIZE, 0);
                // getting size of file
                fseek(fd, 0L, SEEK_END);
                long int sz = ftell(fd);
                fclose(fd);

                // opening file
                int fp = open(filename, O_RDONLY);
                if (fp == -1)
                {
                    perror("Couldn't open file");
                }

                ssize_t n = 0;
                //ssize_t total = 0;
                // printf("size: %d", sz);
                // sending file size to client
                if (write(new_socket, &sz, sizeof(sz)) == -1)
                {
                    perror("write");
                }

                char buffer[BUFF_SIZE] = {0};

                // reading from file and sending to client
                while (1)
                {
                    n = read(fp, buffer, BUFF_SIZE);
                    if (n == 0)
                        break;
                    if (n == -1)
                    {
                        perror("read");
                        break;
                    }
                    if (write(new_socket, buffer, n) == -1)
                    {
                        perror("write");
                        break;
                    }
                }
                // strcpy(status, "\ndownload successfully \n");
                //  send(new_socket, status, strlen(status), 0);
                close(fp);
            }
            // send status
            // if (send(new_socket, status, BUFF_SIZE, 0) > 0)
            // {
            //     printf("status : %s\n",status);
            // }
        }
    }
}
// REMOVE
// REMOVE
// REMOVE
// REMOVE
// REMOVE
// REMOVE
// REMOVE

void remove_file(char *UserID, int sockfd)
{
    char buff[BUFF_SIZE] = {0};
    char filename[BUFF_SIZE] = {0};
    printf(COLOR_GREEN "\n[+]Remove............." COLOR_RESET);
    printf(COLOR_GREEN "\n[+]Enter filename to remove : " COLOR_RESET);
    scanf("%[^\n]", filename);
    getchar();
    // printf("%s\n", filename);
    char *sen = malloc(255 * sizeof(char));
    sprintf(sen, "%s*%s", UserID, filename);
    // printf("send token : %s", sen);
    if (send(sockfd, sen, BUFF_SIZE, 0) == -1)
    {
        errorMsg("?");
    }

    if (recv(sockfd, buff, BUFF_SIZE, 0) == -1)
    {
        errorMsg("?");
    }
    printf(COLOR_BLUE "\n%s" COLOR_RESET, buff);
    memset(buff, '\0', strlen(buff));
    memset(filename, '\0', strlen(filename));
    free(sen);
}

// completed
void remove_res(int sockfd)
{
    MYSQL *con = mysql_init(NULL);
    char buff[BUFF_SIZE] = {0};
    char temp[BUFF_SIZE] = {0};
    char status[BUFF_SIZE] = {0};
    if (recv(sockfd, buff, BUFF_SIZE, 0) > 0)
    {
        strcpy(temp, buff);
        char *ID = strtok(temp, "*");
        printf("UserID : %s\n", ID);
        char *foldername = strtok(NULL, "/");
        printf("Foldername : %s\n", foldername);

        char *UserID = strtok(buff, "*");
        char *filepath = strtok(NULL, "*");
        printf("filepath : %s\n", filepath);
        char *filename = basename(filepath);
        printf("filename : %s\n", filename);
        char *GID = get_groupid_via_foldername(con, foldername);
        printf("GroupID : %s\n", GID);

        if (isGroupAdmin(con, UserID, GID) == 0 && check_file_owner(con, UserID, filepath) == 0)
        {
            printf("[+] The file is not deleted\n");
            strcpy(status, "[+] The file is not deleted maybe you don't have rights to remove \n");
        }
        else
        {
            int del = remove(filepath);
            printf(COLOR_GREEN "[+] Remove file %s\n" COLOR_RESET, filename);
            if (!del)
            {
                con = mysql_init(NULL);
                if (con == NULL)
                {
                    fprintf(stderr, "mysql_init() failed\n");
                }

                if (mysql_real_connect(con, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
                {
                    finish_with_error(con);
                }
                char *que = malloc(SIZE * sizeof(que));
                sprintf(que, "DELETE FROM share_file WHERE FileName LIKE \"%s\"", filepath);
                if (mysql_query(con, que))
                {
                    finish_with_error(con);
                }

                printf(COLOR_BLUE "[+] The file is deleted successfully.\n" COLOR_RESET);
                strcpy(status, "[+] The file is deleted successfully\n");
            }
            else
            {
                printf(COLOR_RESET "[+] The file is not deleted.\n" COLOR_RESET);
                strcpy(status, "[+] The file is not deleted, check filename again\n");
            }
        }
    }
    send(sockfd, status, BUFF_SIZE, 0);
}
