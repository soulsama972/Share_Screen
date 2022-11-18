#include"windowCaptrue.h"

#ifdef _WIN32



Captrue::Captrue(int _fps)
{
	dev = 0;
	dc = 0;
	currTex = 0;
	deskDupl = 0;
    fps = _fps;
}

Captrue::~Captrue()
{
	if(dev)
	{
		dev->Release();
		dev = 0;
	}

	if(dc)
	{
		dc->Release();
		dc = 0;
	}

	if(deskDupl)
	{
		deskDupl->Release();
		deskDupl = 0;
	}

	if(currTex)
	{
		currTex->Release();
		currTex = 0;
	}
}

bool Captrue::init()
{

	// Driver types supported
	D3D_DRIVER_TYPE gDriverTypes[] = {D3D_DRIVER_TYPE_HARDWARE};
	UINT gNumDriverTypes = ARRAYSIZE(gDriverTypes);

	// Feature levels supported
	D3D_FEATURE_LEVEL gFeatureLevels[] = 
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_1
	};

	UINT gNumFeatureLevels = ARRAYSIZE(gFeatureLevels);
    D3D_FEATURE_LEVEL lFeatureLevel;
    IDXGIAdapter* lDxgiAdapter;
    IDXGIDevice* lDxgiDevice;
    UINT Output = 0;
    IDXGIOutput* lDxgiOutput; 
    IDXGIOutput1* lDxgiOutput1; 
    D3D11_TEXTURE2D_DESC texDesc;  
    HRESULT hr(E_FAIL);

    for (UINT DriverTypeIndex = 0; DriverTypeIndex < gNumDriverTypes; ++DriverTypeIndex)
    {
        hr = D3D11CreateDevice(nullptr,gDriverTypes[DriverTypeIndex], nullptr,0, gFeatureLevels,gNumFeatureLevels,D3D11_SDK_VERSION, &dev,&lFeatureLevel, &dc);
        if (SUCCEEDED(hr)) break;
    }

    if (FAILED(hr) || dev == nullptr) return false;

    // Get DXGI device;
    hr = dev->QueryInterface(__uuidof(IDXGIDevice), (void **)&lDxgiDevice);

    if (FAILED(hr)) return false;

    // Get DXGI adapter
    hr = lDxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&lDxgiAdapter);

    if (FAILED(hr)) return false;

    // Get output
    hr = lDxgiAdapter->EnumOutputs(Output,&lDxgiOutput);
    if (FAILED(hr)) return false;
    lDxgiAdapter->Release();

    hr = lDxgiOutput->QueryInterface(__uuidof(IDXGIOutput1), (void**)&lDxgiOutput1);

    if (FAILED(hr)) return false;

    lDxgiOutput->Release();

    // Create desktop duplication
    hr = lDxgiOutput1->DuplicateOutput(dev, &deskDupl);

    if (FAILED(hr)) return false;

    lDxgiOutput1->Release();

    deskDupl->GetDesc(&outputDuplDesc);


    texDesc.Width = outputDuplDesc.ModeDesc.Width;
    texDesc.Height = outputDuplDesc.ModeDesc.Height;
    texDesc.Format = outputDuplDesc.ModeDesc.Format;
    texDesc.ArraySize = 1;
    texDesc.BindFlags = 0;
    texDesc.MiscFlags = 0;
    texDesc.SampleDesc.Count = 1;
    texDesc.SampleDesc.Quality = 0;
    texDesc.MipLevels = 1;
    texDesc.Usage = D3D11_USAGE_STAGING;
    texDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ |D3D11_CPU_ACCESS_WRITE;


    hr = dev->CreateTexture2D(&texDesc, NULL, &currTex);
    if (FAILED(hr) || currTex == nullptr)
    {
        return false;
    } 

    width = texDesc.Width;
    height = texDesc.Height;
    frameBuffer.reserve(width * height * 4);

    return true;
}

void Captrue::captrue()
{
    updateCurrentTexAndFrameBuffer();
}

IDXGIResource* Captrue::getResource()
{
    HRESULT hr(E_FAIL);
    IDXGIResource* deskRes = nullptr;
    while(true) 
    {
        DXGI_OUTDUPL_FRAME_INFO fi {};
        hr = deskDupl->AcquireNextFrame(MS / fps , &fi, &deskRes);

        if(hr == DXGI_ERROR_WAIT_TIMEOUT || fi.LastPresentTime.QuadPart == 0)
        {
            deskDupl->ReleaseFrame();
            return nullptr;
        }

        if(SUCCEEDED(hr)) return deskRes;
    }
}

void Captrue::updateCurrentTexAndFrameBuffer()
{
    HRESULT hr(E_FAIL);
    IDXGIResource* deskRes = nullptr;
    ID3D11Texture2D*deskImg = nullptr;
    D3D11_MAPPED_SUBRESOURCE pRes;

    deskRes = getResource();
    if(!deskRes) return;

    hr = deskRes->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&deskImg);
    if (FAILED(hr))
    {
        deskRes->Release();
        deskDupl->ReleaseFrame();
        return;
    } 
    dc->CopyResource(currTex, deskImg);

    dc->Map(currTex, 0, D3D11_MAP_READ_WRITE, 0, &pRes);
    memcpy(&frameBuffer[0], pRes.pData, width * height * 4);
    dc->Unmap(currTex, 0); 

    deskRes->Release();
    deskImg->Release();
    deskDupl->ReleaseFrame();

}



#else

Captrue::Captrue(int _fps)
{
    fps = _fps;
}

Captrue::~Captrue()
{

}

bool Captrue::init()
{
	return true;
}


char* Captrue::getFrame()
{

	return frameBuffer;
}

#endif
