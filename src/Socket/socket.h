#pragma once
// Windows
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#undef TEXT
#include <winsock2.h>
#include <ws2tcpip.h>

// Linux
#else
#define sprintf_s sprintf
typedef int SOCKET;
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
static const int INVALID_SOCKET = -1;
static const int SOCKET_ERROR   = -1;
#endif

#include <iostream>

class Socket 
{
public:
    Socket();
    void closeConnection();
    char * getMessage();

protected:
    SOCKET sock;
    char message[200];
    bool initWinsock();
    void cleanup();
    void inetPton(const char * host, struct sockaddr_in & saddr_in);

};

// void setUdpTimeout(uint32_t msec)
// {
// #ifdef _WIN32
//     setsockopt(_sock, SOL_SOCKET, SO_RCVTIMEO, (char *) &msec, sizeof(msec));

// #else
//     struct timeval timeout;
//     timeout.tv_sec = msec / 1000;
//     timeout.tv_usec = (msec * 1000) % 1000000;
//     setsockopt(_sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
// #endif
// }