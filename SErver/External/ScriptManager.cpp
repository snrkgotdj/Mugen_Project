#include "stdafx.h"
#include "ScriptManager.h"

#include "ScriptCode\BulletMgr.h"
#include "ScriptCode\CameraScript.h"
#include "ScriptCode\DownColScript.h"
#include "ScriptCode\EffectManager.h"
#include "ScriptCode\EffectScript.h"
#include "ScriptCode\GameManager.h"
#include "ScriptCode\GameMsg.h"
#include "ScriptCode\IoriBullet.h"
#include "ScriptCode\IoriHitCol.h"
#include "ScriptCode\KOMsg.h"
#include "ScriptCode\KyoEffect.h"
#include "ScriptCode\KyoHitCol.h"
#include "ScriptCode\KyoLoad.h"
#include "ScriptCode\LayerManager.h"
#include "ScriptCode\MapScript.h"
#include "ScriptCode\ModeMgr.h"
#include "ScriptCode\NanayaHitCol.h"
#include "ScriptCode\NanayaScript.h"
#include "ScriptCode\NanayaTestScript.h"
#include "ScriptCode\PlayerManager.h"
#include "ScriptCode\PlayerScIori.h"
#include "ScriptCode\PlayerScKyo.h"
#include "ScriptCode\ShadowScript.h"
#include "ScriptCode\SoundLoadMgr.h"
#include "ScriptCode\TestScIori.h"
#include "ScriptCode\TestScKyo.h"
#include "ScriptCode\TestScript.h"
#include "ScriptCode\UIBar.h"
#include "ScriptCode\UIBarBase.h"
#include "ScriptCode\UIBtnOutput.h"
#include "ScriptCode\UICharacterSel.h"
#include "ScriptCode\UICmdList.h"
#include "ScriptCode\UICombo.h"
#include "ScriptCode\UIFont.h"
#include "ScriptCode\UIGuardBar.h"
#include "ScriptCode\UIManager.h"
#include "ScriptCode\UIMatchWait.h"
#include "ScriptCode\UIPanel.h"
#include "ScriptCode\UITimer.h"
#include "ScriptCode\UITitleScreen.h"
#include "ScriptCode\UpColScript.h"

void CScriptMgr::GetAllScriptName(vector<tScriptInfo>& _vecScriptName)
{
	tScriptInfo info;

	info.strScriptName = L"BulletMgr";
	info.dwTypeID = typeid(CBulletMgr).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"CameraScript";
	info.dwTypeID = typeid(CCameraScript).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"DownColScript";
	info.dwTypeID = typeid(CDownColScript).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"EffectManager";
	info.dwTypeID = typeid(CEffectManager).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"EffectScript";
	info.dwTypeID = typeid(CEffectScript).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"GameManager";
	info.dwTypeID = typeid(CGameManager).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"GameMsg";
	info.dwTypeID = typeid(CGameMsg).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"IoriBullet";
	info.dwTypeID = typeid(CIoriBullet).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"IoriHitCol";
	info.dwTypeID = typeid(CIoriHitCol).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"KOMsg";
	info.dwTypeID = typeid(CKOMsg).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"KyoEffect";
	info.dwTypeID = typeid(CKyoEffect).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"KyoHitCol";
	info.dwTypeID = typeid(CKyoHitCol).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"KyoLoad";
	info.dwTypeID = typeid(CKyoLoad).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"LayerManager";
	info.dwTypeID = typeid(CLayerManager).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"MapScript";
	info.dwTypeID = typeid(CMapScript).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"ModeMgr";
	info.dwTypeID = typeid(CModeMgr).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"NanayaHitCol";
	info.dwTypeID = typeid(CNanayaHitCol).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"NanayaScript";
	info.dwTypeID = typeid(CNanayaScript).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"NanayaTestScript";
	info.dwTypeID = typeid(CNanayaTestScript).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"PlayerManager";
	info.dwTypeID = typeid(CPlayerManager).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"PlayerScIori";
	info.dwTypeID = typeid(CPlayerScIori).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"PlayerScKyo";
	info.dwTypeID = typeid(CPlayerScKyo).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"ShadowScript";
	info.dwTypeID = typeid(CShadowScript).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"SoundLoadMgr";
	info.dwTypeID = typeid(CSoundLoadMgr).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"TestScIori";
	info.dwTypeID = typeid(CTestScIori).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"TestScKyo";
	info.dwTypeID = typeid(CTestScKyo).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"TestScript";
	info.dwTypeID = typeid(CTestScript).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"UIBar";
	info.dwTypeID = typeid(CUIBar).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"UIBarBase";
	info.dwTypeID = typeid(CUIBarBase).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"UIBtnOutput";
	info.dwTypeID = typeid(CUIBtnOutput).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"UICharacterSel";
	info.dwTypeID = typeid(CUICharacterSel).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"UICmdList";
	info.dwTypeID = typeid(CUICmdList).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"UICombo";
	info.dwTypeID = typeid(CUICombo).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"UIFont";
	info.dwTypeID = typeid(CUIFont).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"UIGuardBar";
	info.dwTypeID = typeid(CUIGuardBar).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"UIManager";
	info.dwTypeID = typeid(CUIManager).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"UIMatchWait";
	info.dwTypeID = typeid(CUIMatchWait).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"UIPanel";
	info.dwTypeID = typeid(CUIPanel).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"UITimer";
	info.dwTypeID = typeid(CUITimer).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"UITitleScreen";
	info.dwTypeID = typeid(CUITitleScreen).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"UpColScript";
	info.dwTypeID = typeid(CUpColScript).hash_code();
	_vecScriptName.push_back(info);

}

Script * CScriptMgr::GetScript(DWORD _dwTypeID)
{
	if (_dwTypeID == typeid(CBulletMgr).hash_code())
		return new CBulletMgr;
	else if (_dwTypeID == typeid(CCameraScript).hash_code())
		return new CCameraScript;
	else if (_dwTypeID == typeid(CDownColScript).hash_code())
		return new CDownColScript;
	else if (_dwTypeID == typeid(CEffectManager).hash_code())
		return new CEffectManager;
	else if (_dwTypeID == typeid(CEffectScript).hash_code())
		return new CEffectScript;
	else if (_dwTypeID == typeid(CGameManager).hash_code())
		return new CGameManager;
	else if (_dwTypeID == typeid(CGameMsg).hash_code())
		return new CGameMsg;
	else if (_dwTypeID == typeid(CIoriBullet).hash_code())
		return new CIoriBullet;
	else if (_dwTypeID == typeid(CIoriHitCol).hash_code())
		return new CIoriHitCol;
	else if (_dwTypeID == typeid(CKOMsg).hash_code())
		return new CKOMsg;
	else if (_dwTypeID == typeid(CKyoEffect).hash_code())
		return new CKyoEffect;
	else if (_dwTypeID == typeid(CKyoHitCol).hash_code())
		return new CKyoHitCol;
	else if (_dwTypeID == typeid(CKyoLoad).hash_code())
		return new CKyoLoad;
	else if (_dwTypeID == typeid(CLayerManager).hash_code())
		return new CLayerManager;
	else if (_dwTypeID == typeid(CMapScript).hash_code())
		return new CMapScript;
	else if (_dwTypeID == typeid(CModeMgr).hash_code())
		return new CModeMgr;
	else if (_dwTypeID == typeid(CNanayaHitCol).hash_code())
		return new CNanayaHitCol;
	else if (_dwTypeID == typeid(CNanayaScript).hash_code())
		return new CNanayaScript;
	else if (_dwTypeID == typeid(CNanayaTestScript).hash_code())
		return new CNanayaTestScript;
	else if (_dwTypeID == typeid(CPlayerManager).hash_code())
		return new CPlayerManager;
	else if (_dwTypeID == typeid(CPlayerScIori).hash_code())
		return new CPlayerScIori;
	else if (_dwTypeID == typeid(CPlayerScKyo).hash_code())
		return new CPlayerScKyo;
	else if (_dwTypeID == typeid(CShadowScript).hash_code())
		return new CShadowScript;
	else if (_dwTypeID == typeid(CSoundLoadMgr).hash_code())
		return new CSoundLoadMgr;
	else if (_dwTypeID == typeid(CTestScIori).hash_code())
		return new CTestScIori;
	else if (_dwTypeID == typeid(CTestScKyo).hash_code())
		return new CTestScKyo;
	else if (_dwTypeID == typeid(CTestScript).hash_code())
		return new CTestScript;
	else if (_dwTypeID == typeid(CUIBar).hash_code())
		return new CUIBar;
	else if (_dwTypeID == typeid(CUIBarBase).hash_code())
		return new CUIBarBase;
	else if (_dwTypeID == typeid(CUIBtnOutput).hash_code())
		return new CUIBtnOutput;
	else if (_dwTypeID == typeid(CUICharacterSel).hash_code())
		return new CUICharacterSel;
	else if (_dwTypeID == typeid(CUICmdList).hash_code())
		return new CUICmdList;
	else if (_dwTypeID == typeid(CUICombo).hash_code())
		return new CUICombo;
	else if (_dwTypeID == typeid(CUIFont).hash_code())
		return new CUIFont;
	else if (_dwTypeID == typeid(CUIGuardBar).hash_code())
		return new CUIGuardBar;
	else if (_dwTypeID == typeid(CUIManager).hash_code())
		return new CUIManager;
	else if (_dwTypeID == typeid(CUIMatchWait).hash_code())
		return new CUIMatchWait;
	else if (_dwTypeID == typeid(CUIPanel).hash_code())
		return new CUIPanel;
	else if (_dwTypeID == typeid(CUITimer).hash_code())
		return new CUITimer;
	else if (_dwTypeID == typeid(CUITitleScreen).hash_code())
		return new CUITitleScreen;
	else if (_dwTypeID == typeid(CUpColScript).hash_code())
		return new CUpColScript;

	return nullptr;
}

Script * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (_strScriptName == L"BulletMgr")
		return new CBulletMgr;

	if (_strScriptName == L"CameraScript")
		return new CCameraScript;

	if (_strScriptName == L"DownColScript")
		return new CDownColScript;

	if (_strScriptName == L"EffectManager")
		return new CEffectManager;

	if (_strScriptName == L"EffectScript")
		return new CEffectScript;

	if (_strScriptName == L"GameManager")
		return new CGameManager;

	if (_strScriptName == L"GameMsg")
		return new CGameMsg;

	if (_strScriptName == L"IoriBullet")
		return new CIoriBullet;

	if (_strScriptName == L"IoriHitCol")
		return new CIoriHitCol;

	if (_strScriptName == L"KOMsg")
		return new CKOMsg;

	if (_strScriptName == L"KyoEffect")
		return new CKyoEffect;

	if (_strScriptName == L"KyoHitCol")
		return new CKyoHitCol;

	if (_strScriptName == L"KyoLoad")
		return new CKyoLoad;

	if (_strScriptName == L"LayerManager")
		return new CLayerManager;

	if (_strScriptName == L"MapScript")
		return new CMapScript;

	if (_strScriptName == L"ModeMgr")
		return new CModeMgr;

	if (_strScriptName == L"NanayaHitCol")
		return new CNanayaHitCol;

	if (_strScriptName == L"NanayaScript")
		return new CNanayaScript;

	if (_strScriptName == L"NanayaTestScript")
		return new CNanayaTestScript;

	if (_strScriptName == L"PlayerManager")
		return new CPlayerManager;

	if (_strScriptName == L"PlayerScIori")
		return new CPlayerScIori;

	if (_strScriptName == L"PlayerScKyo")
		return new CPlayerScKyo;

	if (_strScriptName == L"ShadowScript")
		return new CShadowScript;

	if (_strScriptName == L"SoundLoadMgr")
		return new CSoundLoadMgr;

	if (_strScriptName == L"TestScIori")
		return new CTestScIori;

	if (_strScriptName == L"TestScKyo")
		return new CTestScKyo;

	if (_strScriptName == L"TestScript")
		return new CTestScript;

	if (_strScriptName == L"UIBar")
		return new CUIBar;

	if (_strScriptName == L"UIBarBase")
		return new CUIBarBase;

	if (_strScriptName == L"UIBtnOutput")
		return new CUIBtnOutput;

	if (_strScriptName == L"UICharacterSel")
		return new CUICharacterSel;

	if (_strScriptName == L"UICmdList")
		return new CUICmdList;

	if (_strScriptName == L"UICombo")
		return new CUICombo;

	if (_strScriptName == L"UIFont")
		return new CUIFont;

	if (_strScriptName == L"UIGuardBar")
		return new CUIGuardBar;

	if (_strScriptName == L"UIManager")
		return new CUIManager;

	if (_strScriptName == L"UIMatchWait")
		return new CUIMatchWait;

	if (_strScriptName == L"UIPanel")
		return new CUIPanel;

	if (_strScriptName == L"UITimer")
		return new CUITimer;

	if (_strScriptName == L"UITitleScreen")
		return new CUITitleScreen;

	if (_strScriptName == L"UpColScript")
		return new CUpColScript;

	return nullptr;
}

wchar_t * CScriptMgr::GetScriptName(Script * _pScript)
{
	if (NULL != dynamic_cast<CBulletMgr*>(_pScript))
		return L"BulletMgr";

	else if (NULL != dynamic_cast<CCameraScript*>(_pScript))
		return L"CameraScript";

	else if (NULL != dynamic_cast<CDownColScript*>(_pScript))
		return L"DownColScript";

	else if (NULL != dynamic_cast<CEffectManager*>(_pScript))
		return L"EffectManager";

	else if (NULL != dynamic_cast<CEffectScript*>(_pScript))
		return L"EffectScript";

	else if (NULL != dynamic_cast<CGameManager*>(_pScript))
		return L"GameManager";

	else if (NULL != dynamic_cast<CGameMsg*>(_pScript))
		return L"GameMsg";

	else if (NULL != dynamic_cast<CIoriBullet*>(_pScript))
		return L"IoriBullet";

	else if (NULL != dynamic_cast<CIoriHitCol*>(_pScript))
		return L"IoriHitCol";

	else if (NULL != dynamic_cast<CKOMsg*>(_pScript))
		return L"KOMsg";

	else if (NULL != dynamic_cast<CKyoEffect*>(_pScript))
		return L"KyoEffect";

	else if (NULL != dynamic_cast<CKyoHitCol*>(_pScript))
		return L"KyoHitCol";

	else if (NULL != dynamic_cast<CKyoLoad*>(_pScript))
		return L"KyoLoad";

	else if (NULL != dynamic_cast<CLayerManager*>(_pScript))
		return L"LayerManager";

	else if (NULL != dynamic_cast<CMapScript*>(_pScript))
		return L"MapScript";

	else if (NULL != dynamic_cast<CModeMgr*>(_pScript))
		return L"ModeMgr";

	else if (NULL != dynamic_cast<CNanayaHitCol*>(_pScript))
		return L"NanayaHitCol";

	else if (NULL != dynamic_cast<CNanayaScript*>(_pScript))
		return L"NanayaScript";

	else if (NULL != dynamic_cast<CNanayaTestScript*>(_pScript))
		return L"NanayaTestScript";

	else if (NULL != dynamic_cast<CPlayerManager*>(_pScript))
		return L"PlayerManager";

	else if (NULL != dynamic_cast<CPlayerScIori*>(_pScript))
		return L"PlayerScIori";

	else if (NULL != dynamic_cast<CPlayerScKyo*>(_pScript))
		return L"PlayerScKyo";

	else if (NULL != dynamic_cast<CShadowScript*>(_pScript))
		return L"ShadowScript";

	else if (NULL != dynamic_cast<CSoundLoadMgr*>(_pScript))
		return L"SoundLoadMgr";

	else if (NULL != dynamic_cast<CTestScIori*>(_pScript))
		return L"TestScIori";

	else if (NULL != dynamic_cast<CTestScKyo*>(_pScript))
		return L"TestScKyo";

	else if (NULL != dynamic_cast<CTestScript*>(_pScript))
		return L"TestScript";

	else if (NULL != dynamic_cast<CUIBar*>(_pScript))
		return L"UIBar";

	else if (NULL != dynamic_cast<CUIBarBase*>(_pScript))
		return L"UIBarBase";

	else if (NULL != dynamic_cast<CUIBtnOutput*>(_pScript))
		return L"UIBtnOutput";

	else if (NULL != dynamic_cast<CUICharacterSel*>(_pScript))
		return L"UICharacterSel";

	else if (NULL != dynamic_cast<CUICmdList*>(_pScript))
		return L"UICmdList";

	else if (NULL != dynamic_cast<CUICombo*>(_pScript))
		return L"UICombo";

	else if (NULL != dynamic_cast<CUIFont*>(_pScript))
		return L"UIFont";

	else if (NULL != dynamic_cast<CUIGuardBar*>(_pScript))
		return L"UIGuardBar";

	else if (NULL != dynamic_cast<CUIManager*>(_pScript))
		return L"UIManager";

	else if (NULL != dynamic_cast<CUIMatchWait*>(_pScript))
		return L"UIMatchWait";

	else if (NULL != dynamic_cast<CUIPanel*>(_pScript))
		return L"UIPanel";

	else if (NULL != dynamic_cast<CUITimer*>(_pScript))
		return L"UITimer";

	else if (NULL != dynamic_cast<CUITitleScreen*>(_pScript))
		return L"UITitleScreen";

	else if (NULL != dynamic_cast<CUpColScript*>(_pScript))
		return L"UpColScript";

	return nullptr;
}

