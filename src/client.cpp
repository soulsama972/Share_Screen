#include"client.h"
#include<chrono>

#ifdef _WIN32

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND Client::createWindow(HINSTANCE hInstance)
{
    const char* CLASS_NAME  = "shareWindow";
    WNDCLASSA wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassA(&wc);
    
    HWND window = CreateWindowExA(0,CLASS_NAME,"Share Screen",WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
                     1600, 900,NULL,NULL,hInstance,NULL);

    ShowWindow(window, SW_SHOW);

    return window;
}

void Client::updateWindow(const HWND hwnd,const cv::Mat& mScreen)
{
    BITMAPINFOHEADER bi;
    RECT rect;
    cv::Mat mWindow;
    HDC dc = GetDC(hwnd);
    GetClientRect(hwnd, &rect);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    cv::resize(mScreen, mWindow, cv::Size(width, height), cv::INTER_LINEAR);

	bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biPlanes = 1;
	bi.biBitCount = 32;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0; 
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;
	bi.biWidth = width;
	bi.biHeight = -height;

    SetDIBitsToDevice(dc, 0, 0, width, height, 0, 0, 0, height, mWindow.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    ReleaseDC(hwnd, dc);
}

void Client::sideThreadUpdateWindow(HWND target)
{
    socket.openConnection();
    std::vector<uchar> buffer;

    while(true) 
    {
        recvData(buffer);
        updateWindow(target, cv::Mat(1440, 2560, CV_8UC4, &buffer[0], 10240));
    }


}


void Client::run()
{

    socket.openConnection();
    std::vector<uchar> buffer;
    std::vector<uchar> b;
    cv::Mat resizeImg;
    RECT rect;
    GetClientRect(GetDesktopWindow(), &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    int fps = 0;
    auto start = std::chrono::high_resolution_clock::now();
    // cv::resize(cv::imdecode(buffer, cv::IMREAD_UNCHANGED), resizeImg, cv::Size(width, height), cv::INTER_LINEAR);
    while(true) 
    {
        if(recvData(buffer))
        {


            fps++;
            //resizeImg = cv::Mat(1440, 2560, CV_8UC4, &buffer[0], 10240);
            cv::resize(cv::imdecode(buffer, cv::IMREAD_UNCHANGED), resizeImg, cv::Size(width, height), cv::INTER_LINEAR);
            cv::imshow("hey", resizeImg);
            cv::waitKey(1);
        }
        //updateWindow(target,  cv::imdecode(buffer, cv::IMREAD_UNCHANGED));

        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
        if(ms.count() > 1000)
        {
            printf("FPS:%d\n", fps);
            fps = 0;
            start = std::chrono::high_resolution_clock::now();
        }


    }



    HWND hWindow = createWindow(GetModuleHandle(NULL));
    std::thread th(&Client::sideThreadUpdateWindow, this, hWindow);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

#endif

Client::Client(const char* host, int port)
{
    socket.init(host, port, false);
}


bool Client::recvData(std::vector<uchar>& buffer)
{
    uint32_t chank = 65000;
    uint32_t totalRecv = 0, len,size;

    if(socket.recvData(&size, 4) == (int)INVALID_SOCKET) return false;
    buffer.resize(size);
    
    while(size > 0)
    {
        len = size < chank ? size : chank;
        len = socket.recvData(&buffer[totalRecv], len);
        if(len == (uint32_t)INVALID_SOCKET) return false;

        totalRecv += len;
        size -= len;
    }

    return true;
}