#include <stdio.h>          // < std(in/out/err)
#include <stdlib.h>
#include <sys/socket.h>     // < socket lib
#include <arpa/inet.h>      // < for sizeof(sockaddr_in)
#include <unistd.h>
#include <string.h>

#define BUFLEN 512          // < max len of bufer
#define TIMEOUT_SECONDS 5

void printerr(char *errmess) {
    perror(errmess);
    exit(1);
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

void read_flag(char *argv[]) {
    if (strcmp(argv[1], "-U") == 0) {
        printf("UDP scanning is selected\n");
        scanUDP(argv[2], argv[3]);
    } else if (strcmp(argv[1], "-T") == 0) {
        printf("TCP connect scanning is selected\n");
        scanTCPconnect(argv[2], argv[3]);
    } else if (strcmp(argv[1], "-sT") == 0) {
        printf("TCP SYN scanning is selected\n");
    } else printerr("Incorrect flag: --help for more information");
}

int main(int argc, char* argv[]) {
    switch(argc) {
        case 1:
            printerr("Incorrect entry: --help for more information");
            break;
        case 2:
            if (strcmp(argv[1], "--help") == 0) {
                printf("\nUse ./netscan -<flag> <ip> <port>\n\n"
                "Flags:\n\t-U - to UDP scan\n\t-T - to TCP connect scan\n\t-sT - to TCP SYN scan\n"
                "In this version you cannot combine flags\n\n");
            }
            break;
        case 3:
            printerr("Incorrect entry: --help for more information");
            break;
        case 4:
            read_flag(argv);
    }
    printf("QUITING!\n");
    return 0;
}