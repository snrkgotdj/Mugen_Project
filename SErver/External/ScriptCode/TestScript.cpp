#include "stdafx.h"
#include "TestScript.h"

#include "TransForm.h"
#include "KeyManager.h"

CTestScript::CTestScript()
{
}


CTestScript::~CTestScript()
{
}

void CTestScript::Update()
{
	Vec3 vecPos = GetTransform()->GetLocalPos();

	if (true == CKeyManager::Manager().GetKeyState(KEY_RIGHT, STATE_PUSH))
	{
		vecPos.x += 150 * DT;
	}
	else if (true == CKeyManager::Manager().GetKeyState(KEY_LEFT, STATE_PUSH))
	{
		vecPos.x -= 150 * DT;
	}
	else if (true == CKeyManager::Manager().GetKeyState(KEY_UP, STATE_PUSH))
	{
		vecPos.y += 150 * DT;
	}
	else if (true == CKeyManager::Manager().GetKeyState(KEY_DOWN, STATE_PUSH))
	{
		vecPos.y -= 150 * DT;
	}

	GetTransform()->SetLocalPos(vecPos);
}