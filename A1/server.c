#include <assert.h>
#include <time.h>
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

struct node * head ;

char bat[MAX] ;
char help[MAX];
char cap[MAX];
char subject[MAX];


struct node {
	char name[MAX];
	struct node * next;
};

void print_list(int sockfd){
	struct node * temp = head;
	bzero(help,MAX);
	while(temp!=NULL){
		strcat(help,temp->name);
		//strcat(help," ");
		printf("%s ",temp->name);
		temp = temp->next;
	}
	//strcat(help,"\n");
	write(sockfd, help, sizeof(help));
	printf("\n");
}

/*
int add_user(struct node * head , char userid[MAX]){
	struct node * temp = head;
	while(temp)
} */

struct node *  add(char userid[] , struct node * list , int sockfd){
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
		printf("%s\n",filename);
		fileptr = fopen (filename, "w++") ;
		if(fileptr != NULL)fclose(fileptr); 
		else printf("1\n");
		strcpy(help,"user successfully added\n");
		write(sockfd, help, sizeof(help));
		return temp;
	}
	else { 
		if(strcmp(list->name , userid) == 0){
			// strings are identical
			strcpy(help,"user ");
			strcat(help,userid);
			strcat(help," already present\n");
			write(sockfd, help, sizeof(help));
			printf("user %s already present\n",userid);
			return list;
		}
		list->next = add(userid,list->next,sockfd);
		return list;
	}
	//list->next = add(userid,list->next);
	return list;
} 


void user_function (char userid[] , int sockfd ){
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
			strcat(filename ,".txt" ); // to create userid.txt string

			user_fileptr = fopen(filename, "r");

            		fseek(user_fileptr,0,SEEK_SET);

			if(user_fileptr == NULL){
				printf("unable to open file %s\n",filename);
			}
			else {
				count =0;
				char file_line[MAX];
				char end_line[MAX] = "###\n";
				while(fgets(file_line , MAX ,user_fileptr )!=NULL) {
                   			// printf("45\n");
					if(strcmp(file_line , end_line) == 0 ){
						// means end of one mail
						count++;
					}
				}
             			 //  printf("%d\n",count);
				mail_pointer = 1;
				strcpy(help,"success user ");
				strcat(help,userid);
				strcat(help," exists and has ");
				char lov[MAX];
				sprintf(lov,"%d",count);
				strcat(help,lov);
				strcat(help," mails\n");

				// help has msg success user userid exists and has count mails\n

				write(sockfd, help, sizeof(help));
				printf("user %s exists and has %d mails\n",userid,count);

			}
			break;
		}
        	temp = temp->next;
		
	}
    	if(flag_user_exists==0){
			strcpy(help , "user ");
			strcat(help,userid);
			strcat(help," not found\n");
			write(sockfd, help, sizeof(help));
			printf("user %s not found\n", userid);
	}
}

void read_mail(int sockfd){
	printf("r  %d\n",mail_pointer);
	if(flag_user==0){
		strcpy(help,"set user first\n");
		write(sockfd, help, sizeof(help));
		printf("set user first\n");
		return;
	}
	else{
		// user exists
		if(mail_pointer == -1){
			// no more mails exist
			strcpy(help,"no more mails\n");
			write(sockfd, help, sizeof(help));
			printf("no more mails\n");
			return;
		}
		else {
            		if(count ==0){
			strcpy(help,"no more mails\n");
			write(sockfd, help, sizeof(help));
                	printf("No mails\n");
                	return;
            	}
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
			int lab = 0;
			char lef[MAX] = "success mail exists\n";
			write(sockfd,lef,sizeof(lef));
			while(fgets(file_line , MAX , user_fileptr  )!=NULL ){
				lab++;
                		printf("%s",file_line);
				write(sockfd,file_line,sizeof(file_line));
			/*	if(lab==3){
					write(sockfd,file_line,sizeof(file_line));
				} */
				if(strcmp(file_line , end_line) == 0 ){
						// means end of one mail
					break;
				}
				
			}
			// need to set mail_pointer;
			if(count==0){
				mail_pointer=-1;
			}
			else if(mail_pointer==count){
				mail_pointer = 1;
			}
			else{
				mail_pointer++;
			}
		}

	}
	return;
}

void send_to_user (char userid[MAX] ,char msg[MAX] , int sockfd , char subj[MAX] ){
	if(flag_user==0){
		strcpy(cap,"set user first\n");
		write(sockfd, cap, sizeof(cap));
		printf("set user first\n");
		return;
	}
	int flag_user_exists =0;
	struct node * temp = head;
	while(temp!=NULL){
		if(strcmp(temp->name , userid) == 0){
			strcpy(cap ,"msg sent successfully\n");
			write(sockfd, cap, sizeof(cap));
           		// printf("in\n");
			// userid exists
			flag_user_exists=1;

			FILE * fptr ;

			char filename[MAX];
			strcpy(filename,userid);
			strcat(filename,".txt");

			fptr = fopen(filename , "a"); // open userid.txt and point to last character

            		if(fptr==NULL){
				printf("failed to open\n");
               			return;
			}


			char temp3[MAX] = "FROM: ";
			strcat(temp3 , current_userid );
			//strcat(temp3 , "\n");
			fputs(temp3 , fptr);

			char temp4[MAX] = "TO: ";
			strcat(temp4 , userid);
			//strcat(temp4 , );
			fputs(temp4 ,fptr );

			time_t t = time(NULL);
    			struct tm *tm = localtime(&t);
    			char s[MAX];
    			assert(strftime(s, sizeof(s), "%c", tm));
    			printf("%s\n", s);
			char temp6[MAX] = "DATE : ";
			strcat(temp6,s);
			strcat(temp6 , "IST\n");
			fputs(temp6,fptr);

			char temp5[MAX] = "SUBJECT : ";

			strcat(temp5,subj);

			fputs(temp5 , fptr);

			fputs(msg ,fptr );

			fputs("###\n",fptr); 

		//	strcpy(cap ,"msg sent successfully\n");
		//	write(sockfd, cap, sizeof(cap));

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
	if(flag_user_exists == 0){
		strcpy(cap , "user ");
		strcat(cap ,userid);
		strcat(cap," not found\n");
		write(sockfd, cap, sizeof(cap));
		printf("user %s doesn't exist\n",userid);
        return;
	}
    return;
}

void done_user (int sockfd){
	flag_user = 0;
	mail_pointer = -1;
	count =0;
    fclose(user_fileptr);
}

void delete_mail(int sockfd){
	printf("d %d\n",mail_pointer);
	if(flag_user==0){
		strcpy(help,"set user first\n");
		write(sockfd, help, sizeof(help));
		printf("set user first\n");
		return;
	}
	else {
		// user exists
		if(mail_pointer == -1){
			// no more mails exist
			strcpy(help,"no more mails\n");
			write(sockfd, help, sizeof(help));
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
					printf("%s\n",file_line);
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

			printf("%s %s %s ",old_name,file_name_temp,new_name);

			if(val!=0){
				printf("renaming failed\n");
			}
			else{
				printf("successfully msg deleted\n");
			}

			strcpy(help,"msg deleted successfully\n");
			write(sockfd, help, sizeof(help));

			//user_fileptr = f1p; // now our user_fileptr 

			fclose(f1p);

			user_fileptr = fopen(old_name,"r");



			count--;
			//mail_pointer++;

			if(count==0){
				mail_pointer = -1;
			}

			else if(mail_pointer>count){
				mail_pointer = 1;
			}


		}
	}
}






// Function designed for chat between client and server. 
void func(int sockfd) 
{ 
	char buff[MAX]; 
	int n,length,i; 
	// infinite loop for chat 
	for (;;) { 
		n=0;length=0;i=0;
		bzero(buff, MAX); 
		bzero(bat, MAX); 
		bzero(help, MAX); 
		bzero(cap, MAX); 
		bzero(subject, MAX); 
		// read the message from client and copy it in buffer 
		read(sockfd, buff, sizeof(buff)); 
		// print buffer which contains the client contents 


		//printf("From client: %s\t To client : ", buff); 

		if(strncmp(buff , "LSTU" , 4) == 0 ) {
			print_list(sockfd);
		}	

		if(strncmp(buff , "ADDU" , 4) == 0 )	{
			length = strlen(buff);
			for(i=5;i<length;i++){
				bat[i-5]=buff[i];
			}  // bat has userid
			head = add(bat,head,sockfd);
		}

		if(strncmp(buff , "USER" , 4) == 0 )  {
			length = strlen(buff);
			for(i=5;i<length;i++){
				bat[i-5]=buff[i];
			}  // bat has userid
			user_function(bat,sockfd);
		}

		if(strncmp(buff , "READ" , 4) == 0 ) {
			read_mail(sockfd);
		}

		if(strncmp(buff , "DELM" , 4) == 0 ) {
			delete_mail(sockfd);
		}

		if(strncmp(buff , "SEND" , 4) == 0 )  {
			length = strlen(buff);
			read(sockfd, subject, sizeof(subject)); 
			read(sockfd, help, sizeof(help)); 
			//read(sockfd, subject, sizeof(subject)); 
			for(i=5;i<length;i++){
				bat[i-5]=buff[i];
			}  // bat has userid
			send_to_user(bat,help,sockfd,subject);
		}

		if(strncmp(buff , "DONE" , 4) == 0 ) {
			done_user(sockfd);
		}

		/*bzero(buff, MAX); 
		n = 0; 
		// copy server message in the buffer 
		while ((buff[n++] = getchar()) != '\n') 
			; 

		// and send that buffer to client 
		write(sockfd, buff, sizeof(buff));  */

		// if msg contains "Exit" then server exit and chat ended. 
		if (strncmp("QUIT", buff, 4) == 0) { 
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

	flag_user = 0;
	count = 0;
	head = NULL;
	num = 1;
	mail_pointer = -1;
	user_fileptr = NULL;



	// Function for chatting between client and server 
	func(connfd); 

	// After chatting close the socket 
	close(sockfd); 
} 

/*
int main(){
    struct node * head = NULL;
    char userid[MAX] = "venu";
    char userid2[MAX] = "tony";
    char userid3[MAX] = "batman";
    char msg[MAX] = "hello world\n";
   // print_list(head);
   // printf("%s\n ",userid);
    head = add(userid,head);
    head = add(userid,head);
    head = add(userid2,head);
    head = add(userid3,head);
    print_list(head);
    user_function(userid3 , head);
  send_to_user(userid,msg,head);
  send_to_user(userid,msg,head);
  done_user();
  /*FILE * fp3 ;
  fp3 = fopen("venu.txt", "r") ; 
	
  char file_line[MAX];
  char end_line[MAX] = "###\n";
  if(fp3!=NULL){
      printf("36\n");
  }

  while( fgets ( file_line, 80, fp3 ) != NULL ) 
		{ 
		
			// Print the dataToBeRead 
			printf( "%s" , file_line ) ; 
		} 

 /* while(fgets(file_line , MAX , fp3  )!=NULL ){
                printf("%s\n",file_line);
				if(strcmp(file_line , end_line) == 0 ){
						// means end of one mail
					break;
				}
				
	} 
  fclose(fp3); 

   // read_mail();
    user_function(userid , head);
    read_mail();
	delete_mail();
    return 0;
    
} */



