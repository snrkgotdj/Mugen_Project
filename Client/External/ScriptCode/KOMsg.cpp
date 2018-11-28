#include "stdafx.h"
#include "KOMsg.h"


CKOMsg::CKOMsg()
	: m_fTime(0.f)
{
}


CKOMsg::~CKOMsg()
{
}

void CKOMsg::Awake()
{
}

void CKOMsg::Start()
{
}

void CKOMsg::Update()
{
	list<CGameObject*> listChild = GetGameObject()->GetChildList();
	list<CGameObject*>::iterator iter = listChild.begin();
	list<CGameObject*>::iterator iterEnd = listChild.end();

	m_fTime += DT;

	for (iter; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTagName() == L"K")
		{
			(*iter)->GetTransform()->SetLocalScale(Vec3(3000.f*m_fTime, 3600.f*m_fTime, 1.f));
		}
		else if ((*iter)->GetTagName() == L"O")
		{
			if(m_fTime>0.5f)
			(*iter)->GetTransform()->SetLocalScale(Vec3(3000.f*(m_fTime-0.5f), 3600.f*(m_fTime - 0.5f), 1.f));
		}

		if ((*iter)->GetTransform()->GetLocalScale().x > 500.f)
			(*iter)->GetTransform()->SetLocalScale(Vec3(500.f, 600.f, 1.f));
	}
}

void CKOMsg::LateUpdate()
{
	if (m_fTime > 2.f)
		GetGameObject()->Die();
}
