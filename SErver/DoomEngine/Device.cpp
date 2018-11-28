#include "Device.h"


CDevice::CDevice()
	:m_hWnd(NULL)
	, m_bWindowMode(false)
	, m_pDevice(NULL)
	, m_pContext(NULL)
	, m_pSwapChain(NULL)
	, m_pTargetView(NULL)
	, m_pDepthStencilView(NULL)
	, m_iMS(4)
	, m_fCol{}
{
}


CDevice::~CDevice()
{
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pContext);

	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pTargetView);
	
	SAFE_RELEASE(m_pDepthStencilTex);
	SAFE_RELEASE(m_pDepthStencilView);

	map<wstring, BUFFER>::iterator iter = m_mapConstBuffer.begin();

	for (; iter != m_mapConstBuffer.end(); ++iter)
	{
		SAFE_RELEASE(iter->second.pConstBuffer);
	}

	for (UINT i = 0; i < (UINT)RS_TYPE::END; ++i)
	{
		SAFE_RELEASE(m_arrRS[i]);
	}
}


int CDevice::init(HWND _hWnd, bool _bWindowMode)
{
	m_hWnd = _hWnd;
	m_bWindowMode = _bWindowMode;

	int iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL eFeatureLv;

	if (FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, iFlag
		, 0, 0, D3D11_SDK_VERSION
		, &m_pDevice, &eFeatureLv, &m_pContext)))
	{
		return RET_FAIL;
	}

	//멀티 샘플 레벨 체크

	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_iMS);
	if(m_iMS <= 0)
	{
		return RET_FAIL;
	}

	if (FAILED(CreateSwapChain()))
	{
		return RET_FAIL;
	}

	if (FAILED(CreateView()))
	{
		return RET_FAIL;
	}

	if (FAILED(CreateViewPort()))
	{
		return RET_FAIL;
	}

	CreateRasterizerState();

	return RET_SUCCESS;
}

void CDevice::ClearTarget()
{
	m_pContext->ClearRenderTargetView(m_pTargetView, m_fCol);
	m_pContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

int CDevice::CreateSwapChain()
{
	DXGI_SWAP_CHAIN_DESC tDesc = {};

	// 버퍼 크기
	tDesc.BufferDesc.Width = WindowSizeX;
	tDesc.BufferDesc.Height = WindowSizeY;

	// 버퍼 갱신 주기
	tDesc.BufferDesc.RefreshRate.Numerator = 60;
	tDesc.BufferDesc.RefreshRate.Denominator = 1;

	// 버퍼의 픽셀 포맷
	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 스캔 라이닝
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// 백버퍼 용도 설정
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// 멀티 샘플 레벨 설정
	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = tDesc.SampleDesc.Count - 1;

	tDesc.OutputWindow = m_hWnd;

	// 스왑체인 옵션
	tDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// 추가 버퍼 개수
	tDesc.BufferCount = 1;

	// 창모드 설정
	tDesc.Windowed = m_bWindowMode;

	IDXGIDevice* pIdxDevice = NULL;
	IDXGIAdapter* pAdapter = NULL;
	IDXGIFactory* pFactory = NULL;

	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pIdxDevice);
	pIdxDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	if (FAILED(pFactory->CreateSwapChain(m_pDevice, &tDesc, &m_pSwapChain)))
	{
		return RET_FAIL;
	}

	SAFE_RELEASE(pFactory);
	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pIdxDevice);

	return RET_SUCCESS;
}

int CDevice::CreateView()
{
	// SwapChain 으로부터 텍스쳐를 얻어온다.
	ID3D11Texture2D* pBackBufferTex = NULL;

	if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTex)))
	{
		return RET_FAIL;
	}

	// 얻어온 텍스쳐로 RenderTargetView를 만든다.
	if (FAILED(m_pDevice->CreateRenderTargetView(pBackBufferTex, 0, &m_pTargetView)))
	{
		return RET_FAIL;
	}

	SAFE_RELEASE(pBackBufferTex);

	// DepthStencilView 용도로 사용할 Texture 를 생성한다.
	D3D11_TEXTURE2D_DESC tDesc = {};

	tDesc.ArraySize = 1;
	tDesc.Width = WindowSizeX;
	tDesc.Height = WindowSizeY;
	tDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;

	tDesc.MipLevels = 1;

	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	m_pDevice->CreateTexture2D(&tDesc, 0, &m_pDepthStencilTex);

	// DepthStencilView 를 만든다
	if (FAILED(m_pDevice->CreateDepthStencilView(m_pDepthStencilTex, 0, &m_pDepthStencilView)))
	{
		return RET_FAIL;
	}

	// RenderTargetView 와 DepthStencilView 를 같이 묶어 OM단계에 전달한다.
	// RenderTarget 여러개랑 DepthStencilView 1 개랑 세트

	m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDepthStencilView);

	return RET_SUCCESS;
}

int CDevice::CreateViewPort()
{
	D3D11_VIEWPORT tDest = {};
	tDest.TopLeftX = 0;
	tDest.TopLeftY = 0;

	tDest.Width = WindowSizeX;
	tDest.Height = WindowSizeY;

	tDest.MaxDepth = 1;
	tDest.MinDepth = 0;

	m_pContext->RSSetViewports(1, &tDest);

	return RET_SUCCESS;
}

int CDevice::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC tDesc = {};

	tDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	tDesc.CullMode = D3D11_CULL_FRONT;
	DEVICE->CreateRasterizerState(&tDesc, &m_arrRS[(UINT)RS_TYPE::RS_BACK]);

	tDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	tDesc.CullMode = D3D11_CULL_NONE;
	DEVICE->CreateRasterizerState(&tDesc, &m_arrRS[(UINT)RS_TYPE::RS_ALL]);

	tDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	tDesc.CullMode = D3D11_CULL_NONE;
	DEVICE->CreateRasterizerState(&tDesc, &m_arrRS[(UINT)RS_TYPE::RS_WIREFRAME]);

	return RET_SUCCESS;
}

int CDevice::AddConstBuffer(const wstring & _strKey, UINT _iSize, D3D11_USAGE _eUsage, UINT _iRegister)
{
	BUFFER buffer = {};

	D3D11_BUFFER_DESC tBufferDesc = {};

	tBufferDesc.ByteWidth = _iSize;
	tBufferDesc.Usage = _eUsage;
	if (tBufferDesc.Usage == D3D11_USAGE_DYNAMIC)
	{
		tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	tBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	if (FAILED(m_pDevice->CreateBuffer(&tBufferDesc, NULL, &buffer.pConstBuffer)))
	{
		return RET_FAIL;
	}

	buffer.iRegister = _iRegister;
	buffer.iSize = _iSize;
	m_mapConstBuffer.insert(make_pair(_strKey, buffer));

	return RET_SUCCESS;
}

const BUFFER * CDevice::FindConstBuffer(const wstring & _strKey)
{
	map<wstring, BUFFER>::iterator iter = m_mapConstBuffer.find(_strKey);

	if (iter == m_mapConstBuffer.end())
		return NULL;

	return &(iter->second);
}