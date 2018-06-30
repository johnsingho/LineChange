#pragma once
#include "resource.h"


struct stSetting2Side {
	CString strLeft;
	CString strRight;
	CString strReplacedCrlf;
	BOOL bTrimLeftRight;
	BOOL bDelEmpty;
	BOOL bReplacedCrlf;
};

class CDlg2Side : public CDialogImpl<CDlg2Side>
{
public:
	enum { IDD = IDD_DLG_2SIDE};

	BEGIN_MSG_MAP(CDlg2Side)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		//COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
	const stSetting2Side& GetSetting() { return m_setting; }
private:
	stSetting2Side m_setting;
	CFont m_fontCmb;
};