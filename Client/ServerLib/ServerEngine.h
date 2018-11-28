#pragma once
#include <WinSock2.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <process.h>
#include <string>
#include <iostream>
#include <tchar.h>

using namespace std;

#pragma comment (lib, "ws2_32")

#define PACKET_SIZE 1024

#define READ_COMPLETE 2
#define WRITE_COMPLETE 4
#define CLOSE_COMPLETE 8

#define SERVER_BEGIN namespace ServerEngine{
#define SERVER_END }
#define SERVER_USING using namespace ServerEngine;

SERVER_BEGIN

#define	SAFE_DELETE(p)	if(p)	{ delete p; p = NULL; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = NULL; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete p; p = NULL; }

#define	DECLARE_SINGLE(Type)	\
	private:\
		static Type*	m_pInst;\
	public:\
		static Type* GetInst()\
		{\
			if(!m_pInst)\
				m_pInst	= new Type;\
			return m_pInst;\
		}\
		static void DestroyInst()\
		{\
			SAFE_DELETE(m_pInst);\
		}\
	private:\
		Type();\
		~Type();

#define	DEFINITION_SINGLE(Type)	Type* Type::m_pInst	= NULL;
#define	GET_SINGLE(Type)		Type::GetInst()
#define	DESTROY_SINGLE(Type)	Type::DestroyInst()



template<typename T>
void Safe_Delete_VecList(T& p)
{
	T::iterator iter;
	T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	p.clear();
}

template<typename T>
void Safe_Delete_Map(T& p)
{
	T::iterator iter;
	T::iterator iterEnd = p.end();

	for (iter = p.begin(); iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	p.clear();
}

typedef struct _tagSocketInfo
{
	SOCKET hSocket;
	SOCKADDR_IN tAddr;
}SOCKETINFO, *PSOCKETINFO;

typedef struct _tagBuffer
{
	OVERLAPPED tOverlapped;
	char cBuffer[PACKET_SIZE];
	WSABUF tBuf;
	unsigned int iType;
}BUFFER, *PBUFFER;

typedef struct _tagPacket
{
	unsigned int iSize;
	unsigned int iHeader;
	char cPacket[PACKET_SIZE];
	class CPacket* pPacket;
}PACKET, *PPACKET;

SERVER_END