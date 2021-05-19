# Mini-Project

## Design and Development of online banking management system

### Name : Sai Manish Sasanpuri
### Registration Number :  IMT2018520

___
## Description

The project aims to develop a banking system that is user-friendly and multi-
functional. The project should have the following functionalities:   
* All account transactional details and customer information are stored in files.
* Account holders have to pass through a login system to enter their accounts.
* The application should possess password-protected administrative access; thus
preventing the whole management system from unauthorized access.
* Three types of login should be created: normal user, joint account user; administrator;
* Once you login as administrator, you should be able to add, delete, modify, search for a
specific account details.
* Once the customer connect to the server, you should get login and password prompt.
After successful login, you should get menu for example:
Do you want to:
	* Deposit
	* Withdraw
	* Balance Enquiry
	* Password Change
	* View details
	* Exit
* If you login as a joint account holder, proper file locking should be implemented. If you
want to view the account details then read lock is to be set else if you want to withdraw
or deposit then write lock should be used to protect the critical data section.
* Use socket programming – Server maintains the data base and service multiple clients
concurrently. Client program can connect to the server and access their specific account
details.
* Use system calls instead of Library functions wherever it is possible in the project:
Process Management, File Management, File Locking, Multithreading and Inter Process
Communication Mechanisms.

___
## How to run the system

1. Firstly run the Create_accounts.c and fill in the sign up details  
```
$ gcc Create_accounts.c -o Create_accounts  
$ ./Create_accounts
```

2. Run the Bank Server, it waits for clients
```
$ gcc -pthread Bank_server.c -o Bank_server  
$ ./Bank_server
```

3. Run the client, it gives which user mode wants to login  
```
$ gcc client.c -o client  
$ ./client
```

