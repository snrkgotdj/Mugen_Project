#pragma once

#include "global.h"

float getRandomNumber(float min, float max);
int getRandomNumber_int(int _min, int _max);

int GetSizeOfFormat(DXGI_FORMAT format);

template<typename T1, typename T2>
void Safe_Delete_Map(map<T1, T2>& _map)
{
	map<T1, T2>::iterator iter = _map.begin();
	for (; iter != _map.end(); ++iter)
	{
		SAFE_DELETE(iter->second);
	}
}

template<typename T1, typename T2>
void Safe_Delete_Map_Res(map<T1, T2>& _map)
{
	map<T1, T2>::iterator iter = _map.begin();
	for (; iter != _map.end(); ++iter)
	{
		if (NULL != iter->second)
			iter->second.Delete();

		iter->second = NULL;
	}
}

template<typename T>
void Safe_Delete_List(list<T>& _list)
{
	list<T>::iterator iter = _list.begin();
	for (; iter != _list.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
}

template<typename T>
void Safe_Delete_Vec(vector<T>& _vector)
{
	vector<T>::iterator iter = _vector.begin();
	for (; iter != _vector.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}
}

template<typename T>
void Safe_Delete_Vec_Res(vector<T>& _vector)
{
	vector<T>::iterator iter = _vector.begin();
	for (; iter != _vector.end(); ++iter)
	{
		(*iter).Delete();
	}
}

void SaveWString(const wstring& _str, FILE* _pFile);
wchar_t* LoadWString(FILE* _pFile);

template<typename T>
void SaveResKey(T* _pRes, FILE* _pFile)
{
	BYTE has = 1;
	if (NULL == _pRes)
		has = 0;

	fwrite(&has, 1, 1, _pFile);

	if (_pRes)
		SaveWString(_pRes->GetKey(), _pFile);
}

wchar_t* LoadResKey(FILE* _pFile);