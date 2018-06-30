#include "stdafx.h"
#include "Dlg2Side.h"



LRESULT CDlg2Side::OnInitDialog(UINT, WPARAM, LPARAM, BOOL &)
{
	// center the dialog on the screen
	CenterWindow();

	m_fontCmb.CreateFont(12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, 
						ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
						DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, _T("MS Sans Serif"));

	m_setting.bTrimLeftRight = TRUE;
	m_setting.bDelEmpty = TRUE;
	m_setting.bReplacedCrlf = TRUE;

	CButton chkBox = GetDlgItem(IDC_CHK_TRIM);
	chkBox.SetCheck(m_setting.bTrimLeftRight ? BST_CHECKED : BST_UNCHECKED);
	chkBox = GetDlgItem(IDC_CHK_DELEMP);
	chkBox.SetCheck(m_setting.bDelEmpty ? BST_CHECKED : BST_UNCHECKED);
	chkBox = GetDlgItem(IDC_CHECK_CRLF);
	chkBox.SetCheck(m_setting.bReplacedCrlf ? BST_CHECKED : BST_UNCHECKED);

	CComboBox cmbBox;
	cmbBox = GetDlgItem(IDC_CMB_LEFT);
	cmbBox.AddString(_T("'"));
	cmbBox.AddString(_T("\""));
	cmbBox.SetFont(m_fontCmb);

	TCHAR szBuf[128]={0};
	cmbBox = GetDlgItem(IDC_CMB_RIGHT);
	cmbBox.GetWindowText(szBuf, 127);
	m_setting.strRight=szBuf;
	cmbBox.AddString(_T("', "));
	cmbBox.AddString(_T("\", "));
	cmbBox.SetFont(m_fontCmb);

	SetWindowPos(true ? HWND_TOPMOST : HWND_NOTOPMOST, 
				0, 0, 0, 0, 
				SWP_NOREDRAW | SWP_NOSIZE | SWP_NOMOVE);
	return 0;
}

LRESULT CDlg2Side::OnOK(WORD, WORD wID, HWND, BOOL &)
{
	TCHAR szBuf[128]={0};
	CComboBox cmbBox;
	cmbBox = GetDlgItem(IDC_CMB_LEFT);
	cmbBox.GetWindowText(szBuf, 127);
	m_setting.strLeft=szBuf;
	cmbBox = GetDlgItem(IDC_CMB_RIGHT);
	cmbBox.GetWindowText(szBuf, 127);
	m_setting.strRight=szBuf;

	CEdit editBox;
	editBox = GetDlgItem(IDC_EDIT_REPCRLF);
	editBox.GetWindowText(szBuf, 127);
	m_setting.strReplacedCrlf=szBuf;

	CButton chkBox = GetDlgItem(IDC_CHK_TRIM);
	m_setting.bTrimLeftRight = (BST_CHECKED == chkBox.GetCheck());
	chkBox = GetDlgItem(IDC_CHK_DELEMP);
	m_setting.bDelEmpty = (BST_CHECKED == chkBox.GetCheck());
	chkBox = GetDlgItem(IDC_CHECK_CRLF);
	m_setting.bReplacedCrlf = (BST_CHECKED == chkBox.GetCheck());

	EndDialog(wID);
	return 0;
}

LRESULT CDlg2Side::OnCancel(WORD, WORD wID, HWND, BOOL &)
{
	EndDialog(wID);
	return 0;
}

