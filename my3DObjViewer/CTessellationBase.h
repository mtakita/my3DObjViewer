#pragma once
class CTessellationBase
{
	bool m_TessellationOnOff;
	int m_TessellationDomain;
	int m_TessellationFactorOuter0;
	int m_TessellationFactorOuter1;
	int m_TessellationFactorOuter2;
	int m_TessellationFactorOuter3;
	int m_TessellationFactorInner0;
	int m_TessellationFactorInner1;

public:
	void SetTessellationOnOff(bool inTessellationOnOff);
	void SetTessellationDomain(int inTessellationDomain);
	void SetTessellationLevelFactorOuter0(int inTessellationFactorOuter0);
	void SetTessellationLevelFactorOuter1(int inTessellationFactorOuter1);
	void SetTessellationLevelFactorOuter2(int inTessellationFactorOuter2);
	void SetTessellationLevelFactorOuter3(int inTessellationFactorOuter3);
	void SetTessellationLevelFactorInner0(int inTessellationFactorInner0);
	void SetTessellationLevelFactorInner1(int inTessellationFactorInner1);

	bool GetTessellationOnOff();
	int GetTessellationDomain();
	int GetTessellationLevelFactorOuter0();
	int GetTessellationLevelFactorOuter1();
	int GetTessellationLevelFactorOuter2();
	int GetTessellationLevelFactorOuter3();
	int GetTessellationLevelFactorInner0();
	int GetTessellationLevelFactorInner1();
};

