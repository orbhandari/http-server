#include <stdio.h> // Provides useful io functions, e.g. printf
#include <sys/socket.h> // Handling sockets
#include <errno.h> // Some functions, when gone wrong, set errno - useful for us to debug
#include <string.h> // Useful string handling and printing functions
#include <netdb.h> // Useful for getaddrinfo, gethostbyname, etc.
#include <stdlib.h> 
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include <netinet/in.h> 

// TODO: Remove SOCKET_PATH 
#define SOCKET_PATH "/tmp/socketfiletmp.socket"
#define LISTEN_BACKLOG 50

#define TRUE 1
#define FALSE 0
#define BOOL char

void handle_error(char* error_text) {
    fprintf(stderr, "%s\n", error_text);
    fprintf(stderr, "Oh blimey! Somethin' went wrong, mate: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    // Check which C version we are using for fun!
    printf("Version of C: %ld\n", __STDC_VERSION__);


    // Require a port where the server listens to. Not needed if IPC.
    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int port = atoi(argv[1]);
    
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
    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0); 
    printf("Socket file descriptor: %d\n", tcp_socket);

    // Check if socket was opened successfully
    if (tcp_socket == -1) {
        handle_error("Socket creation failed.");
    }

    // If a socket has been created successfully, we must bind it to an address.
    // That is, once a baby is born, it exists in the world, 
    // but then we must give it a name and register it in the government directories.
    const struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = htonl(INADDR_ANY) // 0.0.0.0 - is it safe?
    };
    
    // const int temp = 1; 
    // if (setsockopt(tcp_socket, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(int)) < 0) {
    //     perror("setsockopt");
    //     /* Handle error here */
    //     handle_error("Set socket option failed.");    
    // } 

    // On success, 0 is returned. This value is unimportant to us.
    if (bind(tcp_socket, (const struct sockaddr *) &addr, sizeof(addr)) == -1) {
        handle_error("Bind failed.");
    }
    printf("Binding successful.\n");

    printf("Putting into listening state...\n");
    // After binding it, we should listen to incoming connections. This more means "setting into listening state".
    if (listen(tcp_socket, LISTEN_BACKLOG) == -1) {
        handle_error("Listen failed.");
    } 
    printf("Putted into listening state.\n");
    
    socklen_t peer_addr_size;
    struct sockaddr_in peer_addr;
    peer_addr_size = sizeof(peer_addr); 
    
    // A new socket (file descriptor) is created by accept(...) for the communication
    int tcp_socket_talking; 
    // The accept function *blocks* until there exists at least one connector client!
    // TODO: It may be useful to use a timeout here... otherwise 
    tcp_socket_talking = accept(tcp_socket, (struct sockaddr *) &peer_addr, &peer_addr_size); // Why are we passing address of a size var?
    if (tcp_socket_talking == -1) {
        handle_error("Accept connection failed.");
    }
    printf("Accepted connection\n");
    printf("Client information: address = %s, port = %d\n", 
           inet_ntoa(peer_addr.sin_addr), ntohs(peer_addr.sin_port));
    
    int BUFF_SIZE = 50;
    char msg_buffer[BUFF_SIZE];
    
    while (TRUE) {
        // Peek the message and check if the one in msg_buffer if the same or not
        // If the same, continue
        // else, print the new message 
        int rv = recv(tcp_socket_talking, msg_buffer, BUFF_SIZE, MSG_WAITALL);

        if (rv == 0) {
            printf("\nPeer has shutdown.\n");
            break;
        }

        printf("%s", msg_buffer);
    } 

    // Closing the connection 
    if (close(tcp_socket) == -1) { handle_error("Close failed."); }
    
    // Successful execution!
    printf("\nYour bluetewth dewice haz connecketed sussessfolly.\n");
    return 0;
}
