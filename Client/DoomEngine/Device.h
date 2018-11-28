#pragma once
#include "global.h"

class CDevice
{
private:
	HWND m_hWnd;
	bool m_bWindowMode;

	ID3D11Device*			m_pDevice;				// ����̽�
	ID3D11DeviceContext*	m_pContext;				// ���ؽ�Ʈ

	IDXGISwapChain*			m_pSwapChain;			// ���� ü��
	ID3D11RenderTargetView* m_pTargetView;			// ��� Ÿ�� ��
	
	ID3D11Texture2D*		m_pDepthStencilTex;		// ���� ���ٽǿ� �ؽ���
	ID3D11DepthStencilView* m_pDepthStencilView;	// ���� ���ٽ� ��

	UINT					m_iMS;					// ��Ƽ ���ø�
	float					m_fCol[4];				// �ʱ�ȭ ����

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

