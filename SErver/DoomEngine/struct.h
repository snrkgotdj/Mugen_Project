#pragma once
#include "global.h"

typedef struct _tagVector2
	: public XMFLOAT2
{
public:
	_tagVector2()
	{
		x = y = 0.f;
	}

	_tagVector2(float _x, float _y)
	{
		x = _x;
		y = _y;
	}

	_tagVector2(const _tagVector2& v)
	{
		x = v.x;
		y = v.y;
	}

	_tagVector2(float f[2])
	{
		x = f[0];
		y = f[1];
	}

	// +
	_tagVector2 operator +(const _tagVector2& v)	const
	{
		_tagVector2	v1;
		v1.x = x + v.x;
		v1.y = y + v.y;
		return v1;
	}

	_tagVector2 operator +(float f[2])	const
	{
		_tagVector2	v1;
		v1.x = x + f[0];
		v1.y = y + f[1];
		return v1;
	}

	_tagVector2 operator +(float f)	const
	{
		_tagVector2	v1;
		v1.x = x + f;
		v1.y = y + f;
		return v1;
	}

	_tagVector2 operator +(int i)	const
	{
		_tagVector2	v1;
		v1.x = x + i;
		v1.y = y + i;
		return v1;
	}

	// +=
	void operator +=(const _tagVector2& v)
	{
		x += v.x;
		y += v.y;
	}

	void operator +=(float f[2])
	{
		x += f[0];
		y += f[1];
	}

	void operator +=(float f)
	{
		x += f;
		y += f;
	}

	void operator +=(int i)
	{
		x += (float)i;
		y += (float)i;
	}

	// -
	_tagVector2 operator -(const _tagVector2& v)	const
	{
		_tagVector2	v1;
		v1.x = x - v.x;
		v1.y = y - v.y;
		return v1;
	}

	_tagVector2 operator -(float f[2])	const
	{
		_tagVector2	v1;
		v1.x = x - f[0];
		v1.y = y - f[1];
		return v1;
	}

	_tagVector2 operator -(float f)	const
	{
		_tagVector2	v1;
		v1.x = x - f;
		v1.y = y - f;
		return v1;
	}

	_tagVector2 operator -(int i)	const
	{
		_tagVector2	v1;
		v1.x = x - i;
		v1.y = y - i;
		return v1;
	}

	// -=
	void operator -=(const _tagVector2& v)
	{
		x -= v.x;
		y -= v.y;
	}

	void operator -=(float f[2])
	{
		x -= f[0];
		y -= f[1];
	}

	void operator -=(float f)
	{
		x -= f;
		y -= f;
	}

	void operator -=(int i)
	{
		x -= (float)i;
		y -= (float)i;
	}

	// *
	_tagVector2 operator *(const _tagVector2& v)	const
	{
		_tagVector2	v1;
		v1.x = x * v.x;
		v1.y = y * v.y;
		return v1;
	}

	_tagVector2 operator *(float f[2])	const
	{
		_tagVector2	v1;
		v1.x = x * f[0];
		v1.y = y * f[1];
		return v1;
	}

	_tagVector2 operator *(float f)	const
	{
		_tagVector2	v1;
		v1.x = x * f;
		v1.y = y * f;
		return v1;
	}

	_tagVector2 operator *(int i)	const
	{
		_tagVector2	v1;
		v1.x = x * i;
		v1.y = y * i;
		return v1;
	}

	// *=
	void operator *=(const _tagVector2& v)
	{
		x *= v.x;
		y *= v.y;
	}

	void operator *=(float f[2])
	{
		x *= f[0];
		y *= f[1];
	}

	void operator *=(float f)
	{
		x *= f;
		y *= f;
	}

	void operator *=(int i)
	{
		x *= (float)i;
		y *= (float)i;
	}

	// /
	_tagVector2 operator /(const _tagVector2& v)	const
	{
		_tagVector2	v1;
		v1.x = x / v.x;
		v1.y = y / v.y;
		return v1;
	}

	_tagVector2 operator /(float f[2])	const
	{
		_tagVector2	v1;
		v1.x = x / f[0];
		v1.y = y / f[1];
		return v1;
	}

	_tagVector2 operator /(float f)	const
	{
		_tagVector2	v1;
		v1.x = x / f;
		v1.y = y / f;
		return v1;
	}

	_tagVector2 operator /(int i)	const
	{
		_tagVector2	v1;
		v1.x = x / i;
		v1.y = y / i;
		return v1;
	}

	// *=
	void operator /=(const _tagVector2& v)
	{
		x /= v.x;
		y /= v.y;
	}

	void operator /=(float f[2])
	{
		x /= f[0];
		y /= f[1];
	}

	void operator /=(float f)
	{
		x /= f;
		y /= f;
	}

	void operator /=(int i)
	{
		x /= (float)i;
		y /= (float)i;
	}

	void operator ++()
	{
		++x;
		++y;
	}

	void operator --()
	{
		--x;
		--y;
	}

	bool operator ==(const _tagVector2& v)	const
	{
		return x == v.x && y == v.y;
	}

	bool operator ==(float f[2])	const
	{
		return x == f[0] && y == f[1];
	}

	bool operator !=(const _tagVector2& v)	const
	{
		return x != v.x || y != v.y;
	}

	bool operator !=(float f[2])	const
	{
		return x != f[0] || y != f[1];
	}

	void operator =(const _tagVector2& v)
	{
		x = v.x;
		y = v.y;
	}

	void operator =(float f[2])
	{
		x = f[0];
		y = f[1];
	}

	void operator =(float f)
	{
		x = y = f;
	}

	void operator =(int i)
	{
		x = y = (float)i;
	}

	float& operator [](int idx)
	{
		switch (idx)
		{
		case 0:
			return x;
		case 1:
			return y;
		}
	}

	float& operator [](UINT idx)
	{
		switch (idx)
		{
		case 0:
			return x;
		case 1:
			return y;
		}
	}

	float Length()	const
	{
		return sqrtf(x * x + y * y);
	}

	_tagVector2 Normalize()	const
	{
		_tagVector2	v;
		v.x = x / Length();
		v.y = y / Length();
		return v;
	}

	float Dot(_tagVector2& v)	const
	{
		return x * v.x + y * v.y;
	}

	float Dot(float f[2])	const
	{
		return x * f[0] + y * f[1];
	}

	float Distance(const _tagVector2& v)	const
	{
		_tagVector2	v1 = *this - v;
		return v1.Length();
	}

	float Distance(float f[2])	const
	{
		_tagVector2	v1 = *this - f;
		return v1.Length();
	}

	_tagVector2 GetView(const _tagVector2& vTarget)	const
	{
		_tagVector2	v1 = vTarget - *this;
		return v1.Normalize();
	}

	_tagVector2 GetView(float fTarget[2])	const
	{
		_tagVector2	v = fTarget;
		_tagVector2	v1 = v - *this;
		return v1.Normalize();
	}

	float GetAngle(const _tagVector2& v)	const
	{
		_tagVector2	v1 = v.Normalize();
		_tagVector2	v2 = Normalize();

		float	fAngle = v1.Dot(v2);
		return acosf(fAngle);
	}

	float GetAngle(float f[2])	const
	{
		_tagVector2	v = f;
		_tagVector2	v1 = v.Normalize();
		_tagVector2	v2 = Normalize();

		float	fAngle = v1.Dot(v2);
		return acosf(fAngle);
	}

	XMVECTOR Convert()	const
	{
		// XMFLOAT2 타입을 XMVECTOR 타입으로 변환해주는 함수이다.
		return XMLoadFloat2(this);
	}

	void Convert(const XMVECTOR& v)
	{
		XMStoreFloat2(this, v);
	}
}Vec2, *PVec2;

typedef struct _tagVector3
	: public XMFLOAT3
{
public:
	_tagVector3()
		: _tagVector3(0.f, 0.f, 0.f)
	{
		x = y = z = 0.f;
	}

	_tagVector3(float _x, float _y, float _z)
		: XMFLOAT3(_x, _y, _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	_tagVector3(const _tagVector3& v)
		: XMFLOAT3(v.x, v.y, v.z)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	_tagVector3(float f[3])
		: XMFLOAT3(f[0], f[1], f[2])
	{
		x = f[0];
		y = f[1];
		z = f[2];
	}

	_tagVector3(FXMVECTOR _v)
	{
		XMStoreFloat3(this, _v);
	}

	// +
	_tagVector3 operator +(const _tagVector3& v)	const
	{
		_tagVector3	v1;
		v1.x = x + v.x;
		v1.y = y + v.y;
		v1.z = z + v.z;
		return v1;
	}

	_tagVector3 operator +(float f[3])	const
	{
		_tagVector3	v1;
		v1.x = x + f[0];
		v1.y = y + f[1];
		v1.z = z + f[2];
		return v1;
	}

	_tagVector3 operator +(float f)	const
	{
		_tagVector3	v1;
		v1.x = x + f;
		v1.y = y + f;
		v1.z = z + f;
		return v1;
	}

	_tagVector3 operator +(int i)	const
	{
		_tagVector3	v1;
		v1.x = x + (float)i;
		v1.y = y + (float)i;
		v1.z = z + (float)i;
		return v1;
	}

	// +=
	void operator +=(const _tagVector3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
	}

	void operator +=(float f[3])
	{
		x += f[0];
		y += f[1];
		z += f[2];
	}

	void operator +=(float f)
	{
		x += f;
		y += f;
		z += f;
	}

	void operator +=(int i)
	{
		x += (float)i;
		y += (float)i;
		z += (float)i;
	}

	// -
	_tagVector3 operator -(const _tagVector3& v)	const
	{
		_tagVector3	v1;
		v1.x = x - v.x;
		v1.y = y - v.y;
		v1.z = z - v.z;
		return v1;
	}

	_tagVector3 operator -(float f[3])	const
	{
		_tagVector3	v1;
		v1.x = x - f[0];
		v1.y = y - f[1];
		v1.z = z - f[2];
		return v1;
	}

	_tagVector3 operator -(float f)	const
	{
		_tagVector3	v1;
		v1.x = x - f;
		v1.y = y - f;
		v1.z = z - f;
		return v1;
	}

	_tagVector3 operator -(int i)	const
	{
		_tagVector3	v1;
		v1.x = x - i;
		v1.y = y - i;
		v1.z = z - i;
		return v1;
	}

	// -=
	void operator -=(const _tagVector3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
	}

	void operator -=(float f[3])
	{
		x -= f[0];
		y -= f[1];
		z -= f[2];
	}

	void operator -=(float f)
	{
		x -= f;
		y -= f;
		z -= f;
	}

	void operator -=(int i)
	{
		x -= (float)i;
		y -= (float)i;
		z -= (float)i;
	}

	// *
	_tagVector3 operator *(const _tagVector3& v)	const
	{
		_tagVector3	v1;
		v1.x = x * v.x;
		v1.y = y * v.y;
		v1.z = z * v.z;
		return v1;
	}

	_tagVector3 operator *(float f[3])	const
	{
		_tagVector3	v1;
		v1.x = x * f[0];
		v1.y = y * f[1];
		v1.z = z * f[2];
		return v1;
	}

	_tagVector3 operator *(float f)	const
	{
		_tagVector3	v1;
		v1.x = x * f;
		v1.y = y * f;
		v1.z = z * f;
		return v1;
	}

	_tagVector3 operator *(int i)	const
	{
		_tagVector3	v1;
		v1.x = x * (float)i;
		v1.y = y * (float)i;
		v1.z = z * (float)i;
		return v1;
	}

	// *=
	void operator *=(const _tagVector3& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
	}

	void operator *=(float f[3])
	{
		x *= f[0];
		y *= f[1];
		z *= f[2];
	}

	void operator *=(float f)
	{
		x *= f;
		y *= f;
		z *= f;
	}

	void operator *=(int i)
	{
		x *= (float)i;
		y *= (float)i;
		z *= (float)i;
	}

	// /
	_tagVector3 operator /(const _tagVector3& v)	const
	{
		_tagVector3	v1;
		v1.x = x / v.x;
		v1.y = y / v.y;
		v1.z = z / v.z;
		return v1;
	}

	_tagVector3 operator /(float f[3])	const
	{
		_tagVector3	v1;
		v1.x = x / f[0];
		v1.y = y / f[1];
		v1.z = z / f[2];
		return v1;
	}

	_tagVector3 operator /(float f)	const
	{
		_tagVector3	v1;
		v1.x = x / f;
		v1.y = y / f;
		v1.z = z / f;
		return v1;
	}

	_tagVector3 operator /(int i)	const
	{
		_tagVector3	v1;
		v1.x = x / i;
		v1.y = y / i;
		v1.z = z / i;
		return v1;
	}

	_tagVector3 operator -()
	{
		return _tagVector3(-x, -y, -z);
	}

	// *=
	void operator /=(const _tagVector3& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
	}

	void operator /=(float f[3])
	{
		x /= f[0];
		y /= f[1];
		z /= f[2];
	}

	void operator /=(float f)
	{
		x /= f;
		y /= f;
		z /= f;
	}

	void operator /=(int i)
	{
		x /= (float)i;
		y /= (float)i;
		z /= (float)i;
	}

	void operator ++()
	{
		++x;
		++y;
		++z;
	}

	void operator --()
	{
		--x;
		--y;
		--z;
	}

	bool operator ==(const _tagVector3& v)	const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	bool operator ==(float f[3])	const
	{
		return x == f[0] && y == f[1] && z == f[2];
	}

	bool operator !=(const _tagVector3& v)	const
	{
		return x != v.x || y != v.y || z != v.z;
	}

	bool operator !=(float f[3])	const
	{
		return x != f[0] || y != f[1] || z != f[2];
	}

	void operator =(const _tagVector3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	void operator =(float f[3])
	{
		x = f[0];
		y = f[1];
		z = f[2];
	}

	void operator =(float f)
	{
		x = y = z = f;
	}

	void operator =(int i)
	{
		x = y = z = (float)i;
	}

	float& operator [](UINT idx)
	{
		switch (idx)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		}
	}

	float Length()	const
	{
		return sqrtf(x * x + y * y + z * z);
	}

	_tagVector3 Normalize()	const
	{
		_tagVector3	v;
		v.x = x / Length();
		v.y = y / Length();
		v.z = z / Length();
		return v;
	}

	float Dot(const _tagVector3& v)	const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	float Dot(const float f[3])	const
	{
		return x * f[0] + y * f[1] + z * f[2];
	}

	_tagVector3 Cross(const _tagVector3& v)		const
	{
		return _tagVector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	_tagVector3 Cross(float f[3])		const
	{
		_tagVector3	v = f;
		return _tagVector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	float Distance(const _tagVector3& v)	const
	{
		_tagVector3	v1 = *this - v;
		return v1.Length();
	}

	float Distance(float f[3])	const
	{
		_tagVector3	v1 = *this - f;
		return v1.Length();
	}

	_tagVector3 GetView(const _tagVector3& vTarget)	const
	{
		_tagVector3	v1 = vTarget - *this;
		return v1.Normalize();
	}

	_tagVector3 GetView(float fTarget[3])	const
	{
		_tagVector3	v = fTarget;
		_tagVector3	v1 = v - *this;
		return v1.Normalize();
	}

	float GetAngle(const _tagVector3& v)	const
	{
		_tagVector3	v1 = v.Normalize();
		_tagVector3	v2 = Normalize();

		float	fAngle = v1.Dot(v2);
		return acosf(fAngle);
	}

	float GetAngle(float f[3])	const
	{
		_tagVector3	v = f;
		_tagVector3	v1 = v.Normalize();
		_tagVector3	v2 = Normalize();

		float	fAngle = v1.Dot(v2);
		return acosf(fAngle);
	}

	XMVECTOR Convert()
	{
		// XMFLOAT3 타입을 XMVECTOR 타입으로 변환해주는 함수이다.
		return XMLoadFloat3(this);
	}

	//void Convert(const XMVECTOR& v)
	//{
	//	XMStoreFloat3(this, v);
	//}

	void Lerp(_tagVector3 _v, float _fRate)
	{
		*this = _v*_fRate + (*this)*(1.f - _fRate);
	}
}Vec3, *PVec3;

typedef struct _tagVector4
	: public XMFLOAT4
{
	_tagVector4()
	{
		x = y = z = w = 0.f;
	}

	_tagVector4(float _x, float _y, float _z, float _w)
	{
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	_tagVector4(const _tagVector4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	_tagVector4(const Vec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	_tagVector4(float f[4])
	{
		x = f[0];
		y = f[1];
		z = f[2];
		w = f[3];
	}

	// +
	_tagVector4 operator +(const _tagVector4& v)	const
	{
		_tagVector4	v1;
		v1.x = x + v.x;
		v1.y = y + v.y;
		v1.z = z + v.z;
		v1.w = w + v.w;
		return v1;
	}

	_tagVector4 operator +(float f[4])	const
	{
		_tagVector4	v1;
		v1.x = x + f[0];
		v1.y = y + f[1];
		v1.z = z + f[2];
		v1.w = w + f[3];
		return v1;
	}

	_tagVector4 operator +(float f)	const
	{
		_tagVector4	v1;
		v1.x = x + f;
		v1.y = y + f;
		v1.z = z + f;
		v1.w = w + f;
		return v1;
	}

	_tagVector4 operator +(int i)	const
	{
		_tagVector4	v1;
		v1.x = x + i;
		v1.y = y + i;
		v1.z = z + i;
		v1.w = w + i;
		return v1;
	}

	// +=
	void operator +=(const _tagVector4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
	}

	void operator +=(float f[4])
	{
		x += f[0];
		y += f[1];
		z += f[2];
		w += f[3];
	}

	void operator +=(float f)
	{
		x += f;
		y += f;
		z += f;
		w += f;
	}

	void operator +=(int i)
	{
		x += (float)i;
		y += (float)i;
		z += (float)i;
		w += (float)i;
	}

	// -
	_tagVector4 operator -(const _tagVector4& v)	const
	{
		_tagVector4	v1;
		v1.x = x - v.x;
		v1.y = y - v.y;
		v1.z = z - v.z;
		v1.w = w - v.w;
		return v1;
	}

	_tagVector4 operator -(float f[4])	const
	{
		_tagVector4	v1;
		v1.x = x - f[0];
		v1.y = y - f[1];
		v1.z = z - f[2];
		v1.w = w - f[3];
		return v1;
	}

	_tagVector4 operator -(float f)	const
	{
		_tagVector4	v1;
		v1.x = x - f;
		v1.y = y - f;
		v1.z = z - f;
		v1.w = w - f;
		return v1;
	}

	_tagVector4 operator -(int i)	const
	{
		_tagVector4	v1;
		v1.x = x - i;
		v1.y = y - i;
		v1.z = z - i;
		v1.w = w - i;
		return v1;
	}

	// -=
	void operator -=(const _tagVector4& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
	}

	void operator -=(float f[4])
	{
		x -= f[0];
		y -= f[1];
		z -= f[2];
		w -= f[3];
	}

	void operator -=(float f)
	{
		x -= f;
		y -= f;
		z -= f;
		w -= f;
	}

	void operator -=(int i)
	{
		x -= (float)i;
		y -= (float)i;
		z -= (float)i;
		w -= (float)i;
	}

	// *
	_tagVector4 operator *(const _tagVector4& v)	const
	{
		_tagVector4	v1;
		v1.x = x * v.x;
		v1.y = y * v.y;
		v1.z = z * v.z;
		v1.w = w * v.w;
		return v1;
	}

	_tagVector4 operator *(float f[4])	const
	{
		_tagVector4	v1;
		v1.x = x * f[0];
		v1.y = y * f[1];
		v1.z = z * f[2];
		v1.w = w * f[3];
		return v1;
	}

	_tagVector4 operator *(float f)	const
	{
		_tagVector4	v1;
		v1.x = x * f;
		v1.y = y * f;
		v1.z = z * f;
		v1.w = w * f;
		return v1;
	}

	_tagVector4 operator *(int i)	const
	{
		_tagVector4	v1;
		v1.x = x * i;
		v1.y = y * i;
		v1.z = z * i;
		v1.w = w * i;
		return v1;
	}

	// *=
	void operator *=(const _tagVector4& v)
	{
		x *= v.x;
		y *= v.y;
		z *= v.z;
		w *= v.w;
	}

	void operator *=(float f[4])
	{
		x *= f[0];
		y *= f[1];
		z *= f[2];
		w *= f[3];
	}

	void operator *=(float f)
	{
		x *= f;
		y *= f;
		z *= f;
		w *= f;
	}

	void operator *=(int i)
	{
		x *= (float)i;
		y *= (float)i;
		z *= (float)i;
		w *= (float)i;
	}

	// /
	_tagVector4 operator /(const _tagVector4& v)	const
	{
		_tagVector4	v1;
		v1.x = x / v.x;
		v1.y = y / v.y;
		v1.z = z / v.z;
		v1.w = w / v.w;
		return v1;
	}

	_tagVector4 operator /(float f[4])	const
	{
		_tagVector4	v1;
		v1.x = x / f[0];
		v1.y = y / f[1];
		v1.z = z / f[2];
		v1.w = w / f[3];
		return v1;
	}

	_tagVector4 operator /(float f)	const
	{
		_tagVector4	v1;
		v1.x = x / f;
		v1.y = y / f;
		v1.z = z / f;
		v1.w = w / f;
		return v1;
	}

	_tagVector4 operator /(int i)	const
	{
		_tagVector4	v1;
		v1.x = x / i;
		v1.y = y / i;
		v1.z = z / i;
		v1.w = w / i;
		return v1;
	}

	// *=
	void operator /=(const _tagVector4& v)
	{
		x /= v.x;
		y /= v.y;
		z /= v.z;
		w /= v.w;
	}

	void operator /=(float f[4])
	{
		x /= f[0];
		y /= f[1];
		z /= f[2];
		w /= f[3];
	}

	void operator /=(float f)
	{
		x /= f;
		y /= f;
		z /= f;
		w /= f;
	}

	void operator /=(int i)
	{
		x /= (float)i;
		y /= (float)i;
		z /= (float)i;
		w /= (float)i;
	}

	void operator ++()
	{
		++x;
		++y;
		++z;
		++w;
	}

	void operator --()
	{
		--x;
		--y;
		--z;
		--w;
	}

	bool operator ==(const _tagVector4& v)	const
	{
		return x == v.x && y == v.y && z == v.z && w == v.w;
	}

	bool operator ==(float f[4])	const
	{
		return x == f[0] && y == f[1] && z == f[2] && w == f[3];
	}

	bool operator !=(const _tagVector4& v)	const
	{
		return x != v.x || y != v.y || z != v.z || w != v.w;
	}

	bool operator !=(float f[4])	const
	{
		return x != f[0] || y != f[1] || z != f[2] || w != f[3];
	}

	void operator =(const _tagVector4& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	void operator =(const Vec3& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}

	void operator =(float f[4])
	{
		x = f[0];
		y = f[1];
		z = f[2];
		w = f[3];
	}

	void operator =(float f)
	{
		x = y = z = w = f;
	}

	float operator [](int idx)
	{
		switch (idx)
		{
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		}

		return 0.f;
	}

	float Length()	const
	{
		return sqrtf(x * x + y * y + z * z + w * w);
	}

	_tagVector4 Normalize()	const
	{
		_tagVector4	v;
		v.x = x / Length();
		v.y = y / Length();
		v.z = z / Length();
		v.w = w / Length();
		return v;
	}

	float Dot(_tagVector4& v)	const
	{
		return x * v.x + y * v.y + z * v.z + w * v.w;
	}

	float Dot(float f[4])	const
	{
		return x * f[0] + y * f[1] + z * f[2] + w * f[3];
	}

	float Distance(const _tagVector4& v)	const
	{
		_tagVector4	v1 = *this - v;
		return v1.Length();
	}

	float Distance(float f[4])	const
	{
		_tagVector4	v1 = *this - f;
		return v1.Length();
	}

	_tagVector4 GetView(const _tagVector4& vTarget)	const
	{
		_tagVector4	v1 = vTarget - *this;
		return v1.Normalize();
	}

	_tagVector4 GetView(float fTarget[4])	const
	{
		_tagVector4	v = fTarget;
		_tagVector4	v1 = v - *this;
		return v1.Normalize();
	}

	float GetAngle(const _tagVector4& v)	const
	{
		_tagVector4	v1 = v.Normalize();
		_tagVector4	v2 = Normalize();

		float	fAngle = v1.Dot(v2);
		return acosf(fAngle);
	}

	float GetAngle(float f[4])	const
	{
		_tagVector4	v = f;
		_tagVector4	v1 = v.Normalize();
		_tagVector4	v2 = Normalize();

		float	fAngle = v1.Dot(v2);
		return acosf(fAngle);
	}
}Vec4, *PVec4;

typedef XMMATRIX		Matrix;
typedef XMMATRIX*	PMatrix;
typedef	XMVECTOR	Vector;
typedef	XMVECTOR*	PVector;
typedef XMVECTOR	Plane;
typedef XMVECTOR*	PPlane;

const Vec2		Vec2Zero(0.f, 0.f);
const Vec2		Vec2One(1.f, 1.f);

const Vec3		Vec3Zero(0.f, 0.f, 0.f);
const Vec3		Vec3One(1.f, 1.f, 1.f);
const Vec3		Vec3X(1.f, 0.f, 0.f);
const Vec3		Vec3Y(0.f, 1.f, 0.f);
const Vec3		Vec3Z(0.f, 0.f, 1.f);

const Vec4		Vec4Zero(0.f, 0.f, 0.f, 0.f);
const Vec4		Vec4One(1.f, 1.f, 1.f, 1.f);

const Vec4		White(1.f, 1.f, 1.f, 1.f);
const Vec4		Red(1.f, 0.f, 0.f, 1.f);
const Vec4		Green(0.f, 1.f, 0.f, 1.f);
const Vec4		Blue(0.f, 0.f, 1.f, 1.f);
const Vec4		Yellow(1.f, 1.f, 0.f, 1.f);
const Vec4		Cyan(0.f, 1.f, 1.f, 1.f);
const Vec4		Magenta(1.f, 0.f, 1.f, 1.f);
const Vec4		Black(0.f, 0.f, 0.f, 1.f);


struct VTX
{
	Vec3 vtxPos;
	Vec2 vtxUV;
	Vec4 vtxColor;
};

typedef struct _tagIndex16
{
	WORD _1, _2, _3;

	_tagIndex16()
		: _1(0), _2(0), _3(0)
	{}
	_tagIndex16(WORD __1, WORD __2, WORD __3)
		: _1(__1), _2(__2), _3(__3)
	{}

	static int size() { return sizeof(WORD); }
	static DXGI_FORMAT format() { return DXGI_FORMAT_R16_UINT; }
	

}INDEX16;

typedef struct _tagIndex32
{
	DWORD _1, _2, _3;

	_tagIndex32()
		: _1(0), _2(0), _3(0)
	{}
	_tagIndex32(DWORD __1, DWORD __2, DWORD __3)
		: _1(__1), _2(__2), _3(__3)
	{}

	static int size() { return sizeof(DWORD); }
	static DXGI_FORMAT format() { return DXGI_FORMAT_R32_UINT; }

}INDEX32;

struct BUFFER
{
	ID3D11Buffer*	pConstBuffer;
	UINT			iRegister;
	UINT			iSize;
};


typedef struct _tagTransform
{
	XMMATRIX matWorld;
	XMMATRIX matView;
	XMMATRIX matProj;
}tTransform;

struct tShaderParam
{
	SHADER_PARAM eParam;
	UINT		 iRegister;
	UINT		 iTiming;
};

struct tAnimInfo
{
	Vec4 vUV;
	Vec4 vUseAnim;
};

typedef struct _tagAnimFrame
{
	wstring strFileName;
	float fTime;
	int iIdx;
	Vec3 vMove;
	Vec3 vUpColPos;
	Vec3 vUpColScale;
	Vec3 vDownColPos;
	Vec3 vDownColScale;
	Vec3 vHitColPos;
	Vec3 vHitColScale;
	Vec2 vPivot;
}tAnimFrame, *ptAnimFrame;

struct tPOINT
{
	float x;
	float y;
};

typedef struct _tagTextOutput
{
	wstring strText;
	float fPosX;
	float fPosY;
	float fSize;
	UINT iColor;
}tTextOutput;

struct PACKET_POS
{
	bool bPlayer;
	Vec3 vecPos;
};