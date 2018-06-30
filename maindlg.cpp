// maindlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "maindlg.h"
#include <tchar.h>
#include "Dlg2Side.h"

//using namespace ATL;


LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
	SetIcon(hIcon, TRUE);
	HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME), 
		IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
	SetIcon(hIconSmall, FALSE);

	m_richSrc = GetDlgItem(IDC_RICH_SRC);
	m_richTar = GetDlgItem(IDC_RICH_TAR);

	m_cmbOperType = GetDlgItem(IDC_CMB_TYPE);
	m_cmbOperType.AddString(_T("两边添加"));
	m_cmbOperType.SetCurSel(0);
	
	return TRUE;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainDlg::OnConverty(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	DoConvert();
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

void CMainDlg::DoConvert()
{
	int nSel = m_cmbOperType.GetCurSel();
	switch (nSel) 
	{
	case 0:
		{
			Do2SideAdd();
		}
		break;
	default:break;
	}
}

CSimpleArray<CString>* CMainDlg::GetLines()
{
	TCHAR* pBuf;
	int nLen = m_richSrc.GetWindowTextLength();
	pBuf = new TCHAR[nLen+1];
	m_richSrc.GetWindowText(pBuf, nLen);
	pBuf[nLen]=NULL;
	CString strText(pBuf);
	delete[] pBuf;

	CSimpleArray<CString>* pstrArr = new CSimpleArray<CString>();
	const TCHAR* pstrBuf = (LPCTSTR)strText;
	const TCHAR* pstrEnd = pstrBuf+strText.GetLength();
	while(pstrBuf<pstrEnd) {
		const TCHAR* pNext = NULL;
		int  line_len = 0;
		int nDivLen = 0;
		do {
			pNext = _tcsstr(pstrBuf, _T("\r\n"));
			if (pNext) { nDivLen = 2; break; }
			pNext = _tcsstr(pstrBuf, _T("\n"));
			if (pNext) { nDivLen = 1; break; }
		} while (0);
		
		if (pNext) {
			line_len = pNext - pstrBuf;
		}
		else {
			line_len = _tcslen(pstrBuf);
		}
		CString strTemp(pstrBuf, line_len);
		pstrArr->Add(strTemp);

		pstrBuf += line_len + nDivLen;
	}

	return pstrArr;
}

//进行逐行处理
void CMainDlg::Do2SideAdd()
{
	CSimpleArray<CString>* pLines = GetLines();
	if (!pLines || pLines->GetSize() == 0) {
		if(pLines){delete pLines;}
		return;
	}

	CDlg2Side dlgSetting;
	if (IDOK != dlgSetting.DoModal()) {
		return;
	}

	stSetting2Side setting = dlgSetting.GetSetting();
	m_richTar.SetWindowText(_T(""));

	for (int i = 0; i < pLines->GetSize(); i++) {
		CString line( (*pLines)[i]);
		
		if (setting.bTrimLeftRight) {
			line.TrimLeft();
			line.TrimRight();
		}
		if (setting.bDelEmpty) {
			if (!setting.bTrimLeftRight) 
			{ 
				line.TrimLeft(); 
				line.TrimRight();
			}
			if (line.IsEmpty())
			{
				continue; //!
			}
		}
		if (!setting.strLeft.IsEmpty()) {
			line = setting.strLeft + line;
		}
		if (!setting.strRight.IsEmpty()) {
			line += setting.strRight;
		}
		if (!setting.strReplacedCrlf.IsEmpty()) {
			line += setting.strReplacedCrlf;
		}
		if (setting.bReplacedCrlf) {
			line += _T("\r\n");
		}

		m_richTar.AppendText(line, FALSE);
		//m_richTar.AppendText(_T("\r\n"), FALSE);
	}

	delete pLines;
}


