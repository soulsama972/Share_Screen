#include "Socket/tcpSocket.h"
#include "windowCaptrue.h"



class Server
{
public:
    Server();
    void run();
private:
    TcpSocket socket;
    bool sendData(std::vector<uchar> buf);
};