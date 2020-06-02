// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

const int MAX_RESPONSE_BODY_SIZE = 1024;

int main() {
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char r[MAX_RESPONSE_BODY_SIZE] = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 143\n\n";
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0) {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    FILE *fp;
    char s[256]={};
    char rstr[MAX_RESPONSE_BODY_SIZE]={};
    if ((fp = fopen("./index.html", "r")) == NULL) {
        printf("file open error!!\n");
        close(new_socket);
        exit(EXIT_FAILURE);
    }
    while (fgets(s, 256, fp) != NULL) {
        strcat(rstr, s);
        close(new_socket);
    }
    fclose(fp);
    strcat(r, rstr);

    while (1) {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[30000] = {0};
        valread = read(new_socket, buffer, 30000);
        printf("%s\n", buffer);

        write(new_socket, r, strlen(r));
        printf("%s", r);
        printf("\n------------------Hello message sent-------------------\n");
        close(new_socket);
    }
    return 0;
}