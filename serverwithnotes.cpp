#include <iostream> // used to write out to the console
#include <sys/socket.h> // provides core socket API: socket creation, binding, listening, accepting, sending, receiving
#include <netinet/in.h> // gives IPv4 address structure (sockaddr_in) and constants for IP/port setup
#include <unistd.h> // used to close sockets
#include <cstring> // used for memory operations like memset and strlen

using namespace std; // so you don't have to write std::cout or std::cerr. They are under the std namespace

int main() {
    // first create socket (AF_INET = IPv4, SOCK_STREAM = TCP) AF_INET(Address family_internet- IPV4) SOCK_STREAM(Used for TCP)
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) { //valid sockets are 0 or greater, errors are -1 so if serversocker is less than 0 there is an error
        cerr << "Error creating socket" << endl; //cerr is used to print out standard error
        return 1;//return 0 means success so we say return 1 because a failure has occured
    }

    sockaddr_in serverAddress;// The socket is stored in serverSocket (the int). serverAddress stores the address (IP + port), not the socket itself.

    
    memset(&serverAddress, 0, sizeof(serverAddress)); // clear structure to avoid garbage values

    serverAddress.sin_family = AF_INET; // sin_family is typically a field in the sockaddr_in structure used for IPv4 socket addresses in C/C++. It tells the networking API which address family the structure is using.
    serverAddress.sin_port = htons(8080); // convert port to network byte order so the computer will understand it.

    // listen on all network interfaces (localhost + LAN + VM interfaces)
    serverAddress.sin_addr.s_addr = INADDR_ANY; //serverAddress.sin_addr.s_addr is the field where an IPv4 address is stored inside a sockaddr_in struct.

    // bind socket to IP + port
   
        cerr << "Bind failed" << endl;
        close(serverSocket);
        return 1; //return 1 exits the program with an exit code of 1, signaling to the operating system that the program terminated due to an error or failure.
    }

    // start listening for incoming connections
    if (listen(serverSocket, 5) < 0) { // The 1 is the backlog size.It means the OS can keep about 1 pending connection waiting in the queue before your code calls accept().
        //< 1 = queue limit for waiting clients. 0 = success return value from listen()
        cerr << "Listen failed" << endl;
        close(serverSocket);
        return 1;
    }

    cout << "Server running on port 8080" << endl;

    sockaddr_in clientAddress; //sockaddr_in clientAddress declares a variable named clientAddress whose type is sockaddr_in. sockaddr_in is a C/C++ struct used to store an IPv4 socket address. It can hold things like the IP address and port number for one network endpoint.
    socklen_t clientSize = sizeof(clientAddress);

    // accept incoming client connection
    int clientSocket = accept(serverSocket, (sockaddr*)&clientAddress, &clientSize);
    if (clientSocket < 0) {
        cerr << "Accept failed" << endl;
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
    //socket is the actual communication endpoint returned by the socket() function. It is an integer handle that the operating system gives you. You use that handle in functions like bind(), listen(), accept(), and close(). So socket is the thing the OS uses to identify your TCP connection endpoint.server, on the other hand, is a sockaddr_in structure. It does not represent the socket itself. It only stores the address information for the server, such as the IP address and port number. In other words, server describes where the socket should be attached.
}