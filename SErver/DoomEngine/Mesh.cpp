#include "Mesh.h"
#include "Device.h"

#include "Shader.h"

CMesh::CMesh()
	:m_pVB(NULL)
	,m_iVtxCount(0)
	,m_iVtxSize(0)
	,m_eVtxUsage(D3D11_USAGE_DEFAULT)
	,m_pIB(NULL)
	,m_iIdxCount(0)
	,m_iIdxSize(0)
	,m_eIdxUsage(D3D11_USAGE_DEFAULT)
	,m_eIdxFormat(DXGI_FORMAT_UNKNOWN)
	,m_eTopology(D3D10_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_UNDEFINED)
	, m_pLayout(NULL)
	, m_iDescOffset(0)
	, m_pRecentShader(NULL)
{
}

CMesh::~CMesh()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pLayout);
	SAFE_DELETE(m_pVtxSysMem);
	SAFE_DELETE(m_pIdxSysMem);
}

CMesh * CMesh::Create(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage
	, void * _pVtxSysMem, UINT _iIdxCount, UINT _iIdxSize
	, D3D11_USAGE _eIdxUsage, void * _pIdxSysMem, DXGI_FORMAT _eIdxFormat
	, D3D11_PRIMITIVE_TOPOLOGY _eTopology)
{
	D3D11_BUFFER_DESC tBufferDesc = {};

	tBufferDesc.ByteWidth = _iVtxSize * _iVtxCount;
	tBufferDesc.Usage = _eVtxUsage;
	
	if (tBufferDesc.Usage == D3D11_USAGE_DYNAMIC)
	{
		tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = _pVtxSysMem;

	ID3D11Buffer* pVB = NULL;

	if (FAILED(DEVICE->CreateBuffer(&tBufferDesc, &tSub, &pVB)))
	{
		return NULL;
	}

	memset(&tBufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	tBufferDesc.ByteWidth = _iIdxSize* _iIdxCount;
	tBufferDesc.Usage = _eIdxUsage;

	if (D3D11_USAGE_DYNAMIC == tBufferDesc.Usage)
	{
		tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	tSub.pSysMem = _pIdxSysMem;

	ID3D11Buffer* pIB = NULL;

	if (FAILED(DEVICE->CreateBuffer(&tBufferDesc, &tSub, &pIB)))
	{
		return NULL;
	}

	CMesh* pMesh = new CMesh;

	pMesh->m_pVB = pVB;
	pMesh->m_iVtxSize = _iVtxSize;
	pMesh->m_iVtxCount = _iVtxCount;
	pMesh->m_eVtxUsage = _eVtxUsage;

	pMesh->m_pIB = pIB;
	pMesh->m_iIdxSize = _iIdxSize;
	pMesh->m_iIdxCount = _iIdxCount;
	pMesh->m_eIdxUsage = _eIdxUsage;
	pMesh->m_eIdxFormat = _eIdxFormat;

	pMesh->m_eTopology = _eTopology;

	pMesh->SetVtxSysMem(_pVtxSysMem);
	pMesh->SetIdxSysMem(_pIdxSysMem);

	return pMesh;
}

void CMesh::AddDesc(char * _strSemantic, UINT _iSemanticIdx
	, DXGI_FORMAT _eFormat, UINT _iInputSlot, UINT _iInstancing)
{
	D3D11_INPUT_ELEMENT_DESC tDesc;
	tDesc.SemanticName = _strSemantic;
	tDesc.SemanticIndex = _iSemanticIdx;
	tDesc.Format = _eFormat;
	tDesc.InputSlot = _iInputSlot;
	tDesc.InstanceDataStepRate = _iInstancing;
	tDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tDesc.AlignedByteOffset = m_iDescOffset;

	m_iDescOffset += GetSizeOfFormat(_eFormat);

	m_vecDesc.push_back(tDesc);
}

int CMesh::SetShader(CShader * _pShader)
{
	if (m_pRecentShader == _pShader)
	{
		return RET_SUCCESS;
	}
	m_pRecentShader = _pShader;

	SAFE_RELEASE(m_pLayout);

	if (FAILED(DEVICE->CreateInputLayout(&m_vecDesc[0], m_vecDesc.size()
		, m_pRecentShader->GetVSBlob()->GetBufferPointer()
		, m_pRecentShader->GetVSBlob()->GetBufferSize()
		, &m_pLayout)))
	{
		return RET_FAIL;
	}

	return RET_SUCCESS;
}

void CMesh::UpdateData()
{
	CONTEXT->IASetInputLayout(m_pLayout);

	UINT iOffset = 0;
	UINT iSize = m_iVtxSize;

	CONTEXT->IASetVertexBuffers(0, 1, &m_pVB, &iSize, &iOffset);
	CONTEXT->IASetPrimitiveTopology(m_eTopology);
	CONTEXT->IASetIndexBuffer(m_pIB, m_eIdxFormat, 0);
}

void CMesh::Render()
{
	CONTEXT->DrawIndexed(m_iIdxCount, 0, 0);
}

void CMesh::SetVtxSysMem(void * _pVtxSysMem)
{
	m_pVtxSysMem = malloc(m_iVtxCount * m_iVtxSize);
	memcpy(m_pVtxSysMem, _pVtxSysMem, m_iVtxCount* m_iVtxSize);
}

void CMesh::SetIdxSysMem(void * _pIdxSysMem)
{
	m_pIdxSysMem = malloc(m_iIdxCount * m_iIdxSize);
	memcpy(m_pIdxSysMem, _pIdxSysMem, m_iIdxCount* m_iIdxSize);
}
