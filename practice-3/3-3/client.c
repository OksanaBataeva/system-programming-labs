#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(int argc, char const *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    const char *message;

    if (argc != 2) {
        printf("Usage: %s <message>\n", argv[0]);
        return 1;
    }
    message = argv[1];

    // Создание сокета
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Преобразование IP-адреса
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        close(sock);
        return -1;
    }

    // Подключение к серверу
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        close(sock);
        return -1;
    }

    // Отправка сообщения серверу
    if (send(sock, message, strlen(message), 0) < 0) {
        perror("Send failed");
        close(sock);
        return -1;
    }
    printf("Message sent: %s\n", message);

    // Получение ответа от сервера
    int valread = recv(sock, buffer, BUFFER_SIZE, 0);
    if (valread < 0) {
        perror("Receive failed");
    } else {
        printf("Response from server: %s\n", buffer);
    }

    close(sock);
    return 0;
}
