#include<windows.h>
#include<iostream>
#include<chrono>
#include<thread>
#include<mutex>

#include<opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

void updateWindow(const HWND hwnd,const Mat& mScreen)
{
    BITMAPINFOHEADER bi;
    RECT rect;
    Mat mWindow;
    HDC dc = GetDC(hwnd);
    GetClientRect(hwnd, &rect);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    resize(mScreen, mWindow, Size(width, height), INTER_LINEAR);

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

Mat getCVMatFromHWND(HWND hwndTarget)
{
	HDC deviceContext = GetDC(hwndTarget);
	HDC memoryDeviceContext = CreateCompatibleDC(deviceContext);
	RECT rect;
    cv::Mat mScreen;
	BITMAPINFOHEADER bi;

    GetClientRect(hwndTarget, &rect);

	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	HBITMAP bitmap = CreateCompatibleBitmap(deviceContext, width, height);

	SelectObject(memoryDeviceContext, bitmap);

    BitBlt(memoryDeviceContext, 0, 0, width, height, deviceContext, 0, 0, SRCCOPY);
   
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
	mScreen = cv::Mat(height, width, CV_8UC4);

	GetDIBits(memoryDeviceContext, bitmap, 0, height, mScreen.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	DeleteObject(bitmap);
	DeleteDC(memoryDeviceContext);
	ReleaseDC(hwndTarget, deviceContext);

    return mScreen;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);



void sideThreadUpdateWindow(HWND target)
{
    while(true) 
    {
        updateWindow(target, getCVMatFromHWND(GetDesktopWindow()));
    }
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t* CLASS_NAME  = L"shareWindow";
    WNDCLASSW wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassW(&wc);
    
    HWND ownWindow = CreateWindowExW(0,
        CLASS_NAME,L"Share Screen",WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1800, 1000,
        NULL,NULL,hInstance,NULL);


    ShowWindow(ownWindow, SW_SHOW);

    thread th(sideThreadUpdateWindow, ownWindow);
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}


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