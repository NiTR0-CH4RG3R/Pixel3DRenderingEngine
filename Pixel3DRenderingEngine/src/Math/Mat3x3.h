#pragma once
#include "Mat2x2.h"


namespace Math {
	template <typename T>
	struct Mat3x3_generic {
		T element[3][3]{ 0 };

		inline Mat3x3_generic operator + (const Mat3x3_generic& rhs) const { Mat3x3_generic result = *this; for (int row = 0; row < 3; row++) for (int colomn = 0; colomn < 3; colomn++) result.element[row][colomn] += rhs.element[row][colomn]; return result; }
		inline Mat3x3_generic operator - (const Mat3x3_generic& rhs) const { Mat3x3_generic result = *this; for (int row = 0; row < 3; row++) for (int colomn = 0; colomn < 3; colomn++) result.element[row][colomn] -= rhs.element[row][colomn]; return result; }
		inline Mat3x3_generic operator * (const T& rhs) const { Mat3x3_generic result = *this; for (int row = 0; row < 3; row++) for (int colomn = 0; colomn < 3; colomn++) result.element[row][colomn] *= rhs; return result; }

		inline Mat3x3_generic operator * (const Mat3x3_generic& rhs) const {
			Mat3x3_generic result;
			for (int row = 0; row < 3; row++)
				for (int colomn = 0; colomn < 3; colomn++)
					for (int i = 0; i < 3; i++)
						result.element[row][colomn] += (this->element[row][i] * rhs.element[i][colomn]);
			return result;
		}

		inline Mat3x3_generic operator / (const T& rhs) const { Mat3x3_generic result = *this; for (int row = 0; row < 3; row++) for (int colomn = 0; colomn < 3; colomn++) result.element[row][colomn] /= rhs; return result; }
		inline Mat3x3_generic& operator += (const Mat3x3_generic& rhs) { for (int row = 0; row < 3; row++) for (int colomn = 0; colomn < 3; colomn++) this->element[row][colomn] += rhs.element[row][colomn]; return *this; }
		inline Mat3x3_generic& operator -= (const Mat3x3_generic& rhs) { for (int row = 0; row < 3; row++) for (int colomn = 0; colomn < 3; colomn++) this->element[row][colomn] -= rhs.element[row][colomn]; return *this; }
		inline Mat3x3_generic& operator *= (const T& rhs) { for (int row = 0; row < 3; row++) for (int colomn = 0; colomn < 3; colomn++) this->element[row][colomn] *= rhs; return *this; }

		inline Mat3x3_generic& operator *= (const Mat3x3_generic& rhs) {
			Mat3x3_generic temp = *this;
			memset(this->element, 0, sizeof(this->element));
			for (int row = 0; row < 3; row++)
				for (int colomn = 0; colomn < 3; colomn++)
					for (int i = 0; i < 3; i++)
						this->element[row][colomn] += (temp.element[row][i] * rhs.element[i][colomn]);
			return *this;
		}

		inline Mat3x3_generic& operator /= (const T& rhs) { for (int row = 0; row < 3; row++) for (int colomn = 0; colomn < 3; colomn++) this->element[row][colomn] /= rhs; return *this; }

		inline T* operator[](const std::size_t& row) {
			return element[row];
		}

		inline Mat3x3_generic& operator = (const Mat3x3_generic& rhs) { memcpy(this, &rhs, sizeof(Mat3x3_generic)); return *this; }
		inline Mat3x3_generic& operator = (const Mat2x2_generic<T>& rhs) {
			for (int row = 0; row < 2; row++)
				memcpy(this->element[row], &rhs.element[row], sizeof(T) * 2);
			return *this;
		}

		static Mat3x3_generic Identity() {
			Mat3x3_generic result;
			for (int i = 0; i < 3; i++) result.element[i][i] = (T)(1.0);
			return result;
		}
	};

	typedef Mat3x3_generic<float> Mat3x3;
	typedef Mat3x3_generic<double> Mat3x3d;
	typedef Mat3x3_generic<int> Mat3x3i;
}
