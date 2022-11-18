#include"server.h"
#include<chrono>


Server::Server(const char* bindAddress, int port)
{
    socket.init(bindAddress, port, true);
    if(!cap.init())
    {
        printf("cannot initalize the captrue class exiting.. \n");
        exit(-1);
    }
}

void Server::run()
{
    int fps = 0;
    char* b;
    int width,height, size;

    std::vector<uchar> buf;
    std::vector<int> params;
    params.push_back(cv::IMWRITE_JPEG_QUALITY);
    params.push_back(95); // compression quality, ranges between 0-100

    while(true)
    {
        socket.acceptConnection();
        printf("accpetd connection\n");
        auto start = std::chrono::high_resolution_clock::now();

        while(true)
        {
            
            cap.captrue();
            cap.getFrameData(&b, &width,&height, &size);
            cv::Mat wScreen(height, width, CV_8UC4, b);
            if (!cv::imencode(".JPEG", wScreen, buf, params)) printf("JPEG compression failed.\n");
            if(wScreen.data)
            {
                
                if(!sendData(buf)) break;

            }

            fps++;
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
            if(ms.count() > 1000)
            {
                printf("FPS:%d\n", fps);
                fps = 0;
                start = std::chrono::high_resolution_clock::now();
            }


        }
    }

}

bool Server::sendData(cv::Mat buf) const
{
    return sendData(reinterpret_cast<char*>(buf.data), static_cast<uint32_t>(buf.size().area()));
}

bool Server::sendData(std::vector<uchar> buf) const
{
    return sendData(reinterpret_cast<char*>(&buf[0]), static_cast<uint32_t>(buf.size()));
}

bool Server::sendData(const char* buf, int width, int height, int size) const
{

    socket.sendData(&width, 4);
    socket.sendData(&height, 4);
    return sendData(buf, size);
}

bool Server::sendData(const char* buf, uint32_t size) const
{
        uint32_t chank = 65000;
        socket.sendData(&size, 4);
        uint32_t totalSent = 0, len;
        while(size > 0)
        {
            len = size < chank ? size : chank;
            len = socket.sendData((char*)&buf[totalSent], len);
            if(len == (uint32_t)INVALID_SOCKET) return false;

            totalSent += len;
            size -= len;
        }

        return true;

}