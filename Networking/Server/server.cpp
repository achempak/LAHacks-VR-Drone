#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <iostream>
#include <vector>

using namespace std;

const short port = 8080;
const char* camFile = "golden-retriever-puppy.jpeg";  // pathname specifying pathname to camera under /dev
bool done = false; // controls infinite loop in main

void sig_handler(int signum)
{
    switch (signum) {
        case SIGINT:
        case SIGTSTP:
            done = true;
            break;
        default:
            break;
    }
}

int main()
{
    int socketfd, connectfd;
    socklen_t clientlen;
    struct sockaddr_in server_address, client_address;
    
    /* Create and verify socket */
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd == -1) {
        fprintf(stderr, "Cannot create socket.\n");
        exit(1);
    }
    
    bzero(&server_address, sizeof(server_address));
    
    /* Assign IP and Port */
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(port);
    
    /* Bind socket */
    if (bind(socketfd, (struct sockaddr *) &server_address, sizeof(server_address)) < 0){
        fprintf(stderr, "Cannot bind socket.\n");
        exit(1);
    }
    
    /* Listen and verify */
    if (listen(socketfd, 5) == -1) {
        fprintf(stderr, "Cannot listen.\n");
        exit(1);
    }
    
    /* Accept connection from client */
    connectfd = accept(socketfd, (struct sockaddr*)&client_address, &clientlen);
    if (connectfd < 0) {
        fprintf(stderr, "Failed to establish connection.\n");
        exit(1);
    }
    
    signal(SIGINT, sig_handler);
    signal(SIGTSTP, sig_handler);
    
    FILE* camera = fopen(camFile, "rb");

	/* Read data from camera in the path to the specified device file and send it to the client */
    size_t bufSize, bytesRead;
    while (!done) {
        /* Continuously read the whole image file */
        fseek(camera, 0L, SEEK_END);
        bufSize = ftell(camera);
        fseek(camera, 0L, SEEK_SET);
        
        vector<char> buf(bufSize);
        bytesRead = fread(buf.data(), sizeof(char), bufSize, camera);
        send(connectfd, buf.data(), bytesRead, 0);
    }

	close(connectfd);
    close(socketfd);
    exit(0);
}
