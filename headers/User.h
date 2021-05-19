#ifndef USER_H
#define USER_H


typedef struct User{
	int userID;
    int account_no;
	char name[50];
	char name2[50];
	char password[15];
	float balance;
	char status[25];
}User;

#endif