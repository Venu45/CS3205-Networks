#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 
void func(int sockfd) 
{ 
	char buff[MAX];
	char temp[MAX]; 
	char help[MAX];
	char user[MAX];
	int n,i; 
	int length;
	for (;;) { 
		bzero(buff, sizeof(buff)); 
		bzero(temp, sizeof(temp)); 
		bzero(help, sizeof(help)); 
		printf("Main-Prompt> "); 
		n = 0; 
		length = 0;
		while ((buff[n++] = getchar()) != '\n') ; 

		if((strncmp(buff,"L",1) ) == 0  ) {
			strcpy(temp,"LSTU");
			write(sockfd,temp,sizeof(temp));
			bzero(buff, sizeof(buff)); 
			bzero(temp, sizeof(temp)); 

			// server shoild send list of space seperated users
			read(sockfd, buff, sizeof(buff)); 
			printf("%s",buff);
		}  



		if( strncmp(buff, "A",1 ) == 0 ){
			length = strlen(buff);
			for(i=8;i<length;i++){
				temp[i-8]=buff[i];
			}
			// temp has userid
			strcpy(help,"ADDU ");
			strcat(help,temp);
			// help has addu userid
			write(sockfd,help,sizeof(help));
			bzero(buff, sizeof(buff)); 
			bzero(temp, sizeof(temp)); 
			bzero(help, sizeof(help)); 

			read(sockfd, buff, sizeof(buff));

			// server should return either user already exists or user sucessfully added

			printf("%s",buff);
		}

		if( strncmp(buff , "Set" , 3 ) == 0 ) {
			length = strlen(buff);
			for(i=8 ;i<length;i++){
				help[i-8]=buff[i];
			} // help has userid

			strcpy(temp,"USER ");
			strcat(temp,help);

			write(sockfd,temp,sizeof(temp)); // temp has user userid
			bzero(buff, sizeof(buff)); 
			strcpy(user,help);
			user[strcspn(user,"\n")]=0;

			bzero(buff, sizeof(buff)); 
			bzero(temp, sizeof(temp)); 
			bzero(help, sizeof(help)); 

			read(sockfd, buff, sizeof(buff));
			// server should send success and no of mails or error msg;
			printf("%s",buff);

			if( strncmp(buff , "success" , 7) == 0 ) {
				for (;;) { 
					bzero(buff, sizeof(buff)); 
					bzero(temp, sizeof(temp)); 
					bzero(help, sizeof(help)); 
					printf("Sub-Prompt-"); 
					printf("%s",user);
					printf("> ");
					n = 0;
					length = 0; 
					while ((buff[n++] = getchar()) != '\n') ; 

					if(strncmp(buff , "Read" , 4) == 0 ) {
						strcpy(temp , "READM");
						write(sockfd,temp,sizeof(temp));

						bzero(buff, sizeof(buff)); 
						bzero(temp, sizeof(temp)); 
						bzero(help, sizeof(help)); 

						read(sockfd, buff, sizeof(buff));
						if( strncmp(buff , "success" , 7) == 0 ) {
							for(i=0;i<6;i++){
								read(sockfd, buff, sizeof(buff));



								// send either the error msg or contents of the mail

								printf("%s",buff);

								bzero(buff, sizeof(buff)); 

							} 
						}
						else {
							printf("%s",buff);
						}

						/*for(i=0;i<6;i++){
							read(sockfd, buff, sizeof(buff));

						// send either the error msg or contents of the mail
							printf("%s",buff);
							bzero(buff, sizeof(buff)); 
						} */


						/* read(sockfd, buff, sizeof(buff));

						// send either the error msg or contents of the mail
						printf("%s",buff);	 */				
					}

					if(strncmp(buff , "Del" , 3) == 0 ) {
						strcpy(temp,"DELM");

						write(sockfd,temp,sizeof(temp));

						bzero(buff, sizeof(buff)); 
						bzero(temp, sizeof(temp)); 
						bzero(help, sizeof(help)); 

						read(sockfd, buff, sizeof(buff));

						// send either the error msg or mail deleted succesfully
						printf("%s",buff);

					}

					if(strncmp(buff , "Sen" , 3) == 0 ) {
						strcpy(temp , "SEND ");
						length = strlen(buff) ;

						for(i=5;i<length;i++){
							help[i-5] = buff[i];
						}

						strcat(temp,help);

						write(sockfd,temp,sizeof(temp));  // send userid is sent ot server

						bzero(buff, sizeof(buff)); 
						bzero(temp, sizeof(temp)); 
						bzero(help, sizeof(help)); 

						printf("Enter Subject \n");

						n=0;

						while ((temp[n++] = getchar()) != '\n') ;
						
						write(sockfd,temp,sizeof(temp));

						printf("Type Message \n");

						n=0;

						while ((buff[n++] = getchar()) != '\n') ; 

						write(sockfd,buff,sizeof(buff)); // mail content is sent to server

						bzero(buff, sizeof(buff)); 

						read(sockfd, buff, sizeof(buff));


						printf("%s",buff);
					}

					
					if(strncmp(buff , "Don" , 3) == 0 ) {
						strcpy(temp , "DONEU");

						write(sockfd,temp,sizeof(temp));

						bzero(buff, sizeof(buff)); 
						bzero(temp, sizeof(temp)); 
						bzero(help, sizeof(help)); 

						break;

					//	read(sockfd, buff, sizeof(buff));

					}
				}
			}

		}



	/*	write(sockfd, buff, sizeof(buff)); 
		bzero(buff, sizeof(buff)); 
		read(sockfd, buff, sizeof(buff)); 
		printf("From Server : %s", buff);  */
		if ((strncmp(buff, "Quit", 4)) == 0) { 
			strcpy(temp , "QUIT");
			write(sockfd,temp,sizeof(temp));
			printf("Client Exit...\n"); 
			break; 
		} 
	} 
} 

int main() 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

	// socket create and varification 
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
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT); 

	// connect the client socket to server socket 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n"); 

	// function for chat 
	func(sockfd); 

	// close the socket 
	close(sockfd); 
} 
