#pragma once
#include "Vector2.h"
#include "Vector3.h"


namespace Math {
	template<typename T>
	struct Vector4_generic {

		union
		{
			T element[4];
			struct { T x; T y; T z; T w; };
		};

		inline Vector4_generic() : x(0), y(0), z(0), w(1.0) {}
		inline Vector4_generic(T x, T y) : x(x), y(y), z(0), w(1.0) {}
		inline Vector4_generic(T x, T y, T z) : x(x), y(y), z(z), w(1.0) {}
		inline Vector4_generic(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
		inline Vector4_generic(const Vector2_generic<T>& v) : x(v.x), y(v.y), z(0), w(1.0) {}
		inline Vector4_generic(const Vector3_generic<T>& v) : x(v.x), y(v.y), z(v.z), w(1.0) {}
		inline Vector4_generic(const Vector4_generic& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
		inline T Magnitude() { return (T)std::sqrt(x * x + y * y + z * z + w * w); }
		inline Vector4_generic& Normalize() { this->x /= this->Magnitude(); this->y /= this->Magnitude(); this->z /= this->Magnitude(); this->w /= this->Magnitude(); return *this; }
		inline Vector4_generic operator + (const Vector4_generic& rhs)const { return Vector4_generic(this->x + rhs.x, this->y + rhs.y, this->z + rhs.z, this->w + rhs.w); }
		inline Vector4_generic operator - (const Vector4_generic& rhs)const { return Vector4_generic(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z, this->w - rhs.w); }
		inline Vector4_generic operator * (const T& rhs)const { return Vector4_generic(this->x * rhs, this->y * rhs, this->z * rhs, this->w * rhs); }
		inline Vector4_generic operator / (const T& rhs)const { return Vector4_generic(this->x / rhs, this->y / rhs, this->z / rhs, this->w / rhs); }
		inline Vector4_generic& operator += (const Vector4_generic& rhs) { this->x += rhs.x; this->y += rhs.y; this->z += rhs.z; this->w += rhs.w; return *this;}
		inline Vector4_generic& operator -= (const Vector4_generic& rhs) { this->x -= rhs.x; this->y -= rhs.y; this->z -= rhs.z; this->w -= rhs.w; return *this; }
		inline Vector4_generic& operator *= (const T& rhs) { this->x *= rhs; this->y *= rhs; this->z *= rhs; this->w *= rhs; return *this; }
		inline Vector4_generic& operator /= (const T& rhs) { this->x /= rhs; this->y /= rhs; this->z /= rhs; this->w /= rhs; return *this; }
		inline bool& operator == (const Vector4_generic& rhs) { return (this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z) && (this->w == rhs.w); }
		inline bool& operator != (const Vector4_generic& rhs) { return (this->x != rhs.x) && (this->y != rhs.y) && (this->z != rhs.z) && (this->w != rhs.w); }
		inline T& operator [] (std::size_t index) { return element[index]; }

		inline Vector4_generic operator * (const Mat4x4_generic<T>& rhs) const {
			Vector4_generic result;
			for (int colomn = 0; colomn < 4; colomn++)
				for (int row = 0; row < 4; row++)
					result.element[colomn] += this->element[row] * rhs.element[row][colomn];
			return result;
		}

		inline Vector4_generic& operator *= (const Mat4x4_generic<T>& rhs) {
			Vector4_generic temp = *this;
			this->x = 0; this->y = 0; this->z = 0; this->w = 0;
			for (int colomn = 0; colomn < 4; colomn++)
				for (int row = 0; row < 4; row++)
					this->element[colomn] += temp.element[row] * rhs.element[row][colomn];
			return *this;
		}


	};

	typedef Vector4_generic<float> Vector4;
	typedef Vector4_generic<double> Vector4d;
	typedef Vector4_generic<int> Vector4i;

	static const Vector4
		VEC4_Up(0.0f, 1.0f, 0.0f),
		VEC4_Down(0.0f, -1.0f, 0.0f),
		VEC4_Right(1.0f, 0.0f, 0.0f),
		VEC4_Left(-1.0f, 0.0f, 0.0f),
		VEC4_Forward(0.0f, 0.0f, 1.0f),
		VEC4_Backward(0.0f, 0.0f, -1.0f)
		;
}
