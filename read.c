#include <stdio.h> 
//#include <netdb.h> 
//#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
//#include <sys/socket.h> 
//#include <sys/types.h> 
#define MAX 80 
//#define PORT 8080 
//#define SA struct sockaddr 

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

/*
int add_user(struct node * head , char userid[MAX]){
	struct node * temp = head;
	while(temp)
} */

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
		printf("%s\n",filename);
		fileptr = fopen (filename, "w++") ;
		if(fileptr != NULL)fclose(fileptr); 
		else printf("1\n");
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
                    printf("45\n");
					if(strcmp(file_line , end_line) == 0 ){
						// means end of one mail
						count++;
					}
				}
                printf("%d\n",count);
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
            if(count ==0){
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
			while(fgets(file_line , MAX , user_fileptr  )!=NULL ){
                printf("%s\n",file_line);
				if(strcmp(file_line , end_line) == 0 ){
						// means end of one mail
					break;
				}
				
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

void send_to_user (char userid[MAX] ,char msg[MAX] , struct node * head){
	if(flag_user==0){
		printf("set user first\n");
		return;
	}
	int flag_user_exists =0;
	struct node * temp = head;
	while(temp!=NULL){
		if(strcmp(temp->name , userid) == 0){
            printf("in\n");
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
	if(flag_user_exists == 0){
		printf("user %s doesn't exist\n",userid);
        return;
	}
    return;
}

void done_user (){
	flag_user = 0;
	mail_pointer = -1;
	count =0;
    fclose(user_fileptr);
}


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
  FILE * fp3 ;
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
				
	} */
  fclose(fp3);

   // read_mail();
    user_function(userid , head);
    read_mail();
    return 0;
    
}



