#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 12345 // Порт, который будет открыт для прослушивания

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Создаем сокет
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Заполняем структуру sockaddr_in для сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Привязываем сокет к адресу и порту
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Ошибка при привязке сокета к адресу");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Начинаем прослушивание порта
    if (listen(server_socket, 5) == -1) {
        perror("Ошибка при прослушивании порта");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Сервер ожидает соединений на порту %d...\n", PORT);

    // Принимаем соединение от клиента
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket == -1) {
        perror("Ошибка при принятии соединения");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Соединение установлено. Ожидание данных...\n");

    // Обработка данных (в данном примере просто закрываем соединение)
    close(client_socket);
    close(server_socket);

    return 0;
}
