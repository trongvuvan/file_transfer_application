#include "group.h"

char *my_group(char *userID, char *temp)
{
    MYSQL *sql = mysql_init(NULL);
    strcpy(temp, "0");
    if (sql == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return temp;
    }

    if (mysql_real_connect(sql, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return temp;
    }
    char *query = (char *)malloc(255 * sizeof(char));

    sprintf(query, "SELECT GroupName, GroupDescription, GroupID FROM network_group where UserID = \"%s\" ", userID);
    if (mysql_query(sql, query) == 1)
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return temp;
    }
    else
    {
        MYSQL_RES *result;
        result = mysql_store_result(sql);
        if (result->row_count == 0)
        {
            // fprintf(stderr, "%s\n", mysql_error(sql));
            mysql_close(sql);
            return temp;
        }
        else
        {
            MYSQL_ROW row;
            long k = result->row_count;
            printf("row count = %ld", k);
            if (k == 0)
            {
                mysql_free_result(result);
                mysql_close(sql);
                return temp;
            }
            else
            {
                printf("-------- Tra ve my group----------\n");
                strcpy(temp, userID);
                char s[10];
                strcat(temp, "*");
                sprintf(s, "%ld", k);
                strcat(temp, s);
                while ((row = mysql_fetch_row(result)) != NULL)
                {
                    int i = 0;
                    strcat(temp, "*");
                    strcat(temp, row[i]);
                    strcat(temp, "*");
                    strcat(temp, row[i + 1]);
                    strcat(temp, "*");
                    strcat(temp, row[i + 2]);
                    // printf("temp : %s\n", temp);
                }
            }
        }
        mysql_free_result(result);
    }

    mysql_close(sql);
    return temp;
}

char *shareGroup(char *userID, char *temp)
{
    MYSQL *sql = mysql_init(NULL);
    strcpy(temp, "0");
    if (sql == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return temp;
    }

    if (mysql_real_connect(sql, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return temp;
    }
    char *query = (char *)malloc(200 * sizeof(char));
    char *query_check = (char *)malloc(200 * sizeof(char));
    sprintf(query, "SELECT GroupID FROM share_group where UserID = \"%s\" and GroupID not in (SELECT GroupID FROM network_group where UserID = \"%s\" );", userID, userID);

    if (mysql_query(sql, query))
    {
        fprintf(stderr, "1: %s \n", mysql_error(sql));
        mysql_close(sql);
        return temp;
    }
    else
    {
        MYSQL_RES *result;
        MYSQL_RES *result1;
        result = mysql_store_result(sql);
        if (result == NULL)
        {
            fprintf(stderr, "%s\n", mysql_error(sql));
            mysql_close(sql);
            return temp;
        }
        else
        {
            MYSQL_ROW row;
            MYSQL_ROW row1;
            long k = result->row_count;
            printf("row count = %ld\n", k);
            if (k == 0)
            {
                return temp;
            }
            else
            {
                printf("------- Tra ve list My Share Group --------\n");
                // strcpy(temp,user)
                char s[3];
                strcpy(temp, userID);
                strcat(temp, "*");
                sprintf(s, "%ld", k);
                strcat(temp, s);
                while ((row = mysql_fetch_row(result)) != NULL)
                {
                    int i = 0;
                    printf("[%d] : %s\n", i, row[i]);
                    sprintf(query_check, "SELECT GroupID, GroupName FROM network_group WHERE GroupID = \"%s\"", row[i]);
                    mysql_query(sql, query_check);
                    result1 = mysql_store_result(sql);
                    while ((row1 = mysql_fetch_row(result1)) != NULL)
                    {
                        int j = 0;
                        // printf("GroupID[%s]: %s\n",row1[j],row[j+1]);
                        printf("GroupID: %s \t Group Name %s\n", row1[j], row1[j + 1]);
                        strcat(temp, "*");
                        strcat(temp, row1[j]);
                        strcat(temp, "*");
                        strcat(temp, row1[j + 1]);
                        printf("string : %s\n", temp);
                    }
                    k++;
                }
                return temp;
            }
        }
        mysql_free_result(result);
        mysql_free_result(result1);
    }

    mysql_close(sql);
    return temp;
}
int delete_group(char *groupID)
{
    MYSQL *sql = mysql_init(NULL);
    int state = 0;
    if (sql == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return state;
    }

    if (mysql_real_connect(sql, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {

        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return state;
    }
    char *foldername = get_foldername_via_gid(sql, groupID);
    char cmd[255];
    sprintf(cmd, "rm -rf %s", foldername);
    int k = system(cmd);
    if (k == 1)
    {
        return state;
    }
    else
    {
        char *query = (char *)malloc(255 * sizeof(char));
        sprintf(query, "DELETE FROM network_group WHERE GroupID = \"%s\" ", groupID);
        if (mysql_query(sql, query) == 1)
        {
            fprintf(stderr, "0 : %s\n", mysql_error(sql));
            mysql_close(sql);
            return state;
        }
        else
        {
            state = 1; 
        }
        free(query);
        mysql_close(sql);
        return state;
    }
}
int out_group(char *userID, char *groupID)
{
    MYSQL *sql = mysql_init(NULL);
    int state = 0;
    if (sql == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return state;
    }

    if (mysql_real_connect(sql, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {

        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return state;
    }
    char *query = (char *)malloc(255 * sizeof(char));

    sprintf(query, "DELETE FROM share_group WHERE GroupID = \"%s\" AND UserID = \"%s\"", groupID, userID);
    if (mysql_query(sql, query) == 1)
    {
        fprintf(stderr, "0 : %s\n", mysql_error(sql));
        mysql_close(sql);
        return state;
    }
    else
    {
        state = 1;
    }
    mysql_close(sql);
    return state;
}
int create_group(char *userID, char *groupName, char *groupDescrp)
{
    int state = 0;
    MYSQL *sql = mysql_init(NULL);
    if (sql == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return state;
    }
    if (mysql_real_connect(sql, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return state;
    }
    char *query = (char *)malloc(255 * sizeof(char));
    char *query1 = (char *)malloc(255 * sizeof(char));
    char *query2 = (char *)malloc(255 * sizeof(char));
    char *query3 = (char *)malloc(255 * sizeof(char));

    sprintf(query, "INSERT INTO network_group(UserID, GroupName, GroupDescription) VALUES (\"%s\",\"%s\",\"%s\")", userID, groupName, groupDescrp);
    sprintf(query3, "SELECT * FROM network_group WHERE UserID = \"%s\" AND GroupName = \"%s\" ", userID, groupName);
    sprintf(query1, "SELECT * FROM network_group WHERE UserID = \"%s\" AND GroupName = \"%s\" AND GroupDescription = \"%s\"", userID, groupName, groupDescrp);

    if (mysql_query(sql, query3))
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        state = 0;
    }
    else
    {
        MYSQL_RES *result;
        result = mysql_store_result(sql);
        if (result->row_count != 0)
        {
            // printf("%s",result);
            state = 2;
            return state;
        }
        else
        {
            if (mysql_query(sql, query))
            {
                fprintf(stderr, "%s\n", mysql_error(sql));
                mysql_close(sql);
                return state;
            }
            else
            {
                mysql_query(sql, query1);
                MYSQL_RES *result;
                result = mysql_store_result(sql);
                if (result == NULL)
                {
                    fprintf(stderr, "%s\n", mysql_error(sql));
                    mysql_close(sql);
                    return state;
                }
                else
                {
                    MYSQL_ROW row;
                    int i = 0;
                    char groupID[10];
                    while ((row = mysql_fetch_row(result)) != NULL)
                    {
                        printf("-------- tra ve create group----------\n");
                        printf("groupID : %s\n", row[i]);
                        strcpy(groupID, row[i]);
                        printf("groupID : %s\n", groupID);
                    }
                    sprintf(query2, "INSERT INTO share_group(UserID, GroupID) VALUES (\"%s\",\"%s\")", userID, groupID);
                    mysql_query(sql, query2);

                    char cmd[255];
                    strcpy(cmd, groupName);
                    // create folder with groupName
                    sprintf(cmd, "mkdir \"%s\"", groupName);
                    int check = system(cmd);
                    if (!check)
                    {
                        char *query4 = (char *)malloc(255 * sizeof(char));
                        sprintf(query4, "INSERT INTO share_folder(FolderName,UserID,GroupID) values(\"%s\",\"%s\",\"%s\")", groupName, userID, groupID);
                        mysql_query(sql, query4);
                        printf("add folder %s\n", groupName);
                    }
                    else
                    {
                        printf("cannot create %s\n", groupName);
                    }

                    state = 1;
                    return state;
                }
            }
        }
    }

    mysql_close(sql);
    return state;
}
int access_group(char *groupID, char *userID)
{
    MYSQL *sql = mysql_init(NULL);
    int state = 0;
    if (sql == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return state;
    }

    if (mysql_real_connect(sql, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {

        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return state;
    }
    char *query = (char *)malloc(255 * sizeof(char));
    sprintf(query, "SELECT * FROM network_group where GroupID = \"%s\" and UserID = \"%s\" ", groupID, userID);
    if (mysql_query(sql, query) == 1)
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return state;
    }
    else
    {
        MYSQL_RES *result;
        result = mysql_store_result(sql);
        if (result == NULL)
        {
            fprintf(stderr, "%s\n", mysql_error(sql));
            mysql_close(sql);
            return state;
        }
        else
        {
            MYSQL_ROW row;
            char temp[255] = "";
            printf("-------- Tra ve access group----------\n");
            // strcpy(temp, userID);
            while ((row = mysql_fetch_row(result)) != NULL)
            {
                int i = 0;
                printf("Group : %s %s %s %s\n", row[i], row[i + 1], row[i + 2], row[i + 3]);
                strcpy(temp, row[i]);
            }
            printf("check temp : %s\n", temp);
            if (strcmp(temp, groupID) == 0)
            {
                state = 1;
            }
        }
        mysql_free_result(result);
    }
    mysql_close(sql);
    return state;
}
int access_share_group(char *groupID, char *userID)
{
    MYSQL *sql = mysql_init(NULL);
    int state = 0;
    if (sql == NULL)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return state;
    }

    if (mysql_real_connect(sql, db_host, db_user, db_pass, db_name, 0, NULL, 0) == NULL)
    {

        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return state;
    }
    printf("check groupid : %s\n", groupID);
    printf("check userid : %s\n", userID);
    char *query = (char *)malloc(255 * sizeof(char));
    sprintf(query, "SELECT * FROM share_group where GroupID = \"%s\" and UserID = \"%s\" ", groupID, userID);
    if (mysql_query(sql, query) == 1)
    {
        fprintf(stderr, "%s\n", mysql_error(sql));
        mysql_close(sql);
        return state;
    }
    else
    {
        MYSQL_RES *result;
        result = mysql_store_result(sql);
        if (result == NULL)
        {
            fprintf(stderr, "%s\n", mysql_error(sql));
            mysql_close(sql);
            return state;
        }
        else
        {
            MYSQL_ROW row;
            if (result->row_count != 0)
            {
                printf("check row count : %ld\n", result->row_count);
            }
            char temp[255] = "";
            printf("-------- Tra ve access share group----------\n");
            // strcpy(temp, userID);
            while ((row = mysql_fetch_row(result)) != NULL)
            {
                int i = 0;
                printf("Group : %s %s\n", row[i], row[i + 1]);
                strcpy(temp, row[i + 1]);
            }
            printf("check temp : %s\n", temp);
            if (strcmp(temp, groupID) == 0)
            {
                state = 1;
            }
        }
        mysql_free_result(result);
    }
    mysql_close(sql);
    printf("check state : %d\n", state);
    return state;
}