#include <stdio.h>          // < std(in/out/err)
#include <stdlib.h>
#include <sys/socket.h>     // < socket lib
#include <arpa/inet.h>      // < for sizeof(sockaddr_in)
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <sys/fcntl.h>
#include <errno.h>

#define BUFLEN 512          // < max len of bufer
#define TIMEOUT_SECONDS 5

void scanTCP_SYN (char *ip, char *port) {
    int sockfd;
    struct sockaddr_in addr;
    struct timeval timeout;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));

    if (inet_pton(AF_INET, ip, &(addr.sin_addr)) == 0) {
        close(sockfd);
        perror("Address not supported");
    }

    int flag = 1;
    if (setsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int)) == -1) {
        close(sockfd);
        perror("Nagle off error");
    }
    
    fcntl(sockfd, F_SETFL, O_NONBLOCK);                                     // < Set flag on socket O_NONBLOCK ~ fast open
    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        if (errno == EINPROGRESS || errno == EWOULDBLOCK) {                 // < If trying to connect
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(sockfd, &readfds);
            timeout.tv_sec = TIMEOUT_SECONDS;
            timeout.tv_usec = 0;
            if (select(sockfd + 1, NULL, &readfds, NULL, &timeout) > 0) {   // < Waiting for events on socket
                int optval;
                socklen_t optlen = sizeof(optval);
                if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &optval, &optlen) == 0) {
                    if (optval == 0) {
                        printf("Port %d is available.\n", atoi(port));      // SYN+ACK flag getted
                        close(sockfd);
                        return;
                    } else {
                        printf("Port %d is not available.\n", atoi(port));  // < RST flag getted
                        close(sockfd);
                        return;
                    }
                }
            }
        }
    }
}

void scanUDP(char *ip, char *port) {
    int sockfd;
    struct sockaddr_in addr;
    char buffer[BUFLEN];
    fd_set readfds;
    struct timeval timeout;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Error creating socket");
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));

    if (inet_pton(AF_INET, ip, &(addr.sin_addr)) == 0) {
        close(sockfd);
        perror("Address not supported");
    }
    if (sendto(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        close(sockfd);
        perror("Error sending UDP packet");
    }

    FD_ZERO(&readfds);
    FD_SET(sockfd, &readfds);

    timeout.tv_sec = TIMEOUT_SECONDS;
    timeout.tv_usec = 0;

    if (select(sockfd + 1, &readfds, NULL, NULL, &timeout) == 0) {
        close(sockfd);
        printf("Port %d is not available.\n", atoi(port));
        return;
    }
    if (recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL) == -1) {
        close(sockfd);
        perror("Error receiving data");
    }

    printf("Port %d is available.\n", atoi(port));
    close(sockfd);
}

void scanTCPconnect(char *ip, char *port) {
    int sockfd;
    struct sockaddr_in addr;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Error creating socket");
    }
    // < TCP_NODELAY??
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(atoi(port));

    if (inet_pton(AF_INET, ip, &(addr.sin_addr)) == 0) {
        close(sockfd);
        perror("Address not supported");
    }

    if (connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        printf("Port %d is not available.\n", atoi(port));
        close(sockfd);
        return;
    }

    printf("Port %d is available.\n", atoi(port));
    close(sockfd);
}