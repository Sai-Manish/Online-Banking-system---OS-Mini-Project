#ifndef NORMAL_USER_H
#define NORMAL_USER_H


typedef struct normal_User{
	int userID;
	char name[50];
	char password[15];
	int account_no;
	float balance;
	char status[25];
}normal_User;

#endif