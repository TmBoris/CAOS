#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int creat_listener(char *port) {
    struct sockaddr_in6 serv_address;

    int sock;
    if ((sock = socket(AF_INET6, SOCK_STREAM, 0)) < 0) {
        perror("socket failed\n");
        exit(1);
    }

    serv_address.sin6_family = AF_INET6;
    serv_address.sin6_addr = in6addr_any;
    serv_address.sin6_port = htons(atoi(port));

    if (bind(sock, (struct sockaddr *)&serv_address, sizeof(serv_address)) <
        0) {
        perror("bind failed\n");
        exit(1);
    }

    if (listen(sock, SOMAXCONN) < 0) {
        perror("listen failed\n");
        exit(1);
    }

    return sock;
}

int main(int argc, char *argv[]) {

    int sock = creat_listener(argv[1]);

    struct sockaddr_in6 cci_addr;
    socklen_t len = sizeof(cci_addr);
    int connection = accept(sock, (struct sockaddr *)&cci_addr, &len);
    if (connection < 0) {
        return 1;
    }

    char addr[INET6_ADDRSTRLEN];
    int port = ntohs(cci_addr.sin6_port);
    inet_ntop(AF_INET6, &(cci_addr.sin6_addr), addr, INET6_ADDRSTRLEN);

    FILE *file = fdopen(connection, "r");
    if (file == NULL) {
        return 1;
    }
    char *line;
    size_t size = 0;
    while (getline(&line, &size, file) != -1) {
        printf("[%s:%d] %s", addr, port, line);
    }
    printf("\n");
    return 0;
}