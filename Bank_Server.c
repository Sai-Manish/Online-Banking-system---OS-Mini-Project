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
#include "headers/User.h"

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

   
    printf("Server ready to accept connections on port %d...\n", SERVER_PORT);
  

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
			User currUser;
			accType=1;
			length_msg=read(sock_fd,&currUser,sizeof(User));
			printf("Username : %d\n",currUser.userID);
			printf("Password : %s\n",currUser.password);
			currUserID=currUser.userID;
			result=checkUser(currUser,accType);
			write(sock_fd,&result,sizeof(result));
		}
		else if(User_Option==2){
			User currUser;
			accType=2;
			length_msg=read(sock_fd,&currUser,sizeof(User));
			currUserID=currUser.userID;
			printf("Username : %d\n",currUser.userID);
			printf("Password : %s\n",currUser.password);
			result=checkUser(currUser,accType);
			write(sock_fd,&result,sizeof(result));
		}
		else if(User_Option==3){
			User currUser;
			accType=3;
			length_msg=read(sock_fd,&currUser,sizeof(User));
			currUserID=currUser.userID;
			printf("Username : %d\n",currUser.userID);
			printf("Password : %s\n",currUser.password);
			result=checkUser(currUser,accType);
			write(sock_fd,&result,sizeof(result));
		}
		else{
			result=false;
			write(sock_fd,&result,sizeof(result));
		}
		if(result){
            	break;	
        }	
	}

	while(1){
		read(sock_fd,&select,sizeof(int));
		if(User_Option==1 || User_Option==2){
			if(select==1){
				float amount;
				read(sock_fd,&amount,sizeof(float));
				result=depositMoney(accType,currUserID,amount);
				write(sock_fd,&result,sizeof(result));
			}
			else if(select==2){
				float amount;
				read(sock_fd,&amount,sizeof(float));
				result=withdrawMoney(accType,currUserID,amount);
				write(sock_fd,&result,sizeof(result));
			}
			else if(select==3){
				float amount;
				amount=getBalance(accType,currUserID);
				write(sock_fd,&amount,sizeof(float));
			}
			else if(select==4){
				char password[15];
				read(sock_fd,password,sizeof(password));
				result=alterPassword(accType,currUserID,password);
				write(sock_fd,&result,sizeof(result));
			}
			else if(select==5){
				if(User_Option==1){
					User user1=getUser(currUserID,accType);
					write(sock_fd,&user1,sizeof(User));
				}
				else if(User_Option==2){
					User user2=getUser(currUserID,accType);
					write(sock_fd,&user2,sizeof(User));
				}
			}
			else if(select==6){
                break;
            }
		}
		else if(User_Option==3){
			read(sock_fd,&type,sizeof(int));
			if(select==1){
				if(type==1){
					User newUser1;
                    int newaccType = 1
					read(sock_fd,&newUser1,sizeof(User));
					result=addUser(newUser1,newaccType);
					write(sock_fd,&result,sizeof(result));
				}
				else if(type==2){
					User newUser2;
                    int newaccType = 2
					read(sock_fd,&newUser2,sizeof(User));
					result=addUser(newUser2,newaccType);
					write(sock_fd,&result,sizeof(result));
				}
			}
			else if(select==2){
				if(type==1){
					int delUserID1;
                    int newaccType = 1
					read(sock_fd,&delUserID1,sizeof(int));
					result=deletenormal_User(delUserID1);
					write(sock_fd,&result,sizeof(result));
				}
				else if(type==2){
					int delUserID2;
                    int newaccType = 2
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


User getUser(int ID,int accType)
{
    if(accType == 1){
        int i = ID - 1000;
	    int fd=open("NormalUsersAccounts",O_RDONLY,0744);
    }

    else if(accType == 2){
        int i = ID - 2000;
	    int fd=open("JointUsersAccounts",O_RDONLY,0744);
    }


    else if(accType == 3){
        int i = ID - 3000;
	    int fd=open("AdministratorFile",O_RDONLY,0744);
    }
    
    User currUser;

	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);    	     //nth record
	lock.l_len=sizeof(User);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(User),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(currUser));

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return currUser;

    }
}

bool checkUser(User currUser, accType){

    if(accType == 1){
        int i=currUser.userID-1000;
	    int fd=open("NormalUsersAccounts",O_RDONLY,0744);
    }
    else if(accType== 2){
        int i = ID - 2000;
	    int fd=open("JointUsersAccounts",O_RDONLY,0744);
    }
    else if(accType == 3){
        int i = ID - 3000;
	    int fd=open("AdministratorFile",O_RDONLY,0744);
    }
	
	bool result;
	User temp;
	
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);    	     //nth record
	lock.l_len=sizeof(User);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	lseek(fd,(i)*sizeof(User),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(User));
    if(accType == 1 || accType == 2){
	    if(!strcmp(temp.password,currUser.password) && !strcmp(temp.status,"ACTIVE")){	
            result=true;
        }
	    else{
				result=false;
        }
    }

    else if(accType == 3){
        if(!strcmp(temp.password,currUser.password)){	
            result=true;
        }
	    else{		
            result=false;
        }
    }

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool depositMoney(int accType,int ID,float amount){
	
	if(accType==1){
		int i=ID-1000;
		int fd=open("NormalUsersAccounts",O_RDWR,0744);
		bool result = false;
	}
	else if(accType==2){
		int i=ID-2000;
		int fd=open("JointUsersAccounts",O_RDWR,0744);
		bool result = false;
	}
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);    //nth record
	lock.l_len=sizeof(User);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

	User currUser;
	lseek(fd,(i)*sizeof(User),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(User));
		
	if(!strcmp(currUser.status,"ACTIVE")){
		currUser.balance+=amount;
		lseek(fd,(-1)*sizeof(User),SEEK_CUR);
		write(fd,&currUser,sizeof(User));
		result=true;
	}
	else{
			result=false;
	}
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;		
	
}

bool withdrawMoney(int accType,int ID,float amount){

	if(accType==1){
		int i=ID-1000;
		int fd=open("NormalUsersAccounts",O_RDWR,0744);
		bool result = false;
	}
	else if(accType==2){
		int i=ID-2000;
		int fd=open("JointUsersAccounts",O_RDWR,0744);
		bool result = false;
	}
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);    //nth record
	lock.l_len=sizeof(User);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

	User currUser;
	lseek(fd,(i)*sizeof(User),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(User));
		
	if(!strcmp(currUser.status,"ACTIVE") && currUser.balance>=amount){
		currUser.balance-=amount;
		lseek(fd,sizeof(User)*(-1),SEEK_CUR);
		write(fd,&currUser,sizeof(User));
		result=true;
	}

	else{	
		result=false;
	}
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;	
	
}

float getBalance(int accType,int ID){
	float result = 0;
	if(accType==1){
		int i=ID-1000;
		int fd=open("NormalUsersAccounts",O_RDONLY,0744);
		User temp;
	}
	else if(accType==2){
		int i=ID-2000;
		int fd=open("JointUsersAccounts",O_RDONLY,0744);
		User temp;
	}
	int fl1;
	struct flock lock;
	lock.l_type = F_RDLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);    	     //nth record
	lock.l_len=sizeof(User);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();

	lseek(fd,(i)*sizeof(User),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&temp,sizeof(User));
	if(!strcmp(temp.status,"ACTIVE")){
		result=temp.balance;
	}
	else{					
		result=0;
	}

	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool alterPassword(int accType,int ID,char newpassword[10]){
	
	if(accType==1){
		int i=ID-1000;
		int fd=open("NormalUsersAccounts",O_RDWR,0744);
		bool result;
	}
	else if(accType==2){
		int i=ID-2000;
		int fd=open("JointUsersAccounts",O_RDWR,0744);
		bool result;
	}
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);    //nth record
	lock.l_len=sizeof(User);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
		//getchar();
	User currUser;
	lseek(fd,(i)*sizeof(User),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(User));
		
	if(!strcmp(currUser.status,"ACTIVE")){
		strcpy(currUser.password,newpassword);
		lseek(fd,sizeof(normalUser)*(-1),SEEK_CUR);
		write(fd,&currUser,sizeof(User));
		result=true;
	}
	else{
		result=false;
	}
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);

	close(fd);
	return result;
}

bool addUser(User record,int accType){
    
    if(accType == 1){
        int fd=open("NormalUsersAccounts",O_RDWR,0744);
    }
    else if(accType == 2){
        int fd=open("JointUsersAccounts",O_RDWR,0744);
    }
    bool result;
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_END;
	lock.l_start=(-1)*sizeof(User);    //nth record
	lock.l_len=sizeof(User);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	User endUser;
	lseek(fd,(-1)*sizeof(User),SEEK_END);  //changing the file pointer to the selected record
	read(fd,&endUser,sizeof(User));
		
	record.userID=endUser.userID+1;
	record.account_no=endUser.account_no+1;
	strcpy(record.status,"ACTIVE");
	
	int j=write(fd,&record,sizeof(User));
	if(j!=0){	
        result=true;
    }
	else{	
        result=false;
    }
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}

bool deleteUser(int ID,int accType){
    if(accType == 1){
        int i=ID-1000;
	    int fd=open("NormalUsersAccounts",O_RDWR,0744);
	    bool result;
    }

    else if(accType == 2){
        int i=ID-2000;
	    int fd=open("JointUsersAccounts",O_RDWR,0744);
	    bool result;
    }
	
	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);    //nth record
	lock.l_len=sizeof(User);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	User currUser;
	lseek(fd,(i)*sizeof(User),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(User));
	
	if(!strcmp(currUser.status,"ACTIVE")){	
		strcpy(currUser.status,"CLOSED");
		currUser.balance=0;
		
		lseek(fd,(-1)*sizeof(User),SEEK_CUR); 
		int j=write(fd,&currUser,sizeof(User));
		if(j!=0){	
            result=true;
        }
		else{		
            result=false;
        }
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;
}

bool modifyUser(User modUser,int accType){
	
    if(accType == 1){
        int i=modUser.userID-1000;
	    int fd=open("NormalUsersAccounts",O_RDWR,0744);
	    bool result=false;
    }

    else if(accType == 2){
        int i=modUser.userID-2000;
	    int fd=open("JointUsersAccounts",O_RDWR,0744);
	    bool result=false;
    }

	int fl1;
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence=SEEK_SET;
	lock.l_start=(i)*sizeof(User);    //nth record
	lock.l_len=sizeof(User);	             //sizeof(record)
	lock.l_pid=getpid();
	
	fl1=fcntl(fd,F_SETLKW,&lock);	//lock the selected record
	//getchar();

	User currUser;
	lseek(fd,(i)*sizeof(User),SEEK_SET);  //changing the file pointer to the selected record
	read(fd,&currUser,sizeof(normalUser));
	
	if(!strcmp(currUser.status,"ACTIVE") && (modUser.account_no==currUser.account_no)){	
		strcpy(modUser.status,"ACTIVE");
		lseek(fd,(-1)*sizeof(normalUser),SEEK_CUR); 
		int j=write(fd,&modUser,sizeof(normalUser));
		if(j!=0){	
            result=true;
        }
		else {	
            result=false;
        }
	}
	
	lock.l_type=F_UNLCK;
	fcntl(fd,F_SETLK,&lock);
	
	close(fd);
	return result;	
}
