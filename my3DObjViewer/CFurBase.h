#pragma once
class CFurBase
{
	bool m_FurGeometryOnOff;
	int m_FurLayer;
	float m_FurDepth;

public:
	bool GetFurGeometryOnOff();
	int GetFurLayer();
	float GetFurDepth();

	void SetFurGeometryOnOff(bool inTessellationOnOff);
	void SetFurLayer(int inFurLayer);
	void SetFurDepth(float inFurDepth);
};

