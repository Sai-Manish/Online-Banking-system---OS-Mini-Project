#ifndef BANK_SERVER_H
#define BANK_SERVER_H

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include<fcntl.h>
#include <pthread.h>
#include "constants.h"
#include "User.h"

//void* client_Handler(void* s_fd);
User getUser(int ID,int accType);
bool checkUser(User currUser, int accType);
bool depositMoney(int accType,int ID,float amount);
bool withdrawMoney(int accType,int ID,float amount);
float getBalance(int accType,int ID);
bool alterPassword(int accType,int ID,char newpassword[15]);
bool addUser(User record,int accType);
bool deleteUser(int ID,int accType);
bool modifyUser(User modUser,int accType);
void put_Busy(int ID,int accType);
void un_Busy(int ID,int accType);

#endif