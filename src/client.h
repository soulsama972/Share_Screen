#include"Socket/tcpSocket.h"
#include"Socket/udpSocket.h"


#ifdef _WIN32
#include<windows.h>

#else

#endif

#include<iostream>
#include<thread>

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


class Client
{
public:
    Client(char* host);
    void run();

#ifdef _WIN32
    HWND createWindow(HINSTANCE hInstance);
    void updateWindow(const HWND hwnd,const cv::Mat& mScreen);
    void sideThreadUpdateWindow(HWND target);

#else

#endif

private:
    void recvData(std::vector<uchar>& buffer);

    TcpSocket socket;
};