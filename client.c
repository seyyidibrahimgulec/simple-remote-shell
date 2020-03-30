#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>

#define PORT 3490
#define BLOCKSIZE 1000

int main(int argc, char *argv[]) {
    int sockfd, numbytes, i;
    char buf[BLOCKSIZE], buffer[BLOCKSIZE];
    struct hostent *he;
    struct sockaddr_in their_addr; // server's address information

    if(argc != 2) {
        fprintf(stderr, "usage: client hostname\n");
        exit(1);
    }

    if((he=gethostbyname(argv[1])) == NULL) {
        // get the host info
        perror("gethostbyname");
        exit(1);
    }

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT);
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);
    memset(&(their_addr.sin_zero), '\0', 8);


    if(connect(sockfd, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1) {
        perror("connect");
        exit(1);
    }
    while(1) {
        printf("> ");
        fgets(buf, BLOCKSIZE, stdin);
        send(sockfd, buf, BLOCKSIZE, 0);

        numbytes = recv(sockfd, buffer, BLOCKSIZE, 0);

        buffer[numbytes] = '\0';

        printf("%s\n", buffer);

        memset(buffer, 0, BLOCKSIZE);
        memset(buf, 0, BLOCKSIZE);
    }
    close(sockfd);
    return 0;
}
