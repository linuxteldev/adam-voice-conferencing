#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "client.h"
#include "status.h"

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

bool login(int client_id, char* password, char* server_ip, int server_port) {
    PRINT("Login: %d %s at %s %d\n", client_id, password, server_ip, server_port);
    
    int sockfd;
    struct addrinfo hints, *p, *servinfo;
    int rv;
    
    char server_port_s[4];
    sprintf(server_port_s, "%d", server_port);
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(server_ip, server_port_s, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return false;
    }

    // loop through all the results and make a socket
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }
        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }
        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to create socket\n");
        return false;
    }
    
    char s[INET6_ADDRSTRLEN];
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    freeaddrinfo(servinfo);
    
    // Send a message and get an reply
    PRINT("Connecting to %s\n", s);
    int numbytes = 0;
    if ((numbytes = sendto(sockfd, "Hello", sizeof("Hello"), 0,
            p->ai_addr, p->ai_addrlen)) == -1) {
        perror("client: sendto");
        exit(1);
    }
    
    return true;
}

bool logout() {
    PRINT("Logging out\n");
}

bool join_session(unsigned session_id) {
    PRINT("Joining session %d\n", session_id);
}

bool leave_session() {
    PRINT("Leaving session\n");
}

bool create_session(unsigned session_id) {
    PRINT("Creating session %d\n", session_id);
}

bool list() {
    PRINT("Listing Sessions\n");
}

bool quit() {
    PRINT("Quitting\n");
    exit(1);
}

bool send_message(char* message) {
    
}