#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void send_html_page(int client_socket) {
    char *html_content = "<html><head><title>Minha Página</title></head><body><h1>Hello Nickolas</h1></body></html>";
    char response[BUFFER_SIZE];

    snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\nContent-Length: %lu\r\n\r\n%s", strlen(html_content), html_content);
    send(client_socket, response, strlen(response), 0);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Criando o socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Falha ao criar o socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Vinculando o socket a um endereço e porta
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Falha ao vincular o socket");
        exit(EXIT_FAILURE);
    }

    // Habilitando o socket para aceitar conexões
    if (listen(server_socket, 3) < 0) {
        perror("Erro ao tentar ouvir conexões");
        exit(EXIT_FAILURE);
    }

    printf("Servidor iniciado. Aguardando conexões...\n");

    while (1) {
        // Aceitando conexões
        if ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
            perror("Erro ao aceitar conexão");
            exit(EXIT_FAILURE);
        }

        printf("Conexão aceita de %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Enviando a página HTML
        send_html_page(client_socket);

        // Fechando o socket do cliente
        close(client_socket);
    }

    return 0;
}
