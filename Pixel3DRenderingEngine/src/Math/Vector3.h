#pragma once
#include "Vector2.h"
#include "Mat3x3.h"

namespace Math {
	template<typename T>
	struct Vector3_generic {
		union {
			T element[3];
			struct { T x; T y; T z; };
		};

		inline Vector3_generic() : x(0), y(0), z(0) {}
		inline Vector3_generic(T x, T y) : x(x), y(y), z(0) {}
		inline Vector3_generic(T x, T y, T z) : x(x), y(y), z(z) {}
		inline Vector3_generic(const Vector2_generic<T>& v) : x(v.x), y(v.y), z(0) {}
		inline Vector3_generic(const Vector3_generic& v) : x(v.x), y(v.y), z(v.z) {}
		inline T Magnitude() { return (T)std::sqrt(x * x + y * y + z * z); }
		inline Vector3_generic& Normalize() { this->x /= this->Magnitude(); this->y /= this->Magnitude(); this->z /= this->Magnitude(); return *this; }
		inline Vector3_generic operator + (const Vector3_generic& rhs)const { return Vector3_generic(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z); }
		inline Vector3_generic operator - (const Vector3_generic& rhs)const { return Vector3_generic(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z); }
		inline Vector3_generic operator * (const T& rhs) const { return Vector3_generic(this->x * rhs, this->y * rhs, this->z * rhs); }
		
		//inline Vector3_generic operator * (const T& rhs) const { return Vector3_generic(x * T, y * T, z * T); }
		//inline Vector3_generic operator * (const Vector3_generic& lhs, const T& rhs) const { return Vector3_generic(lhs.x * T, lhs.y * T, lhs.z * T); }
		
		inline Vector3_generic operator / (const T& rhs) const { return Vector3_generic(this->x / rhs, this->y / rhs, this->z / rhs); }
			
		inline T operator * (const Vector3_generic& rhs)const { return (T)(this->x * rhs.x + this->y * rhs.y + this->z * rhs.z); }
		inline Vector3_generic operator ^ (const Vector3_generic& rhs) {
			return Vector3_generic
			(
				this->y * rhs.z - this->z * rhs.y,
				this->z * rhs.x - this->x * rhs.z,
				this->x * rhs.y - this->y * rhs.x
			);
		}
			
		inline Vector3_generic& operator += (const Vector3_generic& rhs)  { this->x += rhs.x; this->y += rhs.y; this->z += rhs.z; return *this; }
		inline Vector3_generic& operator -= (const Vector3_generic& rhs)  { this->x -= rhs.x; this->y -= rhs.y; this->z -= rhs.z; return *this; }
		inline Vector3_generic& operator *= (const T& rhs)  { this->x *= rhs; this->y *= rhs; this->z *= rhs; return *this; }
		inline Vector3_generic& operator /= (const T& rhs)  { this->x /= rhs; this->y /= rhs; this->z /= rhs; return *this; }
			
			
		inline bool& operator == (const Vector3_generic& rhs)  { return (this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z); }
		inline bool& operator != (const Vector3_generic& rhs)  { return (this->x != rhs.x) && (this->y != rhs.y) && (this->z != rhs.z); }
		inline T& operator[](std::size_t index)const { return element[index]; }

		inline Vector3_generic operator * (const Mat3x3_generic<T>& rhs) const {
			Vector3_generic result;
			for (int colomn = 0; colomn < 3; colomn++)
				for (int row = 0; row < 3; row++)
					result.element[colomn] += this->element[row] * rhs.element[row][colomn];
			return result;
		}

		inline Vector3_generic& operator *= (const Mat3x3_generic<T>& rhs)  {
			Vector3_generic temp = *this;
			this->x = 0; this->y = 0; this->z = 0;
			for (int colomn = 0; colomn < 3; colomn++)
				for (int row = 0; row < 3; row++)
					*this[colomn] += temp[row] * rhs[row][colomn];
			return *this;
		}

	};

	//template <typename T>
	//inline Vector3_generic<T> operator * (const float& lhs, const T& rhs) { return Vector3_generic<T>(lhs.x * T, lhs.y * T, lhs.z * T); }

	typedef Vector3_generic<float> Vector3;
	typedef Vector3_generic<double> Vector3d;
	typedef Vector3_generic<int> Vector3i;

	static const Vector3
		VEC3_Up(0.0f, 1.0f, 0.0f),
		VEC3_Down(0.0f, -1.0f, 0.0f),
		VEC3_Right(1.0f, 0.0f, 0.0f),
		VEC3_Left(-1.0f, 0.0f, 0.0f),
		VEC3_Forward(0.0f, 0.0f, 1.0f),
		VEC3_Backward(0.0f, 0.0f, -1.0f)
		;
}
