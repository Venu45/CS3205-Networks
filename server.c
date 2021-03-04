#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

// all the global defined here should be used in func function before for loop
int flag_user ;
// holds 0 if no user , holds 1 if user hold by current_userid user 
char current_userid[MAX];

int count; // no of mails for current user

int mail_pointer ; // -1 means invalid any +ve number i  means we need to read ith mail

int num ; // to make sure temporary files have different names

FILE * user_fileptr ;

struct node * Head ;


struct node {
	char name[MAX];
	struct node * next;
};

void print_list(struct node * head){
	struct node * temp = head;
	while(temp!=NULL){
		printf("%s ",temp->name);
		temp = temp->next;
	}
	printf("\n");
}

struct node *  add(char userid[] , struct node * list){
	if(list==NULL) {
		struct node * temp = (struct node * )malloc(sizeof(struct node));
		strcpy(temp->name,userid);
		//temp->name = userid; // need to copy array
		temp->next=NULL;
		FILE * fileptr ;
		char filename[MAX] ;
		char temp1[MAX] = ".txt";
		strcpy(filename , userid);
		strcat(filename ,temp1 );
	//	printf("%s\n",filename);
		fileptr = fopen (filename, "w") ;
		if(fileptr != NULL)fclose(fileptr); 
		else printf("unable to create a file\n");
		return temp;
	}
	else { 
		if(strcmp(list->name , userid) == 0){
			// strings are identical
			printf("user %s already present\n",userid);
			return list;
		}
		list->next = add(userid,list->next);
		return list;
	}
	//list->next = add(userid,list->next);
	return list;
} 

void read_mail(){
	if(flag_user==0){
		printf("set user first\n");
		return;
	}
	else{
		// user exists
		if(mail_pointer == -1){
			// no more mails exist
			printf("no more mails\n");
			return;
		}
		else {
			int cou=0;
			char file_line[MAX];
			char end_line[MAX] = "###\n";

			fseek(user_fileptr,0,SEEK_SET);

			while(cou < (mail_pointer-1)  && fgets(file_line , MAX ,user_fileptr )!=NULL ) {
				if(strcmp(file_line , end_line) == 0 ){
						// means end of one mail
					cou++;
				}
			}
			while(fgets(file_line , MAX , user_fileptr  )!=NULL ){
				if(strcmp(file_line , end_line) == 0 ){
						// means end of one mail
					break;
				}
				printf("%s\n",file_line);
			}
			// need to set mail_pointer;
			if(count==1){
				mail_pointer=-1;
			}
			else if(mail_pointer==count){
				mail_pointer = 1;
			}
		}

	}
	return;
}

void delete_mail(){
	if(flag_user==0){
		printf("set user first\n");
		return;
	}
	else {
		// user exists
		if(mail_pointer == -1){
			// no more mails exist
			printf("no more mails\n");
			return;
		}
		else {
			char file_name_temp[MAX];
			strcpy(file_name_temp,current_userid);
			char temp1[MAX];
			sprintf(temp1,"%d",num);
			num++;
			strcat(file_name_temp,temp1);
			strcat(file_name_temp,".txt");

			FILE * f1p ;
			f1p = fopen(file_name_temp,"w");

			// f1p is new file 

			fseek(user_fileptr,0,SEEK_SET);
			int cou=0;
			char file_line[MAX];
			char end_line[MAX] = "###\n";
			while(fgets(file_line , MAX ,user_fileptr )!=NULL) {
				if(cou!=mail_pointer-1){
					fputs(file_line,f1p);
				}
				if(strcmp(file_line , end_line) == 0 ){
						// means end of one mail
						cou++;
				}
			} // f1p has now deleted the mail


			int val;
			char old_name[MAX];
			strcpy(old_name,current_userid);
			strcat(old_name,".txt");   // old_name is userid.txt

			char new_name[MAX];
			strcpy(new_name,current_userid);
			char temp2[MAX];
			sprintf(temp2,"%d",num);
			num++;
			strcat(new_name,temp2);
			strcat(new_name,".txt");  // new_name is useridnum.txt

			val = rename(old_name,new_name);
			if(val!=0) {
				printf("renaming failed\n");
			}

			val = rename(file_name_temp,old_name); // f1p now has userid.txt name

			if(val!=0){
				printf("renaming failed\n");
			}
			else{
				printf("successfully msg deleted\n");
			}

			user_fileptr = f1p; // now our user_fileptr 

			count--;

			if(count==0){
				mail_pointer = -1;
			}

			else if(mail_pointer>count){
				mail_pointer = 1;
			}


		}
	}
}

void user_function (char userid[] , struct node * head){
	struct node * temp = head;
	int flag_user_exists =0; // 1 if useid user exists
	while(temp!=NULL){
		if(strcmp(temp->name , userid) == 0){
			flag_user_exists=1;
			strcpy(current_userid , userid);
			flag_user = 1;
			char filename[MAX] ;
			char temp1[MAX] = ".txt";
			strcpy(filename , userid);
			strcat(filename ,temp1 ); // to create userid.txt string

			user_fileptr = fopen(filename, "r");

			if(user_fileptr == NULL){
				printf("unable to open file %s\n",filename);
			}
			else {
				count =0;
				char file_line[MAX];
				char end_line[MAX] = "###\n";
				while(fgets(file_line , MAX ,user_fileptr )!=NULL) {
					if(strcmp(file_line , end_line) == 0 ){
						// means end of one mail
						count++;
					}
				}
				mail_pointer = 1;
				printf("user %s exists and has %d mails\n",userid,count);

			}
			break;
		}
		temp = temp->next;
		
	}
	if(flag_user_exists==0){
		printf("user %s not found\n", userid);
	}
}



// send is incomplete

void send_to_user (char userid[MAX] ,char msg[MAX] , struct node * head){
	if(flag_user==0){
		printf("set user first\n");
		return;
	}
	int flag_user_exists =0;
	struct node * temp = head;
	while(temp!=NULL){
		if(strcmp(temp->name , userid) == 0){
			// userid exists
			flag_user_exists=1;

			FILE * fptr ;

			char filename[MAX];
			strcpy(filename,userid);
			strcat(filename,".txt");

			fptr = fopen(filename , "a"); // open userid.txt and point to last character

			if(fptr==NULL){
				printf("failed to open\n");
			}

			char temp3[MAX] = "FROM: ";
			strcat(temp3 , current_userid );
			strcat(temp3 , "\n");
			fputs(temp3 , fptr);

			char temp4[MAX] = "TO: ";
			strcat(temp4 , userid);
			strcat(temp4 , "\n");
			fputs(temp4 ,fptr );

			fputs(msg ,fptr );

			fputs("###\n",fptr); 

			printf("sucessfully sent\n");

			fclose(fptr);

			/*FILE * tempptr ;
			char filename[MAX] ;
			char temp1[MAX];
			sprintf(temp1 , "%d" , num);
			num++;
			strcpy(filename , userid);
			strcat(filename ,temp1 );
			strcat(filename,".txt");
			tempptr = fopen(filename , "w");  // filenamenum.txt
			char temp3[MAX] = "FROM: ";
			strcat(temp3 , current_userid );
			strcat(temp3 , "\n");
			fputs(temp3 , tempptr);

			char temp4[MAX] = "TO: ";
			strcat(temp4 , userid);
			strcat(temp4 , "\n");
			fputs(temp4 ,tempptr );

			fputs(msg ,tempptr );

			fputs("###\n",tempptr); */



			break;
		}
		temp = temp->next;
	}
	if(flag_user_exists==0){
		printf("user %s doesn't exist\n");
	}
}

void done_user (){
	flag_user = 0;
	mail_pointer = -1;
	count =0;
	fclose(user_fileptr);
}


/*
int add_user(struct node * head , char userid[MAX]){
	struct node * temp = head;
	while(temp)
} */

/*struct node *  add(char userid[MAX] , struct node * list){
	if(list==NULL) {
		struct node * temp = (struct node * )malloc(sizeof(struct node));
		strcpy(temp->name,userid);
		//temp->name = userid; // need to copy array
		temp->next=NULL;
		FILE * fileptr ;
		char filename[MAX] ;
		char temp[MAX] = ".txt";
		strcpy(filename , userid);
		strcat(filename ,temp );
		fileptr = fopen( filename, "w") ;
		fclose(fileptr);
		return temp;
	}
	else { 
		if(strcmp(list->name , userid) == 0){
			// strings are identical
			printf("user %s already present",userid);
		}
		list->next = add(userid,list->next);
		return list;
	}
	//list->next = add(userid,list->next);
	return list;
}  */

// Function designed for chat between client and server. 
void func(int sockfd) 
{ 
	char buff[MAX]; 
	int n; 
	// infinite loop for chat 
	for (;;) { 
		bzero(buff, MAX); 

		// read the message from client and copy it in buffer 
		read(sockfd, buff, sizeof(buff)); 
		// print buffer which contains the client contents 
		printf("From client: %s\t To client : ", buff); 
		bzero(buff, MAX); 
		n = 0; 
		// copy server message in the buffer 
		while ((buff[n++] = getchar()) != '\n') 
			; 

		// and send that buffer to client 
		write(sockfd, buff, sizeof(buff)); 

		// if msg contains "Exit" then server exit and chat ended. 
		if (strncmp("exit", buff, 4) == 0) { 
			printf("Server Exit...\n"); 
			break; 
		} 
	} 
} 

// Driver function 
int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n"); 

	// Function for chatting between client and server 
	func(connfd); 

	// After chatting close the socket 
	close(sockfd); 
} 