/*Program to create three or more records in a file*/
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
#include "headers/constants.h"
#include "headers/User.h"

int getNewNormalUserID();
int getNewJointUserID();
int getNewAdminUserID();

int getNewNormalUserID(){
	int fd=open("NormalUsersAccounts",O_RDONLY,0744);
	User record;
	lseek(fd,-sizeof(User),SEEK_END);
	read(fd,&record,sizeof(User));
	close(fd);
	return record.userID+1;
}

int getNewJointUserID(){
	int fd=open("JointUsersAccounts",O_RDONLY,0744);
	User record;
	lseek(fd,-sizeof(User),SEEK_END);
	read(fd,&record,sizeof(User));
	close(fd);
	return record.userID+1;
}

int getNewAdminUserID(){
	int fd=open("AdministratorFile",O_RDONLY,0744);
	User record;
	lseek(fd,-sizeof(User),SEEK_END);
	read(fd,&record,sizeof(User));
	close(fd);
	return record.userID+1;
}

int main(){
	int fd=open("AdministratorFile",O_RDWR | O_CREAT,0744);
	int choice=0;
	User newAdmin;
	printf("Please enter the name of the admin: ");
	scanf(" %s",newAdmin.name);
	printf("Please enter the password(max 15 characters): ");
	scanf(" %s",newAdmin.password);
	newAdmin.userID=3000;
    newAdmin.balance = 0;
    newAdmin.account_no = 0;
    strcpy(newAdmin.name2,"");
    strcpy(newAdmin.status,"Admin");
	printf("Your userID is : %d\n",newAdmin.userID);
	write(fd,&newAdmin,sizeof(newAdmin));
	printf("Do you want to continue(0/1)? ");
	scanf("%d",&choice);
	while(choice){
		printf("Please enter the name of the admin: ");
		scanf(" %[^\n]",newAdmin.name);
		printf("Please enter the password(max 15 characters): ");
		scanf(" %[^\n]",newAdmin.password);
		newAdmin.userID=getNewAdminUserID();
        newAdmin.balance = 0;
        newAdmin.account_no = 0;
        strcpy(newAdmin.name2,"");
        strcpy(newAdmin.status,"Admin");
		printf("Your userID is : %d\n",newAdmin.userID);
		write(fd,&newAdmin,sizeof(User));
		printf("Do you want to continue(0/1)? ");
		scanf("%d",&choice);
	}
	close(fd);

	fd=open("NormalUsersAccounts",O_RDWR | O_CREAT,0744);
	choice=1;
	User newNUser;
	printf("Please enter the name of the normal user: ");
	scanf(" %[^\n]",newNUser.name);
	printf("Please enter the password(max 15 characters): ");
	scanf(" %[^\n]",newNUser.password);
	newNUser.userID=1000;
	newNUser.balance=1000;
	newNUser.account_no=(newNUser.userID-1000)+100000;
    strcpy(newNUser.name2,"");
	printf("Your userID is : %d\n",newNUser.userID);
	strcpy(newNUser.status,"ACTIVE");
	write(fd,&newNUser,sizeof(User));
	printf("Do you want to continue(0/1)? ");
	scanf("%d",&choice);
	while(choice){
		printf("Please enter the name of the normal user: ");
		scanf(" %[^\n]",newNUser.name);
		printf("Please enter the password(max 10 characters): ");
		scanf(" %[^\n]",newNUser.password);
		newNUser.userID=getNewNormalUserID();
		newNUser.balance=1000;
		newNUser.account_no=(newNUser.userID-1000)+100000;
        strcpy(newNUser.name2,"");
		printf("Your userID is : %d\n",newNUser.userID);
		strcpy(newNUser.status,"ACTIVE");
		write(fd,&newNUser,sizeof(User));
		printf("Do you want to continue(0/1)? ");
		scanf("%d",&choice);
	}
	close(fd);

	fd=open("JointUsersAccounts",O_RDWR | O_CREAT,0744);
	choice=1;
	User newJUser;
	printf("Please enter the main name of the joint user: ");
	scanf(" %[^\n]",newJUser.name);
	printf("Please enter the second name of the joint user: ");
	scanf(" %[^\n]",newJUser.name2);
	printf("Please enter the password(max 10 characters): ");
	scanf(" %[^\n]",newJUser.password);
	newJUser.userID=2000;
	newJUser.balance=1000;
	newJUser.account_no=(newJUser.userID-2000)+200000;
	printf("Your userID is : %d\n",newJUser.userID);
	strcpy(newJUser.status,"ACTIVE");
	write(fd,&newJUser,sizeof(User));
	printf("Do you want to continue(0/1)? ");
	scanf("%d",&choice);
	while(choice){
		printf("Please enter the main name of the joint user: ");
		scanf(" %[^\n]",newJUser.name);
		printf("Please enter the second name of the joint user: ");
		scanf(" %[^\n]",newJUser.name2);
		printf("Please enter the password(max 10 characters): ");
		scanf(" %[^\n]",newJUser.password);
		newJUser.userID=getNewJointUserID();
		newJUser.balance=1000;
		newJUser.account_no=(newJUser.userID-2000)+200000;
		printf("Your userID is : %d\n",newJUser.userID);
		strcpy(newJUser.status,"ACTIVE");
		write(fd,&newJUser,sizeof(User));
		printf("Do you want to continue(0/1)? ");
		scanf("%d",&choice);
	}
	close(fd);
	return 0;
}


