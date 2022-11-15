#pragma once

// Windows
#ifdef _WIN32
#include<windows.h>
// Linux
#else
#endif

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>


#ifdef _WIN32
cv::Mat captrue(HWND hwndTarget);
#else

#endif
