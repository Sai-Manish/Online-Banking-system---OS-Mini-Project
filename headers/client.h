#ifndef CLIENT_H
#define CLIENT_H

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdbool.h>
#include "constants.h"
#include "User.h"

void attemptNormalUserLogin(int s_fd);
void attemptJointUserLogin(int s_fd);
void attemptAdminLogin(int s_fd);
void showMenu(int s_fd);
void deposit(int s_fd);
void withdraw(int s_fd);
void balanceEnquiry(int s_fd);
void changePassword(int s_fd);
void viewDetails(int s_fd);
void addAccount(int s_fd);
void deleteAccount(int s_fd);
void modifyAccount(int s_fd);
void searchAccount(int s_fd);
void chooseOption(int s_fd);
void attemptUserLogin(int s_fd);

#endif