#include <stdio.h> // Provides useful io functions, e.g. printf
#include <sys/socket.h> // Handling sockets
#include <errno.h> // Some functions, when gone wrong, set errno - useful for us to debug
#include <string.h> // Useful string handling and printing functions
#include <netdb.h> //
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Check which C version we are using for fun!
    printf("Version of C: %ld\n", __STDC_VERSION__);


    // Require a port where the server listens to
    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    // 
    struct addrinfo hints = {
        .ai_family = AF_UNSPEC;
        .ai_socktype = SOCK_STREAM,
        .ai_flags = AI_PASSIVE;
    };
    struct addrinfo *results, *rp;


    // We get back the lowest available file descriptor
    // int fd = socket(AF_UNIX, SOCK_STREAM, protocol ); 

    // Dummy value
    int fd = 0;
    // Check if socket was opened successfully
    if (fd == -1) {
        // Print the error
        printf("Oh blimey! Somethin' went wrong, mate: %s\n", strerror(errno));
        // Close the program and show failure. If used in main, same as return 1, probably.
        exit(EXIT_FAILURE);
    }

    // Successful execution!
    return 0;
}
