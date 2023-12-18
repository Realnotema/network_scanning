#include <stdio.h>          // < std(in/out/err)
#include "netscan.h"

void printerr(char *errmess) {
    perror(errmess);
    exit(1);
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
                printf("\nUse ./netscan <flag> <ip> <port>\n\n"
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