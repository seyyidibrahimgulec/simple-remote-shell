#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


#define BACKLOG 10
#define LINESIZE 1024
#define BLOCKSIZE 1000


int main(int argc, char *argv[]) {
    int sockfd, new_fd, numbytes;
    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    socklen_t sin_size;
    char buf[BLOCKSIZE], buffer[BLOCKSIZE];
    FILE *fp;

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(atoi(argv[2]));
    my_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(my_addr.sin_zero), '\0', 8);

    if(bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(1);
    }

    if(listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    while(1) {
        sin_size = sizeof(struct sockaddr_in);
        if((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
            perror("accept");
            continue;
        }

        printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));

        numbytes = recv(new_fd, buf, BLOCKSIZE, 0);
        buffer[numbytes] = '\0';
        if(strcmp(buf, argv[4])) {
            close(new_fd);
            return -1;
        }
        memset(buf, 0, BLOCKSIZE);

        numbytes = recv(new_fd, buf, BLOCKSIZE, 0);
        buffer[numbytes] = '\0';
        if(strcmp(buf, argv[6])) {
            close(new_fd);
            return -1;
        }
        memset(buf, 0, BLOCKSIZE);

        while(1) {
            numbytes = recv(new_fd, buf, BLOCKSIZE, 0);
            buf[numbytes] = '\0';

            fp = popen(buf, "r");

            fread(buffer, 1, BLOCKSIZE, fp);
            pclose(fp);

            send(new_fd, buffer, BLOCKSIZE, 0);

            memset(buffer, 0, BLOCKSIZE);
            memset(buf, 0, BLOCKSIZE);
        }
        close(new_fd);
    }

    return 0;
}


