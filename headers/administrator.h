#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H


typedef struct administrator{
	int userID;
    char name[50];
    int account_no;
    float balance;
    char status[25];
	char password[15];
}administrator;

#endif