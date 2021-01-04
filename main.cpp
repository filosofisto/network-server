#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>

using namespace std;

const int BUFFER_LEN = 1024;

int main(int argc, char** argv) {
    cout << "Network Server" << endl;

    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <port | Ex: 80>" << endl;
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);

    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in address;
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port   = htons(port);

    int bind_value = bind(sock, (struct sockaddr *) &address, sizeof(address));
    if (bind_value < 0) {
        cerr << "Could not bind - error: " << bind_value << endl;
        return EXIT_FAILURE;
    }

    int listen_value = listen(sock, 1);
    if (listen_value < 0) {
        cerr << "Could not listen" << endl;
        return EXIT_FAILURE;
    }

    struct sockaddr_in remote_address;
    memset(&remote_address, 0, sizeof(remote_address));
    socklen_t remote_addrlen = sizeof(remote_address);

    cout << "Waiting for new connection...";
    int client_sock = accept(sock, (struct sockaddr*) &remote_address, &remote_addrlen);

    string client_ip = inet_ntoa(remote_address.sin_addr);
    int remote_port = ntohs(remote_address.sin_port);

    cout << "\nAccepted client from IP: " << client_ip << ", Port: " << remote_port << endl;
    char buffer[BUFFER_LEN];

    for(;;) {
        memset(buffer, 0, BUFFER_LEN);

        ssize_t bytes_received = recv(client_sock, buffer, BUFFER_LEN-1, 0);
        if (bytes_received < 0) {
            cerr << "Could not receive" << endl;
            return EXIT_FAILURE;
        }

        if (bytes_received == 0) {
            cout << "Client at " << client_ip << ":" << remote_port << " has disconnected" << endl;
            break;
        }
    }

    cout << "Shuting down socket" << endl;
    shutdown(client_sock, SHUT_RDWR);

    return EXIT_SUCCESS;
}