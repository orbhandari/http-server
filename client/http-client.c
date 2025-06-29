#include <stdio.h> 
#include <sys/socket.h>
#include <string.h>
#include <sys/un.h>
#include <sys/errno.h>
#include <stdlib.h>

// C does not have a boolean type!
#define BOOL char // We use the smallest available type to represent 0 and 1
#define FALSE 0
#define TRUE 1

#define SOCKET_PATH "/tmp/socketfiletmp.socket"
#define BUFF_SIZE 50


void handle_error(char* error_text) {
    fprintf(stderr, "%s\n", error_text);
    fprintf(stderr, "Oh blimey! Somethin' went wrong, mate: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}

int main() {
    

    // Create a socket file descriptor... the client endpoint.
    // We did the same for the server side.
    int sockfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    printf("[Client] Socket file descriptor: %d\n", sockfd);

    if (sockfd == -1) {
        handle_error("[Client] Socket creation failed.");
    }
    
    const struct sockaddr_un addr = {
        .sun_family = AF_UNIX,
        .sun_path = SOCKET_PATH
    };

    
    // Try to connect to a socket
    if (connect(sockfd, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
        printf("[Client] Connect to socket failed. Exiting client.\n");
        handle_error("[Client] Connecting failed.");
        return 1;
    }

    // Successful connection to a socket. Can start communicating.
    char msg[BUFF_SIZE] = "Hello, I'm the HTTP client!"; 
    
    // TODO: Abstract away checking for -1 into handle_any_errors function.
    if (send(sockfd, msg, BUFF_SIZE, MSG_CONFIRM) == -1)  {
        handle_error("[Client] Message sending failed.");
    }

    printf("Message sent to server without errors.\n");

    printf("HTTP client successfully closing.\n");
    return 0;
}
