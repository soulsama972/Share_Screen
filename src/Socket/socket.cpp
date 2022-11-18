#include"socket.h"

Socket::Socket()
{
    *message = 0;
    sock = INVALID_SOCKET;
}

void Socket::closeConnection()
{
#ifdef _WIN32
    closesocket(sock);
#else
    close(sock);
#endif
}

char* Socket::getMessage()
{
    return message;
}

bool Socket::initWinsock()
{
#ifdef _WIN32
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) 
    {
        sprintf_s(message, "WSAStartup() failed with error: %d\n", iResult);
        return false;
    }
#endif
    return true;
}

void Socket::cleanup()
{
#ifdef _WIN32
    WSACleanup();
#endif
}