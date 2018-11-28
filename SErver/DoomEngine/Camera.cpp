#include "Camera.h"
#include "Layer.h"
#include "SceneManager.h"
#include "KeyManager.h"
#include "TimeManager.h"

#include "TransForm.h"
#include "Scene.h"

CCamera::CCamera()
	: m_fFOV(XM_PI/ 4.f)
	, m_fWidth(WindowSizeX)
	, m_fHeight(WindowSizeY)
	, m_fScale(1.f)
	, m_fNear(-1.f)
	, m_fFar(1000.f)
	, m_bPerspective(false)
	, m_iLayerMask(0)
{
}


CCamera::~CCamera()
{
}


void CCamera::Update()
{
	Vec3 LocalPos = GetTransform()->GetLocalPos();

	if (CKeyManager::Manager().GetKeyState(KEY_F1, STATE_PUSH) == true)
	{
		m_fScale += DT;
		if (m_fScale >= 4)
		{
			m_fScale = 4;
		}
		GetCamera()->SetScale(m_fScale);
	}

	else if (CKeyManager::Manager().GetKeyState(KEY_F2, STATE_PUSH) == true)
	{
		m_fScale -= DT;
		if (m_fScale <= 0.05f)
			m_fScale = 0.05f;
		GetCamera()->SetScale(m_fScale);
	}

	////////////////////
	//if (CKeyManager::Manager().GetKeyState(KEY_D, STATE_PUSH) == true)
	//	LocalPos.x += 300 * DT;
	//
	//if (CKeyManager::Manager().GetKeyState(KEY_A, STATE_PUSH) == true)
	//	LocalPos.x -= 300 * DT;
	//
	//if (CKeyManager::Manager().GetKeyState(KEY_S, STATE_PUSH) == true)
	//	LocalPos.y -= 300 * DT;
	//
	//if (CKeyManager::Manager().GetKeyState(KEY_W, STATE_PUSH) == true)
	//	LocalPos.y += 300 * DT;

	GetTransform()->SetLocalPos(LocalPos);
}

void CCamera::FinalUpdate()
{
	Vec3 vWorldPos = GetTransform()->GetWorldPos();

	m_matView = XMMatrixIdentity();
	Vec3* pMatPos = (Vec3*)&m_matView._41;
	*pMatPos = -vWorldPos;


	if (m_bPerspective)
		m_matProj = XMMatrixPerspectiveFovLH(m_fFOV, (float)WindowSizeX / (float)WindowSizeY, m_fNear, m_fFar);
	else
		m_matProj = XMMatrixOrthographicLH(m_fWidth * m_fScale, m_fHeight * m_fScale, m_fNear, m_fFar);

}

void CCamera::Render(CLayer* _pLayer)
{
	int iIdx = _pLayer->GetLayerIdx();
	if (false == (m_iLayerMask& (1 << iIdx)))
	{
		return;
	}

	g_Transform.matView = XMMatrixTranspose(m_matView);
	g_Transform.matProj = XMMatrixTranspose(m_matProj);

	_pLayer->CheckOnCamRange(m_fNear, m_fFar, m_fScale);
	_pLayer->Render();
}

void CCamera::CheckRenderLayer(const wstring & _strLayerName)
{
	CScene* pCurScene = CSceneManager::Manager().GetCurScene();
	CLayer* pLayer = pCurScene->FindLayer(_strLayerName);

	if (NULL == pLayer)
		assert(NULL);

	UINT iIdx = pLayer->GetLayerIdx();
	if (m_iLayerMask & (1 << iIdx))
	{
		m_iLayerMask &= ~(1 << iIdx);
	}

	else
	{
		m_iLayerMask |= (1 << iIdx);
	}

}

void CCamera::Save(FILE* _pFile)
{
	UINT iType = (UINT)COMPONENT_TYPE::CAMERA;
	fwrite(&iType, sizeof(UINT), 1, _pFile);

	fwrite(&m_bPerspective, 1, 1, _pFile);
	fwrite(&m_fFOV, sizeof(float), 1, _pFile);
	fwrite(&m_fWidth, sizeof(float), 1, _pFile);
	fwrite(&m_fHeight, sizeof(float), 1, _pFile);
	fwrite(&m_fScale, sizeof(float), 1, _pFile);
	fwrite(&m_fNear, sizeof(float), 1, _pFile);
	fwrite(&m_fFar, sizeof(float), 1, _pFile);
	fwrite(&m_iLayerMask, sizeof(float), 1, _pFile);
}

void CCamera::Load(FILE* _pFile)
{
	fread(&m_bPerspective, 1, 1, _pFile);
	fread(&m_fFOV, sizeof(float), 1, _pFile);
	fread(&m_fWidth, sizeof(float), 1, _pFile);
	fread(&m_fHeight, sizeof(float), 1, _pFile);
	fread(&m_fScale, sizeof(float), 1, _pFile);
	fread(&m_fNear, sizeof(float), 1, _pFile);
	fread(&m_fFar, sizeof(float), 1, _pFile);
	fread(&m_iLayerMask, sizeof(float), 1, _pFile);
}