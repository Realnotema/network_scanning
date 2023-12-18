#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

void startUDPServer(int port) {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

    // Создание сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Заполнение структуры с данными сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Привязка сокета к адресу и порту
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Error binding socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP server started on port %d\n", port);

    while (1) {
        socklen_t client_addr_len = sizeof(client_addr);

        // Прием данных от клиента
        ssize_t bytesRead = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_addr_len);

        if (bytesRead == -1) {
            perror("Error receiving data");
            continue;
        }

        printf("Received UDP packet from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Отправка ответа обратно клиенту
        if (sendto(sockfd, buffer, bytesRead, 0, (struct sockaddr *)&client_addr, client_addr_len) == -1) {
            perror("Error sending UDP packet");
            continue;
        }

        printf("Sent UDP response to %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    }

    // Закрытие сокета (не достижимо в данном коде)
    close(sockfd);
}

int main() {
    int port = 12345;  // Порт для сервера

    startUDPServer(port);

    return EXIT_SUCCESS;
}
