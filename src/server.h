#include "Socket/tcpSocket.h"
#include "windowCaptrue.h"


#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


class Server
{
public:
    Server(const char* bindAddress, int port);
    void run();
private:
    Captrue cap;
    TcpSocket socket;
    bool sendData(std::vector<uchar> buf) const;
    bool sendData(cv::Mat buf) const;
    bool sendData(const char* buf, uint32_t size) const;
    bool sendData(const char* buf, int width, int height, int size) const;
};