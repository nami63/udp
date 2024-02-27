#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 12345

using namespace std;

struct Data {
    double f_lat;
    double f_lon;
    double f_alt;
    int n_pilot;
    bool mode_auto;
};

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cerr << "WSAStartup failed: " << result << endl;
        return 1;
    }

    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == INVALID_SOCKET) {
        cerr << "Error creating socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cerr << "Error binding socket: " << WSAGetLastError() << endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    cout << "Server running, waiting for data..." << endl;

    Data data;
    int len = sizeof(addr);
    ssize_t bytes_received = recvfrom(sockfd, (char*)&data, sizeof(data), 0, (struct sockaddr*)&addr, &len);
    if (bytes_received == SOCKET_ERROR) {
        cerr << "Error receiving data: " << WSAGetLastError() << endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    cout << "Data received:" << endl;
    cout << "Latitude: " << data.f_lat << endl;
    cout << "Longitude: " << data.f_lon << endl;
    cout << "Altitude: " << data.f_alt << endl;
    cout << "Number of pilots: " << data.n_pilot << endl;
    cout << "Auto mode: " << (data.mode_auto ? "true" : "false") << endl;

    closesocket(sockfd);
    WSACleanup();

    return 0;
}
