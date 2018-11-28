#pragma once
#include "PacketSync.h"

SERVER_BEGIN

class CPacketQueue :
	public CPacketSync
{
private:
	CPacketQueue() {}
	virtual ~CPacketQueue() { SAFE_DELETE_ARRAY(m_tPacket); }

private:
	char m_PacketStream[PACKET_SIZE];
	int m_iRemainData;

public:
	template <typename T>
	bool AddData(T* _pData, unsigned int _iSize)
	{
		Lock();
		memcpy(m_PacketStream + m_iRemainData, _pData, _iSize);
		m_iRemainData += _iSize;
		Unlock();

		return true;
	}

	bool GetData()
	{
		Lock();
		while (true)
		{
			// ���� ���̰� 4����Ʈ���� ������ ����
			if (m_iRemainData < sizeof(DWORD))
				break;;

			//��Ŷ ���� ������
			int iPacketLength = 0;
			CopyMemory(&iPacketLength, m_PacketStream, sizeof(DWORD));

			//���� ���̰� ��Ŷ���̺��� ������ ����
			if (m_iRemainData < iPacketLength)
				break;

			//��Ŷť�� ������������ ����
			if (Full() == true)
				break;

			//��Ŷ���� ������ ����
			PACKET tPacket;
			memcpy(&tPacket, m_PacketStream, iPacketLength);

			//��Ŷť�� ����ֱ�
			int iTail = (m_iTail + 1) % m_iCapacity;
			memcpy(&m_tPacket[iTail], &tPacket, sizeof(PACKET));
			m_iTail = iTail;
			++m_iSize;

			//��Ŷ ��Ʈ������ ��Ŷ��ŭ ������ �̵�
			m_iRemainData -= iPacketLength;
			MoveMemory(m_PacketStream, m_PacketStream + iPacketLength, m_iRemainData);
		}
			
		Unlock();

		return true;
	}

private:
	unsigned int m_iHead;
	unsigned int m_iTail;
	unsigned int m_iSize;
	unsigned int m_iCapacity;
	PACKET* m_tPacket;

public:
	void destroy()
	{
		delete m_tPacket;
	}

	bool init()
	{
		m_iHead = m_iTail = 0;
		m_iSize = 0;
		m_iCapacity = 300;
		m_tPacket = new PACKET[m_iCapacity];
		
		// ��Ŷ ��Ʈ�� �ʱ�ȭ
		m_iRemainData = 0;
		memset(m_PacketStream, 0, sizeof(m_PacketStream));
		return true;
	}

	void Resize(int _iCapacity)
	{
		PACKET* pPacket = new PACKET[_iCapacity];
		memcpy(pPacket, m_tPacket, sizeof(PACKET)* m_iCapacity);
		m_iCapacity = _iCapacity;
		SAFE_DELETE_ARRAY(m_tPacket);
		m_tPacket = pPacket;
	}

	bool Push(const PACKET& _tPacket)
	{
		if (Full() == true)
			return false;

		Lock();
		int iTail = (m_iTail + 1) % m_iCapacity;
		memcpy(&m_tPacket[iTail], &_tPacket, sizeof(PACKET));
		m_iTail = iTail;
		++m_iSize;
		Unlock();

		return true;
	}

	bool Pop(PACKET* _pPacket)
	{
		if (Empty() == true)
			return false;

		Lock();

		int iHead = (m_iHead + 1) % m_iCapacity;
		memcpy(_pPacket, &m_tPacket[iHead], sizeof(PACKET));
		m_iHead = iHead;
		--m_iSize;
		Unlock();

		return true;
	}

	unsigned int Size() const
	{
		return m_iSize;
	}

	void Clear() { m_iHead = m_iTail = m_iSize = 0; }
	bool Full() const {	return (m_iTail + 1) % m_iCapacity == m_iHead; }
	bool Empty() const{	return m_iHead == m_iTail; }

private:
	static CPacketQueue* m_pInst;

public:
	static CPacketQueue* GetInst()
	{
		if (!m_pInst)
			m_pInst = new CPacketQueue;

		return m_pInst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_pInst);
	}
};

SERVER_END