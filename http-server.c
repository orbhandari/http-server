#include <stdio.h> // Provides useful io functions, e.g. printf
#include <sys/socket.h> // Handling sockets
#include <errno.h> // Some functions, when gone wrong, set errno - useful for us to debug
#include <string.h> // Useful string handling and printing functions
#include <netdb.h> // Useful for getaddrinfo, gethostbyname, etc.
#include <stdlib.h> 
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/socketfiletmp.socket"

// The downside to this approach is the lack of customised error message. Easily addable though...
void handle_error() {
    fprintf(stderr, "Oh blimey! Somethin' went wrong, mate: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}

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
        .ai_family = AF_UNSPEC,
        .ai_socktype = SOCK_STREAM,
        .ai_flags = AI_PASSIVE
    };
    struct addrinfo *results, *rp;


    // We get back the lowest available file descriptor
    // AF_UNIX stands for inter-process communication on the same machine
    // SOCK_SEQPACKET is for sequenced and reliable duplex 
    // When we create a "socket", we should ask: what exactly is a *socket*?
    // Obviously, it should at least *represent* an endpoint for communication.
    // Representation is an abstract term, though.
    // When we create a socket, we know at least we will get a file descriptor.
    // This fd *points* to an entry in the file descriptor table, which in turn points to global file table.
    // Hence, a socket should be a file. More precisely, it is a pseudo-file.
    // A pseudo-file is just a file that lives in memory, instead of normal files that can be stored on disc.
    int fd = socket(AF_UNIX, SOCK_SEQPACKET, 0); 
    printf("Socket file descriptor: %d\n", fd);

    // Check if socket was opened successfully
    if (fd == -1) {
        handle_error();
    }

    // If a socket has been created successfully, we must bind it to an address.
    // That is, once a baby is born, it exists in the world, 
    // but then we must give it a name and register it in the government directories.
    const struct sockaddr_un addr = {
        .sun_family = AF_UNIX,
        .sun_path = SOCKET_PATH
    };
    
    // On success, 0 is returned. This value is unimportant to us.
    if (bind(fd, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
        handle_error();
    }

    // Successful execution!
    printf("Your bluetewth dewice haz connecketed sussessfolly.\n");
    return 0;
}
