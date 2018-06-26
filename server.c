/* A server in the internet domain using TCP
   The port number is passed as an argument */

#include "extra.h"
#include <time.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     //Necessary initiallizations
	 //About socket buffer and more
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[3][256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {//Check for port
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) //Check if everything is ok with socket
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) error("ERROR on binding");     
     listnode *head;	
     head = malloc(sizeof(listnode));//Save required space for the list
     head->SourceIP = "-1";
     head->DestIP = "-1";
     head->SMS = "-1";
     head->next = NULL;
     int interval;  
     //MAIN LOOP
     while(1){     	
        int index = 0;
     	int flag = 0;
	char* output = " ";
        char* tmp1; 
        char* tmp2;
	char* merge;
	listnode *b = head;
        listen(sockfd,5);     	
	clilen = sizeof(cli_addr);
     	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     	if (newsockfd < 0) error("ERROR on accept");

	//Get the message and IP from User
        clock_t start_t,end_t;//Initiallize the clock for the exercise
        long double total_t;
        start_t = clock();//Use clock() function
     	bzero(buffer,3*256);
     	n = read(newsockfd,buffer,3*255);
     	if (n < 0) error("ERROR reading from socket");
     	push(head,buffer[0], buffer[1], buffer[2]);
		//Search
        while(b != NULL){
		if (strncmp( buffer[0] ,b->DestIP,strlen(b->DestIP)-1 ) == 0){
			flag=1;
			tmp1 = concat("\nFrom user: ", b->SourceIP);
			tmp2 = concat("  message is: ",b->SMS);
			merge = concat(tmp1,tmp2);                    
			output = concat(merge, output);               
		}else{
			index = index +1; 
			
		} 
		b = b->next;
		
     	}
	//Show messages
	if(!flag) output = "\n----No pending messages----\n";
	n = write(newsockfd,output,strlen(output));	
        if (n < 0) error("ERROR writing to socket");

        close(newsockfd);
        end_t = clock();
        total_t = (long double)(end_t - start_t)/CLOCKS_PER_SEC;
        //printf("\n Time is: %Lf\n", total_t);	
	interval = 3*1000000;
	usleep(interval);
     }
 
     close(sockfd);
     return 0; 
}
