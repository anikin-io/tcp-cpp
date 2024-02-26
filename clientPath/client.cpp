#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

using namespace std;

int connect_to_server(const string& server_host, int server_port) {
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;

    if (inet_pton(AF_INET, server_host.c_str(), &(server_address.sin_addr)) <= 0) {
        cerr << "Error converting address." << endl;
        exit(EXIT_FAILURE);
    }

    server_address.sin_port = htons(server_port);

    if (connect(client_socket, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address)) < 0) {
        cerr << "Failed to connect to the server." << endl;
        exit(EXIT_FAILURE);
    }

    return client_socket;
}

void send_file(int client_socket, const string& file_path) {
    ifstream file(file_path, ios::in | ios::binary);
    if (!file.is_open()) {
        cerr << "Failed to open the file." << endl;
        exit(EXIT_FAILURE);
    }

    if (!file) {
        cerr << "File not found: " << file_path << endl;
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    while ((bytes_read = file.readsome(buffer, sizeof(buffer))) > 0) {
        if (send(client_socket, buffer, bytes_read, 0) < 0) {
            cerr << "Error sending the file." << endl;
            exit(EXIT_FAILURE);
        }
    }

    file.close();
    cout << "File successfully sent" << endl;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " -s <server_address> -p <server_port>" << endl;
        exit(EXIT_FAILURE);
    }

    string server_host;
    int server_port;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            server_host = argv[i + 1];
        }
        else if (strcmp(argv[i], "-p") == 0) {
            server_port = stoi(argv[i + 1]);
        }
    }

    int client_socket = connect_to_server(server_host, server_port);

    string file_path;
    cout << "Enter the file path: ";
    getline(cin, file_path);

    send_file(client_socket, file_path);

    close(client_socket);

    return 0;
}
