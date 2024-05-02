#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

constexpr int PORT = 8080;
constexpr char* SERVER_IP = "127.0.0.1"; // IP address of the server

int main() {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        std::cerr << "Error opening socket\n";
        return 1;
    }

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    if (connect(client_socket, (sockaddr*)&server_address, sizeof(server_address)) < 0) {
        std::cerr << "Error connecting to server\n";
        return 1;
    }

    std::cout << "Connected to server" << std::endl;

    const char* message = "Hello, server!";
    send(client_socket, message, strlen(message), 0);

    close(client_socket);

    return 0;
}