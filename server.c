#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "connection.h"

int main(void) {
    int down_flag = 0;
    int ret;
    int connection_socket;
    int data_socket;
    int result;
    ssize_t r, w;
    struct sockaddr_un name;
    char buffer[BUFFER_SIZE];
    
    // Create a local socket
    // Recall the socket function returns a file descriptor - just an int!
    // SOCK_SEQPACKET means 
    connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (connection_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Sets all the bytes occupied by this struct to 0
    memset(&name, 0, sizeof(name));

    // Bind socket to socket name
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sunpath)-1);
    
    ret = bind(connection_socket, (const struct sockaddr *) &name, sizeof(name));
    
    if (ret == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }




}
