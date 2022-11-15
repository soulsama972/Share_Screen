#pragma once
#include"socket.h"



class TcpSocket : public Socket
{
public:
    TcpSocket();
    TcpSocket(const char * host, const short port,bool serverSide);
    void init(const char * host, const short port,bool serverSide);

    int sendData(void* buf, int len);
    int recvData(void* buf, int len);
    bool isConnected();
    bool acceptConnection();
    bool openConnection();

private:
    SOCKET conn;
    addrinfo *addressInfo;
    char host[200];
    char port[10];
};

