#include "ResPtr.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Texture.h"
#include "Prefab.h"
#include "Material.h"
#include "Res.h"
#include "Sound.h"

template class CResPtr<CMesh>;
template class CResPtr<CTexture>;
template class CResPtr<CPrefab>;
template class CResPtr<CMaterial>;
template class CResPtr<CRes>;
template class CResPtr<CSound>;

template<typename T>
CResPtr<T>::CResPtr()
	: m_pTarget(NULL)
{
}

template<typename T>
CResPtr<T>::CResPtr(T * _pTarget)
	:m_pTarget(_pTarget)
{
	if (NULL != m_pTarget)
		m_pTarget->AddRef();
}

template<typename T>
CResPtr<T>::CResPtr(const CResPtr & _Other)
	:m_pTarget(_Other.m_pTarget)
{
	if (NULL != m_pTarget)
		m_pTarget->AddRef();
}

template<typename T>
CResPtr<T>::~CResPtr()
{
	if (NULL != m_pTarget)
		m_pTarget->SubRef();
}

template<typename T>
void CResPtr<T>::operator=(T* _pTarget)
{
	if (m_pTarget != _pTarget && m_pTarget != NULL)
	{
		m_pTarget->SubRef();
	}

	m_pTarget = _pTarget;

	if (NULL != m_pTarget)
	{
		m_pTarget->AddRef();
	}
}

template<typename T>
void CResPtr<T>::Delete()
{
	if (NULL != m_pTarget)
		delete m_pTarget;

	m_pTarget = NULL;
}
