#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> 
#include <time.h>
#include "QuizDB.h"
#include <unistd.h>


#define MAX 800
#define PORT 8080
#define SA struct sockaddr
#define ARR_SIZE(QuizQ) ( sizeof((QuizQ)) / sizeof((QuizQ[0])) )
   
// Function designed for chat between client and server.
void func(int connfd)
{
    char buff[MAX];
    int n;
    int quiztotal = 0;
    int start;
    
    int i = 1;

    // infinite loop for chat
    for (;;) {

        //Part 1 - Start
        bzero(buff, MAX);
        write(connfd, "Welcome to Unix Programming Quiz!\nThe quiz comprises five questions posed to you one after the other\nYou have only one attempt to answer a question.\nYour final score will be sent to you after a conclusion of the quiz.\nTo start the quiz, press Y and <enter>.\n To quit the quiz press q and <enter>.\nPress ctrl+C at any time to end connection", 340);

        bzero(buff, MAX);
        read(connfd, buff, sizeof(buff)); //Read 1
        
        if(strncmp("Y", buff, 1) == 0){
            write(connfd, "Start the quiz!", 15); //Send 2
            sleep(1);
            while(i <= 5) {
                srand(time(NULL));
                int q1place = rand() % ARR_SIZE(QuizQ);
                char* q1 = QuizQ[q1place];
                char* q1A = QuizA[q1place];
                        bzero(buff, MAX);
                write(connfd, q1, strlen(q1));
                bzero(buff, MAX);

                bzero(buff, MAX);
                read(connfd, buff, sizeof(buff));

                if(strcmp(q1A, buff) == 0){
                    write(connfd, "Correct!", 8); //Send 6: Whether or not Q1 was correct
                    quiztotal++;
                    printf("\nCorrect! Q1A: %s and Client Response: %s\nDo they match?\nQuiz Total: %d\n", q1A, buff, quiztotal);
                } else {
                write(connfd, "Incorrect\n", 9); //Send 6: Whether or not Q1 was correct
                printf("\nIncorrect. Q1A: %s and Client Response: %s\nDo they match?\nQuiz Total: %d\n", q1A, buff, quiztotal);
                }
                        sleep(1);
                i++;
            }
        } else {
            write(connfd, "End Quiz", 8); //Send 2
            break;
        }
        char quiztotalsent[20];
        sprintf(quiztotalsent, "%d", quiztotal);
        bzero(buff, MAX);
        write(connfd, quiztotalsent, strlen(quiztotalsent));

        break;

        
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
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");
   
    // Function for chatting between client and server
    func(connfd);
   
    // After chatting close the socket
    close(sockfd);
}