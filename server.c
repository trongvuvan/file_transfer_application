#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>
#include <libgen.h>
#include <unistd.h>
#include <mysql/mysql.h>

#define BUFF_SIZE 4096
#define MIN_SIZE 255

#define SIGN_IN 0
#define SIGN_UP 1
#define PROFILE 2
#define MY_GROUP 3
#define ACCESS_GROUP 4
#define CREATE_GROUP 5
#define SHARE_GROUP 6
#define LIST_MEMBER 7
#define ADD_MEMBER 8
#define DELETE_MEMBER 9
#define DELETE_GROUP 10
#define CREATE_DIR 11
#define REMOVE_DIR 12
#define UPLOAD_FILE 13
#define DOWNLOAD_FILE 14
#define REMOVE_FILE 15
#define OUT_GROUP 16
#define ACCESS_SHARE_GROUP 17
#define LIST_FOLDER 18
#define LIST_FILE 19
#define LIST_ALL_USER 20


#include "menu.h"
#include "file.h"
#include "user.h"
#include "folder.h"
#include "show.h"
#include "group.h"

void *connection_handler(void *client_socket)
{
    int socket = *(int *)client_socket;

    char server_message[100] = "_____________________________WELCOME TO MY PROJECT_____________________________\n";
    send(socket, server_message, sizeof(server_message), 0);

    while (1)
    {
        int read_len;
        char client_message[BUFF_SIZE] = "";
        read_len = recv(socket, client_message, BUFF_SIZE, 0);
        // printf("Check read_len : %d\n", read_len);
        if (read_len > 0)
        {
            client_message[read_len] = '\0';
            if (strlen(client_message) > 0)
            {
                char temp[read_len];
                strcpy(temp, client_message);

                char *func_code = strtok(temp, "*");
                // printf("Token get : %s %s\n", func_code, client_message);
                int check_code = atoi(func_code);

                switch (check_code)
                {
                case LIST_FOLDER:
                {
                    printf("------------List folder --------------\n");
                    char *code = strtok(client_message, "*");
                    char *groupID = strtok(NULL, "*");
                    char temp[1024] = "";
                    printf("code : %s\n", code);
                    printf("groupID : %s\n", groupID);
                    char *status = list_folder(groupID, temp);
                    printf("status : %s\n", status);
                    if (strcmp(temp, "0") == 0)
                    {
                        send(socket, temp, strlen(temp), 0);
                    }
                    else
                    {
                        send(socket, status, strlen(status), 0);
                    }
                    break;
                }
                case LIST_FILE:
                {
                    printf("------------List file --------------\n");
                    char *code = strtok(client_message, "*");
                    char *groupID = strtok(NULL, "*");
                    char temp[1024] = "";
                    printf("code : %s\n", code);
                    printf("groupID : %s\n", groupID);
                    char *status = list_file(groupID, temp);
                    printf("status : %s, %ld\n", status, strlen(status));
                    if (strcmp(status, "0") == 0)
                    {
                        printf("gui cai nay 1\n");
                        if (send(socket, temp, strlen(temp), 0) < 0)
                        {
                            printf("send error 1\n");
                        }
                    }
                    else
                    {
                        printf("gui cai nay 2\n");
                        if (send(socket, status, strlen(status), 0) < 0)
                        {
                            printf("send error 2\n");
                        }
                        else
                        {
                            printf("gui thanh cong\n");
                        }
                    }
                    break;
                }
                case SIGN_IN:
                {
                    char *code = strtok(client_message, "*");
                    char *email = strtok(NULL, "*");
                    char *password = strtok(NULL, "*");
                    char temp[255] = "";
                    printf("code : %s\n", code);
                    printf("email : %s\n", email);
                    printf("pass : %s\n", password);
                    char *status = sign_in(email, password, temp);
                    printf("status : %s\n", status);
                    if (strcmp(temp, "0") == 0)
                    {
                        int send_mes_client = send(socket, temp, sizeof(temp), 0);
                        printf(" byte send 0 : %d\n", send_mes_client);
                    }
                    else
                    {
                        int send_mes_client = send(socket, status, strlen(status), 0);
                        printf(" byte send 1 : %d\n", send_mes_client);
                    }
                    break;
                }
                case SIGN_UP:
                {
                    char *code = strtok(client_message, "*");
                    char *username = strtok(NULL, "*");
                    char *email = strtok(NULL, "*");
                    char *password = strtok(NULL, "*");
                    printf(" code : %s\n", code);
                    printf("username: %s\n ", username);
                    printf("email : %s\n", email);
                    printf("pass : %s\n", password);
                    int status = sign_up(username, email, password);
                    printf("status : %d\n", status);
                    if (status == 1)
                    {
                        char status[MIN_SIZE] = "1";
                        send(socket, status, sizeof(status), 0);
                    }
                    else if (status == 2)
                    {
                        char status[MIN_SIZE] = "2";
                        send(socket, status, sizeof(status), 0);
                    }
                    else
                    {
                        char status[MIN_SIZE] = "0";
                        send(socket, status, sizeof(status), 0);
                    }
                    break;
                }
                case PROFILE:
                {
                    char *code = strtok(client_message, "*");
                    char *userID = strtok(NULL, "*");
                    char temp[1024] = "";
                    printf("code : %s\n", code);
                    printf("userID : %s\n", userID);
                    char *status = show_profile(userID, temp);
                    printf("status : %s\n", status);
                    if (strcmp(temp, "0") == 0)
                    {
                        send(socket, temp, strlen(temp), 0);
                    }
                    else
                    {
                        send(socket, status, strlen(status), 0);
                    }
                    break;
                }
                case LIST_ALL_USER:
                {
                    printf("------------List all user--------------\n");
                    char *code = strtok(client_message, "*");
                    char *userID = strtok(NULL, "*");
                    char temp[1024] = "";
                    printf("code : %s\n", code);
                    printf("userID : %s\n", userID);
                    char *status = show_all_user(temp);
                    printf("status : %s\n", status);
                    if (strcmp(temp, "0") == 0)
                    {
                        send(socket, temp, strlen(temp), 0);
                    }
                    else
                    {
                        send(socket, status, strlen(status), 0);
                    }
                    break;
                }
                case MY_GROUP:
                {
                    printf("------------My group --------------\n");
                    char *code = strtok(client_message, "*");
                    char *userID = strtok(NULL, "*");
                    char temp[1024] = "";
                    printf("code : %s\n", code);
                    printf("userID : %s\n", userID);
                    char *status = my_group(userID, temp);
                    printf("status : %s\n", status);
                    printf("temp : %s\n", temp);
                    if (strcmp(status, "0") == 0)
                    {
                        send(socket, temp, strlen(temp), 0);
                    }
                    else
                    {
                        send(socket, status, strlen(status), 0);
                    }
                    break;
                }
                case ACCESS_GROUP:
                {
                    printf("------------Access group --------------\n");
                    char *code = strtok(client_message, "*");
                    char *groupID = strtok(NULL, "*");
                    char *userID = strtok(NULL, "*");
                    printf("code : %s\n", code);
                    printf("groupID : %s\n", groupID);
                    printf("userID : %s\n", userID);
                    int status = access_group(groupID, userID);
                    printf("status : %d\n", status);

                    if (status == 0)
                    {
                        char buff[2] = "0";
                        send(socket, buff, strlen(buff), 0);
                    }
                    else
                    {
                        char buff[2] = "1";
                        send(socket, buff, strlen(buff), 0);
                    }
                    break;
                }
                case ACCESS_SHARE_GROUP:
                {
                    printf("------------Access share group --------------\n");
                    char *code = strtok(client_message, "*");
                    char *groupID = strtok(NULL, "*");
                    char *userID = strtok(NULL, "*");
                    printf("code : %s\n", code);
                    printf("groupID : %s\n", groupID);
                    printf("userID : %s\n", userID);
                    int status = access_share_group(groupID, userID);
                    printf("status : %d\n", status);

                    if (status == 0)
                    {
                        char buff[2] = "0";
                        send(socket, buff, strlen(buff), 0);
                    }
                    else
                    {
                        char buff[2] = "1";
                        send(socket, buff, strlen(buff), 0);
                    }
                    break;
                }
                case CREATE_GROUP:
                {
                    char *code = strtok(client_message, "*");
                    char *userID = strtok(NULL, "*");
                    char *groupName = strtok(NULL, "*");
                    char *groupDescrp = strtok(NULL, "*");
                    printf("---------------TAO NHOM------------- \n");
                    printf("code : %s\n", code);
                    printf("userID : %s\n", userID);
                    printf("group name : %s\n", groupName);
                    printf("group des : %s\n", groupDescrp);
                    int status = create_group(userID, groupName, groupDescrp);
                    printf("status : %d\n", status);
                    if (status == 0)
                    {
                        char status[MIN_SIZE] = "0";
                        send(socket, status, sizeof(status), 0);
                    }
                    else if (status == 1)
                    {
                        char status[MIN_SIZE] = "1";
                        send(socket, status, sizeof(status), 0);
                    }
                    else
                    {
                        char status[MIN_SIZE] = "2";
                        send(socket, status, sizeof(status), 0);
                    }
                    break;
                }
                case LIST_MEMBER:
                {
                    printf("------------List member --------------\n");
                    char *code = strtok(client_message, "*");
                    char *groupID = strtok(NULL, "*");
                    char temp[1024] = "";
                    printf("code : %s\n", code);
                    printf("groupID : %s\n", groupID);
                    char *status = list_member(groupID, temp);
                    printf("status : %s\n", status);
                    if (strcmp(temp, "0") == 0)
                    {
                        send(socket, temp, strlen(temp), 0);
                    }
                    else
                    {
                        send(socket, status, strlen(status), 0);
                    }
                    break;
                }

                case SHARE_GROUP:
                {
                    printf("------------My share group --------------\n");
                    char *code = strtok(client_message, "*");
                    char *userID = strtok(NULL, "*");
                    char temp[1024] = "";
                    printf("code : %s\n", code);
                    printf("userID : %s\n", userID);
                    char *status = shareGroup(userID, temp);
                    printf("status : %s\n", status);
                    printf("temp : %s\n", temp);
                    if (strcmp(temp, "0") == 0)
                    {
                        send(socket, temp, strlen(temp), 0);
                    }
                    else
                    {
                        send(socket, status, strlen(status), 0);
                    }
                    break;
                }
                case ADD_MEMBER:
                {
                    char *code = strtok(client_message, "*");
                    char *email = strtok(NULL, "*");
                    char *groupID = strtok(NULL, "*");
                    printf(" code : %s\n", code);
                    printf("email: %s\n ", email);
                    printf("groupID: %s\n ", groupID);
                    int status = add_member(email, groupID);
                    printf("status : %d\n", status);
                    if (status == 1)
                    {
                        char status[MIN_SIZE] = "1";
                        send(socket, status, sizeof(status), 0);
                    }
                    else if (status == 2)
                    {
                        char status[MIN_SIZE] = "2";
                        send(socket, status, sizeof(status), 0);
                    }
                    else
                    {
                        char status[MIN_SIZE] = "0";
                        send(socket, status, sizeof(status), 0);
                    }
                    break;
                }
                case DELETE_MEMBER:
                {
                    printf("------------Delete member --------------\n");
                    char *code = strtok(client_message, "*");
                    char *email = strtok(NULL, "*");
                    char *groupID = strtok(NULL, "*");
                    printf("code : %s\n", code);
                    printf("email : %s\n", email);
                    printf("groupID : %s\n", groupID);
                    int status = delete_member(email, groupID);
                    printf("status : %d\n", status);

                    if (status == 0)
                    {
                        char buff[2] = "0";
                        send(socket, buff, strlen(buff), 0);
                    }
                    else if (status == 2)
                    {
                        char buff[2] = "2";
                        send(socket, buff, strlen(buff), 0);
                    }
                    else
                    {
                        char buff[2] = "1";
                        send(socket, buff, strlen(buff), 0);
                    }
                    break;
                }
                case DELETE_GROUP:
                {
                    printf("------------Delete group --------------\n");
                    printf("check chuoi nhan dc : %s\n", client_message);
                    char *code = strtok(client_message, "*");
                    char *groupID = strtok(NULL, "*");
                    printf("code : %s\n", code);
                    printf("groupID : %s\n", groupID);
                    int status = delete_group(groupID);
                    printf("status : %d\n", status);

                    if (status == 0)
                    {
                        char buff[2] = "0";
                        send(socket, buff, strlen(buff), 0);
                    }
                    else
                    {
                        char buff[2] = "1";
                        send(socket, buff, strlen(buff), 0);
                    }
                    break;
                }
                case OUT_GROUP:
                {
                    printf("------------Out group --------------\n");
                    printf("check chuoi nhan dc : %s\n", client_message);
                    char *code = strtok(client_message, "*");
                    char *userID = strtok(NULL, "*");
                    char *groupID = strtok(NULL, "*");
                    printf("code : %s\n", code);
                    printf("userID : %s\n", userID);
                    printf("groupID : %s\n", groupID);
                    int status = out_group(userID, groupID);
                    printf("status : %d\n", status);

                    if (status == 0)
                    {
                        char buff[2] = "0";
                        send(socket, buff, strlen(buff), 0);
                    }
                    else
                    {
                        char buff[2] = "1";
                        send(socket, buff, strlen(buff), 0);
                    }
                    break;
                }
                case REMOVE_FILE:
                    remove_res(socket);
                    break;
                case UPLOAD_FILE:
                    write_file(socket);
                    break;
                case DOWNLOAD_FILE:
                    download_file_res(socket);
                    break;
                case CREATE_DIR:
                    makedir_res(socket);
                    break;
                case REMOVE_DIR:
                    removedir_res(socket);
                    break;

                default:
                    break;
                }
            }
        }
        else
        {
            printf("Client exit! !\n");
            break;
        }
    }

    return 0;
}

int main(int argc, const char *argv[])
{
    if( argc < 2)
    {
        printf("Enter port pleaseeeee \n");
    }
    int port = atoi(argv[1]);
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1)
    {
        perror("Socket initialisation failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        printf("Server socket created successfully\n");
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // bind the socket to the specified IP addr and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
    {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
    {
        printf("Socket successfully binded..\n");
    }

    if (listen(server_socket, 100) != 0)
    {
        printf("Listen failed...\n");
        exit(0);
    }
    else
    {
        printf("Server listening..\n");
    }

    int no_threads = 0;
    pthread_t threads[100];
    while (1)
    {
        printf("Listening...\n");
        int client_socket = accept(server_socket, NULL, NULL);
        puts("Connection accepted");
        if (pthread_create(&threads[no_threads], NULL, connection_handler, &client_socket) < 0)
        {
            perror("Could not create thread");
            return 1;
        }
        if (client_socket < 0)
        {
            printf("server acccept failed...\n");
            exit(0);
        }
        else
        {
            printf("Server acccept the client...\n");
        }
        puts("Handler assigned");
        no_threads++;
    }
    int k = 0;
    for (k = 0; k < 100; k++)
    {
        pthread_join(threads[k], NULL);
    }
    close(server_socket);

    return 0;
}
