#include "menu.h"
int validate_login(char *email, char *password)
{
    // strcpy(check_password,password);
    char tmp1[50], tmp2[50];
    strcpy(tmp1, email);
    strcpy(tmp2, password);
    if (strstr(tmp1, "@gmail.com") != NULL)
    {
        if (strlen(strstr(tmp1, "@gmail.com")) == 10)
        {
            if (strlen(tmp2) < 6)
            {
                return 0;
            }
            else
                return 1;
        }
    }
    return 0;
}

void login_page(){
    printf(COLOR_BLUE"\n_____________________YOU MUST BE LOGGED TO USE THE PROGRAM____________________\n"COLOR_RESET);
    printf("\n");
    printf("\t\t\t\t1. Sign in.\n");
    printf("\t\t\t\t2. Sign up.\n");
    printf("\t\t\t\t3. Exit.\n");
    printf(COLOR_BLUE"________________________________________________________________________________________\n"COLOR_RESET);
}
void home_page(){
    printf(COLOR_BLUE"\n_____________________________________HOME_____________________________________\n"COLOR_RESET);
    printf("\n");
    printf("\t\t\t1. Your profile\n");
    printf("\t\t\t2. User list\n");
    printf("\t\t\t3. My group\n");
    printf("\t\t\t4. My share group\n");
    printf("\t\t\t5. Create a group\n");
    printf("\t\t\t6. Logout\n");
    printf(COLOR_BLUE"_________________________________________________________________________________\n"COLOR_RESET);
}
void admin_page(){
    printf(COLOR_BLUE"\n__________________________________MY GROUP PAGE________________________________\n"COLOR_RESET);
    printf("\n");
    printf("\t\t\t1. Member info.\n");
    printf("\t\t\t2. List folder.\n");
    printf("\t\t\t3. List file.\n");
    printf("\t\t\t4. Upload file.\n");
    printf("\t\t\t5. Download file.\n");
    printf("\t\t\t6. Remove file.\n");
    printf("\t\t\t7. Create Folder \n");
    printf("\t\t\t8. Delele Folder \n");
    printf("\t\t\t9. Add member\n");
    printf("\t\t\t10. Delete memeber\n");
    printf("\t\t\t11. Delete group\n");
    printf("\t\t\t12. Back\n");
    printf(COLOR_BLUE"________________________________________________________________________________________\n"COLOR_RESET);
}
void share_group_page(){
    printf(COLOR_BLUE"\n___________________________________GROUP SHARE PAGE_____________________________\n\n"COLOR_RESET);
    printf("\t\t\t1. Member info.\n");
    printf("\t\t\t2. List folder.\n");
    printf("\t\t\t3. List file.\n");
    printf("\t\t\t4. Upload file.\n");
    printf("\t\t\t5. Download file.\n");
    printf("\t\t\t6. Remove file.\n");
    printf("\t\t\t7. Create Folder. \n");
    printf("\t\t\t8. Out group.\n");
    printf("\t\t\t9. Back .\n");
    printf(COLOR_BLUE"________________________________________________________________________________________\n"COLOR_RESET);
}
