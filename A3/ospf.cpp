#include <pthread.h>
#include <bits/stdc++.h>
#include <stdio.h>
#include <fstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#define f first
#define s second
using namespace std;

#define PORT 22000
#define MAXLINE 1024
#define infinite 100000

pthread_t  x_func , y_func , z_func , rec;

pthread_mutex_t lock1;

int id ;
char * input ;
char * output ;
int x , y , z ;
FILE *ifp ;
ofstream fout;
int n , m;
int neighbourno; // no of neighbours of id

vector<int> neigh; // list of neighbours of id

vector< vector<pair <int,int> > > ingraph; // stores input min and max

vector< vector <int> > graph ; // stores whole edges cost 

vector< vector <int> > temgraph ; // srcid node stores its neighbour costs here

vector<int> seqno; // stores sequence no of every node

vector <int> tem;

pair <int , int> pair1;

vector < pair <int , int > > sets;

int sockfd;
struct sockaddr_in servaddr;
/*
string to_string(int n)
    {
  	string s;
	if(n==0)return "0";
  	stack<char> S;
        while(n>0)
        {
            S.push('0'+(n%10));
            n = (n/10);
        }
        while(!S.empty())
        {
               s.push_back(S.top()); S.pop();
        }
        return s;
     }
 */

string convertToString(char* a, int size)
{
    string s = a;
    return s;
}

void * send_hello(void * arg ){
   // string msg;
	//cout<<"in send hello"<<endl;
    int flag = 1;
    while(1){
	//cout<<"in send hello while "<<endl;
        sleep(x);
	for(int kk=0;kk<neigh.size();kk++){
		string msg = "HELLO:";
                msg = msg + to_string(id);
		//cout<<msg<<endl;
                struct sockaddr_in  cliaddr;

                cliaddr.sin_family = AF_INET; // IPv4
	        cliaddr.sin_addr.s_addr = INADDR_ANY;
	        cliaddr.sin_port = htons(PORT+neigh[kk]);

                socklen_t len = sizeof(cliaddr);

                sendto(sockfd, msg.c_str(), msg.size(),MSG_CONFIRM, (const struct sockaddr*) &cliaddr,len);	
	}
      /*  for(int ii=id+1;ii<n;ii++){
            if(temgraph[id][ii] != -1){
                // means there is an edge
                string msg = "HELLO:";
                msg = msg + to_string(id);
		//cout<<msg<<endl;
                struct sockaddr_in  cliaddr;

                cliaddr.sin_family = AF_INET; // IPv4
	        cliaddr.sin_addr.s_addr = INADDR_ANY;
	        cliaddr.sin_port = htons(PORT+ii);

                socklen_t len = sizeof(cliaddr);

                sendto(sockfd, msg.c_str(), msg.size(),MSG_CONFIRM, (const struct sockaddr*) &cliaddr,len);
            }
        } */
    }
}

void * send_LSA(void * arg){

   // cout<<"in send lsa"<<endl;
    int flag = 1;
    while (1){
        sleep(y);
        seqno[id]++;
        string lsa = "LSA:" + to_string(id) + ":" + to_string(seqno[id]) + ":" + to_string(neighbourno) + ":" ;
	pthread_mutex_lock(&lock1);
        for(int jj = 0; jj<neigh.size() ; jj++){
            lsa = lsa + to_string(neigh[jj]) + ":" + to_string(temgraph[id][neigh[jj]]) + ":" ;
        }
	pthread_mutex_unlock(&lock1);
        for(int kk=0;kk<neigh.size();kk++){
            struct sockaddr_in  cliaddr;

            cliaddr.sin_family = AF_INET; // IPv4
	    cliaddr.sin_addr.s_addr = INADDR_ANY;
	    cliaddr.sin_port = htons(PORT+neigh[kk]);

            socklen_t len = sizeof(cliaddr);

            sendto(sockfd, lsa.c_str(), lsa.size(),MSG_CONFIRM, (const struct sockaddr*) &cliaddr,len);
        }
    }
}

void * do_dix(void * arg){
    int flag = 1;
    int count = 0;
    vector<int> set;
    vector<int > dist;
    vector <int> prev;
    for(int i=0;i<n;i++){
        set.push_back(0);
        dist.push_back(infinite);
        prev.push_back(0);
    }
    while(1){
        sleep(z);
        count ++;
	pthread_mutex_lock(&lock1);
        for(int ii=0;ii<n;ii++){
            for(int jj=0;jj<n;jj++){
                graph[ii][jj] = temgraph[ii][jj];
		//cout<<graph[ii][jj]<<" " ;
		//fout<<graph[ii][jj]<<" " ;
            }
	 //   cout<<endl;
	   // fout<<endl;
        }
	pthread_mutex_unlock(&lock1);
        for(int i=0;i<n;i++){
            set[i] = 0;
            dist[i]=infinite;
            prev[i] = 0;
        }
        //void dij ( vector< vector<int> > graph , int N , vector<int > set , vector<int> dist , int srcid , vector<int> prev ){
        set[id]=1;
        dist[id] = 0;
        prev[id] = id;
        // path[srcid].push_back(srcid);
        for(int ii=0;ii<n;ii++){
            if(graph[id][ii] != -1 ){
                dist[ii] = graph[id][ii];
                prev[ii] = id;
                // path[ii][0] = srcid;
            }
        }
        int minid = -1;
        int min = infinite;
        for(int ii=1;ii<n;ii++){
            minid = -1;
            min = infinite;

            // getting the not set  min veretx 
            for(int jj=0;jj<n;jj++){
                if(set[jj]!=1){
                    // vertex is not considered for min dist yet
                    if(min>dist[jj]){
                        min = dist[jj];
                        minid = jj;
                    }
                }
            }

        

            set[minid] = 1;

            // updating its neighbours
            for(int kk=0;kk<n;kk++){
                if(graph[minid][kk]!= -1 ){
                    // edge exist
                    if(dist[kk] > (graph[minid][kk] + min )  ){
                        dist[kk] = graph[minid][kk] + min ;
                        prev[kk] = minid;
                    }
                }
            }


        }
	fout<<"==========================================\n";
	fout<<"Routing Table for Node no. ";
	fout<<id<<" at time "<<z*count<<endl;
	fout<<"------------------------------------------\n";
	fout<<"|  Destination   |   Path   |   Cost      |\n";
	fout<<"------------------------------------------\n";
	//for(int ii=0;ii<n;ii++){
		//fout<<dist[ii]<<" ";
	//}
	//fout<<endl;

        for(int ii=0;ii<n;ii++){

            //cout<<dist[ii]<<" "; 
           
            if(ii != id){
                vector<int> pat;
                int temp = ii;
                //  pat.push_back(ii);
                pat.push_back(temp);
                while(temp != id){
                    temp = prev[temp];
                    pat.push_back(temp);
                }

                fout<<"|       "<<ii<<"        |     ";
                for(int jj = pat.size()-1; jj>=0;jj--){
                    fout<<pat[jj];
                }
                fout<<"   |     "<<dist[ii]<<"     |"<<endl;

            }
        }
        fout<<endl;
    }

}

void * rec_func(void * arg){
	//cout<<"in reciever"<<endl;
    int flag = 1;
    int srcid,cost;
    while (1){
	//cout<<"in while of reciever"<<endl;
        struct sockaddr_in cliadd;
        socklen_t le;
        char buffer[MAXLINE];
        int n= recvfrom(sockfd, buffer, MAXLINE,MSG_WAITALL, ( struct sockaddr*) &cliadd, &le);
        buffer[n]='\0';

        int buff_size = sizeof(buffer) / sizeof(char);
  
        string s_buff = convertToString(buffer, buff_size);
	//cout<<s_buff<<endl;
	//fout<<s_buff<<endl;
        int pos = s_buff.find(":");

        if(pos == 5){
            // we recieved hello srcid msg
            string st = s_buff.substr(pos + 1);
            sscanf(st.c_str() , "%d" , &srcid );
            // srcid has the id
            cost = ( rand() % (ingraph[id][srcid].s - ingraph[id][srcid].f + 1 ) ) + ingraph[id][srcid].f ;

            // need to send hello reply with cost

            string msg = "HELLOREPLY:" + to_string(id) + ":" + to_string(srcid) + ":" + to_string(cost) + ":" ;

	    temgraph[id][srcid] = cost;
	    temgraph[srcid][id] = cost;


            struct sockaddr_in  cliaddr;

            cliaddr.sin_family = AF_INET; // IPv4
	    cliaddr.sin_addr.s_addr = INADDR_ANY;
	    cliaddr.sin_port = htons(PORT+srcid);

            socklen_t len = sizeof(cliaddr);

            sendto(sockfd, msg.c_str(), msg.size(),MSG_CONFIRM, (const struct sockaddr*) &cliaddr,len);
            
        }
        else if(pos == 3){
		//cout<<s_buff<<endl;
            // we recieved LSA packet
            // LSA:srcid:seqno:noofneighbours:neighid:cost
            string sublsa = s_buff.substr(pos+1); // sublsa has srcid:seqno:noofneighbours:neighid:cost
            pos = sublsa.find(":");
            string nodeid = sublsa.substr(0,pos); // nodeid has srcid
	   // sscanf(nodeid, "%d", &srcid);
	 //   cout<<nodeid<<endl;
            srcid = stoi(nodeid);
            sublsa = sublsa.substr(pos+1); // sublsa has seqno:noofneighbours:neighid:cost
            pos = sublsa.find(":");
            string sequenceno = sublsa.substr(0,pos);
	  //  int seno;
	  //  sscanf(sequenceno, "%d", &seno);
		//cout<<sequenceno<<endl;
            int seno = stoi(sequenceno);
            if(id!= srcid && seqno[srcid] < seno ){
                // need to store this information and pass it to neighbours
                seqno[srcid] = seno;
                sublsa = sublsa.substr(pos+1); // sublsa has noofneighbours:neighid:cost
                pos = sublsa.find(":");
                string friends = sublsa.substr(0,pos);
		//int necount;
		//sscanf(friends, "%d", &necount);
		//cout<<friends<<endl;
                int necount = stoi(friends); // no of neighbours
                sublsa = sublsa.substr(pos+1); // sublsa has neighid:cost
                int nej , jcost;
                for(int pp = 0; pp<necount ; pp++){
                    pos = sublsa.find(":");
                    friends = sublsa.substr(0,pos);
		    int nej;
		   // sscanf(friends, "%d", &nej);
			//cout<<friends<<endl;
                    nej = stoi(friends);
                    sublsa = sublsa.substr(pos+1); // sublsa has cost:
                    pos = sublsa.find(":");
                    friends = sublsa.substr(0,pos);
		    int jcost;
		   // sscanf(friends, "%d", &jcost);
		  //  cout<<friends<<endl;
                    jcost = stoi(friends);
                    temgraph[srcid][nej] = jcost;
                    temgraph[nej][srcid] = jcost;
		    sublsa = sublsa.substr(pos+1);
		    // sending lsa to neighbours

		    
                }

		for(int kk=0;kk<neigh.size();kk++){
                    struct sockaddr_in  cliaddr;

                    cliaddr.sin_family = AF_INET; // IPv4
	                cliaddr.sin_addr.s_addr = INADDR_ANY;
	                cliaddr.sin_port = htons(PORT+neigh[kk]);

                    socklen_t len = sizeof(cliaddr);

                    sendto(sockfd, s_buff.c_str(), s_buff.size(),MSG_CONFIRM, (const struct sockaddr*) &cliaddr,len);
                 }

                
                
            }
        }
        else {
            // we recieved hello reply message
            string subj = s_buff.substr(pos+1); // it has j:i:cost
		//cout<<subj<<endl;
            pos = subj.find(":");
	  //  cout<<pos<<endl;
            int j;
            string jval = subj.substr(0,pos); // jval has j
	   // sscanf(jval, "%d", &j);
	   // cout<<jval<<endl;
            j = stoi(jval);
            subj = subj.substr(pos+1); // subj has i:cost
            pos = subj.find(":");
            subj = subj.substr(pos+1); // subj has cost

	  //  int edgecost;
	 //   sscanf(subj, "%d", &edgecost);
	   // cout<<subj<<endl;

            int edgecost = stoi(subj);
		//cout<<edgecost<<endl;

            temgraph[id][j] = edgecost;
            temgraph[j][id] = edgecost;

        }
        
    }
}


int main(int argc , char * argv[]){

    neighbourno = 0;

    // taking inputs from command line
  /*  int id ;
    char * input ;
    char * output ;
   // string input ; 
   // string output ;
    int x , y , z ; */

    if(argc!=7){
        cout<<"invalid no of arguments"<<endl;
    }

    else {
        sscanf(argv[1], "%d", &id);
        input = argv[2];
        output = argv[3];
        sscanf(argv[4], "%d", &x);
        sscanf(argv[5], "%d", &y);
        sscanf(argv[6], "%d", &z);
        cout<<id<<" "<<input<<" "<<output<<" "<<x<<" "<<y<<" "<<z<<endl; 
    }

    

    

    // opening input and output files

  //  ifstream fin;
  //  fin.open(input);

  //  FILE *ifp;

    ifp    = fopen( input , "r");

    fout.open(output);

   // ofstream fout;

    
    //Checking if file ptr is NULL
    if ( ifp == NULL )  {
        printf( "input file failed to open." );
        return 0;
    }  

  //  int n , m;
 //   vector< vector<pair <int,int> > > ingraph;

  //  vector< vector <int> > graph ;

  //  vector <int> tem;

 //   pair <int , int> pair1;

 //   vector < pair <int,int > > set;

    fscanf( ifp, "%d", &n );  

    fscanf( ifp, "%d", &m );  

	cout<<n<<" "<<m<<endl;

    pair1.f = -1;
    pair1.s = -1;

    for(int i=0;i<n;i++){
        sets.push_back(pair1);
        tem.push_back(-1);
        seqno.push_back(-1);
      //  dist.push_back(infinite);
    }
    for(int i=0;i<n;i++){
        ingraph.push_back(sets);
        graph.push_back(tem);
        temgraph.push_back(tem);
    }

    int i,j,min,max ;

    for(int kk=0;kk<m;kk++){
        fscanf( ifp, "%d", &i );  
        fscanf( ifp, "%d", &j );  
        fscanf( ifp, "%d", &min );  
        fscanf( ifp, "%d", &max );  
        ingraph[i][j].f = min;
        ingraph[i][j].s = max;
        ingraph[j][i].f = min;
        ingraph[j][i].s = max;
	cout<<i<<" "<<j<<" "<<min<<" "<<max<<endl;
        graph[i][j] = 0;
        graph[j][i] = 0;
        temgraph[i][j]=0;
        temgraph[j][i]=0;
        if(i==id ){
	    cout<<i<<endl;
            neighbourno++;
            neigh.push_back(j);
        }
        if(j==id){
	    cout<<j<<endl;
            neighbourno++;
            neigh.push_back(i);
        }
    }

    


   // fin.close();


  //  int sockfd;
  //  struct sockaddr_in servaddr, cliaddr,cliadd;
	struct sockaddr_in cliadd,cliaddr;
    // Creating socket file descriptor
	if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

    memset(&servaddr, 0, sizeof(servaddr));
 //   memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
	servaddr.sin_family = AF_INET; // IPv4
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(PORT+id);

    // Bind the socket with the server address
	if ( bind(sockfd, (const struct sockaddr *)&servaddr,
			sizeof(servaddr)) < 0 )
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

    sleep(10);

  //  pthread_t  x_func , y_func , z_func , rec;

    if (pthread_mutex_init(&lock1, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    } 


    pthread_create(&rec, NULL , rec_func , NULL );
    pthread_create(&x_func, NULL , send_hello , NULL );
    pthread_create(&y_func, NULL , send_LSA , NULL );
    pthread_create(&z_func, NULL , do_dix , NULL );
    pthread_join(rec,NULL); 

    pthread_join(x_func, NULL);
    pthread_join(y_func, NULL);
    pthread_join(z_func, NULL);

    pthread_mutex_destroy(&lock1);
  //  pthread_join(rec,NULL); 

   /* string msg = "hello from s1";

    cliaddr.sin_family = AF_INET; // IPv4
	cliaddr.sin_addr.s_addr = INADDR_ANY;
	cliaddr.sin_port = htons(PORT+id+2);

    socklen_t len = sizeof(cliaddr);
    socklen_t le;
   // sleep(8);
    sendto(sockfd, msg.c_str(), msg.size(),
		MSG_CONFIRM, (const struct sockaddr*) &cliaddr,
			len);

    cout<<"hello msg sent to s3" <<endl;
    char buffer[MAXLINE];
    memset(&cliaddr, 0, sizeof(cliaddr));
    int n= recvfrom(sockfd, buffer, MAXLINE,
				MSG_WAITALL, ( struct sockaddr*) &cliadd,
				&le);
    buffer[n]='\0';
    printf("%s\n",buffer);
	//printf("Hello message sent.\n");  */

    fclose( ifp );
    fout.close();
  //  fclose(fop);

    return 0;
}
