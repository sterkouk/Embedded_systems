#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h> 


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    char buffer[3][256];
    int fd;
    struct ifreq ifr;
    char iface[] = "eth0";
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name , iface , IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    //Save to buffer[0]
    char* IP;
    IP = inet_ntoa(( (struct sockaddr_in *)&ifr.ifr_addr )->sin_addr);
    strncpy(buffer[0], IP, sizeof(buffer[0]) );
    buffer[0][sizeof(buffer[0]) - 1] = '\0';
    //Necessary initiallizations
	//like server
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char message[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");
     	
    //Put receivers IP into the buffer[1] as it was asked
    printf("\n>Please enter the receivers IP:  ");
    bzero(buffer[1],256);
    fgets(buffer[1],255,stdin);
    //Put message into the buffer[2] as it was asked
    printf(">Please enter your message:  ");
    bzero(buffer[2],256);
    fgets(buffer[2],255,stdin);
    //Write to server
    n = write(sockfd,buffer,256*3);
    if (n < 0) 
         error("ERROR writing to socket");
    //Show messages
    bzero(message,256);
    n = read(sockfd, message, 255);
    if (n < 0) 
	error("ERROR reading from socket");
    printf("\n- - - - - - - - - - - - -");
    printf("\nHere are your pending messages:");
    printf("%s\n\n",message);
    close(sockfd);
    return 0;
}
