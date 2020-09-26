#pragma once
#include "Mat2x2.h"
#include "Mat3x3.h"

namespace Math {
	template <typename T>
	struct Mat4x4_generic {
		T element[4][4]{ 0 };

		inline Mat4x4_generic operator + (const Mat4x4_generic& rhs) const { Mat4x4_generic result = *this; for (int row = 0; row < 4; row++) for (int colomn = 0; colomn < 4; colomn++) result.element[row][colomn] += rhs.element[row][colomn]; return result; }
		inline Mat4x4_generic operator - (const Mat4x4_generic& rhs) const { Mat4x4_generic result = *this; for (int row = 0; row < 4; row++) for (int colomn = 0; colomn < 4; colomn++) result.element[row][colomn] -= rhs.element[row][colomn]; return result; }
		inline Mat4x4_generic operator * (const T& rhs) { Mat4x4_generic result = *this; for (int row = 0; row < 4; row++) for (int colomn = 0; colomn < 4; colomn++) result.element[row][colomn] *= rhs; return result; }

		inline Mat4x4_generic operator * (const Mat4x4_generic& rhs) const {
			Mat4x4_generic result;
			for (int row = 0; row < 4; row++)
				for (int colomn = 0; colomn < 4; colomn++)
					for (int i = 0; i < 4; i++)
						result.element[row][colomn] += (this->element[row][i] * rhs.element[i][colomn]);
			return result;
		}

		inline Mat4x4_generic operator / (const T& rhs) const { Mat4x4_generic result = *this; for (int row = 0; row < 4; row++) for (int colomn = 0; colomn < 4; colomn++) result.element[row][colomn] /= rhs; return result; }
		inline Mat4x4_generic& operator += (const Mat4x4_generic& rhs)  { for (int row = 0; row < 4; row++) for (int colomn = 0; colomn < 4; colomn++) this->element[row][colomn] += rhs.element[row][colomn]; return *this; }
		inline Mat4x4_generic& operator -= (const Mat4x4_generic& rhs)  { for (int row = 0; row < 4; row++) for (int colomn = 0; colomn < 4; colomn++) this->element[row][colomn] -= rhs.element[row][colomn]; return *this; }
		inline Mat4x4_generic& operator *= (const T& rhs)  { for (int row = 0; row < 4; row++) for (int colomn = 0; colomn < 4; colomn++) this->element[row][colomn] *= rhs; return *this; }

		inline Mat4x4_generic& operator *= (const Mat4x4_generic& rhs)  {
			Mat4x4_generic temp = *this;
			memset(this->element, 0, sizeof(this->element));
			for (int row = 0; row < 4; row++)
				for (int colomn = 0; colomn < 4; colomn++)
					for (int i = 0; i < 4; i++)
						this->element[row][colomn] += (temp.element[row][i] * rhs.element[i][colomn]);
			return *this;
		}

		inline Mat4x4_generic& operator /= (const T& rhs)  { for (int row = 0; row < 4; row++) for (int colomn = 0; colomn < 4; colomn++) this->element[row][colomn] /= rhs; return *this; }

		inline T* operator[](const std::size_t& row) {
			return element[row];
		}

		inline Mat4x4_generic& operator = (const Mat4x4_generic& rhs)  { memcpy(this, &rhs, sizeof(Mat4x4_generic)); return *this; }
		inline Mat4x4_generic& operator = (const Mat3x3_generic<T>& rhs)  {
			for (int row = 0; row < 3; row++)
				memcpy(this->element[row], &rhs.element[row], sizeof(T) * 3);
			return *this;
		}
		inline Mat4x4_generic& operator = (const Mat2x2_generic<T>& rhs)  {
			for (int row = 0; row < 2; row++)
				memcpy(this->element[row], &rhs.element[row], sizeof(T) * 2);
			return *this;
		}

		static Mat4x4_generic Identity() {
			Mat4x4_generic result;
			for (int i = 0; i < 4; i++) result.element[i][i] = (T)(1.0);
			return result;
		}
	};

	typedef Mat4x4_generic<float>	Mat4x4;
	typedef Mat4x4_generic<double>	Mat4x4d;
	typedef Mat4x4_generic<int>		Mat4x4i;
}
