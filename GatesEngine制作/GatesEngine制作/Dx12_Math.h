#pragma once
#include"Vector.h"
class Dx12_Math
{
private:
	//OŠpŠÖ”—p‚Ì‰~”z—ñ‚ğì‚é—\’è
public:
	static float Sqrt(float v);
	static float Distance(const Vector2& vec);
	static float Distance(const Vector3& vec);
	static Vector2 Normalize(const Vector2& vec);
	static Vector3 Normalize(const Vector3& vec);
	static float Dot(const Vector2& vec1, const Vector2& vec2);
	static float Dot(const Vector3& vec1, const Vector3& vec2);
	static float Cross(const Vector2& vec1, const Vector2& vec2);
	static Vector3 Cross(const Vector3& vec1, const Vector3& vec2);
	static Vector3 Normal(const Vector3& vec1, const Vector3& vec2, const Vector3& vec3);
};

