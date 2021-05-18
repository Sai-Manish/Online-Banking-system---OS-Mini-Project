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
#include "headers/normal_User.h"
#include "headers/joint_User.h"
#include "headers/administrator.h"
#include "headers/constants.h"

int main(){
    int server_fd, new_server_fd, opt=1;
    struct sockaddr_in server_addr;
    socklen_t addrlen = sizeof(server_addr);

    // First create the server
    if(!(server_fd = socket(AF_INET, SOCK_STREAM, 0))){
        perror("server socket");
        exit(EXIT_FAILURE);
    }

    // Then configure the protocols for the socket
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("server setsocketopt");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Binf the server to the protocols and the ports
    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("server bind");
        exit(EXIT_FAILURE);
    }

    // Start listening on the port, with a limit on the number of connections
    if(listen(server_fd, MAX_CONNECTIONS) < 0){
        perror("server listen");
        exit(EXIT_FAILURE);
    }

    #ifdef LOGGING
        printf("[LOG] Server ready to accept connections on port %d...\n", SERVER_PORT);
    #endif

    // Main event loop
    while(1){
        new_server_fd = accept(server_fd, (struct sockaddr *)&server_addr, (socklen_t*)&addrlen);

        if(new_server_fd > 0){
			write(1,"Client got connected!!!\n",sizeof("Client got connected!!!\n"));
            pthread_t threadID;
            int* socket_ptr = malloc(sizeof(int));
            *socket_ptr = new_server_fd;

            pthread_create(&threadID, NULL, client_Handler, (void *)socket_ptr);
        }
    }
	pthread_exit(NULL);
    close(server_fd);
    return 0;
}

void* client_Handler(void* s_fd){
	int sock_fd = *((int *)s_fd);
	int length_msg,select,type,User_Option,accType,currUserID;
	bool result;
	while(1){
		read(sock_fd,&User_Option,sizeof(User_Option));
		printf("Option : %d\n",User_Option);

		if(User_Option==1){
			normal_User currUser;
			//accType=1;
			length_msg=read(sock_fd,&currUser,sizeof(normal_User));
			printf("Username : %d\n",currUser.userID);
			printf("Password : %s\n",currUser.password);
			currUserID=currUser.userID;
			result=checknormal_User(currUser);
			write(sock_fd,&result,sizeof(result));
		}
		else if(User_Option==2){
			joint_User currUser;
			//accType=2;
			length_msg=read(sock_fd,&currUser,sizeof(joint_User));
			currUserID=currUser.userID;
			printf("Username : %d\n",currUser.userID);
			printf("Password : %s\n",currUser.password);
			result=checkjoint_User(currUser);
			write(sock_fd,&result,sizeof(result));
		}
		else if(User_Option==3){
			administrator currUser;
			//accType=3;
			length_msg=read(sock_fd,&currUser,sizeof(administrator));
			currUserID=currUser.userID;
			printf("Username : %d\n",currUser.userID);
			printf("Password : %s\n",currUser.password);
			result=checkadministrator(currUser);
			write(sock_fd,&result,sizeof(result));
		}
		else{
			result=false;
			write(sock_fd,&result,sizeof(result));
		}
		if(result)	break;		
	}

	while(1){
		read(sock_fd,&select,sizeof(int));
		if(User_Option==1 || User_Option==2){
			if(select==1){
				float amount;
				read(sock_fd,&amount,sizeof(float));
				result=depositMoney(currUser.accType,currUserID,amount);
				write(sock_fd,&result,sizeof(result));
			}
			else if(select==2){
				float amount;
				read(sock_fd,&amount,sizeof(float));
				result=withdrawMoney(currUser.accType,currUserID,amount);
				write(sock_fd,&result,sizeof(result));
			}
			else if(select==3){
				float amount;
				amount=getBalance(currUser.accType,currUserID);
				write(sock_fd,&amount,sizeof(float));
			}
			else if(select==4){
				char pwd[10];
				read(sock_fd,pwd,sizeof(pwd));
				result=alterPassword(accType,currUserID,pwd);
				write(sock_fd,&result,sizeof(result));
			}
			else if(select==5){
				if(User_Option==1){
					normal_User user1=getnormal_User(currUserID);
					write(sock_fd,&user1,sizeof(normal_User));
				}
				else if(User_Option==2){
					joint_User user2=getjoint_User(currUserID);
					write(sock_fd,&user2,sizeof(joint_User));
				}
			}
			else if(select==6)	break;
		}
		else if(User_Option==3){
			read(sock_fd,&type,sizeof(int));
			if(select==1){
				if(type==1){
					normal_User newUser1;
					read(sock_fd,&newUser1,sizeof(normal_User));
					result=addnormal_User(newUser1);
					write(sock_fd,&result,sizeof(result));
				}
				else if(type==2){
					joint_User newUser2;
					read(sock_fd,&newUser2,sizeof(joint_User));
					result=addjoint_User(newUser2);
					write(sock_fd,&result,sizeof(result));
				}
			}
			else if(select==2){
				if(type==1){
					int delUserID1;
					read(sock_fd,&delUserID1,sizeof(int));
					result=deletenormal_User(delUserID1);
					write(sock_fd,&result,sizeof(result));
				}
				else if(type==2){
					int delUserID2;
					read(sock_fd,&delUserID2,sizeof(int));
					result=deletejoint_User(delUserID2);
					write(sock_fd,&result,sizeof(result));
				}
			}
			else if(select==3){
				if(type==1){
					normal_User modUser1;
					read(sock_fd,&modUser1,sizeof(normal_User));
					result=modifynormal_User(modUser1);
					write(sock_fd,&result,sizeof(result));
				}
				else if(type==2){
					joint_User modUser2;
					read(sock_fd,&modUser2,sizeof(joint_User));
					result=modifyjoint_User(modUser2);
					write(sock_fd,&result,sizeof(result));
				}
			}
			else if(select==4){
				if(type==1){
					normal_User searchUser1;
					int userID1;
					read(sock_fd,&userID1,sizeof(int));
					searchUser1=getnormal_User(userID1);
					write(sock_fd,&searchUser1,sizeof(normal_User));
				}
				else if(type==2){
					joint_User searchUser2;
					int userID2;
					read(sock_fd,&userID2,sizeof(int));
					searchUser2=getjoint_User(userID2);
					write(sock_fd,&searchUser2,sizeof(joint_User));
				}
			}
			else if(select==5)	break;
		}
	}
	close(sock_fd);
	write(1,"Client session ended!!!\n",sizeof("Client session ended!!!\n"));
	return;
	
	//Task(sock_fd);
}

/*void Task(int sock_fd){

}*/


normal_User getNormalUser(int ID){
	int i=ID-1000;
	normal_User currUser;
	int fd=open("NormalUsersAccounts",O_RDONLY,0744);
	
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normal_User);    	     //nth record
	lock.l_len=sizeof(normal_User);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(normal_User),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(normal_User));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return currUser;
}

joint_User getJointUser(int ID){
	int i=ID-1000;
	jointUser currUser;
	int fd=open("JointUsersAccounts",O_RDONLY,0744);
	
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    	     //nth record
	lock.l_len=sizeof(jointUser);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(jointUser));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return currUser;
}

admin getAdmin(int ID){
	int i=ID-1000;
	admin currUser;
	int fd=open("AdministratorFile",O_RDONLY,0744);
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(admin);    	     //nth record
	lock.l_len=sizeof(admin);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(admin),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(admin));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return currUser;
}

bool checkNormalUser(normal_User currUser){
	int i=currUser.userID-1000;
	int fd=open("NUfile",O_RDONLY,0744);
	bool result;
	normalUser temp;
	
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normalUser);    	     //nth record
	lock.l_len=sizeof(normalUser);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(normalUser));
	if(!strcmp(temp.password,currUser.password) && !strcmp(temp.status,"ACTIVE"))	result=true;
	else						result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool checkJointUser(jointUser currUser){
	int i=currUser.userID-1000;
	int fd=open("JUfile",O_RDONLY,0744);
	bool result;
	jointUser temp;
	
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    	     //nth record
	lock.l_len=sizeof(jointUser);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(jointUser));
	if(!strcmp(temp.password,currUser.password) && !strcmp(temp.status,"ACTIVE"))	result=true;
	else						result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool checkAdmin(admin currUser){
	int i=currUser.userID-1000;
	int fd=open("ADfile",O_RDONLY,0744);
	bool result;
	admin temp;
	
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(admin);    	     //nth record
	lock.l_len=sizeof(admin);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(admin),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(admin));
	if(!strcmp(temp.password,currUser.password))	result=true;
	else						result=false;

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool depositMoney(int accType,int ID,float amount){
	int i=ID-1000;
	if(accType==1){
		int fd=open("NUfile",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normalUser);    //nth record
		lock.l_len=sizeof(normalUser);	             //sizeof(record)
		lock.l_pid=getpid();
	
		fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		normalUser currUser;
		lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(normalUser));
		
		if(!strcmp(currUser.status,"ACTIVE")){
			currUser.balance+=amount;
			lseek(fd,sizeof(normalUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(normalUser));
			result=true;
		}
		else	result=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;		
	}
	else if(accType==2){
		int fd=open("JUfile",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    //nth record
		lock.l_len=sizeof(jointUser);	             //sizeof(record)
		lock.l_pid=getpid();
	
		fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		jointUser currUser;
		lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(jointUser));
		
		if(!strcmp(currUser.status,"ACTIVE")){
			currUser.balance+=amount;
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(jointUser));
			result=true;
		}
		else	result=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;	
	}
	return false;
}

bool withdrawMoney(int accType,int ID,float amount){
	int i=ID-1000;
	if(accType==1){
		int fd=open("NUfile",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normalUser);    //nth record
		lock.l_len=sizeof(normalUser);	             //sizeof(record)
		lock.l_pid=getpid();
	
		fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		normalUser currUser;
		lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(normalUser));
		
		if(!strcmp(currUser.status,"ACTIVE") && currUser.balance>=amount){
			currUser.balance-=amount;
			lseek(fd,sizeof(normalUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(normalUser));
			result=true;
		}
		else	result=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;	
	}
	else if(accType==2){
		int fd=open("JUfile",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    //nth record
		lock.l_len=sizeof(jointUser);	             //sizeof(record)
		lock.l_pid=getpid();
	
		fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		jointUser currUser;
		lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(jointUser));
		
		if(!strcmp(currUser.status,"ACTIVE") && currUser.balance>=amount){
			currUser.balance-=amount;
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(jointUser));
			result=true;
		}
		else	result=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	return false;
}

float getBalance(int accType,int ID){
	int i=ID-1000;
	float result;
	if(accType==1){
		int i=ID-1000;
		int fd=open("NUfile",O_RDONLY,0744);
		normalUser temp;
	
		int fl1;
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normalUser);    	     //nth record
		lock.l_len=sizeof(normalUser);	             //sizeof(record)
		lock.l_pid=getpid();
	
		fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&temp,sizeof(normalUser));
		if(!strcmp(temp.status,"ACTIVE"))	result=temp.balance;
		else					result=0;

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	else if(accType==2){
		int i=ID-1000;
		int fd=open("JUfile",O_RDONLY,0744);
		jointUser temp;
	
		int fl1;
		struct flock lock;
		lock.l_type = F_RDLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    	     //nth record
		lock.l_len=sizeof(jointUser);	             //sizeof(record)
		lock.l_pid=getpid();
	
		fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&temp,sizeof(jointUser));
		if(!strcmp(temp.status,"ACTIVE"))	result=temp.balance;
		else					result=0;

		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	return 0;
}

bool alterPassword(int accType,int ID,char newPwd[10]){
	int i=ID-1000;
	if(accType==1){
		int fd=open("NUfile",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(normalUser);    //nth record
		lock.l_len=sizeof(normalUser);	             //sizeof(record)
		lock.l_pid=getpid();
	
		fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		normalUser currUser;
		lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(normalUser));
		
		if(!strcmp(currUser.status,"ACTIVE")){
			strcpy(currUser.password,newPwd);
			lseek(fd,sizeof(normalUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(normalUser));
			result=true;
		}
		else	result=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	else if(accType==2){
		int fd=open("JUfile",O_RDWR,0744);
		bool result;
		int fl1;
		struct flock lock;
		lock.l_type = F_WRLCK;
		lock.l_whence=SEEK_SET;
		lock.l_start=(i)*sizeof(jointUser);    //nth record
		lock.l_len=sizeof(jointUser);	             //sizeof(record)
		lock.l_pid=getpid();
	
		fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

		jointUser currUser;
		lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
		read(fd,&currUser,sizeof(jointUser));
		
		if(!strcmp(currUser.status,"ACTIVE")){
			strcpy(currUser.password,newPwd);
			lseek(fd,sizeof(jointUser)*(-1),SEEK_CUR);
			write(fd,&currUser,sizeof(jointUser));
			result=true;
		}
		else	result=false;
		lock.l_type=F_UNLCK;
		fcntl(fd,F_SETLK,&lock);

		close(fd);
		return result;
	}
	return false;
}

bool addNormalUser(normalUser record){
	int fd=open("NUfile",O_RDWR,0744);
	bool result;
	
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(normalUser);    //nth record
	lock.l_len=sizeof(normalUser);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	normalUser endUser;
	lseek(fd,(-1)*sizeof(normalUser),SEEK_END);  //changing the file pointer to the selected record
	read(fd,&endUser,sizeof(normalUser));
		
	record.userID=endUser.userID+1;
	record.account_no=endUser.account_no+1;
	strcpy(record.status,"ACTIVE");
	
	int j=write(fd,&record,sizeof(normalUser));
	if(j!=0)	result=true;
	else	result=false;
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool addJointUser(jointUser record){
	int fd=open("JUfile",O_RDWR,0744);
	bool result;
	
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(jointUser);    //nth record
	lock.l_len=sizeof(jointUser);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	jointUser endUser;
	lseek(fd,(-1)*sizeof(jointUser),SEEK_END);  //changing the file pointer to the selected record
	read(fd,&endUser,sizeof(jointUser));
		
	record.userID=endUser.userID+1;
	record.account_no=endUser.account_no+1;
	strcpy(record.status,"ACTIVE");
	
	int j=write(fd,&record,sizeof(jointUser));
	if(j!=0)	result=true;
	else	result=false;
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool deleteNormalUser(int ID){
	int i=ID-1000;
	int fd=open("NUfile",O_RDWR,0744);
	bool result;
	
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normalUser);    //nth record
	lock.l_len=sizeof(normalUser);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	normalUser currUser;
	lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(normalUser));
	
	if(!strcmp(currUser.status,"ACTIVE")){	
		strcpy(currUser.status,"CLOSED");
		currUser.balance=0;
		
		lseek(fd,(-1)*sizeof(normalUser),SEEK_CUR); 
		int j=write(fd,&currUser,sizeof(normalUser));
		if(j!=0)	result=true;
		else		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool deleteJointUser(int ID){
	int i=ID-1000;
	int fd=open("JUfile",O_RDWR,0744);
	bool result;
	
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    //nth record
	lock.l_len=sizeof(jointUser);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	jointUser currUser;
	lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(jointUser));
	
	if(!strcmp(currUser.status,"ACTIVE")){	
		strcpy(currUser.status,"CLOSED");
		currUser.balance=0;
		
		lseek(fd,(-1)*sizeof(jointUser),SEEK_CUR); 
		int j=write(fd,&currUser,sizeof(jointUser));
		if(j!=0)	result=true;
		else		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool modifyNormalUser(normalUser modUser){
	int i=modUser.userID-1000;
	int fd=open("NUfile",O_RDWR,0744);
	bool result=false;
	
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(normalUser);    //nth record
	lock.l_len=sizeof(normalUser);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	normalUser currUser;
	lseek(fd,(i)*sizeof(normalUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(normalUser));
	
	if(!strcmp(currUser.status,"ACTIVE") && (modUser.account_no==currUser.account_no)){	
		strcpy(modUser.status,"ACTIVE");
		lseek(fd,(-1)*sizeof(normalUser),SEEK_CUR); 
		int j=write(fd,&modUser,sizeof(normalUser));
		if(j!=0)	result=true;
		else		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool modifyJointUser(jointUser modUser){
	int i=modUser.userID-1000;
	int fd=open("JUfile",O_RDWR,0744);
	bool result=false;
	
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(jointUser);    //nth record
	lock.l_len=sizeof(jointUser);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	jointUser currUser;
	lseek(fd,(i)*sizeof(jointUser),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(jointUser));
	
	if(!strcmp(currUser.status,"ACTIVE")  && (modUser.account_no==currUser.account_no)){	
		strcpy(modUser.status,"ACTIVE");
		lseek(fd,(-1)*sizeof(jointUser),SEEK_CUR); 
		int j=write(fd,&modUser,sizeof(jointUser));
		if(j!=0)	result=true;
		else		result=false;
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}