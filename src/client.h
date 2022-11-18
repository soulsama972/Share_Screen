#include"Socket/tcpSocket.h"

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
    Client(const char* host, int port);
    void run();

#ifdef _WIN32
    HWND createWindow(HINSTANCE hInstance);
    void updateWindow(const HWND hwnd,const cv::Mat& mScreen);
    void sideThreadUpdateWindow(HWND target);

#else

#endif

private:
    bool recvData(std::vector<uchar>& buffer);

    TcpSocket socket;
};