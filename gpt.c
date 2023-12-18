#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *hostname = argv[1];

    struct hostent *host_info = gethostbyname(hostname);

    if (host_info == NULL) {
        herror("gethostbyname");
        exit(EXIT_FAILURE);
    }

    struct in_addr **addr_list = (struct in_addr **)host_info->h_addr_list;

    for (int i = 0; addr_list[i] != NULL; ++i) {
        printf("IP Address: %s\n", inet_ntoa(*addr_list[i]));
    }

    return 0;
}
