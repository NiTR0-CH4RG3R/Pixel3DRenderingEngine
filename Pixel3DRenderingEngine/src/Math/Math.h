#pragma once

#include "Mat2x2.h"
#include "Mat3x3.h"
#include "Mat4x4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

	namespace Math {

		static float fPi = 3.1415926535897932384626433832795f;
		static double dPi = 3.1415926535897932384626433832795;

		static float fDegToRadian(const float& degrees) {
			return degrees * fPi / 180.0f;
		}

		static double dDegToRadian(const double& degrees) {
			return degrees * dPi / 180.0;
		}

		static Mat4x4 Mat4MakeProjectionMatrix(const float& AspectRatio, const float& FOV, const float& ZNear, const float& ZFar) {

			const float fFOVTan = 1.0f / std::tanf(fDegToRadian(FOV / 2.0f));
			const float fQ = (ZFar) / (ZFar - ZNear);

			Mat4x4 ProjectionMatrix;
			ProjectionMatrix[0][0] = fFOVTan;
			ProjectionMatrix[1][1] = fFOVTan * AspectRatio;
			ProjectionMatrix[2][2] = fQ;
			ProjectionMatrix[2][3] = 1.0f;
			ProjectionMatrix[3][2] = -fQ * ZNear;

			return ProjectionMatrix;
		}

		static Mat4x4 Mat4MakeProjectionMatrix(const unsigned int& ScreenWidth, const unsigned int& ScreenHeight, const float& FOV, const float& ZNear, const float& ZFar) {
			return Mat4MakeProjectionMatrix((float)ScreenWidth / (float)ScreenHeight, FOV, ZNear, ZFar);
		}

		static Mat4x4 Mat4MakeRotationZ(const float& theta) {
			const float fSinTheta = std::sinf(theta);
			const float fCosTheta = std::cosf(theta);

			return {
					fCosTheta, fSinTheta, 0.0f, 0.0f,
				   -fSinTheta, fCosTheta, 0.0f, 0.0f,
						 0.0f,      0.0f, 1.0f, 0.0f,
						 0.0f,      0.0f, 0.0f, 1.0f
			};

		}

		static Mat4x4 Mat4MakeRotationY(const float& theta) {
			const float fSinTheta = std::sinf(theta);
			const float fCosTheta = std::cosf(theta);

			return {
					fCosTheta, 0.0f, -fSinTheta, 0.0f,
						 0.0f, 1.0f,	   0.0f, 0.0f,
				    fSinTheta, 0.0f,  fCosTheta, 0.0f,
						 0.0f, 0.0f,       0.0f, 1.0f
			};

		}

		static Mat4x4 Mat4MakeRotationX(const float& theta) {
			const float fSinTheta = std::sinf(theta);
			const float fCosTheta = std::cosf(theta);

			return {
					1.0f,		0.0f,		0.0f, 0.0f,
					0.0f,  fCosTheta,  fSinTheta, 0.0f,
					0.0f, -fSinTheta,  fCosTheta, 0.0f,
					0.0f,		0.0f,       0.0f, 1.0f
			};

		}

		static Mat4x4 Mat4MakeTranslation(const Vector3& v) {
			return {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				 v.x,  v.y,  v.z, 1.0f
			};
		}

		static Mat4x4 Mat4MakeTranslationInv(const Vector3& v) {
			return {
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				 -v.x,  -v.y,  -v.z, 1.0f
			};
		}

		static Mat4x4 Mat4MakeTranslation(const float& x, const float& y, const float& z) {
			return Mat4MakeTranslation({ x,y,z });
		}

		static Mat4x4 Mat4MakeTranslationInv(const float& x, const float& y, const float& z) {
			return Mat4MakeTranslationInv({ x,y,z });
		}

		static Mat4x4 Mat4MakeRotationZXY(const Vector3& rotation) {
			return Mat4MakeRotationZ(rotation.z) * Mat4MakeRotationX(rotation.x) * Mat4MakeRotationY(rotation.y);
		}

		static Mat4x4 Mat4MakeRotationZXYInv(const Vector3& rotation) {
			return Mat4MakeRotationZ(-rotation.z) * Mat4MakeRotationX(-rotation.x) * Mat4MakeRotationY(-rotation.y);
		}

		static Mat4x4 Mat4MakeRotationZXY(const float& x, const float& y, const float& z) {
			return Mat4MakeRotationZXY({ x, y, z });
		}

		static Mat4x4 Mat4MakeRotationZXYInv(const float& x, const float& y, const float& z) {
			return Mat4MakeRotationZXYInv({ x, y, z });
		}

		static Mat4x4 Mat4MakeRotationXYZ(const Vector3& rotation) {
			return Mat4MakeRotationX(rotation.x) * Mat4MakeRotationY(rotation.y) * Mat4MakeRotationZ(rotation.z);
		}

		static Mat4x4 Mat4MakeRotationXYZInv(const Vector3& rotation) {
			return Mat4MakeRotationX(-rotation.x) * Mat4MakeRotationY(-rotation.y) * Mat4MakeRotationZ(-rotation.z);
		}


		static Mat4x4 Mat4MakeRotationXYZ(const float& x, const float& y, const float& z) {
			return Mat4MakeRotationXYZ({ x, y, z });
		}

		static Mat4x4 Mat4MakeRotationXYZInv(const float& x, const float& y, const float& z) {
			return Mat4MakeRotationXYZInv({ x, y, z });
		}

		static Mat4x4 Mat4RotateAroundAxis(const Vector3& axis, const float& theta) {
			const float fSinTheta = std::sinf(theta);
			const float fCosTheta = std::cosf(theta);

			const float t = 1 - fCosTheta;

			return{
				t * std::powf(axis.x,2.0f) + fCosTheta,		t * axis.x * axis.y - fSinTheta * axis.z,	t * axis.x * axis.z + fSinTheta * axis.y,	0.0f,
				t * axis.x * axis.y + fSinTheta * axis.z,	t * std::powf(axis.y,2) + fCosTheta,		t * axis.y * axis.z - fSinTheta * axis.x,	0.0f,
				t * axis.x * axis.z - fSinTheta * axis.y,	t * axis.y * axis.z + fSinTheta * axis.x,	t * std::powf(axis.z,2) + fCosTheta,		0.0f,
				0.0f,										0.0f,										0.0f,										1.0f

			};
		}



		static Mat3x3 Mat3MakeRotationZ(const float& theta) {
			const float fSinTheta = std::sinf(theta);
			const float fCosTheta = std::cosf(theta);

			return {
					fCosTheta, fSinTheta, 0.0f,
				   -fSinTheta, fCosTheta, 0.0f,
						 0.0f,      0.0f, 1.0f

			};

		}

		static Mat3x3 Mat3MakeRotationY(const float& theta) {
			const float fSinTheta = std::sinf(theta);
			const float fCosTheta = std::cosf(theta);

			return {
					fCosTheta, 0.0f, -fSinTheta, 
						 0.0f, 1.0f,	   0.0f, 
					fSinTheta, 0.0f,  fCosTheta 
			};

		}

		static Mat3x3 Mat3MakeRotationX(const float& theta) {
			const float fSinTheta = std::sinf(theta);
			const float fCosTheta = std::cosf(theta);

			return {
					1.0f,		0.0f,		0.0f, 
					0.0f,  fCosTheta,  fSinTheta, 
					0.0f, -fSinTheta,  fCosTheta 
			};

		}

		static Mat3x3 Mat3MakeRotationZXY(const Vector3& rotation) {
			return Mat3MakeRotationZ(rotation.z) * Mat3MakeRotationX(rotation.x) * Mat3MakeRotationY(rotation.y);
		}

		static Mat3x3 Mat3MakeRotationZXYInv(const Vector3& rotation) {
			return Mat3MakeRotationZ(-rotation.z) * Mat3MakeRotationX(-rotation.x) * Mat3MakeRotationY(-rotation.y);
		}

		static Mat3x3 Mat3MakeRotationZXY(const float& x, const float& y, const float& z) {
			return Mat3MakeRotationZXY({ x, y, z });
		}

		static Mat3x3 Mat3MakeRotationZXYInv(const float& x, const float& y, const float& z) {
			return Mat3MakeRotationZXYInv({ x, y, z });
		}

		static Mat3x3 Mat3MakeRotationXYZ(const Vector3& rotation) {
			return Mat3MakeRotationX(rotation.x) * Mat3MakeRotationY(rotation.y) * Mat3MakeRotationZ(rotation.z);
		}

		static Mat3x3 Mat3MakeRotationXYZInv(const Vector3& rotation) {
			return Mat3MakeRotationX(-rotation.x) * Mat3MakeRotationY(-rotation.y) * Mat3MakeRotationZ(-rotation.z);
		}


		static Mat3x3 Mat3MakeRotationXYZ(const float& x, const float& y, const float& z) {
			return Mat3MakeRotationXYZ({ x, y, z });
		}

		static Mat3x3 Mat3MakeRotationXYZInv(const float& x, const float& y, const float& z) {
			return Mat3MakeRotationXYZInv({ x, y, z });
		}

		static Vector3 Vec3CrossProduct(const Vector3& a, const Vector3 b) {
			return Vector3(
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x
			);
		}

		static Vector4 Vec4CrossProduct(const Vector4& a, const Vector4 b) {
			return Vector4(
				a.y * b.z - a.z * b.y,
				a.z * b.x - a.x * b.z,
				a.x * b.y - a.y * b.x
			);
		}

		static float Vec3DotProduct(const Vector3& a, const Vector3 b) {
			return
				a.x * b.x + a.y * b.y + a.z * b.z
				;
		}

		static float Vec4DotProduct(const Vector4& a, const Vector4 b) {
			return
				a.x * b.x + a.y * b.y + a.z * b.z
				;
		}

	}
