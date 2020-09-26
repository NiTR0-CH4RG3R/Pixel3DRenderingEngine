#pragma once
#include <memory>

namespace Math {
	template <typename T>
	struct Mat2x2_generic {
		//[Row] [Column]
		T element[2][2]{ 0 };


		inline Mat2x2_generic operator + (const Mat2x2_generic& rhs) const { Mat2x2_generic result = *this; for (int row = 0; row < 2; row++) for (int colomn = 0; colomn < 2; colomn++) result.element[row][colomn] += rhs.element[row][colomn]; return result; }
		inline Mat2x2_generic operator - (const Mat2x2_generic& rhs) const { Mat2x2_generic result = *this; for (int row = 0; row < 2; row++) for (int colomn = 0; colomn < 2; colomn++) result.element[row][colomn] -= rhs.element[row][colomn]; return result; }
		inline Mat2x2_generic operator * (const T& rhs) const { Mat2x2_generic result = *this; for (int row = 0; row < 2; row++) for (int colomn = 0; colomn < 2; colomn++) result.element[row][colomn] *= rhs; return result; }

		inline Mat2x2_generic operator * (const Mat2x2_generic& rhs) const {
			Mat2x2_generic result;
			for (int row = 0; row < 2; row++)
				for (int colomn = 0; colomn < 2; colomn++)
					for (int i = 0; i < 2; i++)
						result.element[row][colomn] += (this->element[row][i] * rhs.element[i][colomn]);
			return result;
		}

		inline Mat2x2_generic operator / (const T& rhs) const { Mat2x2_generic result = *this; for (int row = 0; row < 2; row++) for (int colomn = 0; colomn < 2; colomn++) result.element[row][colomn] /= rhs; return result; }
		inline Mat2x2_generic& operator += (const Mat2x2_generic& rhs) { for (int row = 0; row < 2; row++) for (int colomn = 0; colomn < 2; colomn++) this->element[row][colomn] += rhs.element[row][colomn]; return *this; }
		inline Mat2x2_generic& operator -= (const Mat2x2_generic& rhs) { for (int row = 0; row < 2; row++) for (int colomn = 0; colomn < 2; colomn++) this->element[row][colomn] -= rhs.element[row][colomn]; return *this; }
		inline Mat2x2_generic& operator *= (const T& rhs) { for (int row = 0; row < 2; row++) for (int colomn = 0; colomn < 2; colomn++) this->element[row][colomn] *= rhs; return *this; }

		inline Mat2x2_generic& operator *= (const Mat2x2_generic& rhs) {
			Mat2x2_generic temp = *this;
			memset(this->element, 0, sizeof(this->element));
			for (int row = 0; row < 2; row++)
				for (int colomn = 0; colomn < 2; colomn++)
					for (int i = 0; i < 2; i++)
						this->element[row][colomn] += (temp.element[row][i] * rhs.element[i][colomn]);
			return *this;
		}

		inline Mat2x2_generic& operator /= (const T& rhs) { for (int row = 0; row < 2; row++) for (int colomn = 0; colomn < 2; colomn++) this->element[row][colomn] /= rhs; return *this; }

		inline T* operator[](const std::size_t& row) {
			return element[row];
		}

		inline Mat2x2_generic& operator = (const Mat2x2_generic& rhs) { memcpy(this, &rhs, sizeof(Mat2x2_generic)); return *this; }


		static Mat2x2_generic Identity() {
			Mat2x2_generic result;
			for (int i = 0; i < 2; i++) result.element[i][i] = (T)(1.0);
			return result;
		}
	};

	typedef Mat2x2_generic<float> Mat2x2;
	typedef Mat2x2_generic<double> Mat2x2d;
	typedef Mat2x2_generic<int> Mat2x2i;
}