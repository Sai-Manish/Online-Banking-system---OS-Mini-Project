void Task(int sock_fd){
	int msgLength,select,type,option,accType,currUserID;
	bool result;
	while(1){
		read(sock_fd,&option,sizeof(option));
		printf("Option : %d\n",option);
        User currUser;
		if(option==1){
			currUser.accType=1;
			msgLength=read(sock_fd,&currUser,sizeof(User));
			printf("Username : %d\n",currUser.userID);
			printf("Password : %s\n",currUser.password);
			currUserID=currUser.userID;
			result=check_User(currUser);
			write(sock_fd,&result,sizeof(result));
		}
		else if(option==2){
			currUser.accType=2;
			msgLength=read(sock_fd,&currUser,sizeof(User));
			currUserID=currUser.userID;
			printf("Username : %d\n",currUser.userID);
			printf("Password : %s\n",currUser.password);
			result=check_User(currUser);
			write(sock_fd,&result,sizeof(result));
		}
		else if(option==3){
		
			currUser.accType=3;
			msgLength=read(sock_fd,&currUser,sizeof(User));
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
		if(option==1 || option==2){
			if(select==1){
				float amt;
				read(sock_fd,&amt,sizeof(float));
				result=depositMoney(currUser.accType,currUserID,amt);
				write(sock_fd,&result,sizeof(result));
			}
			else if(select==2){
				float amt;
				read(sock_fd,&amt,sizeof(float));
				result=withdrawMoney(currUser.accType,currUserID,amt);
				write(sock_fd,&result,sizeof(result));
			}
			else if(select==3){
				float amt;
				amt=getBalance(currUser.accType,currUserID);
				write(sock_fd,&amt,sizeof(float));
			}
			else if(select==4){
				char pwd[15];
				read(sock_fd,pwd,sizeof(pwd));
				result=alterPassword(currUser.accType,currUserID,pwd);
				write(sock_fd,&result,sizeof(result));
			}
			else if(select==5){
				if(option==1){
					User user1=getnormal_User(currUserID);
					write(sock_fd,&user1,sizeof(User));
				}
				else if(option==2){
					User user2=getjoint_User(currUserID);
					write(sock_fd,&user2,sizeof(User));
				}
			}
			else if(select==6){	
                break;
            }
		}
		else if(option==3){
			read(sock_fd,&type,sizeof(int));
			if(select==1){
				if(type==1){
					User newUser1;
					read(sock_fd,&newUser1,sizeof(User));
					result=addnormal_User(newUser1);
					write(sock_fd,&result,sizeof(result));
				}
				else if(type==2){
					User newUser2;
					read(sock_fd,&newUser2,sizeof(User));
					result=addjoint_User(newUser2);
					write(sock_fd,&result,sizeof(result));
				}
			}
			else if(select==2){
				if(type==1){
					int delUserID1;
					read(sock_fd,&delUserID1,sizeof(int));
					result=delete_User(delUserID1);
					write(sock_fd,&result,sizeof(result));
				}
				else if(type==2){
					int delUserID2;
					read(sock_fd,&delUserID2,sizeof(int));
					result=delete_User(delUserID2);
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
}


//////


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

////////// 
bool checkNormalUser(normal_User currUser){
	int i=currUser.userID-1000;
	int fd=open("NormalUsersAccounts",O_RDONLY,0744);
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
	int fd=open("JointUsersAccounts"rd,O_RDONLY,0744);
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

//////////////////////////////

bool addNormalUser(normalUser record){
	int fd=open("NormalUsersAccounts",O_RDWR,0744);
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
	int fd=open("JointUsersAccounts",O_RDWR,0744);
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

////////////

bool deleteNormalUser(int ID){
	int i=ID-1000;
	int fd=open("NormalUsersAccounts",O_RDWR,0744);
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
	int fd=open("JointUsersAccounts",O_RDWR,0744);
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


///////////
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
	int fd=open("JointUsersAccounts",O_RDWR,0744);
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

/////////////////////////////////////////////////////////////////////////

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