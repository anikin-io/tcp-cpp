#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <csignal>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <ctime>
#include <sys/stat.h>

#define BUFFER_SIZE 1024
#define MAX_THREADS 5

using namespace std;

volatile sig_atomic_t terminate_flag = 0;
int server_socket;
size_t thread_count;
pthread_t threads[MAX_THREADS];

string get_current_time() {
    time_t current_time = time(nullptr);
    char buffer[80];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d_%H-%M-%S", localtime(&current_time));
    return string(buffer);
}

class ClientThread {
public:
    ClientThread(int client_socket, sockaddr_in client_address, const string& file_dir, size_t max_file_size)
        : client_socket(client_socket), client_address(client_address), file_dir(file_dir), max_file_size(max_file_size) {}

    void run() {
                string file_name = get_current_time() + "_" +
                    to_string(ntohl(client_address.sin_addr.s_addr)) + "_" +
                    to_string(ntohs(client_address.sin_port)) + ".txt";
        
                string file_path = file_dir + "/" + file_name;
                ofstream file(file_path, ios::out | ios::binary);
        
                char buffer[BUFFER_SIZE];
                size_t total_received = 0;
                ssize_t bytes_received;
        
                while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
                    file.write(buffer, bytes_received);
                    total_received += bytes_received;
        
                    if (total_received >= max_file_size) {
                        cout << "Превышен лимит размера файла. Соединение с "
                            << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port)
                            << " закрыто." << endl;
                        break;
                    }
                }
        
                if (bytes_received < 0) {
                    cerr << "Ошибка отправки. Файл не сохранен: " << file_path << endl;
                    remove(file_path.c_str());
                }
        
                close(client_socket);
                cout << "Файл получен от " << inet_ntoa(client_address.sin_addr)
                    << " и сохранен как " << file_path << endl;
    }

private:
    int client_socket;
    sockaddr_in client_address;
    string file_dir;
    size_t max_file_size;
};

void* start_client_thread(void* arg) {
    ClientThread* client_thread = static_cast<ClientThread*>(arg);
    client_thread->run();
    delete client_thread;
    return nullptr;
}

void start_server(const string& host, int port, int max_threads, const string& file_dir, size_t max_file_size) {
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    thread_count = 0;

    sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(host.c_str());
    server_address.sin_port = htons(port);

    bind(server_socket, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address));
    listen(server_socket, 5);

    cout << "Сервер слушает на " << host << ":" << port << endl;

    while (!terminate_flag) {
        sockaddr_in client_address;
        socklen_t client_address_size = sizeof(client_address);

        int client_socket = accept(server_socket, reinterpret_cast<struct sockaddr*>(&client_address), &client_address_size);

        cout << "Соединение с " << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << endl;

        if (thread_count < max_threads) {
            ClientThread* client_thread = new ClientThread(client_socket, client_address, file_dir, max_file_size);
            pthread_create(&threads[thread_count], nullptr, start_client_thread, client_thread);
            thread_count++;
        }
        else {
            cout << "Достигнуто максимальное количество потоков, соединение отклонено." << endl;
            close(client_socket);
        }
    }

    close(server_socket);
}

void signal_handler(int signum) {
    if (signum == SIGINT || signum == SIGTERM || signum == SIGHUP) {
        cout << "Выход с сохранением..." << endl;
        terminate_flag = 1;

        for (size_t i = 0; i < thread_count; ++i) {
            pthread_join(threads[i], nullptr);
        }

        cout << "Сервер завершен." << endl;

        close(server_socket);

        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char* argv[]) {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGHUP, signal_handler);

    string host = "127.0.0.1";
    int port = 12345;
    int max_threads = 5;
    string file_dir = "uploads";
    size_t max_file_size = 1048576;

    if (argc > 1) {
        host = argv[1];
    }

    if (argc > 2) {
        port = stoi(argv[2]);
    }

    if (argc > 3) {
        max_threads = stoi(argv[3]);
    }

    if (argc > 4) {
        file_dir = argv[4];
    }

    if (argc > 5) {
        max_file_size = stoull(argv[5]);
    }

    pid_t pid, sid;
    pid = fork();
    if (pid < 0) {
        cerr << "Ошибка создания дочернего процесса.\n";
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }
    umask(0); 

    sid = setsid();
    if (sid < 0) {
        cerr << "Ошибка создания новой сессии.\n";
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    start_server(host, port, max_threads, file_dir, max_file_size);

    return 0;
}


