#pragma once

#include <WinSock2.h>
#pragma comment (lib, "ws2_32")

#define _XM_NO_INTRINSICS_

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXPackedVector.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")


#include <windows.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <random>

using namespace std;


#include "define.h"
#include "struct.h"
#include "func.h"

extern tTransform	g_Transform;
extern UINT			g_arrSPRegister[(UINT)SHADER_PARAM::END];
extern wchar_t*     g_arrSPName[(UINT)SHADER_PARAM::END];
extern int g_iCloseOrder;