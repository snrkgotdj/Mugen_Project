#include "CCore.h"
#include "Device.h"

#include "PathManager.h"
#include "KeyManager.h"
#include "TimeManager.h"
#include "ShaderManager.h"
#include "SceneManager.h"
#include "ResManager.h"
#include "FontMgr.h"

#include "GameObject.h"
#include "TransForm.h"
#include "MeshRender.h"

#include "Mesh.h"
#include "Shader.h"
#include "Sound.h"

int g_iCloseOrder = 0;

CCore::CCore()
	:m_hWnd(NULL)
	, m_fAccTime(0)
{
}

CCore::~CCore()
{
	CSceneManager::Manager().DestoryInst();
	CResManager::Manager().DestoryInst();
}

int CCore::init(HWND _hWnd, bool _bWindowMode)
{
	m_hWnd = _hWnd;

	m_hDC = GetDC(m_hWnd);

	if (FAILED(CDevice::GetInst().init(_hWnd, _bWindowMode)))
	{
		MessageBox(_hWnd, _T("장치 초기화 실패"), _T("확인"), MB_OK);
		return RET_FAIL;
	}

	float fCol[4] = { 1.f,1.f, 1.f, 1.f };
	CDevice::GetInst().SetClearColor(fCol);

	//매니저 초기화
	CPathManager::init();
	CTimeManager::Manager().init();
	CKeyManager::Manager().init();
	CShaderManager::Manager().init();
	CResManager::Manager().init();
	CSceneManager::Manager().init();
	CFontMgr::Manager().init();

	CDevice::GetInst().AddConstBuffer(L"Transform"
		, sizeof(tTransform)
		, D3D11_USAGE_DYNAMIC
		, 0);

	CDevice::GetInst().AddConstBuffer(g_arrSPName[(UINT)SHADER_PARAM::SP_INT_END]
		, sizeof(int)* ((UINT)SHADER_PARAM::SP_INT_END - (UINT)SHADER_PARAM::SP_INT_0)
		, D3D11_USAGE_DYNAMIC
		, g_arrSPRegister[(UINT)SHADER_PARAM::SP_INT_END]);

	CDevice::GetInst().AddConstBuffer(g_arrSPName[(UINT)SHADER_PARAM::SP_FLOAT_END]
		, sizeof(float)* ((UINT)SHADER_PARAM::SP_FLOAT_END - (UINT)SHADER_PARAM::SP_FLOAT_0)
		, D3D11_USAGE_DYNAMIC
		, g_arrSPRegister[(UINT)SHADER_PARAM::SP_FLOAT_END]);

	CDevice::GetInst().AddConstBuffer(g_arrSPName[(UINT)SHADER_PARAM::SP_VEC4_END]
		, sizeof(Vec4)* ((UINT)SHADER_PARAM::SP_VEC4_END - (UINT)SHADER_PARAM::SP_VEC4_0)
		, D3D11_USAGE_DYNAMIC
		, g_arrSPRegister[(UINT)SHADER_PARAM::SP_VEC4_END]);

	CDevice::GetInst().AddConstBuffer(L"AnimationInfo"
		, sizeof(tAnimInfo)
		, D3D11_USAGE_DYNAMIC
		, 12);

	return RET_SUCCESS;
}

int CCore::Run()
{
	Update();

	Render();

	return 0;
}

void CCore::Update()
{
	CTimeManager::Manager().Update();
	m_fAccTime += CTimeManager::Manager().GetFrameTime();
	if (m_fAccTime >= 1.f / 60.f)
	{
		CKeyManager::Manager().Update();
		CSceneManager::Manager().progress();
		CSound::g_pSystem->update();
		m_fAccTime = 0;
	}
}
void CCore::Render()
{
	CDevice::GetInst().ClearTarget();

	//CSceneManager::Manager().Render();
	CTimeManager::Manager().Render(m_hDC);

	CDevice::GetInst().Present();

	//CTimeManager::Manager().Render(m_hDC);
}

void CCore::FPSRender()
{
	CTimeManager::Manager().Render(m_hDC);
}

