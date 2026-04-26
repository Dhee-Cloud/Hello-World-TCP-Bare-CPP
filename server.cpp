#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

using namespace std;

//creating the socket and attaching the IP and Port
int main () {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        cerr << "error in creating socket" << endl;
        return 1;
    }

    sockaddr_in serverAddress;

    memset(&serverAddress, 0, sizeof(serverAddress));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

//binding the socket 
    if (bind(serverSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0 ) {
        cerr << "server binding error" << endl ;
        close(serverSocket);
        return 1; 
    }

// start listening for incoming connections
 if (listen(serverSocket, 5) < 0) {
    cerr << "error listening" << endl ;
    close(serverSocket);
    return 1;
}

cout << "server listening on port 8080" << endl;

sockaddr_in client;
socklen_t clientSize = sizeof(client);

//accept incoming connections
 int clientSocket = accept(serverSocket, (sockaddr*)&client, &clientSize);
    if (clientSocket < 0) {
        cerr << "Accepting failed" << endl;
        close(serverSocket);
        return 1;
    }

// send message to client
   string message = "Hello World";
    send(clientSocket, message.c_str(), message.length(), 0);

// close connections
    close(clientSocket);
    close(serverSocket);

    return 0;

}

