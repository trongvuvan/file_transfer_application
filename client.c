#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
//#define SERVER_ADDR "192.168.37.135"
#define SERVER_ADDR "127.0.0.1"

#define BUFF_SIZE 4096

#define SIGN_IN "0"
#define SIGN_UP "1"
#define PROFILE "2"
#define MY_GROUP "3"
#define ACCESS_GROUP "4"
#define CREATE_GROUP "5"
#define LIST_MEMBER "7"
#define SHARE_GROUP "6"
#define ADD_MEMBER "8"
#define DELETE_MEMBER "9"
#define DELETE_GROUP "10"
#define CREATE_DIR "11"
#define REMOVE_DIR "12"
#define UPLOAD_FILE "13"
#define DOWNLOAD_FILE "14"
#define REMOVE_FILE "15"
#define OUT_GROUP "16"
#define ACCESS_SHARE_GROUP "17"
#define LIST_FOLDER "18"
#define LIST_FILE "19"
#define LIST_ALL_USER "20"

#include "menu.h"
#include "file.h"
#include "user.h"
#include "folder.h"
#include "show.h"
#include "group.h"

int main(int argc, const char *argv[])
{
    // create a socket
    int network_socket;
    if( argc < 2)
    {
        printf("Enter port pleaseeeee \n");
    }
    int port = atoi(argv[1]);
    // specify an address for the socket
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    network_socket = socket(AF_INET, SOCK_STREAM, 0);
    int connection_status = connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    // check for connection_status
    if (connection_status == -1)
    {
        printf("The connection has error\n\n");
    }
    if (connection_status == 0)
    {
        // receive data from the server
        char mes_rev_first[100] = {0};
        recv(network_socket, mes_rev_first, 100, 0);
        printf("%s", mes_rev_first);
        memset(mes_rev_first, '\0', (strlen(mes_rev_first) + 1));
        int choice;
        do
        {
            char email[255] = {0};
            char password[255] = {0};
            login_page();
            printf(COLOR_GREEN"Enter your choice : "COLOR_RESET);
            scanf("%d", &choice);
            getchar();
            switch (choice)
            {
            case 1:
            {
                system("clear");
                printf(COLOR_BLUE "\n______________________________________________________________________________\n");
                printf("\t\t\t\t SIGN IN\n\n");
                printf("______________________________________________________________________________\n" COLOR_RESET);
                // int state = 0;
                int read_len;
                char mes_rev[255] = {0};
                char *userID;
                printf(COLOR_GREEN"\nEnter Email\t\t: "COLOR_RESET);
                scanf("%s", email);
                int validate_login(char *email, char *password);
                printf(COLOR_GREEN"\nEnter Password\t\t: "COLOR_RESET);
                scanf("%s", password);
                char message[BUFF_SIZE] = "";
                // char sign_in_code[10] = "0";
                strcat(message, SIGN_IN);
                strcat(message, "*");
                strcat(message, email);
                strcat(message, "*");
                strcat(message, password);
                send(network_socket, message, sizeof(message), 0);
                memset(message, '\0', (strlen(message) + 1));
                read_len = recv(network_socket, mes_rev, sizeof(mes_rev), 0);
                mes_rev[read_len] = '\0';
                if (strcmp(mes_rev, "0") == 0)
                {
                    printf(COLOR_RED "\n\t\t\t\tLogin failed, Try again!\n\n" COLOR_RESET);
                }
                else
                {
                    // printf(COLOR_GREEN"\n___________________________________________________________________________________\n");
                    // printf("\t\t\t\t Login successfully \n\n");
                    // printf("________________________________________________________________________________________\n"COLOR_RESET);
                    mes_rev[read_len] = '\0';
                    userID = strtok(mes_rev, "*");
                    char constUserID[10];
                    strcpy(constUserID, userID);
                    char *userName = strtok(NULL, "*");
                    system("clear");
                    printf(COLOR_GREEN "\n\t\t\t Login successfully : Hello %s\n" COLOR_RESET, userName);
                    int choose;
                    do
                    {

                        home_page();
                        printf(COLOR_GREEN"\nYour choice: "COLOR_RESET);
                        scanf("%d", &choose);
                        getchar();
                        switch (choose)
                        {
                        case 1:
                        {
                            char mes_send_prof[1024] = {0};
                            char mes_recv_prof[1024] = {0};
                            strcpy(mes_send_prof, PROFILE);
                            strcat(mes_send_prof, "*");
                            strcat(mes_send_prof, constUserID);
                            send(network_socket, mes_send_prof, sizeof(mes_send_prof), 0);
                            memset(mes_send_prof, '\0', (strlen(mes_send_prof) + 1));
                            read_len = recv(network_socket, mes_recv_prof, sizeof(mes_recv_prof), 0);
                            // printf("read_len : %d\n", read_len);
                            mes_recv_prof[read_len] = '\0';
                            userID = strtok(mes_recv_prof, "*");

                            char *userName = strtok(NULL, "*");
                            char *email = strtok(NULL, "*");

                            printf(COLOR_BLUE "___________________________MY PROFILRE_________________________\n" COLOR_RESET);
                            printf("\t\tUserID   : \t%s\n", userID);
                            printf("\t\tUserName : \t%s\n", userName);
                            printf("\t\tEmail    : \t%s\n\n", email);
                            printf(COLOR_BLUE "_________________________________________________________________\n" COLOR_RESET);
                            
                            break;
                        }
                        case 2:
                        {
                            char mes_send_showall[1024] = {0};
                            char mes_recv_showall[1024] = {0};
                            strcpy(mes_send_showall, LIST_ALL_USER);
                            strcat(mes_send_showall, "*");
                            strcat(mes_send_showall, constUserID);

                            send(network_socket, mes_send_showall, sizeof(mes_send_showall), 0);
                            memset(mes_send_showall, '\0', (strlen(mes_send_showall) + 1));
                            read_len = recv(network_socket, mes_recv_showall, sizeof(mes_recv_showall), 0);
                            mes_recv_showall[read_len] = '\0';
                            //printf("%s",mes_recv_showall);
                            if (strcmp(mes_recv_showall, "0") == 0)
                            {
                                printf(COLOR_RED "Have no one\n" COLOR_RESET);
                            }
                            else
                            {
                                userID = strtok(mes_recv_showall, "*");
                                char *length1 = strtok(NULL, "*");
                                int size = atoi(length1);
                                char userlist[size][255];
                                char *p;
                                int j = 0;
                                p = strtok(NULL, "*");
                                strcpy(&userlist[j][255], p);
                                while (p != NULL)
                                {
                                    j++;
                                    p = strtok(NULL, "*");
                                    //printf("%s",p);
                                    if (p == NULL)
                                    {
                                        break;
                                    }
                                    strcpy(&(userlist[j][255]), p);
                                }
                                printf(COLOR_BLUE "____________________________LIST USER______________________________\n");
                                printf("%-11s| %-26s | %-21s | %-3s \n", "STT", "UserID", "UserName", "Email");
                                printf("__________________________________________________________________________\n\n" COLOR_RESET);
                                int k = 0;
                                for (int i = 0; i < (atoi(length1) * 3); i += 3)
                                {
                                    printf("%-11d| %-26s | %-21s | %-3s \n", k + 1, &userlist[i][255], &userlist[i + 1][255], &userlist[i + 2][255]);
                                    k++;
                                }
                                printf(COLOR_BLUE"__________________________________________________________________________\n\n" COLOR_RESET);
                                break;
                            }
                            break;
                        }
                        case 3:
                        {

                            char mes_send_myGroup[1024] = {0};
                            char mes_recv_myGroup[1024] = {0};
                            strcpy(mes_send_myGroup, MY_GROUP);
                            strcat(mes_send_myGroup, "*");
                            strcat(mes_send_myGroup, constUserID);

                            send(network_socket, mes_send_myGroup, sizeof(mes_send_myGroup), 0);
                            memset(mes_send_myGroup, '\0', (strlen(mes_send_myGroup) + 1));
                            read_len = recv(network_socket, mes_recv_myGroup, sizeof(mes_recv_myGroup), 0);
                            mes_recv_myGroup[read_len] = '\0';
                            if (strcmp(mes_recv_myGroup, "0") == 0)
                            {
                                printf(COLOR_RED "You have not any group\n" COLOR_RESET);
                            }
                            else
                            {
                                userID = strtok(mes_recv_myGroup, "*");
                                char *length1 = strtok(NULL, "*");
                                int size = atoi(length1);
                                char listGroup[size][255];
                                char *p;
                                int j = 0;
                                p = strtok(NULL, "*");
                                strcpy(&listGroup[j][255], p);
                                while (p != NULL)
                                {
                                    j++;
                                    p = strtok(NULL, "*");
                                    if (p == NULL)
                                    {
                                        break;
                                    }
                                    strcpy(&(listGroup[j][255]), p);
                                }

                                printf(COLOR_BLUE "____________________________MY GROUP______________________________\n");
                                // printf("\nSTT\t| Group Name\t\t\t| Description\t\t| Group ID\n");
                                printf("%-11s| %-26s | %-21s | %-3s \n", "STT", "GroupName", "Description", "GroupID");
                                printf("____________________________________________________________________\n\n" COLOR_RESET);
                                int k = 0;
                                for (int i = 0; i < (atoi(length1) * 3); i += 3)
                                {
                                    printf("%-11d| %-26s | %-21s | %-3s \n", k + 1, &listGroup[i][255], &listGroup[i + 1][255], &listGroup[i + 2][255]);
                                    k++;
                                }
                                printf(COLOR_BLUE"____________________________________________________________________\n\n" COLOR_RESET);
                                char input[2];
                                printf("Do you want to access the group, enter y(or Y)/ NO(enter any key) :");
                                scanf("%[^\n]", input);
                                getchar();
                                if (strcmp(input, "y") == 0 || strcmp(input, "Y") == 0)
                                {
                                    char groupID[5];
                                    printf("Enter ID group : ");
                                    scanf("%[^\n]", groupID);
                                    getchar();
                                    char mes_send_group_id[BUFF_SIZE];
                                    char mes_recv_group_id[BUFF_SIZE];
                                    strcpy(mes_send_group_id, ACCESS_GROUP);
                                    strcat(mes_send_group_id, "*");
                                    strcat(mes_send_group_id, groupID);
                                    strcat(mes_send_group_id, "*");
                                    strcat(mes_send_group_id, constUserID);
                                    send(network_socket, mes_send_group_id, sizeof(mes_send_group_id), 0);
                                    memset(mes_send_group_id, '\0', (strlen(mes_send_group_id) + 1));
                                    read_len = recv(network_socket, mes_recv_group_id, sizeof(mes_recv_group_id), 0);
                                    mes_recv_myGroup[read_len] = '\0';
                                    int check = atoi(mes_recv_group_id);
                                    // printf("check result : %d\n", check);
                                    if (check == 0)
                                    {
                                        printf(COLOR_RED "Failed, ID group does not exist!, Try again.\n" COLOR_RESET);
                                    }
                                    else
                                    {
                                        // do nothing
                                        int choose1;
                                        do
                                        {

                                            admin_page();

                                            printf(COLOR_GREEN"Your choose : "COLOR_RESET);
                                            scanf("%d", &choose1);
                                            getchar();
                                            switch (choose1)
                                            {
                                            case 1:
                                            {

                                                char mes_send_list_member[BUFF_SIZE] = {0};
                                                char mes_recv_list_member[BUFF_SIZE] = {0};
                                                strcpy(mes_send_list_member, LIST_MEMBER);
                                                strcat(mes_send_list_member, "*");
                                                strcat(mes_send_list_member, groupID);
                                                send(network_socket, mes_send_list_member, sizeof(mes_send_list_member), 0);
                                                memset(mes_send_group_id, '\0', (strlen(mes_send_group_id) + 1));
                                                read_len = recv(network_socket, mes_recv_list_member, sizeof(mes_recv_list_member), 0);
                                                mes_recv_list_member[read_len] = '\0';
                                                // printf("chekc tring : %s\n", mes_recv_list_member);
                                                if (strcmp(mes_recv_list_member, "0") == 0)
                                                {
                                                    printf(COLOR_RED "Doesnt have member\n" COLOR_RESET);
                                                }
                                                else
                                                {
                                                    // do nothing
                                                    strtok(mes_recv_list_member, "*");
                                                    char *length1 = strtok(NULL, "*");
                                                    int length = atoi(length1);
                                                    char listMember[length][255];
                                                    char *p;
                                                    int j = 0;
                                                    p = strtok(NULL, "*");
                                                    strcpy(&listMember[j][255], p);
                                                    // printf("list[0] : %s\n", &listMember[j][255]);
                                                    while (p != NULL)
                                                    {
                                                        j++;
                                                        p = strtok(NULL, "*");
                                                        //  printf(" p : %s\n", p);
                                                        if (p == NULL)
                                                        {
                                                            break;
                                                        }
                                                        else
                                                        {
                                                            strcpy(&(listMember[j][255]), p);
                                                        }
                                                    }
                                                    printf(COLOR_BLUE "________________________________LIST MEMBER________________________________\n" COLOR_RESET);
                                                    // printf("\nSTT\t| User ID\t\t| UserName\t\t| Role\n");
                                                    printf("%-11s| %-15s| %-18s| %-3s \n", "STT", "UserID", "UserName", "Role");
                                                    printf(COLOR_BLUE "___________________________________________________________________________\n" COLOR_RESET);

                                                    int k = 0;

                                                    for (int i = 0; i < (length * 2); i += 2)
                                                    {
                                                        if (strcmp(constUserID, &(listMember[i][255])) == 0)
                                                        {
                                                            printf("%-11d| %-15s| %-18s| %-3s \n", k + 1, &listMember[i][255], &listMember[i + 1][255], "Admin");
                                                            k++;
                                                        }
                                                        else
                                                        {
                                                            printf("%-11d| %-15s| %-18s| %-3s \n", k + 1, &listMember[i][255], &listMember[i + 1][255], "Member");
                                                            k++;
                                                        }
                                                    }
                                                    printf(COLOR_BLUE "___________________________________________________________________________\n" COLOR_RESET);
                                                }
                                                break;
                                            }

                                            case 2:
                                            {

                                                char mes_send_list_folder[BUFF_SIZE] = {0};
                                                char mes_recv_list_folder[BUFF_SIZE] = {0};
                                                strcpy(mes_send_list_folder, LIST_FOLDER);
                                                strcat(mes_send_list_folder, "*");
                                                strcat(mes_send_list_folder, groupID);
                                                send(network_socket, mes_send_list_folder, sizeof(mes_send_list_folder), 0);
                                                memset(mes_send_list_folder, '\0', (strlen(mes_send_list_folder) + 1));
                                                read_len = recv(network_socket, mes_recv_list_folder, sizeof(mes_recv_list_folder), 0);
                                                mes_recv_list_folder[read_len] = '\0';
                                                // printf("check string : %s\n", mes_recv_list_folder);
                                                if (strcmp(mes_recv_list_folder, "0") == 0)
                                                {
                                                    printf(COLOR_RED "There are no folder in this group\n" COLOR_RESET);
                                                    break;
                                                }
                                                else
                                                {
                                                    // do nothing
                                                    strtok(mes_recv_list_folder, "*");
                                                    char *length1 = strtok(NULL, "*");
                                                    int length = atoi(length1);
                                                    char listFoler[length][255];
                                                    char *p;
                                                    int j = 0;
                                                    p = strtok(NULL, "*");
                                                    strcpy(&listFoler[j][255], p);
                                                    // printf("list[0] : %s\n", &listFoler[j][255]);
                                                    while (p != NULL)
                                                    {
                                                        j++;
                                                        p = strtok(NULL, "*");
                                                        //  printf(" p : %s\n", p);
                                                        if (p == NULL)
                                                        {
                                                            break;
                                                        }
                                                        else
                                                        {
                                                            strcpy(&(listFoler[j][255]), p);
                                                        }
                                                    }
                                                    printf(COLOR_BLUE "_______________________________________LIST FOLDER__________________________________\n");
                                                    // printf("\nSTT\t| Folder Name\t\t |  Owner\n");
                                                    printf("%-11s| %-28s|  %-10s \n", "STT", "FolderName", "Owner");
                                                    printf("___________________________________________________________________________________________\n\n" COLOR_RESET);

                                                    int k = 0;
                                                    for (int i = 0; i < (length * 2); i += 2)
                                                    {

                                                        printf("%-11d| %-28s|  %-10s \n", k + 1, &listFoler[i][255], &listFoler[i + 1][255]);
                                                        k++;
                                                    }
                                                    printf(COLOR_BLUE "___________________________________________________________________________________________\n\n" COLOR_RESET);
                                                    break;
                                                }
                                                break;
                                            }
                                            case 3:
                                            {

                                                char mes_send_list_file[BUFF_SIZE] = {0};
                                                char mes_recv_list_file[BUFF_SIZE] = {0};
                                                strcpy(mes_send_list_file, LIST_FILE);
                                                strcat(mes_send_list_file, "*");
                                                strcat(mes_send_list_file, groupID);
                                                send(network_socket, mes_send_list_file, sizeof(mes_send_list_file), 0);
                                                memset(mes_send_list_file, '\0', (strlen(mes_send_list_file) + 1));
                                                read_len = recv(network_socket, mes_recv_list_file, sizeof(mes_recv_list_file), 0);
                                                mes_recv_list_file[read_len] = '\0';
                                                // printf("check string : %s\n", mes_recv_list_file);
                                                if (strcmp(mes_recv_list_file, "0") == 0)
                                                {
                                                    printf(COLOR_RED "There are no files in this folder\n" COLOR_RESET);
                                                    break;
                                                }
                                                else
                                                {
                                                    // do nothing
                                                    strtok(mes_recv_list_file, "*");
                                                    char *length1 = strtok(NULL, "*");
                                                    int length = atoi(length1);
                                                    // printf("check length : %d\n", length);
                                                    if (atoi(length1) > 0)
                                                    {
                                                        char listFile[length * 2][255];
                                                        char *p;
                                                        int j = 0;
                                                        p = strtok(NULL, "*");
                                                        strcpy(&listFile[j][255], p);
                                                        // printf("list[0] : %s\n", &listFile[j][255]);
                                                        while (p != NULL)
                                                        {
                                                            j++;
                                                            p = strtok(NULL, "*");
                                                            // printf(" p[%d] : %s\n", j, p);
                                                            if (p == NULL)
                                                            {
                                                                break;
                                                            }
                                                            else
                                                            {
                                                                strcpy(&(listFile[j][255]), p);
                                                                // printf("list[%d] : %s\n", j, &listFile[j][255]);
                                                            }

                                                            // printf(" listFile[%d] : %s\n",j, &listFile[j][255]);
                                                        }
                                                        for (int i = 0; i < 6; i++)
                                                        {
                                                            // printf(" listFile[%d] : %s\n", i, &listFile[i][255]);
                                                        }

                                                        printf(COLOR_BLUE "________________________________________LIST FILE__________________________________________\n");
                                                        // printf("\nSTT\t| File Name\t\t\t |  Owner\n");
                                                        printf("%-11s| %-28s|  %-10s \n", "STT", "FileName", "Owner");
                                                        printf("___________________________________________________________________________________________\n" COLOR_RESET);

                                                        int k = 0;
                                                        for (int i = 0; i < (atoi(length1) * 2); i += 2)
                                                        {

                                                            printf("%-11d| %-28s|  %-10s \n", k + 1, &listFile[i][255], &listFile[i + 1][255]);
                                                            k++;
                                                        }
                                                        printf(COLOR_BLUE "___________________________________________________________________________________________\n" COLOR_RESET);
                                                    }
                                                    else
                                                    {
                                                        printf(COLOR_RED "There are no files in this folder\n" COLOR_RESET);
                                                        break;
                                                    }
                                                }
                                                break;
                                            }
                                            case 4: // UPLOAD
                                            {

                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", UPLOAD_FILE);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                upload_file(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 5: // download
                                            {

                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", DOWNLOAD_FILE);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                download_file(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 6:
                                            {

                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", REMOVE_FILE);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                remove_file(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 7: // makedir
                                            {
                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", CREATE_DIR);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                makedir(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 8: // removedir
                                            {

                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", REMOVE_DIR);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                removedir(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 9:
                                            {

                                                // printf("check groupID : %s\n", groupID);
                                                char mes_send_add_member[255] = {0};
                                                char mes_recv_add_member[255] = {0};
                                                char email[255];
                                                printf("Enter email of member to add : ");
                                                scanf("%[^\n]", email);
                                                getchar();
                                                strcpy(mes_send_add_member, ADD_MEMBER);
                                                strcat(mes_send_add_member, "*");
                                                strcat(mes_send_add_member, email);
                                                strcat(mes_send_add_member, "*");
                                                strcat(mes_send_add_member, groupID);
                                                send(network_socket, mes_send_add_member, sizeof(mes_send_add_member), 0);
                                                memset(mes_send_add_member, '\0', (strlen(mes_send_add_member) + 1));
                                                read_len = recv(network_socket, mes_recv_add_member, sizeof(mes_recv_add_member), 0);
                                                mes_recv_add_member[read_len] = '\0';
                                                int check = atoi(mes_recv_add_member);
                                                //  printf("check result : %d\n", check);
                                                if (check == 0)
                                                {
                                                    printf(COLOR_RED " Add member %s failed, Try again.\n" COLOR_RESET, email);
                                                }
                                                else if (check == 2)
                                                {
                                                    printf(COLOR_RED " Add member %s failed, have already exist !!.\n" COLOR_RESET, email);
                                                }
                                                else
                                                {
                                                    printf(COLOR_GREEN " Add member %s successfull !!.\n" COLOR_RESET, email);
                                                }
                                                break;
                                            }
                                            case 10:
                                            {

                                                char mes_send_del_member[255] = {0};
                                                char mes_recv_del_member[255] = {0};
                                                char email[255];
                                                printf("Enter email of member to delete : ");
                                                scanf("%[^\n]", email);
                                                getchar();
                                                strcpy(mes_send_del_member, DELETE_MEMBER);
                                                strcat(mes_send_del_member, "*");
                                                strcat(mes_send_del_member, email);
                                                strcat(mes_send_del_member, "*");
                                                strcat(mes_send_del_member, groupID);
                                                send(network_socket, mes_send_del_member, sizeof(mes_send_del_member), 0);
                                                memset(mes_send_del_member, '\0', (strlen(mes_send_del_member) + 1));
                                                read_len = recv(network_socket, mes_recv_del_member, sizeof(mes_recv_del_member), 0);
                                                mes_recv_del_member[read_len] = '\0';
                                                int check = atoi(mes_recv_del_member);
                                                // printf("check result : %d\n", check);
                                                if (check == 0)
                                                {
                                                    printf(COLOR_RED " Delete member %s failed, Try again.\n" COLOR_RESET, email);
                                                }
                                                else if (check == 2)
                                                {
                                                    printf(COLOR_RED " Delete member failed,  %s does not exist on group !!.\n" COLOR_RESET, email);
                                                }
                                                else
                                                {
                                                    printf(COLOR_GREEN " Delete member %s successfull !!.\n" COLOR_RESET, email);
                                                }
                                                break;
                                            }
                                            case 11:
                                            {

                                                char mes_send_del_group[255] = {0};
                                                char mes_recv_del_group[255] = {0};
                                                char sureDel[255];
                                                printf("Are you sure you want to delete this group?");
                                                printf("Enter y( or Y)/ NO (any key) : ");
                                                scanf("%[^\n]", sureDel);
                                                getchar();
                                                if (strcmp(sureDel, "y") == 0 || strcmp(sureDel, "Y") == 0)
                                                {

                                                    strcpy(mes_send_del_group, DELETE_GROUP);
                                                    strcat(mes_send_del_group, "*");
                                                    strcat(mes_send_del_group, groupID);
                                                    send(network_socket, mes_send_del_group, sizeof(mes_send_del_group), 0);
                                                    memset(mes_send_del_group, '\0', (strlen(mes_send_del_group) + 1));
                                                    read_len = recv(network_socket, mes_recv_del_group, sizeof(mes_recv_del_group), 0);
                                                    mes_recv_del_group[read_len] = '\0';
                                                    int check = atoi(mes_recv_del_group);
                                                    printf("check result : %d\n", check);
                                                    if (check == 0)
                                                    {
                                                        printf(COLOR_RED " Delete group failed, Try again.\n" COLOR_RESET);
                                                    }
                                                    else
                                                    {
                                                        printf(COLOR_GREEN " Delete group successfull !!.\n" COLOR_RESET);
                                                        choose1 = 12;
                                                        break;
                                                    }
                                                }
                                                else
                                                {
                                                    break;
                                                }
                                                break;
                                            }
                                            default:
                                            {
                                                break;
                                            }
                                            }

                                        } while (choose1 != 12);
                                        break;
                                    }
                                }
                            }
                            break;
                        }
                        case 4:
                        {

                            char mes_send_myShareGroup[1024] = {0};
                            char mes_recv_myShareGroup[1024] = {0};
                            strcpy(mes_send_myShareGroup, SHARE_GROUP);
                            strcat(mes_send_myShareGroup, "*");
                            strcat(mes_send_myShareGroup, constUserID);

                            send(network_socket, mes_send_myShareGroup, sizeof(mes_send_myShareGroup), 0);
                            memset(mes_send_myShareGroup, '\0', (strlen(mes_send_myShareGroup) + 1));

                            read_len = recv(network_socket, mes_recv_myShareGroup, sizeof(mes_recv_myShareGroup), 0);
                            //  printf("read_len : %d\n", read_len);
                            mes_recv_myShareGroup[read_len] = '\0';

                            if (strcmp(mes_recv_myShareGroup, "0") == 0)
                            {
                                printf(COLOR_RED "No group found \n" COLOR_RESET);
                            }
                            else
                            {

                                // do nothing
                                userID = strtok(mes_recv_myShareGroup, "*");
                                char *length1 = strtok(NULL, "*");
                                int length = atoi(length1);
                                char listGroup[length][255];
                                char *p;
                                int j = 0;
                                p = strtok(NULL, "*");
                                strcpy(&listGroup[j][255], p);
                                while (p != NULL)
                                {
                                    j++;
                                    p = strtok(NULL, "*");
                                    if (p == NULL)
                                    {
                                        break;
                                    }
                                    strcpy(&(listGroup[j][255]), p);
                                }
                                printf(COLOR_BLUE "____________________________MY SHARE GROUP____________________________\n" COLOR_RESET);
                                // printf("STT\t|Group ID\t\t|\tGroupName\t\n");
                                printf("%-11s|  %-13s |\t%-22s\n", "STT", "GroupID", "GroupName");
                                printf(COLOR_BLUE "__________________________________________________________________\n" COLOR_RESET);
                                int k = 0;
                                for (int i = 0; i < (length * 2); i += 2)
                                {

                                    printf("%-11d|  %-13s |\t%-22s\n", k + 1, &listGroup[i][255], &listGroup[i + 1][255]);
                                    k++;
                                }
                                printf(COLOR_BLUE "__________________________________________________________________\n" COLOR_RESET);
                                char input[2];
                                printf("Do you want to access the share group, enter y (or Y)/ NO(enter any key):");
                                scanf("%[^\n]", input);
                                getchar();
                                if (strcmp(input, "y") == 0 || strcmp(input, "Y") == 0)
                                {
                                    char groupID[5];
                                    printf("Enter ID group: ");
                                    scanf("%[^\n]", groupID);
                                    getchar();
                                    char mes_send_group_id[255];
                                    char mes_recv_group_id[255];
                                    strcpy(mes_send_group_id, ACCESS_SHARE_GROUP);
                                    strcat(mes_send_group_id, "*");
                                    strcat(mes_send_group_id, groupID);
                                    strcat(mes_send_group_id, "*");
                                    strcat(mes_send_group_id, constUserID);
                                    // printf("check mes send : %s\n", mes_send_group_id);
                                    send(network_socket, mes_send_group_id, sizeof(mes_send_group_id), 0);
                                    memset(mes_send_group_id, '\0', (strlen(mes_send_group_id) + 1));
                                    read_len = recv(network_socket, mes_recv_group_id, sizeof(mes_recv_group_id), 0);
                                    mes_recv_myShareGroup[read_len] = '\0';
                                    int check = atoi(mes_recv_group_id);
                                    // printf("check result : %d\n", check);
                                    if (check == 0)
                                    {
                                        printf(COLOR_RED "Failed, ID group does not exist!, Try again.\n" COLOR_RESET);
                                    }
                                    else
                                    {
                                        int choose2;
                                        do
                                        {

                                            share_group_page();

                                            printf(COLOR_GREEN"Your choose : "COLOR_RESET);
                                            scanf("%d", &choose2);
                                            getchar();
                                            switch (choose2)
                                            {
                                            case 1:
                                            {
                                                // printf("userID : %s\n", constUserID);
                                                char mes_send_list_member[255];
                                                char mes_recv_list_member[255];
                                                strcpy(mes_send_list_member, LIST_MEMBER);
                                                strcat(mes_send_list_member, "*");
                                                strcat(mes_send_list_member, groupID);
                                                send(network_socket, mes_send_list_member, sizeof(mes_send_list_member), 0);
                                                memset(mes_send_group_id, '\0', (strlen(mes_send_group_id) + 1));
                                                read_len = recv(network_socket, mes_recv_list_member, sizeof(mes_recv_list_member), 0);
                                                mes_recv_list_member[read_len] = '\0';
                                                // printf("test chuoi : %s\n", mes_recv_list_member);
                                                if (strcmp(mes_send_list_member, "0") == 0)
                                                {
                                                    printf(COLOR_RED "Have no member\n" COLOR_RESET);
                                                }
                                                else
                                                {
                                                    // do nothing
                                                    strtok(mes_recv_list_member, "*");
                                                    char *length1 = strtok(NULL, "*");
                                                    int length = atoi(length1);
                                                    char listMember[length][255];
                                                    char *p;
                                                    int j = 0;
                                                    p = strtok(NULL, "*");
                                                    strcpy(&listMember[j][255], p);
                                                    //  printf("list[0] : %s\n", &listMember[j][255]);
                                                    while (p != NULL)
                                                    {
                                                        j++;
                                                        p = strtok(NULL, "*");
                                                        // printf(" p : %s\n", p);
                                                        if (p == NULL)
                                                        {
                                                            break;
                                                        }
                                                        strcpy(&(listMember[j][255]), p);
                                                    }
                                                    printf(COLOR_BLUE "_____________________________LIST MEMBER_________________________\n" COLOR_RESET);
                                                    // printf("STT\t| User ID\t\t| UserName\t\t| Role\n");
                                                    printf("%-11s| %-15s| %-18s| %-3s \n", "STT", "UserID", "UserName", "Role");
                                                    printf(COLOR_BLUE "__________________________________________________________________\n" COLOR_RESET);
                                                    int k = 0;

                                                    for (int i = 0; i < (length * 2); i += 2)
                                                    {
                                                        if (i == 0)
                                                        {
                                                            printf("%-11d| %-15s| %-18s| %-3s \n", k + 1, &listMember[i][255], &listMember[i + 1][255], "Admin");
                                                            k++;
                                                        }
                                                        else
                                                        {
                                                            printf("%-11d| %-15s| %-18s| %-3s \n", k + 1, &listMember[i][255], &listMember[i + 1][255], "Member");
                                                            k++;
                                                        }
                                                    }
                                                    printf(COLOR_BLUE "__________________________________________________________________\n" COLOR_RESET);
                                                }
                                                break;
                                            }
                                            case 2:
                                            {

                                                char mes_send_list_folder[1024] = {0};
                                                char mes_recv_list_folder[1024] = {0};
                                                strcpy(mes_send_list_folder, LIST_FOLDER);
                                                strcat(mes_send_list_folder, "*");
                                                strcat(mes_send_list_folder, groupID);
                                                send(network_socket, mes_send_list_folder, sizeof(mes_send_list_folder), 0);
                                                memset(mes_send_list_folder, '\0', (strlen(mes_send_list_folder) + 1));
                                                read_len = recv(network_socket, mes_recv_list_folder, sizeof(mes_recv_list_folder), 0);
                                                mes_recv_list_folder[read_len] = '\0';
                                                // printf("check string : %s\n", mes_recv_list_folder);
                                                if (strcmp(mes_recv_list_folder, "0") == 0)
                                                {
                                                    printf(COLOR_RED "There are no folder in this group\n" COLOR_RESET);
                                                    break;
                                                }
                                                else
                                                {
                                                    // do nothing
                                                    strtok(mes_recv_list_folder, "*");
                                                    char *length1 = strtok(NULL, "*");
                                                    int length = atoi(length1);
                                                    char listFoler[length][255];
                                                    char *p;
                                                    int j = 0;
                                                    p = strtok(NULL, "*");
                                                    strcpy(&listFoler[j][255], p);
                                                    // printf("list[0] : %s\n", &listFoler[j][255]);
                                                    while (p != NULL)
                                                    {
                                                        j++;
                                                        p = strtok(NULL, "*");
                                                        //  printf(" p : %s\n", p);
                                                        if (p == NULL)
                                                        {
                                                            break;
                                                        }
                                                        else
                                                        {
                                                            strcpy(&(listFoler[j][255]), p);
                                                        }
                                                    }
                                                    printf(COLOR_BLUE "________________________LIST FOLDER_________________________\n" COLOR_RESET);
                                                    // printf("\nSTT\t| Folder Name\t\t |  Own\n");
                                                    printf("%-11s| %-28s|  %-10s \n", "STT", "FolderName", "Owner");
                                                    printf(COLOR_BLUE "___________________________________________________________________________\n" COLOR_RESET);
                                                    int k = 0;
                                                    for (int i = 0; i < (length * 2); i += 2)
                                                    {

                                                        printf("%-11d| %-28s|  %-10s \n", k + 1, &listFoler[i][255], &listFoler[i + 1][255]);
                                                        k++;
                                                    }
                                                    printf(COLOR_BLUE "___________________________________________________________________________\n" COLOR_RESET);
                                                }
                                                break;
                                            }
                                            case 3:
                                            {

                                                char mes_send_list_file[1024] = {0};
                                                char mes_recv_list_file[1024] = {0};
                                                strcpy(mes_send_list_file, LIST_FILE);
                                                strcat(mes_send_list_file, "*");
                                                strcat(mes_send_list_file, groupID);
                                                send(network_socket, mes_send_list_file, sizeof(mes_send_list_file), 0);
                                                memset(mes_send_list_file, '\0', (strlen(mes_send_list_file) + 1));
                                                read_len = recv(network_socket, mes_recv_list_file, sizeof(mes_recv_list_file), 0);
                                                mes_recv_list_file[read_len] = '\0';
                                                // printf("check string : %s\n", mes_recv_list_file);
                                                if (strcmp(mes_recv_list_file, "0") == 0)
                                                {
                                                    printf(COLOR_RED "There are no files in this folder\n" COLOR_RESET);
                                                    break;
                                                }
                                                else
                                                {
                                                    // do nothing
                                                    strtok(mes_recv_list_file, "*");
                                                    char *length1 = strtok(NULL, "*");
                                                    int length = atoi(length1);
                                                    // printf("check length : %d\n", length);
                                                    if (atoi(length1) > 0)
                                                    {
                                                        char listFile[length * 2][255];
                                                        char *p;
                                                        int j = 0;
                                                        p = strtok(NULL, "*");
                                                        strcpy(&listFile[j][255], p);
                                                        // printf("list[0] : %s\n", &listFile[j][255]);
                                                        while (p != NULL)
                                                        {
                                                            j++;
                                                            p = strtok(NULL, "*");
                                                            // printf(" p[%d] : %s\n", j, p);
                                                            if (p == NULL)
                                                            {
                                                                break;
                                                            }
                                                            else
                                                            {
                                                                strcpy(&(listFile[j][255]), p);
                                                                // printf("list[%d] : %s\n", j, &listFile[j][255]);
                                                            }

                                                            // printf(" listFile[%d] : %s\n",j, &listFile[j][255]);
                                                        }
                                                        for (int i = 0; i < 6; i++)
                                                        {
                                                            // printf(" listFile[%d] : %s\n", i, &listFile[i][255]);
                                                        }
                                                        printf(COLOR_BLUE "_________________________________LIST FILE__________________________________\n");
                                                        printf("\nSTT\t| File Name\t\t\t |  Own\n");
                                                        printf("___________________________________________________________________________\n" COLOR_RESET);

                                                        int k = 0;
                                                        for (int i = 0; i < (atoi(length1) * 2); i += 2)
                                                        {

                                                            printf("%-11d| %-28s|  %-10s \n", k + 1, &listFile[i][255], &listFile[i + 1][255]);
                                                            k++;
                                                        }
                                                        printf(COLOR_BLUE "___________________________________________________________________________\n" COLOR_RESET);
                                                    }
                                                    else
                                                    {
                                                        printf(COLOR_RED "There are no files in this folder\n" COLOR_RESET);
                                                        break;
                                                    }
                                                }
                                                break;
                                            }
                                            case 4:
                                            {

                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", UPLOAD_FILE);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                upload_file(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 5:
                                            {

                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", DOWNLOAD_FILE);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                download_file(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 6:
                                            {

                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", REMOVE_FILE);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                remove_file(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 7:
                                            {

                                                char mes_request[255] = {0};
                                                sprintf(mes_request, "%s*", CREATE_DIR);
                                                send(network_socket, mes_request, strlen(mes_request), 0);
                                                makedir(constUserID, network_socket);
                                                bzero(mes_request, strlen(mes_request));
                                                break;
                                            }
                                            case 8:
                                            {

                                                char mes_send_out_group[255] = {0};
                                                char mes_recv_out_group[255] = {0};
                                                char sureOut[255];
                                                printf("Do you want to leave this group?");
                                                printf("Enter y( or Y)/ NO (any key) : ");
                                                scanf("%[^\n]", sureOut);
                                                getchar();
                                                if (strcmp(sureOut, "y") == 0 || strcmp(sureOut, "Y") == 0)
                                                {
                                                    strcpy(mes_send_out_group, OUT_GROUP);
                                                    strcat(mes_send_out_group, "*");
                                                    strcat(mes_send_out_group, constUserID);
                                                    strcat(mes_send_out_group, "*");
                                                    strcat(mes_send_out_group, groupID);
                                                    send(network_socket, mes_send_out_group, sizeof(mes_send_out_group), 0);
                                                    memset(mes_send_out_group, '\0', (strlen(mes_send_out_group) + 1));
                                                    read_len = recv(network_socket, mes_recv_out_group, sizeof(mes_recv_out_group), 0);
                                                    mes_recv_out_group[read_len] = '\0';
                                                    int check = atoi(mes_recv_out_group);
                                                    printf("check result : %d\n", check);
                                                    if (check == 0)
                                                    {
                                                        printf(" Out group failed, Try again.\n");
                                                    }
                                                    else
                                                    {
                                                        printf(" Leave group successfull !!.\n");
                                                        choose2 = 9;
                                                        break;
                                                    }
                                                }
                                                else
                                                {
                                                    break;
                                                }
                                                break;
                                            }
                                            default:
                                                break;
                                            }
                                        } while (choose2 != 9);
                                    }
                                }
                            }
                            break;
                        }

                        case 5:
                        {
                            system("clear");
                            char mes_send_create_group[255] = {0};
                            char mes_recv_create_group[255] = {0};
                            char groupName[255] = {0};
                            char group_descrp[255] = {0};
                            strcpy(mes_send_create_group, CREATE_GROUP);
                            strcat(mes_send_create_group, "*");
                            // printf("user id test  : %s\n", userID);
                            strcat(mes_send_create_group, constUserID);
                            strcat(mes_send_create_group, "*");

                            printf("Enter Group Name : ");
                            scanf("%[^\n]%*c", groupName);
                            // ltrim(groupName);
                            // rtrim(groupName);
                            printf("Enter Group Description : ");
                            scanf("%[^\n]%*c", group_descrp);
                            // ltrim(group_descrp);
                            // rtrim(group_descrp);
                            strcat(mes_send_create_group, groupName);
                            strcat(mes_send_create_group, "*");
                            strcat(mes_send_create_group, group_descrp);

                            send(network_socket, mes_send_create_group, sizeof(mes_send_create_group), 0);
                            memset(mes_send_create_group, '\0', (strlen(mes_send_create_group) + 1));
                            read_len = recv(network_socket, mes_recv_create_group, sizeof(mes_recv_create_group), 0);
                            // printf("read_len : %d\n", read_len);
                            mes_recv_create_group[read_len] = '\0';
                            // printf(" test chuoi nhan : %s\n", mes_recv_create_group);
                            int status = atoi(mes_recv_create_group);
                            // printf("status: %d",status);
                            if (status == 1)
                            {
                                printf(COLOR_GREEN "Create group \"%s\" successfull !\n" COLOR_RESET, groupName);
                            }
                            else if (status == 2)
                            {
                                printf(COLOR_RED "Group name existed, please try again !\n" COLOR_RESET);
                            }
                            else
                            {
                                printf(COLOR_RED "Group creation failed, try again !\n" COLOR_RESET);
                            }
                            break;
                        }
                        case 6:
                        {
                            break;
                        }
                        default:
                            printf("Function does not exist, please try again \n");
                            break;
                        }
                        }
                        while (choose != 6)
                            ;
                    }
                    break;
                }
            case 2:
            {
                system("clear");
                char username[255];
                printf("___________________________________________________________________________\n");
                printf("\t\t\t SIGN UP \n");
                printf("___________________________________________________________________________\n");
                printf("\t\t EMAIL: ");
                scanf("%[^\n]", email);
                getchar();
                printf("\t\t USERNAME: ");
                scanf("%[^\n]", username);
                getchar();
                printf("\t\t PASSWORD: ");
                scanf("%[^\n]", password);
                getchar();
                int state_validate;
                int state = 0;
                state_validate = validate_login(email, password);
                if (state_validate == 1)
                {
                    char message[BUFF_SIZE] = "";
                    char sign_up_code[10] = "1";
                    strcat(message, sign_up_code);
                    strcat(message, "*");
                    strcat(message, username);
                    strcat(message, "*");
                    strcat(message, email);
                    strcat(message, "*");
                    strcat(message, password);
                    send(network_socket, message, sizeof(message), 0);
                    memset(message, '\0', (strlen(message) + 1));
                    char mes_rev[255];
                    int read_len = recv(network_socket, mes_rev, sizeof(mes_rev), 0);
                    mes_rev[read_len] = '\0';
                    state = atoi(mes_rev);
                    memset(mes_rev, '\0', strlen(mes_rev) + 1);
                    // printf("%d\n", state);
                    if (state == 1)
                    {
                        printf(COLOR_GREEN "Register successfully ! Please login to user\n" COLOR_RESET);
                        break;
                    }
                    else if (state == 2)
                    {
                        printf(COLOR_RED "Failed, Email have already exist.\n" COLOR_RESET);
                        break;
                    }
                    else
                    {
                        // printf("%d\n", state);
                        printf(COLOR_RED "Register failed, Try again!\n" COLOR_RESET);
                        break;
                    }
                }
                else
                {
                    printf(COLOR_RED "Email or password not validated ! Please resgister " COLOR_RESET);
                    break;
                }

                // do everthing
                break;
            }
            case 3:
            {
                system("clear");
                printf("\n\n______________________Chuc ban A+______________________\n\n");
                exit(-1);
                break;
            }
            default:
            {
                printf("Function does not exist. Please try again!\n");
                break;
            }
            }
            }
            while (choice != 3)
                ;
        }
        close(network_socket);

        printf("\n________________See you again________________\n");
        return 0;
    }
