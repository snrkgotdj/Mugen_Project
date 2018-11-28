#include "stdafx.h"
#include "ScriptManager.h"

#include "ScriptCode\BulletScript.h"
#include "ScriptCode\CameraScript.h"
#include "ScriptCode\MonsterBulletScript.h"
#include "ScriptCode\MonsterScript.h"
#include "ScriptCode\PlayerScript.h"
#include "ScriptCode\ShieldScript.h"

void CScriptMgr::GetAllScriptName(vector<tScriptInfo>& _vecScriptName)
{
	tScriptInfo info;

	info.strScriptName = L"BulletScript";
	info.dwTypeID = typeid(BulletScript).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"CameraScript";
	info.dwTypeID = typeid(CCameraScript).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"MonsterBulletScript";
	info.dwTypeID = typeid(MonsterBulletScript).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"MonsterScript";
	info.dwTypeID = typeid(MonsterScript).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"PlayerScript";
	info.dwTypeID = typeid(PlayerScript).hash_code();
	_vecScriptName.push_back(info);

	info.strScriptName = L"ShieldScript";
	info.dwTypeID = typeid(ShieldScript).hash_code();
	_vecScriptName.push_back(info);
}

Script * CScriptMgr::GetScript(DWORD _dwTypeID)
{
	if (_dwTypeID == typeid(BulletScript).hash_code())
		return new BulletScript;
	else if (_dwTypeID == typeid(CameraScript).hash_code())
		return new CameraScript;
	else if (_dwTypeID == typeid(MonsterBulletScript).hash_code())
		return new MonsterBulletScript;
	else if (_dwTypeID == typeid(MonsterScript).hash_code())
		return new MonsterScript;
	else if (_dwTypeID == typeid(PlayerScript).hash_code())
		return new PlayerScript;
	else if (_dwTypeID == typeid(ShieldScript).hash_code())
		return new ShieldScript;

	return nullptr;
}

wchar_t * CScriptMgr::GetScriptName(Script * _pScript)
{
	if (NULL != dynamic_cast<BulletScript*>(_pScript))
		return L"BulletScript";

	else if (NULL != dynamic_cast<CameraScript*>(_pScript))
		return L"CameraScript";

	else if (NULL != dynamic_cast<MonsterBulletScript*>(_pScript))
		return L"MonsterBulletScript";

	else if (NULL != dynamic_cast<PlayerScript*>(_pScript))
		return L"PlayerScript";

	else if (NULL != dynamic_cast<MonsterScript*>(_pScript))
		return L"MonsterScript";

	else if (NULL != dynamic_cast<ShieldScript*>(_pScript))
		return L"ShieldScript";

	return nullptr;
}
