// maindlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINDLG_H__DD16F41E_6DE2_44FF_9932_E874FA54128C__INCLUDED_)
#define AFX_MAINDLG_H__DD16F41E_6DE2_44FF_9932_E874FA54128C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <memory>



class CMainDlg : public CDialogImpl<CMainDlg>, public CDynamicDialogLayout<CMainDlg>, public CUpdateUI<CMainDlg>
				,public CMessageFilter, public CIdleHandler
{
public:
	enum { IDD = IDD_MAINDLG };

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDC_CONVERT, OnConverty)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		CHAIN_MSG_MAP(CDynamicDialogLayout<CMainDlg>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnConverty(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSysCommand(UINT, WPARAM, LPARAM, BOOL&);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();
private:
	void DoConvert();
	std::unique_ptr<CSimpleArray<CString>> GetLines();
	void Do2SideAdd();
	void DoSlashEscape();
	void DoSlashUnEscape();
	void DoURLHexUnEscape();
	void DoToLinuxSlash();
	void DoToWindowSlash();
	void DoToMingWPath();
	void DoFromMingWPath();
	void DoEscapeHtmlChar();
	void DoUnEscapeHtmlChar();


private:
	CRichEditCtrl m_richSrc;
	CRichEditCtrl m_richTar;
	CComboBox m_cmbOperType;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDLG_H__DD16F41E_6DE2_44FF_9932_E874FA54128C__INCLUDED_)
