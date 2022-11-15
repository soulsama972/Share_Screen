#include"server.h"



Server::Server()
{
    socket.init("0.0.0.0", 8888, true);
}

void Server::run()
{
    HWND target = GetDesktopWindow();
    cv::Mat wScreen = captrue(target);   

    std::vector<uchar> buf;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(100); // compression quality, ranges between 0-100

    while(true)
    {
        socket.acceptConnection();
        printf("accpetd connection\n");
        while(true)
        {
            wScreen = captrue(target);
            if (!cv::imencode(".jpg", wScreen, buf, params)) printf("JPEG compression failed.\n");
            sendData(buf);
        }
    }

}

bool Server::sendData(std::vector<uchar> buf)
{
    uint32_t size = buf.size();
    uint32_t chank = 65000;

    socket.sendData(&size, 4);
    uint32_t totalSent = 0, len;
    while(size > 0)
    {
        len = size < chank ? size : chank;
        len = socket.sendData(&buf[totalSent], len);
        if(len == INVALID_SOCKET) return false;

        totalSent += len;
        size -= len;
    }

    return true;
}