#include "pch.h"
#include "CGeometryBase.h"
#include "mymath.h"
#include <GL/gl3w.h>
#include "etc.h"

#define _USE_MATH_DEFINES
#include <math.h>

using namespace mymath;

Matrixf4x4 CGeometryBase::GetTranslateMatrix4x4_2() {

	Matrixf4x4 transMatrix = translate2(m_PosX, m_PosY, m_PosZ ); 
	return transMatrix;

}

Matrixf4x4 CGeometryBase::GetTranslateMatrix4x4NegZ() {

	Matrixf4x4 transMatrix = translate2(m_PosX, m_PosY, m_PosZ);

	// WorldSpace and EyeSpace have z-Axis in opposite direction.
	transMatrix[0][2] = -transMatrix[0][2];
	transMatrix[1][2] = -transMatrix[1][2];
	transMatrix[2][2] = -transMatrix[2][2];

	return transMatrix;

}

Matrixf4x4 CGeometryBase::GetTranslateMatrix4x4Rev() {

	Matrixf4x4 transMatrix = translate2(-m_PosX, -m_PosY, -m_PosZ );
	return transMatrix;

}

Matrixf4x4 CGeometryBase::GetTranslateMatrix4x4RevNegZ() {

	Matrixf4x4 transMatrix = translate2(-m_PosX, -m_PosY, -m_PosZ);

	// WorldSpace and EyeSpace have z-Axis in opposite direction.
	transMatrix[0][2] = -transMatrix[0][2];
	transMatrix[1][2] = -transMatrix[1][2];
	transMatrix[2][2] = -transMatrix[2][2];

	return transMatrix;

}

Matrixf3x3 CGeometryBase::GetScaleMatrix3x3() {

	Matrixf3x3 scaleMatrix = scale3x3(m_k);
	return scaleMatrix;
}

Matrixf4x4 CGeometryBase::GetScaleMatrix4x4() {

	Matrixf4x4 scaleMatrix = scale(m_k);
	return scaleMatrix;

}
Matrixf3x3 CGeometryBase::GetRotateMatrix3x3() {

	// Rotate about x-axis.
	Matrixf3x3 rotateAboutXMatriX = rotate3x3( 1.0f, 0.0f, 0.0f, m_xAxis * ( M_PI / 180.0f ));

	// Rotate about y-axis.
	Matrixf3x3 rotateAboutYMatriY = rotate3x3( 0.0f, 1.0f, 0.0f, m_yAxis * (M_PI / 180.0f));

	// Rotate about z-axis.
	Matrixf3x3 rotateAboutZMatriZ = rotate3x3( 0.0f, 0.0f, 1.0f, m_zAxis * (M_PI / 180.0f));

	// Combine all.
	Matrixf3x3 rotateMatrix = rotateAboutZMatriZ * rotateAboutYMatriY * rotateAboutXMatriX;

	return rotateMatrix;
}

Matrixf3x3 CGeometryBase::GetRotateMatrix3x3Rev() {

	// Rotate about x-axis.
	Matrixf3x3 rotateAboutXMatriX = rotate3x3(1.0f, 0.0f, 0.0f, -m_xAxis * (M_PI / 180.0f));

	// Rotate about y-axis.
	Matrixf3x3 rotateAboutYMatriY = rotate3x3(0.0f, 1.0f, 0.0f, -m_yAxis * (M_PI / 180.0f));

	// Rotate about z-axis.
	Matrixf3x3 rotateAboutZMatriZ = rotate3x3(0.0f, 0.0f, 1.0f, -m_zAxis * (M_PI / 180.0f));

	// Combine all.
	Matrixf3x3 rotateMatrix = rotateAboutZMatriZ * rotateAboutYMatriY * rotateAboutXMatriX;

	return rotateMatrix;
}

Matrixf4x4 CGeometryBase::GetRotateMatrix4x4() {

	// Rotate about x-axis.
	Matrixf4x4 rotateAboutXMatriX = rotate(1.0f, 0.0f, 0.0f, m_xAxis * (M_PI / 180.0f));

	// Rotate about y-axis.
	Matrixf4x4 rotateAboutYMatriY = rotate(0.0f, 1.0f, 0.0f, m_yAxis * (M_PI / 180.0f));

	// Rotate about z-axis.
	Matrixf4x4 rotateAboutZMatriZ = rotate(0.0f, 0.0f, 1.0f, m_zAxis * (M_PI / 180.0f));

	// Combine all.
	Matrixf4x4 rotateMatrix = rotateAboutZMatriZ * rotateAboutYMatriY * rotateAboutXMatriX;

	return rotateMatrix;
}

Matrixf4x4 CGeometryBase::GetRotateMatrix4x4( float rotX, float rotY, float rotZ ) {

	// Rotate about x-axis.
	Matrixf4x4 rotateAboutXMatriX = rotate(1.0f, 0.0f, 0.0f, rotX * (M_PI / 180.0f));

	// Rotate about y-axis.
	Matrixf4x4 rotateAboutYMatriY = rotate(0.0f, 1.0f, 0.0f, rotY * (M_PI / 180.0f));

	// Rotate about z-axis.
	Matrixf4x4 rotateAboutZMatriZ = rotate(0.0f, 0.0f, 1.0f, rotZ * (M_PI / 180.0f));

	// Combine all.
	Matrixf4x4 rotateMatrix = rotateAboutZMatriZ * rotateAboutYMatriY * rotateAboutXMatriX;

	return rotateMatrix;
}

Matrixf4x4 CGeometryBase::GetRotateMatrix4x4Rev() {

	// Rotate about x-axis.
	Matrixf4x4 rotateAboutXMatriX = rotate(1.0f, 0.0f, 0.0f, -m_xAxis * (M_PI / 180.0f));

	// Rotate about y-axis.
	Matrixf4x4 rotateAboutYMatriY = rotate(0.0f, 1.0f, 0.0f, -m_yAxis * (M_PI / 180.0f));

	// Rotate about z-axis.
	Matrixf4x4 rotateAboutZMatriZ = rotate(0.0f, 0.0f, 1.0f, -m_zAxis * (M_PI / 180.0f));

	// Combine all.
	Matrixf4x4 rotateMatrix = rotateAboutZMatriZ * rotateAboutYMatriY * rotateAboutXMatriX;

	return rotateMatrix;
}


Matrixf4x4 CGeometryBase::GetTransformMatrix4x4_2() {

	Matrixf4x4 translateMatrix = GetTranslateMatrix4x4_2();
	Matrixf4x4 scalingMatrix = GetScaleMatrix4x4();
	Matrixf4x4 rotateMatrix = GetRotateMatrix4x4();

	Matrixf4x4 combinedMatrix = scalingMatrix * rotateMatrix * translateMatrix;

	return combinedMatrix;

}

Matrixf4x4 CGeometryBase::GetTransformMatrix4x4Rev() {

	Matrixf4x4 translateMatrixRev = GetTranslateMatrix4x4_2();
	Matrixf4x4 scalingMatrix = GetScaleMatrix4x4();
	Matrixf4x4 rotateMatrixRev = GetRotateMatrix4x4Rev();

	Matrixf4x4 combinedMatrix = scalingMatrix * rotateMatrixRev * translateMatrixRev;

	return combinedMatrix;

}

bool CGeometryBase::GetEulerAngle(Matrixf3x3 rotMatInW, float* rotX, float* rotY, float* rotZ)
{
	float yAxis, xAxis, zAxis;

	float sp = -rotMatInW[2][1];
	if (sp <= -1.0f) {
		yAxis = -1.570796f;
	}
	else if (sp >= 1.0f) {
		yAxis = 1.570796f;
	}
	else {
		yAxis = asin(sp);
	}

	if (fabs(sp) > 0.9999f) {
		zAxis = 0.0f;
		xAxis = atan2(-rotMatInW[0][2], rotMatInW[0][0]);
	}
	else {
		xAxis = atan2(rotMatInW[2][0], rotMatInW[2][2]);
		zAxis = atan2(rotMatInW[0][1], rotMatInW[1][1]);
	}

	*rotX = xAxis * (180.f / M_PI);
	*rotY = yAxis * (180.f / M_PI);
	*rotZ = zAxis * (180.f / M_PI);

	return true;
}

void CGeometryBase::GetDeltaComponentInW( Vectorf3 deltaInE, Matrixf3x3 rotMat, float* deltaX,float* deltaY, float* deltaZ )
{
	float delXInW = deltaInE * rotMat[0];
	float delYInW = deltaInE * rotMat[1];
	float delZInW = deltaInE * rotMat[2];

	*deltaX = delXInW;
	*deltaY = delYInW;
	*deltaZ = delZInW;

	return;
}
