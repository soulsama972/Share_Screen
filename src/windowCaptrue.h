#pragma once

// Windows
#ifdef _WIN32
#include<windows.h>
#include<d3d11.h>
#include<dxgi1_2.h>
// Linux
#else
#endif

#include<iostream>
#include<vector>

constexpr int MAX_FPS = 60;
constexpr int MS = 1000;

class Captrue
{

public:
    ~Captrue();
    Captrue(int fps = MAX_FPS);
    bool init();
    
    inline void getFrameData(char ** buffer, int * _width, int * _height,int* size)
    {
        *buffer = frameBuffer.data();
        *_width = width;
        *_height = height;
        *size = width * height * 4;
    }

    void captrue();

private:
    int fps;
    int width;
    int height;
    std::vector<char> frameBuffer;

#ifdef _WIN32
    ID3D11Device* dev;
    ID3D11DeviceContext* dc;
    IDXGIOutputDuplication* deskDupl;
    DXGI_OUTDUPL_DESC outputDuplDesc;
    ID3D11Texture2D* currTex;

    void updateCurrentTexAndFrameBuffer();
    
    /*
    call Release when you done using;
    */
    IDXGIResource* getResource();
#else

#endif

};