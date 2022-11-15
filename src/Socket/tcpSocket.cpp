#include"tcpSocket.h"


TcpSocket::TcpSocket() :conn(INVALID_SOCKET), addressInfo(NULL) {}

TcpSocket::TcpSocket(const char * host, const short port, bool serverSide) :conn(INVALID_SOCKET), addressInfo(NULL)
{
    init(host, port, serverSide);
}

void TcpSocket::init(const char * host, const short port, bool serverSide)
{
    sprintf_s(this->host, "%s", host);
    sprintf_s(this->port, "%d", port);

    if (!initWinsock()) return;

    addrinfo hints = {0};
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int iResult = getaddrinfo(host, this->port, &hints, &addressInfo);
    if ( iResult != 0 ) 
    {
        sprintf_s(message, "getaddrinfo() failed with error: %d", iResult);
        cleanup();
        return;
    }

    sock = socket(addressInfo->ai_family, addressInfo->ai_socktype, addressInfo->ai_protocol);
    if (sock == INVALID_SOCKET) 
    {
        sprintf_s(message, "socket() failed");
        cleanup();
        return;
    }

    if(!serverSide) return;

    if (bind(sock, addressInfo->ai_addr, (int)addressInfo->ai_addrlen) == SOCKET_ERROR) 
    {
        closesocket(sock);
        sock = INVALID_SOCKET;
        sprintf_s(message, "bind() failed");
        return;
    }

}

int TcpSocket::sendData(void* buf, int len)
{
    return send(conn, (const char *)buf, len, 0);
}

int TcpSocket::recvData(void* buf, int len)
{
    return recv(conn, (char*)buf, len, 0);
}

bool TcpSocket::isConnected()
{
    int errorCode;
    int errorCodeSize = sizeof(errorCode);
    return getsockopt(conn, SOL_SOCKET, SO_ERROR, (char*)&errorCode, &errorCodeSize) == 0 && errorCode == 0;
}

bool TcpSocket::acceptConnection()
{
    if (listen(sock, 1)  ==  (int)INVALID_SOCKET) 
    {
        sprintf_s(message, "listen() failed");
        return false;
    }
    conn = accept(sock, (struct sockaddr *)NULL, NULL);
    if (conn == (long long unsigned int)SOCKET_ERROR) 
    {
        sprintf_s(message, "accept() failed");
        return false;
    }
    return true;
}

bool TcpSocket::openConnection()
{
    if (connect(sock, addressInfo->ai_addr, (int)addressInfo->ai_addrlen) == SOCKET_ERROR) 
    {
        closesocket(sock);
        sock = INVALID_SOCKET;
        sprintf_s(message, "connect() failed; please make sure server is running");
        return false;
    }
    conn = sock;
    return true;
}