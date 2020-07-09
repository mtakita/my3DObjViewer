#pragma once


// CImporterWavefrontDialog ダイアログ

class CImporterWavefrontDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CImporterWavefrontDialog)

public:
	CImporterWavefrontDialog(CWnd* pParent = nullptr);   // 標準コンストラクター
	virtual ~CImporterWavefrontDialog();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
//	enum { IDD = IDD_IMPORTER_WAVEFRONT_PROPPAGE };
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
public:
	afx_msg void OnBnClickedTransform();
	afx_msg void OnBnClickedImporterWavefrontOk();
	afx_msg void OnBnClickedImporterWavefrontCancel();
	// transform
//	CButton m_transform;
	int m_transform;
	int m_forward;
	int m_up;
	virtual void OnOK();
};
