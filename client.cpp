#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT 12345
#define REMOTE_IP "255.255.255.255"

#pragma comment(lib, "Ws2_32.lib")

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
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Failed to initialize Winsock." << endl;
        return 1;
    }

    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == INVALID_SOCKET) {
        cerr << "Error creating socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, REMOTE_IP, &addr.sin_addr);

    Data data;
    data.f_lat = 12.96093;
    data.f_lon = 77.65333;
    data.f_alt = 860.0;
    data.n_pilot = 2;
    data.mode_auto = true;

    int bytes_sent = sendto(sockfd, reinterpret_cast<const char*>(&data), sizeof(data), 0, reinterpret_cast<sockaddr*>(&addr), sizeof(addr));
    if (bytes_sent == SOCKET_ERROR) {
        cerr << "Error sending data: " << WSAGetLastError() << endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    } else {
        cout << "Bytes sent: " << bytes_sent << endl;
    }

    cout << "Data sent successfully." << endl;

    closesocket(sockfd);
    WSACleanup();

    return 0;
}
