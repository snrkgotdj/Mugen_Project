#pragma once
#include "global.h"

class CGameObject;

class MouseManager
{
private:
	CGameObject* m_pGameObject;
	wstring m_str;
	bool m_bDragOn;

public:
	void SetGameObject(CGameObject* _pObj) { m_bDragOn = true; m_pGameObject = _pObj; }
	void SetText(const wstring& _str) { m_str = _str; }
	const wstring& GetText() { return m_str; }

	CGameObject* GetGameObject() 
	{ 
		if (m_bDragOn == true)
		{
			m_bDragOn = false;
			return m_pGameObject;
		}
		return NULL;
	}

public:
	static MouseManager& Manager()
	{
		static MouseManager inst;
		return inst;
	}

private:
	MouseManager();
	~MouseManager();
};

