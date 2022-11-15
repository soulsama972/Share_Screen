#include"windowCaptrue.h"

#ifdef _WIN32

cv::Mat captrue(HWND hwndTarget)
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

#else

#endif

