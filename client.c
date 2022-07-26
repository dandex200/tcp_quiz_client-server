#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> 
#include <arpa/inet.h>

#define MAX 800
#define PORT 8080
#define SA struct sockaddr
void func(int sockfd)
{
    char buff[MAX];
    int n;
    char a;
    char b;
    char c;
    int i = 1;
    int qnum = 1;

    for (;;) {
        //Part 1 - Start
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff)); //Read 1
        printf("\n%s\n", buff);

        bzero(buff, sizeof(buff));
        printf("\nType Y or q: "); 
        n = 0;
        while((a = getchar()) != '\n') { 
            buff[n++] = a; 
        }
        write(sockfd, buff, sizeof(buff)); //Send 1
        sleep(1);
        if(strncmp("q", buff, 1) == 0){
            break;
        }  
        bzero(buff, sizeof(buff)); //Read Start Quiz!
        read(sockfd, buff, sizeof(buff));
        printf("\n%s\n", buff);
        while(i <= 5) {
            bzero(buff, sizeof(buff));
            read(sockfd, buff, sizeof(buff)); //Read 4: Read Question 2
            printf("\nQuestion %d: %s\n", qnum, buff);
            bzero(buff, sizeof(buff));
            printf("\nYour Response: ");
            n = 0;
            while((c = getchar()) != '\n') { 
            buff[n++] = c; 
            }
            write(sockfd, buff, sizeof(buff));

            bzero(buff, sizeof(buff));
            read(sockfd, buff, sizeof(buff));
            printf("\nYour Response Was: %s\n", buff);
            sleep(1);

            qnum++;
            i++;
        }
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("\nYour quiz score is %s / 5. Goodbye!\n", buff);


        break;
    }
}
   
int main()
{
    int sockfd, connfd;
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