#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdbool.h>
#include "headers/constants.h"
#include "headers/User.h"

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

int option,currUserID;

int main(){
	struct sockaddr_in server;
	int s_fd,msgLength;
	char buff[50];
	char result;

	//connection establishment
	int client_fd = 0, login_status;
    char Buffer[1024] = "";

    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("client socket");
        exit(EXIT_FAILURE);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(CLIENT_PORT);

    if(inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) <= 0){
        perror("ip address");
        exit(EXIT_FAILURE);
    }

    if(connect(client_fd, (struct sockaddr*)&server, sizeof(server)) < 0){
        perror("client connect");
        exit(EXIT_FAILURE);
    }
	printf("Connected to server!!!\n\n");
	chooseOption(client_fd);
	showMenu(client_fd);	

	close(client_fd);

	return 0;
}

void chooseOption(int s_fd){
	while(1){
		write(1,"1 : Normal-User Login\n",sizeof("1 : Normal-User Login\n"));
		write(1,"2 : Joint Account-User Login\n",sizeof("2 : Joint Account-User Login\n"));
		write(1,"3 : Admin Login\n",sizeof("3 : Admin Login\n"));
		
		write(1,"Choose an option : ",sizeof("Choose an option : "));
		scanf("%d",&option);
		printf("Option : %d\n",option);
		//option=option-2608;

		if(option == 1){
			attemptUserLogin(s_fd); 
			break;
		}
		else if(option == 2){
			attemptUserLogin(s_fd);
			break;
		}
		else if(option == 3){
			attemptUserLogin(s_fd);
			break;
		}
		else write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
	}
	return;
}

void showMenu(int s_fd){
	int select;
	if(option==1 || option==2){
		write(1,"1 : Deposit\n",sizeof("1 : Deposit\n"));
		write(1,"2 : Withdraw\n",sizeof("2 : Withdraw\n"));
		write(1,"3 : Balance Enquiry\n",sizeof("3 : Balance Enquiry\n"));
		write(1,"4 : Password Change\n",sizeof("4 : Password Change\n"));
		write(1,"5 : View Details\n",sizeof("5 : View Details\n"));
		write(1,"6 : Exit\n",sizeof("6 : Exit\n"));
	
		write(1,"Choose an option : ",sizeof("Choose an option : "));
		scanf("%d",&select);
		printf("Option : %d\n",select);
		
		switch(select){
		case 1 :
			deposit(s_fd);
			break;
		case 2 :
			withdraw(s_fd);
			break;
		case 3 :
			balanceEnquiry(s_fd);
			break;
		case 4 :
			changePassword(s_fd);
			break;
		case 5 :
			viewDetails(s_fd);
			break;
		case 6 :
			write(s_fd,&select,sizeof(int));
			write(1,"Thank you\n",sizeof("Thank you\n"));
			exit(0);
		default :
			write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
			showMenu(s_fd);
			break;
		}
	}
	else if(option==3){
		write(1,"1 : Add Account\n",sizeof("1 : Add Account\n"));
		write(1,"2 : Delete Account\n",sizeof("2 : Delete Account\n"));
		write(1,"3 : Modify Account\n",sizeof("3 : Modify Account\n"));
		write(1,"4 : Search Account\n",sizeof("4 : Search Account\n"));
		write(1,"5 : Exit\n",sizeof("6 : Exit\n"));

		write(1,"Choose an option : ",sizeof("Choose an option : "));
		scanf("%d",&select);
		printf("Option : %d\n",select);
		
		switch(select){
		case 1 :
			addAccount(s_fd);
			break;
		case 2 :
			deleteAccount(s_fd);
			break;
		case 3 :
			modifyAccount(s_fd);
			break;
		case 4 :
			searchAccount(s_fd);
			break;
		case 5 :
			write(s_fd,&select,sizeof(int));
			write(1,"Thank you\n",sizeof("Thank you\n"));
			exit(0);
		default :
			write(1,"Invalid option!!\n\n",sizeof("Invalid option!!\n\n"));
			showMenu(s_fd);
			break;
		}
	}
}

void attemptUserLogin(int s_fd){
	bool result;
	User currUser;
	write(1,"User ID : ",sizeof("User ID : "));
	scanf("%d",&currUser.userID);
	currUserID=currUser.userID;
	write(1,"Password : ",sizeof("Password : "));
	scanf("%s",currUser.password);

	//to the server
	write(s_fd,&option,sizeof(int));
	write(s_fd,&currUser,sizeof(User));

	read(s_fd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"Invalid login!!\n\n",sizeof("Invalid login!!\n\n"));
		chooseOption(s_fd);
	}
	else{
		write(1,"Succesfully logged in!!\n\n",sizeof("Succesfully logged in!!\n\n"));
	}
	return;
}

void deposit(int s_fd){
	float amount;
	int select=1;
	bool result;

	write(1,"Amount to Deposit : Rs.",sizeof("Amount to Deposit : Rs."));
	scanf("%f",&amount);

	while(amount<=0){
		printf("Enter a valid amount!!\n");
		write(1,"Amount to Deposit : Rs.",sizeof("Amount to Deposit : Rs."));
		scanf("%f",&amount);
	}

	//to the server
	write(s_fd,&select,sizeof(int));
	write(s_fd,&amount,sizeof(float));

	read(s_fd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"Error depositing your money!!\n\n",sizeof("Error depositing your money!!\n\n"));
	}
	else{
		write(1,"Succesfully deposited!!\n\n",sizeof("Succesfully deposited!!\n\n"));
	}
	showMenu(s_fd);
	return;
}

void withdraw(int s_fd){
	float amount;
	int select=2;
	bool result;

	write(1,"Amount to Withdraw : Rs.",sizeof("Amount to Withdraw : Rs."));
	scanf("%f",&amount);

	while(amount<=0){
		printf("Enter a valid amount!!\n");
		write(1,"Amount to Withdraw : Rs.",sizeof("Amount to Withdraw : Rs."));
		scanf("%f",&amount);
	}

	//to the server
	write(s_fd,&select,sizeof(int));
	write(s_fd,&amount,sizeof(float));

	read(s_fd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"Error withdrawing your money, please check your balance!!\n\n",sizeof("Error depositing your money, please check your balance!!\n\n"));
	}
	else{
		write(1,"Succesfully withdraw!!\n\n",sizeof("Succesfully withdraw!!\n\n"));
	}
	showMenu(s_fd);
	return;
}

void balanceEnquiry(int s_fd){
	float amount;
	int select=3;
	int len;

	//to the server
	write(s_fd,&select,sizeof(int));
	
	len=read(s_fd,&amount,sizeof(float));	//from server

	write(1,"Available Balance :: Rs.",sizeof("Available Balance :: Rs."));
	printf("%0.2f\n\n",amount);

	showMenu(s_fd);
	return;
}

void changePassword(int s_fd){
	int select=4;
	char nPassword[15];
	bool result;

	write(1,"Enter the new password(max 10 characters) : ",sizeof("Enter the new password(max 10 characters) : "));
	scanf("%s",nPassword);

	//to the server
	write(s_fd,&select,sizeof(int));
	write(s_fd,nPassword,sizeof(nPassword));

	read(s_fd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"Error changing your password!!\n\n",sizeof("Error changing your password!!\n\n"));
	}
	else{
		write(1,"Succesfully changed your password!!\n\n",sizeof("Succesfully changed your password!!\n\n"));
	}
	showMenu(s_fd);
	return;
}

void viewDetails(int s_fd){
	int select=5;

	//to the server
	write(s_fd,&select,sizeof(int));

	if(option==1){
		User currUser1;
		read(s_fd,&currUser1,sizeof(User));
		
		printf("User ID : %d\n",currUser1.userID);
		printf("Name : %s\n",currUser1.name);
		printf("Account Number : %d\n",currUser1.account_no);
		printf("Available Balance : Rs.%0.2f\n",currUser1.balance);
		printf("Status : %s\n\n",currUser1.status);
	}
	else if(option==2){
		User currUser2;
		read(s_fd,&currUser2,sizeof(User));
		
		printf("User ID : %d\n",currUser2.userID);
		printf("Main Account Holder's Name : %s\n",currUser2.name);
		printf("Other Account Holder's Name : %s\n",currUser2.name2);
		printf("Account Number : %d\n",currUser2.account_no);
		printf("Available Balance : Rs.%0.2f\n",currUser2.balance);
		printf("Status : %s\n\n",currUser2.status);
	}
	showMenu(s_fd);
	return;
}

void addAccount(int s_fd){
	int select=1;
	int type;
	bool result;

	write(s_fd,&select,sizeof(int));

	write(1,"Enter the type account(1: Normal Account 2: Joint Account) : ",sizeof("Enter the type account(1: Normal Account 2: Joint Account) : "));
	scanf("%d",&type);

	write(s_fd,&type,sizeof(int));

	if(type==1){
		User addUser1;
		write(1,"Name of the account holder : ",sizeof("Name of the account holder : "));
		scanf(" %[^\n]",addUser1.name);
		write(1,"Password(max 10 characters) : ",sizeof("Password(max 10 characters) : "));
		scanf("%s",addUser1.password);
		write(1,"Initial Deposit : Rs.",sizeof("Initial Deposit : Rs."));
		scanf("%f",&addUser1.balance);
		write(s_fd,&addUser1,sizeof(User));
	}

	if(type==2){
		User addUser2;
		write(1,"Name of the primary account holder : ",sizeof("Name of the primary account holder : "));
		scanf(" %[^\n]",addUser2.name);
		write(1,"Name of the other account holder : ",sizeof("Name of the other account holder : "));
		scanf(" %[^\n]",addUser2.name2);
		write(1,"Password(max 10 characters) : ",sizeof("Password(max 10 characters) : "));
		scanf("%s",addUser2.password);
		write(1,"Initial Deposit : Rs.",sizeof("Initial Deposit : Rs."));
		scanf("%f",&addUser2.balance);
		write(s_fd,&addUser2,sizeof(User));
	}
	
	read(s_fd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"Error adding the account!!\n\n",sizeof("Error adding the account!!\n\n"));
	}
	else{
		write(1,"Succesfully added the account!!\n\n",sizeof("Succesfully added the account!!\n\n"));
	}
	showMenu(s_fd);
	return;
}

void deleteAccount(int s_fd){
	int select=2;
	int type,userID;
	bool result;

	write(s_fd,&select,sizeof(int));

	write(1,"Enter the type account(1: Normal Account 2: Joint Account) : ",sizeof("Enter the type account(1: Normal Account 2: Joint Account) : "));
	scanf("%d",&type);
	
	write(s_fd,&type,sizeof(int));

	write(1,"User ID : ",sizeof("User ID : "));
	scanf("%d",&userID);
	write(s_fd,&userID,sizeof(int));
	
	read(s_fd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"Error deleting the account ,please re-check the User ID!!\n\n",sizeof("Error deleting the account ,please re-check the User ID!!\n\n"));
	}
	else{
		write(1,"Succesfully deleted the account!!\n\n",sizeof("Succesfully deleted the account!!\n\n"));
	}
	showMenu(s_fd);
	return;
}

void modifyAccount(int s_fd){
	int select=3;
	int type;
	bool result;

	write(s_fd,&select,sizeof(int));

	write(1,"Enter the type account(1: Normal Account 2: Joint Account) : ",sizeof("Enter the type account(1: Normal Account 2: Joint Account) : "));
	scanf("%d",&type);

	write(s_fd,&type,sizeof(int));

	if(type==1){
		User modUser1;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&modUser1.userID);
		write(1,"Account Number : ",sizeof("Account Number : "));
		scanf("%d",&modUser1.account_no);
		write(1,"New Name of the account holder : ",sizeof("New Name of the account holder : "));
		scanf(" %[^\n]",modUser1.name);
		write(1,"New Password(max 10 characters) : ",sizeof("New Password(max 10 characters) : "));
		scanf("%s",modUser1.password);
		write(1,"New Balance : ",sizeof("New Balance : "));
		scanf("%f",&modUser1.balance);
		write(s_fd,&modUser1,sizeof(User));
	}

	if(type==2){
		User modUser2;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&modUser2.userID);
		write(1,"Account Number : ",sizeof("Account Number : "));
		scanf("%d",&modUser2.account_no);
		write(1,"New Name of the primary account holder : ",sizeof("New Name of the primary account holder : "));
		scanf(" %[^\n]",modUser2.name);
		write(1,"New Name of the other account holder : ",sizeof("New Name of the other account holder : "));
		scanf(" %[^\n]",modUser2.name2);
		write(1,"New Password(max 10 characters) : ",sizeof("New Password(max 10 characters) : "));
		scanf("%s",modUser2.password);
		write(1,"New Balance : ",sizeof("New Balance : "));
		scanf("%f",&modUser2.balance);
		write(s_fd,&modUser2,sizeof(User));
	}
	
	read(s_fd,&result,sizeof(result)); //from the server

	if(!result){
		write(1,"Error modifying the account ,please re-check the User ID and Account No!!\n\n",sizeof("Error modifying the account ,please re-check the User ID and Account No!!\n\n"));
	}
	else{
		write(1,"Succesfully modified the account!!\n\n",sizeof("Succesfully modified the account!!\n\n"));
	}
	showMenu(s_fd);
	return;
}

void searchAccount(int s_fd){
	int select=4;
	int type,len;
	bool result;

	write(s_fd,&select,sizeof(int));

	write(1,"Enter the type account(1: Normal Account 2: Joint Account) : ",sizeof("Enter the type account(1: Normal Account 2: Joint Account) : "));
	scanf("%d",&type);

	write(s_fd,&type,sizeof(int));

	if(type==1){
		User searchUser1;
		int userID;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&userID);
		write(s_fd,&userID,sizeof(int));
		
		len=read(s_fd,&searchUser1,sizeof(User));
		if(len==0){
			write(1,"Please re-check the User ID!!\n\n",sizeof("Please re-check the User ID!!\n\n"));
		}
		else{
			printf("User ID : %d\n",searchUser1.userID);
			printf("Name : %s\n",searchUser1.name);
			printf("Account Number : %d\n",searchUser1.account_no);
			printf("Available Balance : Rs.%0.2f\n",searchUser1.balance);
			printf("Status : %s\n\n",searchUser1.status);
		}
	}

	if(type==2){
		User searchUser2;
		int userID1;
		write(1,"User ID : ",sizeof("User ID : "));
		scanf("%d",&userID1);
		write(s_fd,&userID1,sizeof(int));
		
		len=read(s_fd,&searchUser2,sizeof(User));
		if(len==0){
			write(1,"Please re-check the User ID!!\n\n",sizeof("Please re-check the User ID!!\n\n"));
		}
		else{
			printf("User ID : %d\n",searchUser2.userID);
			printf("Main Account Holder's Name : %s\n",searchUser2.name);
			printf("Other Account Holder's Name : %s\n",searchUser2.name2);
			printf("Account Number : %d\n",searchUser2.account_no);
			printf("Available Balance : Rs.%0.2f\n",searchUser2.balance);
			printf("Status : %s\n\n",searchUser2.status);
		}
	}
	showMenu(s_fd);
	return;
}
