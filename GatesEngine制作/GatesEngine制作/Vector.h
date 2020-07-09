#pragma once
struct Vector2
{
	float x;
	float y;

	//コンストラクタ
	Vector2()
	{
		x = 0;
		y = 0;
	}
	Vector2(float arg_x, float arg_y)
	{
		x = arg_x;
		y = arg_y;
	}
	//演算子のオーバーロード
	const Vector2 operator +(const Vector2& arg) const
	{
		return Vector2(x + arg.x, y + arg.y);
	}
	const Vector2 operator -(const Vector2& arg) const
	{
		return Vector2(x - arg.x, y - arg.y);
	}
	const Vector2 operator *(const float& arg) const
	{
		return Vector2(x*arg, y*arg);

	}
	const Vector2 operator /(const float& arg) const
	{
		return Vector2(x / arg, y / arg);
	}
	Vector2 operator +=(const Vector2& arg)
	{
		x += arg.x;
		y += arg.y;
		return *this;
	}
	Vector2 operator -=(const Vector2& arg)
	{
		x -= arg.x;
		y -= arg.y;
		return *this;
	}
	Vector2 operator *=(const float& arg)
	{
		x *= arg;
		y *= arg;
		return *this;

	}
	Vector2 operator /=(const float& arg)
	{
		x /= arg;
		y /= arg;
		return *this;
	}
};
struct Vector3
{
	float x;
	float y;
	float z;

	//コンストラクタ
	Vector3()
	{
		x = 0;
		y = 0;
		z = 0;
	}
	Vector3(float arg_x, float arg_y, float arg_z)
	{
		x = arg_x;
		y = arg_y;
		z = arg_z;
	}
	//演算子のオーバーロード
	const Vector3 operator +(const Vector3& arg) const
	{
		return Vector3(x + arg.x, y + arg.y, z + arg.z);
	}
	const Vector3 operator -(const Vector3& arg) const
	{
		return Vector3(x - arg.x, y - arg.y, z - arg.z);
	}
	const Vector3 operator *(const float& arg) const
	{
		return Vector3(x*arg, y*arg, z*arg);

	}
	const Vector3 operator /(const float& arg) const
	{
		return Vector3(x / arg, y / arg, z / arg);
	}
	Vector3 operator +=(const Vector3& arg)
	{
		x += arg.x;
		y += arg.y;
		z += arg.z;
		return *this;
	}
	Vector3 operator -=(const Vector3& arg)
	{
		x -= arg.x;
		y -= arg.y;
		z -= arg.z;
		return *this;
	}
	Vector3 operator *=(const float& arg)
	{
		x *= arg;
		y *= arg;
		z *= arg;
		return *this;
	}
	Vector3 operator /=(const float& arg)
	{
		x /= arg;
		y /= arg;
		z /= arg;
		return *this;
	}
};