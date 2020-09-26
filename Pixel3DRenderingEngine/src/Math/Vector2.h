#pragma once
#include "Mat2x2.h"


namespace Math {
	template<typename T>
	struct Vector2_generic {

		union
		{
			T element[2];
			struct { T x; T y; };
		};

		inline Vector2_generic() : x(0), y(0) {}
		inline Vector2_generic(T x, T y) : x(x), y(y) {}
		inline Vector2_generic(const Vector2_generic& v) : x(v.x), y(v.y) {}
		inline T Magnitude() const { return (T)std::sqrt(x * x + y * y); }
		inline Vector2_generic& Normalize()  { this->x /= this->Magnitude(); this->y /= this->Magnitude(); return *this; }
		inline Vector2_generic operator + (const Vector2_generic& rhs)const { return Vector2_generic(this->x + rhs.x, this->y + rhs.y); }
		inline Vector2_generic operator - (const Vector2_generic& rhs)const { return Vector2_generic(this->x - rhs.x, this->y - rhs.y); }
		inline Vector2_generic operator * (const T& rhs)const { return Vector2_generic(this->x * rhs, this->y * rhs); }
		inline Vector2_generic operator / (const T& rhs)const { return Vector2_generic(this->x / rhs, this->y / rhs); }
			
		inline T operator * (const Vector2_generic& rhs)const { return (T)(this->x * rhs.x + this->y * rhs.y); }
			
		inline Vector2_generic& operator += (const Vector2_generic& rhs) { this->x += rhs.x; this->y += rhs.y; return *this; }
		inline Vector2_generic& operator -= (const Vector2_generic& rhs) { this->x -= rhs.x; this->y -= rhs.y; return *this; }
		inline Vector2_generic& operator *= (const T& rhs) { this->x *= rhs; this->y *= rhs; return *this; }
		inline Vector2_generic& operator /= (const T& rhs) { this->x /= rhs; this->y /= rhs; return *this; }
		inline bool& operator == (const Vector2_generic& rhs) { return (this->x == rhs.x) && (this->y == rhs.y); }
		inline bool& operator != (const Vector2_generic& rhs) { return (this->x != rhs.x) && (this->y != rhs.y); }
		inline T& operator[] (std::size_t index)  { return element[index]; }



		inline Vector2_generic operator * (const Mat2x2_generic<T>& rhs) const {
			Vector2_generic result;
			for (int colomn = 0; colomn < 2; colomn++)
				for (int row = 0; row < 2; row++)
					result.element[colomn] += this->element[row] * rhs.element[row][colomn];
			return result;
		}

		inline Vector2_generic& operator *= (const Mat2x2_generic<T>& rhs) {
			Vector2_generic temp  = *this;
			this->x = 0; this->y = 0;
			for (int colomn = 0; colomn < 2; colomn++)
				for (int row = 0; row < 2; row++)
					*this[colomn] += temp[row] * rhs[row][colomn];
			return *this;
		}

	};

	typedef Vector2_generic<float> Vector2;
	typedef Vector2_generic<double> Vector2d;
	typedef Vector2_generic<int> Vector2i;
}
