#include <stdio.h> 
#include <sys/socket.h>
#include <string.h>
#include <sys/un.h>
#include <sys/errno.h>
#include <stdlib.h>
#include <netinet/in.h>

// C does not have a boolean type!
#define BOOL char // We use the smallest available type to represent 0 and 1
#define FALSE 0
#define TRUE 1

#define BUFF_SIZE 50


void handle_error(char* error_text) {
    fprintf(stderr, "%s\n", error_text);
    fprintf(stderr, "Oh blimey! Somethin' went wrong, mate: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}

int main() {
    

    // Create a socket file descriptor... the client endpoint.
    // We did the same for the server side.
    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    printf("[Client] Socket file descriptor: %d\n", tcp_socket);

    if (tcp_socket == -1) {
        handle_error("[Client] Socket creation failed.");
    }
    
    const struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(6969),
        .sin_addr = htonl(INADDR_LOOPBACK)
    };

    
    // Try to connect to a socket
    if (connect(tcp_socket, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
        printf("[Client] Connect to socket failed. Exiting client.\n");
        handle_error("[Client] Connecting failed.");
        return 1;
    }

    // Successful connection to a socket. Can start communicating.
    char msg[BUFF_SIZE] = "Hello, I'm the HTTP client!\n"; 
    
    // TODO: Abstract away checking for -1 into handle_any_errors function.
    if (send(tcp_socket, msg, BUFF_SIZE, MSG_CONFIRM) == -1)  {
        handle_error("[Client] Message sending failed.");
    }

    // IPC message to server based on user input
    char userMsg[BUFF_SIZE];
    while (strcmp(userMsg, "q") != 0) {
        printf("Enter the message to send to server: ");
        fgets(userMsg, BUFF_SIZE, stdin);
        // If msg is over buffer size, it is automatically cut off?
        if (send(tcp_socket, userMsg, BUFF_SIZE, MSG_CONFIRM) == -1) {
            handle_error("[Client] User message sending failed.");
        }
    }


    printf("\nHTTP client successfully closing.\n");
    return 0;
}
