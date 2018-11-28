#pragma once

#define WindowSizeX 1200
#define WindowSizeY 600

#define RET_FAIL -1
#define RET_SUCCESS 1

#define SAFE_RELEASE(p) if(p != NULL) { p->Release(); p = NULL; }
#define SAFE_DELETE(p) if(p != NULL) { delete p; p = NULL; }

#define CLONE(TYPE) TYPE* Clone(){return new TYPE(*this);}


#define DEVICE CDevice::GetInst().GetDevice()
#define CONTEXT CDevice::GetInst().GetContext()
#define DT CTimeManager::Manager().GetDeltaTime()

#define INPUT(key,state)	CKeyManager::Manager().GetKeyState(key,KEY_STATE::state)
#define KEYDOWN(key)		CKeyManager::Manager().GetKeyState(key,KEY_STATE::STATE_DOWN)
#define KEYUP(key)			CKeyManager::Manager().GetKeyState(key,KEY_STATE::STATE_UP)
#define KEYPUSH(key)		CKeyManager::Manager().GetKeyState(key,KEY_STATE::STATE_PUSH)

#define INPUT_PLAYER(key,state)	    CKeyManager::Manager().GetKeyState(key,KEY_STATE::state, m_b1Player)
#define KEYDOWN_PLAYER(key)			CKeyManager::Manager().GetKeyState(key,KEY_STATE::STATE_DOWN, m_b1Player)
#define KEYUP_PLAYER(key)			CKeyManager::Manager().GetKeyState(key,KEY_STATE::STATE_UP, m_b1Player)
#define KEYPUSH_PLAYER(key)			CKeyManager::Manager().GetKeyState(key,KEY_STATE::STATE_PUSH, m_b1Player)

#define ABGR(R, G, B, A) ((A<<24)|(B<<16)|(G << 8)| R)

#define MAX_LAYER 32

typedef unsigned long long DLONG;

typedef XMMATRIX Matrix;

enum class COMPONENT_TYPE
{
	TRANSFORM = 0,
	MESHRENDER,
	COLLIDER,
	ANIMATOR_EX,
	CAMERA,
	SCRIPT,
	END,

	COLLIDER_2D,
	COLLIDER_3D,
};

enum class SHADER_TYPE
{
	ST_VERTEX = 0x01,
	ST_HULL = 0x02,
	ST_DOMAIN = 0x04,
	ST_GEOMETRY = 0x08,
	ST_COMPUTE = 0x10,
	ST_PIXEL = 0x20,
};

enum class SHADER_PARAM
{
	SP_INT_0,
	SP_INT_1,
	SP_INT_2,
	SP_INT_3,
	SP_INT_END,
	SP_FLOAT_0,
	SP_FLOAT_1,
	SP_FLOAT_2,
	SP_FLOAT_3,
	SP_FLOAT_END,
	SP_VEC4_0,
	SP_VEC4_1,
	SP_VEC4_2,
	SP_VEC4_3,
	SP_VEC4_END,
	SP_TEX_0,
	SP_TEX_1,
	SP_TEX_2,
	SP_TEX_3,
	SP_TEX_END,
	END,
};

enum class RS_TYPE
{
	RS_FRONT,
	RS_BACK,
	RS_ALL,
	RS_WIREFRAME,
	END
};

enum MOVE_TYPE
{
	MOVE_TOP,
	MOVE_RIGHT,
	MOVE_LEFT,
	MOVE_BOTTOM,
	MOVE_END
};