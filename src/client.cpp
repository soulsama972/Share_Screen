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
    cv::Mat resizeImg;

    RECT rect;
    GetClientRect(GetDesktopWindow(), &rect);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    while(true) 
    {


        recvData(buffer);
        cv::resize(cv::imdecode(buffer, cv::IMREAD_COLOR), resizeImg, cv::Size(width, height), cv::INTER_LINEAR);
        cv::imshow("aa", resizeImg);
        cv::waitKey(1);
        //updateWindow(target, cv::imdecode(buffer, cv::IMREAD_COLOR));

    }


}


void Client::run()
{

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

Client::Client(char* host)
{
    socket.init(host, 8888, false);
}


void Client::recvData(std::vector<uchar>& buffer)
{
    uint32_t chank = 65000;
    uint32_t size;
    socket.recvData(&size, 4);
    buffer.resize(size);
    uint32_t totalSent = 0, len;
    uchar* buf = new uchar[size];
    while(size > 0)
    {
        len = size < chank ? size : chank;
        len = socket.recvData(buf + totalSent, len);
        if(len == INVALID_SOCKET) break;

        totalSent += len;
        size -= len;
    }

    memcpy(&buffer[0], buf, totalSent);
    delete[] buf;

}