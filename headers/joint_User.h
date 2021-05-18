#ifndef JOINT_USER_H
#define JOINT_USER_H

typedef struct joint_User{
	int userID;
	char name1[50];
	char name2[50];
	char password[15];
	int account_no;
	float balance;
	char status[25];
    int accType = 2;
}joint_User;

#endif