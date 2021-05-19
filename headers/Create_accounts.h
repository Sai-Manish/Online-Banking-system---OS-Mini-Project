#ifndef CREATE_ACCOUNT_H
#define CREATE_ACCOUNT_H

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include "constants.h"
#include "User.h"

int getNewNormalUserID();
int getNewJointUserID();
int getNewAdminUserID();
void chooseOption();
void Create_NormalUser(); 
void Create_JointUser();
void Create_Admin();
#endif