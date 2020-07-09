#pragma once

#ifndef _CGEOMETRYBASE_H_
#define _CGEOMETRYBASE_H_

#include "mymath.h"
using namespace mymath;

class CGeometryBase
{
public:
	CGeometryBase() : m_PosX{ 0.0f }, m_PosY{ 0.0f }, m_PosZ{ 0.0f }, m_k{ 1.0f }, m_xAxis{ 0.0f }, m_yAxis{ 0.0f }, m_zAxis{ 0.0f }{
	}

	CGeometryBase(float wPosX, float wPosY, float wPosZ, float k, float xAxis, float yAxis, float zAxis) : m_PosX{ wPosX }, m_PosY{ wPosY }, m_PosZ{ wPosZ }, m_k{ k }, m_xAxis{ xAxis }, m_yAxis{ yAxis }, m_zAxis{ zAxis }{
	}

	Matrixf4x4 GetTranslateMatrix4x4_2();
	Matrixf4x4 GetTranslateMatrix4x4Rev();
	Matrixf4x4 GetTranslateMatrix4x4NegZ();
	Matrixf4x4 GetTranslateMatrix4x4RevNegZ();

	Matrixf3x3 GetScaleMatrix3x3();
	Matrixf4x4 GetScaleMatrix4x4();

	Matrixf3x3 GetRotateMatrix3x3();
	Matrixf3x3 GetRotateMatrix3x3Rev();
	Matrixf3x3 GetRotateMatrix3x3RevNegX();

	Matrixf4x4 GetRotateMatrix4x4();
	Matrixf4x4 GetRotateMatrix4x4(float rotX, float rotY, float rotZ);
	Matrixf4x4 GetRotateMatrix4x4Rev();


	Matrixf4x4 GetTransformMatrix4x4_2();
	Matrixf4x4 GetTransformMatrix4x4Rev();

	void SetPosX(float x) { m_PosX = x; }
	void SetPosY(float y) { m_PosY = y; }
	void SetPosZ(float z) { m_PosZ = z; }
	Vectorf3 GetPos() { return Vectorf3(m_PosX, m_PosY, m_PosZ); }
	Vectorf3 GetPosRev() { return Vectorf3(-m_PosX, -m_PosY, -m_PosZ); }

	void AddPos( float moveX, float moveY, float moveZ ) { 
		m_PosX += moveX; 
		m_PosY += moveY;
		m_PosZ += moveZ;
	}

	void AddRotate(float rotateX, float rotateY, float rotateZ) {
		m_xAxis += rotateX;
		m_yAxis += rotateY;
		m_zAxis += rotateZ;
	}

	void SetScale(float newK ) { m_k = newK; }

	void SetRotateX(float x) { m_xAxis = x; }
	void SetRotateY(float y) { m_yAxis = y; }
	void SetRotateZ(float z) { m_zAxis = z; }

	float GetPosX() { return m_PosX; }
	float GetPosY() { return m_PosY; }
	float GetPosZ() { return m_PosZ; }

	float GetScale() { return m_k; }

	float GetRotateX() { return m_xAxis; }
	float GetRotateY() { return m_yAxis; }
	float GetRotateZ() { return m_zAxis; }

	bool GetEulerAngle( Matrixf3x3 rotMatInW, float* rotX, float* rotY, float* rotZ );
	void GetDeltaComponentInW(Vectorf3 moveInE, Matrixf3x3 rotMat, float* deltaX, float* deltaY, float* deltaZ);

public:
	// Translation.
	float m_PosX;
	float m_PosY;
	float m_PosZ;

	// Scale
	float m_k;

	// Rotation
	float m_xAxis;
	float m_yAxis;
	float m_zAxis;

};


#endif
