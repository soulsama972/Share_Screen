
#include<windows.h>
#include<iostream>
#include<chrono>
#include<thread>
using namespace std;


HDC hdcScreen;
HDC hdcWindow;
HDC hdcMemDC = NULL;
HBITMAP hbmScreen = NULL;
BITMAPINFOHEADER bi;
uint8_t* buffer;

void q(HDC dest, int xDest,int yDest,int wDest, int hDest, HDC hdcSrc,int xSrc, int ySrc,int wSrc,int hSrc)
{

    printf("%d %d %d %d\n", xDest, yDest, xSrc, ySrc);
    while(true)
    {
        StretchBlt(hdcMemDC,xDest, yDest, wDest,hDest, hdcWindow, xSrc, ySrc, wSrc, hSrc,SRCCOPY);
        GetDIBits(hdcWindow, hbmScreen, 0, bi.biHeight,buffer, (BITMAPINFO*)&bi, DIB_RGB_COLORS);
    }


}

void CaptureScreen(BITMAPINFOHEADER bi, uint8_t* buffer, uint32_t bufferSize,const RECT& rcClient)
{
    // HDC hdcScreen;
    // HDC hdcWindow;
    // HDC hdcMemDC = NULL;
    // HBITMAP hbmScreen = NULL;
    HWND hWnd = GetDesktopWindow();

    hdcScreen = GetDC(NULL);
    hdcWindow = GetDC(hWnd);

    // Create a compatible DC, which is used in a BitBlt from the window DC.
    hdcMemDC = CreateCompatibleDC(hdcWindow);
    //if (!hdcMemDC) MessageBoxA(0, "CreateCompatibleDC has failed", "Failed", MB_OK);

    // Create a compatible bitmap from the Window DC.
    hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
    //if (!hbmScreen) MessageBoxA(0, "CreateCompatibleBitmap Failed", "Failed", MB_OK);
    // Select the compatible bitmap into the compatible memory DC.
    SelectObject(hdcMemDC, hbmScreen);



    BITMAPFILEHEADER bmfHeader;
    DWORD dwBytesWritten = 0;
    HANDLE hFile = NULL;
    HANDLE hDIB = NULL;
    DWORD dwBmpSize = 0;

    int counter = 0;
    auto start = std::chrono::high_resolution_clock::now();
    std::thread ths[4];
    int w = (rcClient.right - rcClient.left) / 4;
    int h = rcClient.bottom - rcClient.top;
    for(int i = 0; i < 4;i++)
        ths[i] = std::thread(q, hdcMemDC, i * w , 0, w, h, hdcWindow,i * w, 0, w, h);

    while (!GetAsyncKeyState(VK_F1))
    {
        //Bit block transfer into our compatible memory DC.
        //BitBlt(hdcMemDC,0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top,hdcWindow,0, 0,SRCCOPY);
        //StretchBlt(hdcMemDC,0, 0, 320, 1440,hdcWindow,0, 0, 320, 1440,SRCCOPY);

        //GetDIBits(hdcWindow, hbmScreen, 0, bi.biHeight,buffer, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

        counter++;
        if(counter == 60)
        {
            counter = 0;
            auto sec = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start);
            std::cout << "Elapsed Time: " << sec.count() << " seconds" << std::endl;
            start = std::chrono::high_resolution_clock::now();
        }
        
            // A file is created, this is where we will save the screen capture.
            hFile = CreateFileA("captureqwsx.bmp",GENERIC_WRITE,0,NULL,CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

            // Add the size of the headers to the size of the bitmap to get the total file size.
            DWORD dwSizeofDIB = bufferSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

            // Offset to where the actual bitmap bits start.
            bmfHeader.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

            // Size of the file.
            bmfHeader.bfSize = dwSizeofDIB;

            // bfType must always be BM for Bitmaps.
            bmfHeader.bfType = 0x4D42; // BM.
            WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten, NULL);
            WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
            WriteFile(hFile, (LPSTR)buffer, bufferSize, &dwBytesWritten, NULL);

            CloseHandle(hFile);
    }


    DeleteObject(hbmScreen);
    DeleteObject(hdcMemDC);
    ReleaseDC(NULL, hdcScreen);
    ReleaseDC(hWnd, hdcWindow);
}


BITMAPINFOHEADER getBitMapHeader(HWND hWnd, uint32_t& bufferSize)
{
    BITMAPINFOHEADER bi;
    RECT rcClient;
    BITMAP bmpScreen;
    GetClientRect(hWnd, &rcClient);
    HDC hdcWindow = GetDC(hWnd);
    
    // Create a compatible bitmap from the Window DC.
    HBITMAP hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

    // Get the BITMAP from the HBITMAP.
    GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmpScreen.bmWidth;
    bi.biHeight = bmpScreen.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    bufferSize = ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

    DeleteObject(hbmScreen);
    ReleaseDC(hWnd, hdcWindow);

    return bi;
}

int main(void)
{
    uint32_t bufferSize = 0;
    bi = getBitMapHeader( GetDesktopWindow(), bufferSize);
    buffer = new uint8_t[bufferSize];
    // Get the client area for size calculation.
    RECT rcClient;
    GetClientRect(GetDesktopWindow(), &rcClient);
    CaptureScreen(bi, buffer, bufferSize,rcClient);
    delete[] buffer;
    return 0;
}