//
// Created by dek on 5/14/18.
//

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define RCVBUFSIZE 60

void DieWithError(char *errorMessage); //Error handlig
int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in servAddr;
    unsigned short servPort;
    char *query;
    char *servIP;
    char *sendMessage;
    char buffer[RCVBUFSIZE];
    unsigned int sendMgsLength;
    int bytesRcvd;
    int totalByteRcvd;


    servIP = "222.231.20.14";
    sendMessage = "GET /search.nsp?enc=euc_kr&page=1&q=blog HTTP/1.1\n Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*\nReferer: http://www.1noon.com/\nAccept-Language: ko\nUA-CPU: x86\nAccept-Encoding: gzip, deflate\nUser-Agent: Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.2; SV1; .NET CLR 1.1.4322)\nHost: total.1noon.com\nConnection: Keep-Alive\n\n";
    servPort = 80; // For HTTP

    /* Create a reliable, stream socket using TCP */
    if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed\n");
    else fprintf(stdout, "socket create\n");

    memset(&servAddr, 0, sizeof(servAddr)); //Zero ou structure
    servAddr.sin_family = AF_INET; //Internet address family
    servAddr.sin_addr.s_addr = inet_addr(servIP); //Server IP address
    servAddr.sin_port = htons(servPort);//Server port

    /* Establish the connection to the web server */
    if(connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        DieWithError("connet() failed\n");
    else fprintf(stdout, "socket connect\n");

    sendMgsLength = strlen(sendMessage);

    /* send the string to the server */
    if(send(sock, sendMessage, sendMgsLength, 0) != sendMgsLength)
        DieWithError("send() sent a different number of bytes than expected\n");
    else fprintf(stdout, "send message\n");

    /* receive the html from the web server */
    while(1){
        bytesRcvd = recv(sock, buffer, RCVBUFSIZE-1,0);
        if(bytesRcvd <= 0)
            break;
        buffer[bytesRcvd] = '\0'; //buffer의 값이 문자열임을 알려주기 위하여...
        printf("%s", buffer);
    }

    close(sock);
    exit(0);
}

void DieWithError(char *errorMessage)
{
    printf(errorMessage);
    exit(-1);
}

