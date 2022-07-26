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

    // Infinite loop for chat
    for (;;) {

        //Part 1 - Start
        bzero(buff, MAX);
        write(connfd, "Welcome to Unix Programming Quiz!\nThe quiz comprises five questions posed to you one after the other\nYou have only one attempt to answer a question.\nYour final score will be sent to you after a conclusion of the quiz.\nTo start the quiz, press Y and <enter>.\n To quit the quiz press q and <enter>.\nPress ctrl+C at any time to end connection", 340);

        bzero(buff, MAX);
        read(connfd, buff, sizeof(buff)); //Read 1
        
        if(strncmp("Y", buff, 1) == 0){
            write(connfd, "Start the quiz!", 15); //Send 1
            sleep(1);
            while(i <= 5) {
                srand(time(NULL));
                int q1place = rand() % ARR_SIZE(QuizQ);
                char* q1 = QuizQ[q1place];
                char* q1A = QuizA[q1place];
                        bzero(buff, MAX);
                write(connfd, q1, strlen(q1)); //Send 2
                bzero(buff, MAX);

                bzero(buff, MAX);
                read(connfd, buff, sizeof(buff)); //Read 2

                if(strcmp(q1A, buff) == 0){
                    write(connfd, "Correct!", 8); //Send 3
                    quiztotal++;
                    printf("\nCorrect! Q1A: %s and Client Response: %s\nDo they match?\nQuiz Total: %d\n", q1A, buff, quiztotal);
                } else {
                write(connfd, "Incorrect\n", 9); //Send 3
                printf("\nIncorrect. Q1A: %s and Client Response: %s\nDo they match?\nQuiz Total: %d\n", q1A, buff, quiztotal);
                }
                sleep(1);
                i++;
            }
        } else {
            write(connfd, "End Quiz", 8); //Send 4
            break;
        }
        char quiztotalsent[20];
        sprintf(quiztotalsent, "%d", quiztotal);
        bzero(buff, MAX);
        write(connfd, quiztotalsent, strlen(quiztotalsent)); //Send 5

        break;

        
    }
}
   
// Driver function
int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
   
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);
   
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
   
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
   
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");
   
    func(connfd);
   
    close(sockfd);
}