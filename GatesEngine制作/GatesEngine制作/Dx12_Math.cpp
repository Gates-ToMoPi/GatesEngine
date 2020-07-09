#include "Dx12_Math.h"

float Dx12_Math::Sqrt(float v)
{
	float s1 = 1, s2;
	if (v <= 0) {
		return 0;/*ƒGƒ‰[ˆ—*/
	}
	do
	{
		s2 = s1;
		s1 = (v / s1 + s1) / 2;
	} while (s1 != s2);
	return s1;
}

float Dx12_Math::Distance(const Vector2 & vec)
{
	return Sqrt((vec.x*vec.x) + (vec.y*vec.y));
}

float Dx12_Math::Distance(const Vector3 & vec)
{
	return Sqrt((vec.x*vec.x) + (vec.y*vec.y) + (vec.z*vec.z));
}

Vector2 Dx12_Math::Normalize(const Vector2 & vec)
{
	float length = Distance(vec);
	return Vector2(vec / length);
}

Vector3 Dx12_Math::Normalize(const Vector3 & vec)
{
	float length = Distance(vec);
	return Vector3(vec / length);
}

float Dx12_Math::Dot(const Vector2 & vec1, const Vector2 & vec2)
{
	return ((vec1.x * vec2.x) + (vec1.y, vec2.y));
}

float Dx12_Math::Dot(const Vector3 & vec1, const Vector3 & vec2)
{
	return ((vec1.x + vec2.x) + (vec1.x + vec2.x) + (vec1.x + vec2.x));
}

float Dx12_Math::Cross(const Vector2 & vec1, const Vector2 & vec2)
{
	return ((vec1.x*vec2.y) - (vec1.y*vec2.x));
}

Vector3 Dx12_Math::Cross(const Vector3 & vec1, const Vector3 & vec2)
{
	Vector3 cross;
	cross.x = (vec1.y*vec2.z) - (vec1.z*vec2.y);
	cross.y = (vec1.z*vec2.x) - (vec1.x*vec2.z);
	cross.z = (vec1.x*vec2.y) - (vec1.y*vec2.x);
	return cross;
}

Vector3 Dx12_Math::Normal(const Vector3 & vec1, const Vector3 & vec2, const Vector3& vec3)
{
	Vector3 ab = vec2 - vec1;
	Vector3 ac = vec3 - vec1;
	Vector3 cross = Cross(ab, ac);
	float length = Distance(cross);
	return cross / length;
}
