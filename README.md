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
* The application possess password-protected administrative access; thus
preventing the whole management system from unauthorized access.
* Three types of login are created: Normal User, Joint Account User, Administrator.
* Once you login as administrator, you should be able to add, delete, modify, search for a
specific account.
* Once the customer connects to the server, you get a login and password prompt.
After successful login, if you are Normal or Joint user you will get a menu for example:
Do you want to:
	* Deposit
	* Withdraw
	* Balance Enquiry
	* Password Change
	* View details
	* Exit
* If you login as a joint account holder, proper file locking is implemented. If you
want to view the account details then read lock is set else if you want to withdraw
or deposit then write lock is used to protect the critical data section.
* Socket programming is used – Server maintains the data base and service multiple clients
concurrently. Client program connects to the server and access their specific account
details.
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

3. Run the client, After connecting to server it asks for User login  
```
$ gcc client.c -o client  
$ ./client
```

