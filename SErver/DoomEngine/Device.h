#pragma once
#include "global.h"

class CDevice
{
private:
	HWND m_hWnd;
	bool m_bWindowMode;

	ID3D11Device*			m_pDevice;				// 디바이스
	ID3D11DeviceContext*	m_pContext;				// 컨텍스트

	IDXGISwapChain*			m_pSwapChain;			// 스왚 체인
	ID3D11RenderTargetView* m_pTargetView;			// 출력 타겟 뷰
	
	ID3D11Texture2D*		m_pDepthStencilTex;		// 깊이 스텐실용 텍스쳐
	ID3D11DepthStencilView* m_pDepthStencilView;	// 깊이 스텐실 뷰

	UINT					m_iMS;					// 멀티 샘플링
	float					m_fCol[4];				// 초기화 색상

	map<wstring, BUFFER>	m_mapConstBuffer;

	ID3D11RasterizerState* m_arrRS[(UINT)RS_TYPE::END];



public:
	void SetClearColor(float _fCol[4]){	memcpy(m_fCol, _fCol, sizeof(float) * 4);}
	void ClearTarget();
	void Present() { m_pSwapChain->Present(0, 0); }

	ID3D11Device* GetDevice() { return m_pDevice; }
	ID3D11DeviceContext* GetContext() { return m_pContext; }

	void SetRSState(RS_TYPE _eRSType) { CONTEXT->RSSetState(m_arrRS[(UINT)_eRSType]); }

private:
	int CreateSwapChain();
	int CreateView();
	int CreateViewPort();
	int CreateRasterizerState();

public:
	int AddConstBuffer(const wstring& _strKey, UINT _iSize, D3D11_USAGE _eUsage, UINT _iRegister);
	const BUFFER* FindConstBuffer(const wstring & _strKey);

public:
	int init(HWND _hWnd, bool _bWindowMode);

public:
	static CDevice& GetInst()
	{
		static CDevice inst;
		return inst;
	}

private:
	CDevice();
	~CDevice();
};

