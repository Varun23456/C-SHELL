#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

void remove_tags(char *data)
{
    data = strchr(data, '<');

    char *buffer = malloc(strlen(data) + 1);
    if (buffer == NULL)
    {
        perror("malloc");
        return;
    }

    char *p = buffer;
    const char *src = data;

    while (*src)
    {
        if (*src == '<')
        {
            while (*src != '>')
            {
                src++;
            }

            if (*src)
            {
                src++;
            }
        }
        else
        {
            *p = *src;
            p++;
            src++;
        }
    }

    *p = '\0'; 

    printf("%s\n", buffer);

    free(buffer);
    return;
}

void find_ManPage(char *command)
{
    struct addrinfo hints, *res;
    int sockfd;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo("man.he.net", "80", &hints, &res) != 0)
    {
        perror("getaddrinfo");
        exit(0);
    }

    sockfd = socket(res->ai_family, res->ai_socktype, 0);
    if (sockfd == -1)
    {
        perror("socket");
        freeaddrinfo(res);
        exit(0);
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1)
    {
        perror("connect");
        close(sockfd);
        freeaddrinfo(res);
        exit(0);
    }

    char request[4096];
    strcpy(request, "GET http://man.he.net/?topic=");
    strcat(request, command);
    strcat(request, "&section=all HTTP/1.1\r\nHost: man.he.net\r\nConnection: close\r\n\r\n");

    if (send(sockfd, request, strlen(request), 0) == -1)
    {
        perror("send");
        close(sockfd);
        exit(0);
    }

    int bytes_received;
    char buffer[4096];
    char data[50000] ;
    data[0] = '\0';
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytes_received] = '\0';
        strncat(data, buffer, sizeof(data) - strlen(data) - 1);
    }

    if (bytes_received == -1)
    {
        perror("recv");
    }

    remove_tags(data);
    close(sockfd);
    freeaddrinfo(res);
    return;
}


