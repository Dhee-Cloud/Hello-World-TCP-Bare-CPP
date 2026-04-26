#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>

using namespace std;

//create the socket and attach IP and port
int main() {
	// create client TCP socket
	int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket < 0) {
		cerr << "Error creating client socket" << endl;
		return 1;
	}

	// create and clear server address structure
	sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));

	// set server address family and port, set server IP to connect to
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(8080);
	string serverIp = "127.0.0.1";

    
	// convert IP string to binary address
	if (inet_pton(AF_INET, serverIp.c_str(), &serverAddress.sin_addr) <= 0) {
		cerr << "Invalid server IP address" << endl;
		close(clientSocket);
		return 1;
	}

	// connect client socket to server
	if (connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
		cerr << "Connection to server failed" << endl;
		close(clientSocket);
		return 1;
	}

	// string buffer to receive server message
	string serverMessage(1024, '\0');

	// receive message from server
	int bytesReceived = recv(clientSocket, &serverMessage[0], serverMessage.size() - 1, 0);
	if (bytesReceived < 0) {
		cerr << "Receiving message failed" << endl;
		close(clientSocket);
		return 1;
	}

	// keep only the bytes that were received
	string messageReceived = serverMessage.substr(0, bytesReceived);

	// print server message
	cout << messageReceived << endl;

	// close client socket
	close(clientSocket);
	return 0;
}
