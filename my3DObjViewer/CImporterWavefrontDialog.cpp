// CImporterWavefrontDialog.cpp : 実装ファイル
//

#include "pch.h"
#include "CMy3DObjViewerApp.h"
#include "CImporterWavefrontDialog.h"
#include "afxdialogex.h"


// CImporterWavefrontDialog ダイアログ

IMPLEMENT_DYNAMIC(CImporterWavefrontDialog, CDialogEx)

CImporterWavefrontDialog::CImporterWavefrontDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_IMPORTER_WAVEFRONT_PROPPAGE, pParent)
{

}

CImporterWavefrontDialog::~CImporterWavefrontDialog()
{
}

void CImporterWavefrontDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDOK, m_transform);

	DDX_Check(pDX, IDC_IMPORTER_WAVEFRONT_CHECK_TRANSFROM, m_transform);
	DDX_CBIndex(pDX, IDC_IMPORTER_WAVEFRONT_FORWARD, m_forward);
	DDX_CBIndex(pDX, IDC_IMPORTER_WAVEFRONT_UP, m_up);


}


BEGIN_MESSAGE_MAP(CImporterWavefrontDialog, CDialogEx)
	ON_BN_CLICKED(IDC_IMPORTER_WAVEFRONT_CHECK_TRANSFROM, &CImporterWavefrontDialog::OnBnClickedTransform)
END_MESSAGE_MAP()


// CImporterWavefrontDialog メッセージ ハンドラー


void CImporterWavefrontDialog::OnCancel()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	CDialogEx::OnCancel();
}


void CImporterWavefrontDialog::OnBnClickedTransform()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	UpdateData(TRUE);

	CWnd* pWndForward = GetDlgItem(IDC_IMPORTER_WAVEFRONT_FORWARD);
	CWnd* pWndUp = GetDlgItem(IDC_IMPORTER_WAVEFRONT_UP );

	ASSERT(pWndForward != NULL);
	ASSERT(pWndUp != NULL);

	if (m_transform == 1) {
		pWndForward->EnableWindow(TRUE);
		pWndUp->EnableWindow(TRUE);
	}
	else {
		pWndForward->EnableWindow(FALSE);
		pWndUp->EnableWindow(FALSE);
	}
}




void CImporterWavefrontDialog::OnOK()
{
	// TODO: ここに特定なコードを追加するか、もしくは基底クラスを呼び出してください。

	UpdateData(TRUE);

	if (m_transform != 0 )
	{
		// 0: (blank)
		// 1: +X
		// 2: +Y
		// 3: +Z
		// 4: -X
		// 5: -Y
		// 6: -Z

		if (
			(m_forward == 1 || m_forward == 4) && (m_up == 1 || m_up == 4) ||
			(m_forward == 2 || m_forward == 5) && (m_up == 2 || m_up == 5) ||
			(m_forward == 3 || m_forward == 6) && (m_up == 3 || m_up == 6)
			) {
			AfxMessageBox(_T("ForwardとUpに同じ軸は指定できません。"));
			return;
		}

		if (m_forward == 0 || m_up == 0) {
			AfxMessageBox(_T("ForwardとUpに軸を指定してください。"));
			return;
		}
	}

	CDialogEx::OnOK();
}


